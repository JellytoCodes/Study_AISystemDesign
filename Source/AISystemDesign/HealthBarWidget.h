// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class AISYSTEMDESIGN_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	virtual void NativeConstruct() override;
	void SetBarValuePercent(float const value);

private :
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	class UProgressBar* healthValue = nullptr;
};
