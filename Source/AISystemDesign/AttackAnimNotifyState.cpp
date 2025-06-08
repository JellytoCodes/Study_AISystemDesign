#include "AttackAnimNotifyState.h"
#include "CPPAICharacterBase.h"

void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, float TotalDuration)
{
	//Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if(MeshComp && MeshComp->GetOwner())
	{
		if(auto* const character = Cast<ACPPAICharacterBase>(MeshComp->GetOwner()))
		{
			character->AttackStart();
		}
	}
}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation)
{
	//Super::NotifyEnd(MeshComp, Animation);

	if(MeshComp && MeshComp->GetOwner())
	{
		if(auto* const character = Cast<ACPPAICharacterBase>(MeshComp->GetOwner()))
		{
			character->AttackEnd();
		}
	}
}
