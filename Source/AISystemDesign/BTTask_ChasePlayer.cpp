// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NPCAIController.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer)
:UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if(auto* const NPCCon = Cast<ANPCAIController>(OwnerComp.GetAIOwner()))
    {
        auto const playerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
        APawn* npc = NPCCon->GetPawn();
        if (!npc) return EBTNodeResult::Failed;

        // npc 위치와 플레이어 위치 벡터
        FVector npcLocation = npc->GetActorLocation();

        // 원하는 최소 거리(띄우기)
        float minDistance = 90.f;

        // 현재 거리
        float currentDist = FVector::Dist(npcLocation, playerLocation);

        // 목표 위치 보정: 너무 가까우면 멈춤
        if (currentDist <= minDistance)
        {
            NPCCon->StopMovement();
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return EBTNodeResult::Succeeded;
        }

        // 플레이어 방향 단위 벡터
        FVector dirToPlayer = (playerLocation - npcLocation).GetSafeNormal();

        // 목표 위치 = 플레이어 위치에서 minDistance만큼 멀리
        FVector targetLocation = playerLocation - dirToPlayer * minDistance;

        // 이동 명령
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(NPCCon, targetLocation);

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return EBTNodeResult::Succeeded;
    }
	return EBTNodeResult::Failed;
}