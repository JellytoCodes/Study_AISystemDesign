// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPCAIController.h"
#include "NPC.h"
#include "PatrolPath.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(FObjectInitializer const &ObjectInitializer)
:UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//attempt to get the NPC's controller
	if(auto* const NPCCon = Cast<ANPCAIController>(OwnerComp.GetAIOwner()))
	{
		//attempt to get the blackboard component from the behaviour tree
		if(auto* const BlackboardComp = OwnerComp.GetBlackboardComponent())
		{
			//get the current patrol path index from the blackboard
			auto const index = BlackboardComp->GetValueAsInt(GetSelectedBlackboardKey());

			//get the NPC
			if(auto* NPC = Cast<ANPC>(NPCCon->GetPawn()))
			{
				//get the current patrol path vector from the NPC - this is local to the patrol path actor
				auto const Point = NPC->GetPatrolPath()->GetPatrolPoint(index);

				auto const globalPoint = NPC->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
				BlackboardComp->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, globalPoint);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}