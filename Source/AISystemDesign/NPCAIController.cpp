// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AISystemDesignCharacter.h"
#include "NPC.h"

ANPCAIController::ANPCAIController(FObjectInitializer const &ObjectInitializer)
{
	SetupPerceptionSystem();
}

void ANPCAIController::OnPossess(APawn *inPawn)
{
	Super::OnPossess(inPawn);

	if(ANPC* const npc = Cast<ANPC>(inPawn))
	{
		if(UBehaviorTree* const tree = npc->GetBehaviorTree())
		{
			UBlackboardComponent* blackboard;
			UseBlackboard(tree->BlackboardAsset, blackboard);
			Blackboard = blackboard;
			RunBehaviorTree(tree);
		}
	}
}

void ANPCAIController::SetupPerceptionSystem()
{
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if(sightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		sightConfig->SightRadius = 500.f;
		sightConfig->LoseSightRadius = sightConfig->SightRadius + 25.f;
		sightConfig->PeripheralVisionAngleDegrees = 90.f;
		sightConfig->SetMaxAge(5.f);
		sightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		sightConfig->DetectionByAffiliation.bDetectEnemies = true;
		sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		sightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*sightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCAIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*sightConfig);
	}
}

void ANPCAIController::OnTargetDetected(AActor *Actor, FAIStimulus const Stimulus)
{
	if(auto* const ch = Cast<AAISystemDesignCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}
