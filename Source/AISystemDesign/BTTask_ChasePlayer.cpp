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

        // npc ��ġ�� �÷��̾� ��ġ ����
        FVector npcLocation = npc->GetActorLocation();

        // ���ϴ� �ּ� �Ÿ�(����)
        float minDistance = 90.f;

        // ���� �Ÿ�
        float currentDist = FVector::Dist(npcLocation, playerLocation);

        // ��ǥ ��ġ ����: �ʹ� ������ ����
        if (currentDist <= minDistance)
        {
            NPCCon->StopMovement();
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return EBTNodeResult::Succeeded;
        }

        // �÷��̾� ���� ���� ����
        FVector dirToPlayer = (playerLocation - npcLocation).GetSafeNormal();

        // ��ǥ ��ġ = �÷��̾� ��ġ���� minDistance��ŭ �ָ�
        FVector targetLocation = playerLocation - dirToPlayer * minDistance;

        // �̵� ���
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(NPCCon, targetLocation);

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return EBTNodeResult::Succeeded;
    }
	return EBTNodeResult::Failed;
}