// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPAICharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "HealthBarWidget.h"
#include "Consts.h"
#include "NPC.h"
#include "AISystemDesignCharacter.h"

ACPPAICharacterBase::ACPPAICharacterBase() :
	WidgetComponent{CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue"))}, curHealth{maxHealth},
	RightFistCollisionBox{CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox"))}
{
	PrimaryActorTick.bCanEverTick = true;

	if(WidgetComponent)
	{
		WidgetComponent->SetupAttachment(RootComponent);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComponent->SetRelativeLocation(defs::HealthBarZ);
		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/AI/BP_HealthBar.BP_HealthBar_C'"));
		if(WidgetClass.Succeeded())
		{
			WidgetComponent->SetWidgetClass(WidgetClass.Class);
		}
	}

	if(RightFistCollisionBox)
	{
		RightFistCollisionBox->SetBoxExtent(defs::RightFirstBoxSize, false);
		FAttachmentTransformRules const rules{EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false};
		RightFistCollisionBox->AttachToComponent(GetMesh(), rules, defs::RightFirstSocketName);
		RightFistCollisionBox->SetRelativeLocation(defs::CollisionBoxLocation);
		
	}
}

void ACPPAICharacterBase::BeginPlay()
{
	Super::BeginPlay();

	RightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPPAICharacterBase::OnAttackOverlapBegin);
	RightFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACPPAICharacterBase::OnAttackOverlapEnd);
	
}

void ACPPAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(auto const widget = Cast<UHealthBarWidget>(WidgetComponent->GetUserWidgetObject()))
	{
		widget->SetBarValuePercent(curHealth / maxHealth);
	}
}

void ACPPAICharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACPPAICharacterBase::GetHealth() const
{
	return curHealth;
}

float ACPPAICharacterBase::GetMaxHealth() const
{
	return maxHealth;
}

void ACPPAICharacterBase::SetHealth(float const newHealth)
{
	curHealth = newHealth;

	auto const Player = Cast<AAISystemDesignCharacter>(this);
	auto const Enemy = Cast<ANPC>(this);
	
	if(Player && curHealth <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("You Lose!!!"));
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
	}
	else if(Enemy && curHealth <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("You Win!!!"));
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
	}


}

void ACPPAICharacterBase::AttackStart()
{
	RightFistCollisionBox->SetCollisionProfileName("Fist");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(true);
}

void ACPPAICharacterBase::AttackEnd()
{
	RightFistCollisionBox->SetCollisionProfileName("Fist");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(false);
}

void ACPPAICharacterBase::OnAttackOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(OtherActor == this) return;

	if(auto const Enemy = Cast<ANPC>(OtherActor))
	{
		auto const newHealth = Enemy->GetHealth() - Enemy->GetMaxHealth() * .1f;
		Enemy->SetHealth(newHealth);
	}
	else if(auto const Player = Cast<AAISystemDesignCharacter>(OtherActor))
	{
		auto const newHealth = Player->GetHealth() - Player->GetMaxHealth() * .07f;
		Player->SetHealth(newHealth);
	}
}

void ACPPAICharacterBase::OnAttackOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{

}