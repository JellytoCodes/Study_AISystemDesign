// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementPathIndex.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPCAIController.h"
#include "NPC.h"
#include "PatrolPath.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(FObjectInitializer const &ObjectInitializer)
:UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// try and get the AI Controller
	if(auto* const NPCCon = Cast<ANPCAIController>(OwnerComp.GetAIOwner()))
	{
		// try and get the NPC
		if(auto* NPC = Cast<ANPC>(NPCCon->GetPawn()))
		{
			// try and get the blackboard
			if(auto* const BlackboardComp = OwnerComp.GetBlackboardComponent())
			{
				// get number of patrol points and min and max indices
				auto const noOfPoint = NPC->GetPatrolPath()->GetPatrolPointNum();
				int32 const MinIndex = 0;
				int32 const MaxIndex = noOfPoint - 1;
				auto Index = BlackboardComp->GetValueAsInt(GetSelectedBlackboardKey());

				// change direction if we are at the first or last index if we are in bidirectional mode
				if(bBiDirectional)
				{
					if(Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					else if(Index >= MaxIndex && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}
				}

				// write new of index to blackboard
				BlackboardComp->SetValueAsInt(GetSelectedBlackboardKey(), (Direction == EDirectionType::Forward ? ++Index : --Index) % noOfPoint);

				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	// something went wrong so fail
	return EBTNodeResult::Failed;
}