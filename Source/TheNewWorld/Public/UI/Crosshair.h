// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Crosshair.generated.h"

class ABaseCharacter;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class THENEWWORLD_API UCrosshair : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& Geometry, float InDeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	ABaseCharacter* CharacterREF;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
	UWidgetAnimation* CrosshairFire;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
	UWidgetAnimation* HitMarker;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
	UWidgetAnimation* OnAim;

	UFUNCTION(BlueprintCallable)
	void PlayOnCrosshairFire(bool bCanAttack);

	UFUNCTION(BlueprintCallable)
	void PlayOnHitMarker();

	UFUNCTION(BlueprintCallable)
	void PlayOnAim(bool bCanAim);
	
};
