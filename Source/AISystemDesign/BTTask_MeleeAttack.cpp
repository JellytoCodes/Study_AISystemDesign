// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"
#include "NPCAIController.h"
#include "NPC.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if(OutOfRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return  EBTNodeResult::Succeeded;
	}

	auto const* const NPCCon = OwnerComp.GetAIOwner();
	auto* const NPC = Cast<ANPC>(NPCCon->GetPawn());
	
	if(auto* const iCombat = Cast<ICombatInterface>(NPC))
	{
		iCombat->Execute_MeleeAttack(NPC);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return  EBTNodeResult::Type();
}

bool UBTTask_MeleeAttack::MontageHasFinished(ANPC *const NPC)
{

	return NPC->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(NPC->GetMontage());
}
