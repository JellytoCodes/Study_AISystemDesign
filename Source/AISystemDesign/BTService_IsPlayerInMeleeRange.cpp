// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerInMeleeRange.h"
#include "NPCAIController.h"
#include "NPC.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameWork/Character.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_IsPlayerInMeleeRange::UBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	// get controller and NPC
	auto const* const NPCCon = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	auto const* const NPC = Cast<ANPC>(NPCCon->GetPawn());

	// get player Character
	auto const* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// write true of false to the blackboard key depending on whether or not the player is in range
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		GetSelectedBlackboardKey(), NPC->GetDistanceTo(player) <= MeleeRange);
}
