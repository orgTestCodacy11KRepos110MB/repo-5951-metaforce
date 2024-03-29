set(CHARACTER_SOURCES
        CharacterCommon.hpp CharacterCommon.cpp
        CAssetFactory.hpp CAssetFactory.cpp
        CCharacterFactory.hpp CCharacterFactory.cpp
        CModelData.hpp CModelData.cpp
        CAnimData.hpp CAnimData.cpp
        CCharAnimTime.hpp CCharAnimTime.cpp
        IMetaAnim.hpp IMetaAnim.cpp
        IMetaTrans.hpp
        IVaryingAnimationTimeScale.hpp
        CAnimationDatabase.hpp
        CAnimationDatabaseGame.hpp CAnimationDatabaseGame.cpp
        CTransitionDatabase.hpp
        CTransitionDatabaseGame.hpp CTransitionDatabaseGame.cpp
        CHierarchyPoseBuilder.hpp CHierarchyPoseBuilder.cpp
        CPoseAsTransforms.hpp CPoseAsTransforms.cpp
        CCharLayoutInfo.hpp CCharLayoutInfo.cpp
        CLayoutDescription.hpp
        CSegIdList.hpp CSegIdList.cpp
        CSegId.hpp
        TSegIdMap.hpp
        CIkChain.hpp CIkChain.cpp
        CSkinRules.hpp CSkinRules.cpp
        CAnimCharacterSet.hpp CAnimCharacterSet.cpp
        CAnimationSet.hpp CAnimationSet.cpp
        CCharacterSet.hpp CCharacterSet.cpp
        CCharacterInfo.hpp CCharacterInfo.cpp
        CPASDatabase.hpp CPASDatabase.cpp
        CPASAnimState.hpp CPASAnimState.cpp
        CPASParmInfo.hpp CPASParmInfo.cpp
        CPASAnimInfo.hpp CPASAnimInfo.cpp
        CPASAnimParm.hpp
        CPASAnimParmData.hpp CPASAnimParmData.cpp
        CEffectComponent.hpp CEffectComponent.cpp
        CAnimation.hpp CAnimation.cpp
        CAnimationManager.hpp CAnimationManager.cpp
        CTransition.hpp CTransition.cpp
        CTransitionManager.hpp CTransitionManager.cpp
        CMetaAnimFactory.hpp CMetaAnimFactory.cpp
        CMetaAnimPlay.hpp CMetaAnimPlay.cpp
        CMetaAnimBlend.hpp CMetaAnimBlend.cpp
        CMetaAnimPhaseBlend.hpp CMetaAnimPhaseBlend.cpp
        CMetaAnimRandom.hpp CMetaAnimRandom.cpp
        CMetaAnimSequence.hpp CMetaAnimSequence.cpp
        CMetaTransFactory.hpp CMetaTransFactory.cpp
        CMetaTransMetaAnim.hpp CMetaTransMetaAnim.cpp
        CMetaTransTrans.hpp CMetaTransTrans.cpp
        CMetaTransPhaseTrans.hpp CMetaTransPhaseTrans.cpp
        CMetaTransSnap.hpp CMetaTransSnap.cpp
        CAnimTreeLoopIn.hpp CAnimTreeLoopIn.cpp
        CAnimTreeSequence.hpp CAnimTreeSequence.cpp
        CSequenceHelper.hpp CSequenceHelper.cpp
        CAnimTreeAnimReaderContainer.hpp CAnimTreeAnimReaderContainer.cpp
        CTreeUtils.hpp CTreeUtils.cpp
        CAnimTreeBlend.hpp CAnimTreeBlend.cpp
        CAnimTreeNode.hpp CAnimTreeNode.cpp
        CAnimTreeTimeScale.hpp CAnimTreeTimeScale.cpp
        CAnimTreeTransition.hpp CAnimTreeTransition.cpp
        CAnimTreeTweenBase.hpp CAnimTreeTweenBase.cpp
        CAnimTreeSingleChild.hpp CAnimTreeSingleChild.cpp
        CAnimTreeDoubleChild.hpp CAnimTreeDoubleChild.cpp
        CAnimPlaybackParms.hpp
        IAnimReader.hpp IAnimReader.cpp
        CPrimitive.hpp CPrimitive.cpp
        CHalfTransition.hpp CHalfTransition.cpp
        CTimeScaleFunctions.hpp CTimeScaleFunctions.cpp
        CParticleData.hpp CParticleData.cpp
        CParticleDatabase.hpp CParticleDatabase.cpp
        CParticleGenInfo.hpp CParticleGenInfo.cpp
        CAnimPOIData.hpp CAnimPOIData.cpp
        CPOINode.hpp CPOINode.cpp
        CBoolPOINode.hpp CBoolPOINode.cpp
        CInt32POINode.hpp CInt32POINode.cpp
        CSoundPOINode.hpp CSoundPOINode.cpp
        CParticlePOINode.hpp CParticlePOINode.cpp
        CAnimSourceReader.hpp CAnimSourceReader.cpp
        CAnimSource.hpp CAnimSource.cpp
        CFBStreamedAnimReader.hpp CFBStreamedAnimReader.cpp
        CFBStreamedCompression.hpp CFBStreamedCompression.cpp
        CAllFormatsAnimSource.hpp CAllFormatsAnimSource.cpp
        CSegStatementSet.hpp CSegStatementSet.cpp
        CAnimPerSegmentData.hpp
        CAdditiveAnimPlayback.hpp CAdditiveAnimPlayback.cpp
        CActorLights.hpp CActorLights.cpp
        CAnimSysContext.hpp
        CBodyState.hpp CBodyState.cpp
        CAdditiveBodyState.hpp CAdditiveBodyState.cpp
        CBodyStateCmdMgr.hpp CBodyStateCmdMgr.cpp
        CBodyController.hpp CBodyController.cpp
        CGroundMovement.hpp CGroundMovement.cpp
        CSteeringBehaviors.hpp CSteeringBehaviors.cpp
        CBodyStateInfo.hpp CBodyStateInfo.cpp
        CBoneTracking.hpp CBoneTracking.cpp
        CRagDoll.hpp CRagDoll.cpp)

runtime_add_list(Character CHARACTER_SOURCES)
