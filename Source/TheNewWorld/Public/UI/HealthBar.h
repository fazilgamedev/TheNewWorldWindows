// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class ABaseCharacter;
class UProgressBar;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class THENEWWORLD_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& Geometry, float InDeltaTime) override;
	
public:

	UPROPERTY()
	ABaseCharacter* CharacterREF;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UFUNCTION(BlueprintPure)
	float UpdateHealth();

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
	UWidgetAnimation* OnHit;

	
};
