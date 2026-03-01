// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class ABaseCharacter;
class UCrosshair;
class UCompassBar;
class UHealthBar;
class UWeaponSlots;

/**
 * 
 */
UCLASS()
class THENEWWORLD_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	ABaseCharacter* CharacterREF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCrosshair* CrosshairWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCrosshair> CrosshairWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCompassBar* CompassBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCompassBar> CompassBarWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthBar* HealthBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UHealthBar> HealthBarWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWeaponSlots* WeaponSlotsWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWeaponSlots> WeaponSlotsWidgetClass;

protected:
	
	virtual void BeginPlay() override;
	
	
};
