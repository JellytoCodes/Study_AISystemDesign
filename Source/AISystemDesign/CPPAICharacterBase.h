// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPPAICharacterBase.generated.h"

UCLASS()
class AISYSTEMDESIGN_API ACPPAICharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACPPAICharacterBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float GetHealth() const;
	float GetMaxHealth() const;
	void SetHealth(float const newHealth);

	void AttackStart();
	void AttackEnd();

protected:
	virtual void BeginPlay() override;

private :
	UPROPERTY()
	class UWidgetComponent* WidgetComponent;
	float const maxHealth = 100.f;
	float curHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RightFistCollisionBox;

	UFUNCTION()
	void OnAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
