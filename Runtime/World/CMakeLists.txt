set(WORLD_SOURCES
        CWorld.hpp CWorld.cpp
        CWorldLight.hpp CWorldLight.cpp
        IGameArea.hpp IGameArea.cpp
        CGameArea.hpp CGameArea.cpp
        CPlayer.hpp CPlayer.cpp
        CPlayerEnergyDrain.hpp CPlayerEnergyDrain.cpp
        CEnergyDrainSource.hpp CEnergyDrainSource.cpp
        CPlayerCameraBob.hpp CPlayerCameraBob.cpp
        CScriptCameraShaker.hpp CScriptCameraShaker.cpp
        CMorphBall.hpp CMorphBall.cpp
        CMorphBallShadow.hpp CMorphBallShadow.cpp
        CActor.hpp CActor.cpp
        CAi.hpp CAi.cpp
        CAiFuncMap.hpp CAiFuncMap.cpp
        CStateMachine.hpp CStateMachine.cpp
        CPatterned.hpp CPatterned.cpp
        CKnockBackController.hpp CKnockBackController.cpp
        CPathFindArea.hpp CPathFindArea.cpp
        CPathFindRegion.hpp CPathFindRegion.cpp
        CPathFindSearch.hpp CPathFindSearch.cpp
        CPathFindSpline.cpp
        CPhysicsActor.hpp CPhysicsActor.cpp
        CEntity.hpp CEntity.cpp
        CPhysicsActor.hpp CPhysicsActor.cpp
        CWorldTransManager.hpp CWorldTransManager.cpp
        CEnvFxManager.hpp CEnvFxManager.cpp
        CActorModelParticles.hpp CActorModelParticles.cpp
        CTeamAiTypes.hpp
        ScriptObjectSupport.hpp ScriptObjectSupport.cpp
        ScriptLoader.hpp ScriptLoader.cpp
        CScriptActor.hpp CScriptActor.cpp
        CScriptWaypoint.hpp CScriptWaypoint.cpp
        CScriptDoor.hpp CScriptDoor.cpp
        CScriptTrigger.hpp CScriptTrigger.cpp
        CScriptTimer.hpp CScriptTimer.cpp
        CScriptCounter.hpp CScriptCounter.cpp
        CScriptEffect.hpp CScriptEffect.cpp
        CScriptSteam.hpp CScriptSteam.cpp
        CScriptRipple.hpp CScriptRipple.cpp
        CScriptBallTrigger.hpp CScriptBallTrigger.cpp
        CScriptPlatform.hpp CScriptPlatform.cpp
        CScriptSound.hpp CScriptSound.cpp
        CScriptGenerator.hpp CScriptGenerator.cpp
        CScriptDock.hpp CScriptDock.cpp
        CScriptActorKeyframe.hpp CScriptActorKeyframe.cpp
        CScriptWater.hpp CScriptWater.cpp
        CScriptGrapplePoint.hpp CScriptGrapplePoint.cpp
        CScriptSpiderBallAttractionSurface.hpp CScriptSpiderBallAttractionSurface.cpp
        CScriptPickupGenerator.hpp CScriptPickupGenerator.cpp
        CScriptPointOfInterest.hpp CScriptPointOfInterest.cpp
        CScriptAreaAttributes.hpp CScriptAreaAttributes.cpp
        CScriptVisorFlare.hpp CScriptVisorFlare.cpp
        CScriptWorldTeleporter.hpp CScriptWorldTeleporter.cpp
        CScriptCameraWaypoint.hpp CScriptCameraWaypoint.cpp
        CScriptCoverPoint.hpp CScriptCoverPoint.cpp
        CScriptSpiderBallWaypoint.hpp CScriptSpiderBallWaypoint.cpp
        CScriptSpawnPoint.hpp CScriptSpawnPoint.cpp
        CScriptCameraHint.hpp CScriptCameraHint.cpp
        CScriptPickup.hpp CScriptPickup.cpp
        CScriptMemoryRelay.hpp CScriptMemoryRelay.cpp
        CScriptRandomRelay.hpp CScriptRandomRelay.cpp
        CScriptRelay.hpp CScriptRelay.cpp
        CScriptHUDMemo.hpp CScriptHUDMemo.cpp
        CScriptCameraFilterKeyframe.hpp CScriptCameraFilterKeyframe.cpp
        CScriptCameraBlurKeyframe.hpp CScriptCameraBlurKeyframe.cpp
        CScriptDamageableTrigger.hpp CScriptDamageableTrigger.cpp
        CScriptDebris.hpp CScriptDebris.cpp
        CScriptDebugCameraWaypoint.hpp CScriptDebugCameraWaypoint.cpp
        CScriptDistanceFog.hpp CScriptDistanceFog.cpp
        CScriptDockAreaChange.hpp CScriptDockAreaChange.cpp
        CScriptActorRotate.hpp CScriptActorRotate.cpp
        CScriptSpecialFunction.hpp CScriptSpecialFunction.cpp
        CScriptPlayerHint.hpp CScriptPlayerHint.cpp
        CScriptPlayerStateChange.hpp CScriptPlayerStateChange.cpp
        CScriptTargetingPoint.hpp CScriptTargetingPoint.cpp
        CScriptEMPulse.hpp CScriptEMPulse.cpp
        CScriptPlayerActor.hpp CScriptPlayerActor.cpp
        CFishCloud.hpp CFishCloud.cpp
        CFishCloudModifier.hpp CFishCloudModifier.cpp
        CScriptSwitch.hpp CScriptSwitch.cpp
        CWallWalker.hpp CWallWalker.cpp
        CWallCrawlerSwarm.hpp CWallCrawlerSwarm.cpp
        CScriptAiJumpPoint.hpp CScriptAiJumpPoint.cpp
        CScriptRoomAcoustics.hpp CScriptRoomAcoustics.cpp
        CScriptColorModulate.hpp CScriptColorModulate.cpp
        CScriptStreamedMusic.hpp CScriptStreamedMusic.cpp
        CScriptMidi.hpp CScriptMidi.cpp
        CRepulsor.hpp CRepulsor.cpp
        CScriptGunTurret.hpp CScriptGunTurret.cpp
        CScriptCameraPitchVolume.hpp CScriptCameraPitchVolume.cpp
        CTeamAiMgr.hpp CTeamAiMgr.cpp
        CSnakeWeedSwarm.hpp CSnakeWeedSwarm.cpp
        CScriptSpindleCamera.hpp CScriptSpindleCamera.cpp
        CScriptCameraHintTrigger.hpp CScriptCameraHintTrigger.cpp
        CAmbientAI.hpp CAmbientAI.cpp
        CScriptBeam.hpp CScriptBeam.cpp
        CScriptMazeNode.hpp CScriptMazeNode.cpp
        CScriptShadowProjector.hpp CScriptShadowProjector.cpp
        CScriptStreamedMusic.hpp CScriptStreamedMusic.cpp
        CScriptRoomAcoustics.hpp CScriptRoomAcoustics.cpp
        CScriptControllerAction.hpp CScriptControllerAction.cpp
        CVisorFlare.hpp CVisorFlare.cpp
        CScriptVisorGoo.hpp CScriptVisorGoo.cpp
        CGrappleParameters.hpp
        CActorParameters.hpp
        CLightParameters.hpp
        CScannableParameters.hpp
        CVisorParameters.hpp
        CAnimationParameters.hpp
        CDamageInfo.hpp CDamageInfo.cpp
        CDamageVulnerability.hpp CDamageVulnerability.cpp
        CFluidPlaneManager.hpp CFluidPlaneManager.cpp
        CFluidUVMotion.hpp CFluidUVMotion.cpp
        CPatternedInfo.hpp CPatternedInfo.cpp
        CHealthInfo.hpp CHealthInfo.cpp
        CPatterned.hpp CPatterned.cpp
        CHUDMemoParms.hpp
        CWorldShadow.hpp CWorldShadow.cpp
        CProjectedShadow.hpp CProjectedShadow.cpp
        CGameLight.hpp CGameLight.cpp
        CFluidPlane.hpp CFluidPlane.cpp
        CFluidPlaneCPU.hpp CFluidPlaneCPU.cpp
        CFluidPlaneGPU.hpp CFluidPlaneGPU.cpp
        CFluidPlaneDoor.hpp CFluidPlaneDoor.cpp
        CRippleManager.hpp CRippleManager.cpp
        CRipple.hpp CRipple.cpp
        CDestroyableRock.hpp CDestroyableRock.cpp
        CEffect.hpp CEffect.cpp
        CHUDBillboardEffect.hpp CHUDBillboardEffect.cpp
        CExplosion.hpp CExplosion.cpp
        CIceImpact.hpp CIceImpact.cpp
        CMarkerGrid.hpp CMarkerGrid.cpp
        CFire.hpp CFire.cpp
        CEntityInfo.hpp)

runtime_add_list(World WORLD_SOURCES)
