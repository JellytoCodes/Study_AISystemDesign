// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class AISYSTEMDESIGN_API ANPCAIController : public AAIController
{
	GENERATED_BODY()

public :
	explicit ANPCAIController(FObjectInitializer const& ObjectInitializer);

protected :
	virtual void OnPossess(APawn* inPawn) override;

private :
	class UAISenseConfig_Sight* sightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
};
