#include "Runtime/Collision/CCollisionActorManager.hpp"

#include "Runtime/CStateManager.hpp"
#include "Runtime/Collision/CCollisionActor.hpp"
#include "Runtime/Collision/CMaterialList.hpp"
#include "Runtime/World/CActor.hpp"

#include "TCastTo.hpp" // Generated file, do not modify include path

namespace metaforce {

CCollisionActorManager::CCollisionActorManager(CStateManager& mgr, TUniqueId owner, TAreaId area,
                                               const std::vector<CJointCollisionDescription>& descs, bool active)
: x10_ownerId(owner), x12_active(active) {
  if (TCastToConstPtr<CActor> act = mgr.GetObjectById(x10_ownerId)) {
    const zeus::CTransform xf = act->GetTransform();
    const CAnimData* animData = act->GetModelData()->GetAnimationData();
    const zeus::CVector3f scale = act->GetModelData()->GetScale();
    const zeus::CTransform scaleXf = zeus::CTransform::Scale(scale);

    x0_jointDescriptions.reserve(descs.size());
    for (const CJointCollisionDescription& desc : descs) {
      CJointCollisionDescription modDesc = desc;
      modDesc.ScaleAllBounds(scale);
      const zeus::CTransform locXf = GetWRLocatorTransform(*animData, modDesc.GetPivotId(), xf, scaleXf);

      if (modDesc.GetNextId().IsInvalid()) {
        // We only have the pivot id
        const TUniqueId newId = mgr.AllocateUniqueId();
        CCollisionActor* newAct = nullptr;

        if (modDesc.GetType() == CJointCollisionDescription::ECollisionType::Sphere) {
          newAct = new CCollisionActor(newId, area, x10_ownerId, active, modDesc.GetRadius(), modDesc.GetMass(),
                                       desc.GetName());
        } else if (modDesc.GetType() == CJointCollisionDescription::ECollisionType::OBB) {
          newAct = new CCollisionActor(newId, area, x10_ownerId, modDesc.GetBounds(), modDesc.GetPivotPoint(), active,
                                       modDesc.GetMass(), desc.GetName());
        } else {
          newAct = new CCollisionActor(newId, area, x10_ownerId, modDesc.GetBounds(), active, modDesc.GetMass(),
                                       desc.GetName());
        }

        newAct->SetTransform(locXf);
        mgr.AddObject(newAct);
        x0_jointDescriptions.push_back(desc);
        x0_jointDescriptions.back().SetCollisionActorId(newId);
      } else { // We have another bone in to connect to!
        const zeus::CTransform locXf2 = GetWRLocatorTransform(*animData, modDesc.GetNextId(), xf, scaleXf);
        const float dist = (locXf2.origin - locXf.origin).magnitude();

        if (modDesc.GetType() != CJointCollisionDescription::ECollisionType::OBBAutoSize) {
          const TUniqueId newId = mgr.AllocateUniqueId();
          auto* newAct = new CCollisionActor(newId, area, x10_ownerId, active, modDesc.GetRadius(), modDesc.GetMass(),
                                             desc.GetName());

          newAct->SetTransform(locXf);
          mgr.AddObject(newAct);
          x0_jointDescriptions.push_back(CJointCollisionDescription::SphereCollision(
              modDesc.GetPivotId(), modDesc.GetRadius(), modDesc.GetName(), 0.001f));
          x0_jointDescriptions.back().SetCollisionActorId(newId);

          const u32 numSeps = u32(dist / modDesc.GetMaxSeparation());
          if (numSeps != 0) {
            x0_jointDescriptions.reserve(x0_jointDescriptions.capacity() + numSeps);
            const float pitch = dist / float(numSeps + 1);
            for (u32 i = 0; i < numSeps; ++i) {
              const float separation = pitch * float(i + 1);
              x0_jointDescriptions.push_back(CJointCollisionDescription::SphereSubdivideCollision(
                  modDesc.GetPivotId(), modDesc.GetNextId(), modDesc.GetRadius(), separation,
                  CJointCollisionDescription::EOrientationType::One, modDesc.GetName(), 0.001f));

              const TUniqueId newId2 = mgr.AllocateUniqueId();
              auto* newAct2 = new CCollisionActor(newId2, area, x10_ownerId, active, modDesc.GetRadius(),
                                                  modDesc.GetMass(), desc.GetName());
              if (modDesc.GetOrientationType() == CJointCollisionDescription::EOrientationType::Zero) {
                newAct2->SetTransform(zeus::CTransform::Translate(locXf.origin + (separation * locXf.basis[1])));
              } else {
                const zeus::CVector3f delta = (locXf2.origin - locXf.origin).normalized();
                zeus::CVector3f upVector = locXf.basis[2];

                if (zeus::close_enough(std::fabs(delta.dot(upVector)), 1.f)) {
                  upVector = locXf.basis[1];
                }

                const zeus::CTransform lookAt = zeus::lookAt(zeus::skZero3f, delta, upVector);
                newAct2->SetTransform(zeus::CTransform::Translate(locXf.origin + (separation * lookAt.basis[1])));
              }

              mgr.AddObject(newAct2);
              x0_jointDescriptions.back().SetCollisionActorId(newId2);
            }
          }
        } else {
          if (dist <= FLT_EPSILON) {
            continue;
          }

          zeus::CVector3f bounds = modDesc.GetBounds();
          bounds.y() += dist;
          auto* newAct =
              new CCollisionActor(mgr.AllocateUniqueId(), area, x10_ownerId, bounds,
                                  zeus::CVector3f(0.f, 0.5f * dist, 0.f), active, modDesc.GetMass(), desc.GetName());

          if (modDesc.GetOrientationType() == CJointCollisionDescription::EOrientationType::Zero) {
            newAct->SetTransform(locXf);
          } else {
            const zeus::CVector3f delta = (locXf2.origin - locXf.origin).normalized();
            zeus::CVector3f upVector = locXf.basis[2];

            if (zeus::close_enough(std::fabs(delta.dot(upVector)), 1.f)) {
              upVector = locXf.basis[1];
            }

            newAct->SetTransform(zeus::lookAt(locXf.origin, locXf.origin + delta, upVector));
          }

          mgr.AddObject(newAct);
          x0_jointDescriptions.push_back(desc);
          x0_jointDescriptions.back().SetCollisionActorId(newAct->GetUniqueId());
        }
      }
    }
  }
}

void CCollisionActorManager::Destroy(CStateManager& mgr) {
  for (const CJointCollisionDescription& desc : x0_jointDescriptions) {
    mgr.FreeScriptObject(desc.GetCollisionActorId());
  }

  x13_destroyed = true;
}

void CCollisionActorManager::SetActive(CStateManager& mgr, bool active) {
  x12_active = active;
  for (const CJointCollisionDescription& jDesc : x0_jointDescriptions) {
    if (TCastToPtr<CActor> act = mgr.ObjectById(jDesc.GetCollisionActorId())) {
      const bool curActive = act->GetActive();
      if (curActive != active) {
        act->SetActive(active);
        if (!active) {
          Update(0.f, mgr, EUpdateOptions::WorldSpace);
        }
      }
    }
  }
}

void CCollisionActorManager::AddMaterial(CStateManager& mgr, const CMaterialList& list) {
  for (const CJointCollisionDescription& jDesc : x0_jointDescriptions) {
    if (TCastToPtr<CActor> act = mgr.ObjectById(jDesc.GetCollisionActorId())) {
      act->AddMaterial(list);
    }
  }
}

void CCollisionActorManager::SetMovable(CStateManager& mgr, bool movable) {
  if (x14_movable == movable) {
    return;
  }

  x14_movable = movable;

  for (const CJointCollisionDescription& jDesc : x0_jointDescriptions) {
    if (TCastToPtr<CCollisionActor> act = mgr.ObjectById(jDesc.GetCollisionActorId())) {
      act->SetMovable(x14_movable);
      act->SetUseInSortedLists(x14_movable);
    }
  }
}

void CCollisionActorManager::Update(float dt, CStateManager& mgr, EUpdateOptions opts) {
  if (!x14_movable) {
    SetMovable(mgr, true);
  }

  if (!x12_active) {
    return;
  }

  if (const TCastToConstPtr<CActor> act = mgr.ObjectById(x10_ownerId)) {
    const CAnimData& animData = *act->GetModelData()->GetAnimationData();
    const zeus::CTransform actXf = act->GetTransform();
    const zeus::CTransform scaleXf = zeus::CTransform::Scale(act->GetModelData()->GetScale());

    for (const CJointCollisionDescription& jDesc : x0_jointDescriptions) {
      if (TCastToPtr<CCollisionActor> cAct = mgr.ObjectById(jDesc.GetCollisionActorId())) {
        const zeus::CTransform pivotXf = GetWRLocatorTransform(animData, jDesc.GetPivotId(), actXf, scaleXf);
        zeus::CVector3f origin = pivotXf.origin;

        if (jDesc.GetType() == CJointCollisionDescription::ECollisionType::OBB ||
            jDesc.GetType() == CJointCollisionDescription::ECollisionType::OBBAutoSize) {
          if (jDesc.GetOrientationType() == CJointCollisionDescription::EOrientationType::Zero) {
            cAct->SetTransform(zeus::CQuaternion(pivotXf.basis).toTransform(cAct->GetTranslation()));
          } else {
            const zeus::CTransform nextXf = GetWRLocatorTransform(animData, jDesc.GetNextId(), actXf, scaleXf);
            cAct->SetTransform(zeus::CQuaternion(zeus::lookAt(pivotXf.origin, nextXf.origin, pivotXf.basis[2]).basis)
                                   .toTransform(cAct->GetTranslation()));
          }
        } else if (jDesc.GetType() == CJointCollisionDescription::ECollisionType::SphereSubdivide) {
          if (jDesc.GetOrientationType() == CJointCollisionDescription::EOrientationType::Zero) {
            origin += jDesc.GetMaxSeparation() * pivotXf.basis[1];
          } else {
            const zeus::CTransform nextXf = GetWRLocatorTransform(animData, jDesc.GetNextId(), actXf, scaleXf);
            origin += zeus::lookAt(origin, nextXf.origin, pivotXf.basis[2]).basis[1] * jDesc.GetMaxSeparation();
          }
        }

        if (opts == EUpdateOptions::ObjectSpace) {
          cAct->MoveToOR(cAct->GetTransform().transposeRotate(origin - cAct->GetTranslation()), dt);
        } else {
          cAct->SetTranslation(origin);
        }
      }
    }
  }
}

zeus::CTransform CCollisionActorManager::GetWRLocatorTransform(const CAnimData& animData, CSegId id,
                                                               const zeus::CTransform& worldXf,
                                                               const zeus::CTransform& localXf) {
  zeus::CTransform locXf = animData.GetLocatorTransform(id, nullptr);
  const zeus::CVector3f origin = worldXf * (localXf * locXf.origin);
  locXf = worldXf.multiplyIgnoreTranslation(locXf);
  locXf.origin = origin;
  return locXf;
}

std::optional<zeus::CVector3f> CCollisionActorManager::GetDeviation(const CStateManager& mgr, CSegId seg) {
  for (const CJointCollisionDescription& desc : x0_jointDescriptions) {
    if (desc.GetPivotId() != seg) {
      continue;
    }

    if (const TCastToConstPtr<CActor> act = mgr.GetObjectById(x10_ownerId)) {
      if (const TCastToConstPtr<CCollisionActor> colAct = mgr.GetObjectById(desc.GetCollisionActorId())) {
        const zeus::CTransform xf =
            GetWRLocatorTransform(*act->GetModelData()->GetAnimationData(), desc.GetPivotId(), act->GetTransform(),
                                  zeus::CTransform::Scale(act->GetModelData()->GetScale()));

        return {colAct->GetTranslation() - xf.origin};
      }
    }
  }

  return std::nullopt;
}

} // namespace metaforce
