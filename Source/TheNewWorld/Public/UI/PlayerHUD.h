// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class ABaseCharacter;
class UCrosshair;
class UCompassBar;
class UHealthBar;

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

	UPROPERTY()
	UCrosshair* CrosshairWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCrosshair> CrosshairWidgetClass;

	UPROPERTY()
	UCompassBar* CompassBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCompassBar> CompassBarWidgetClass;

	UPROPERTY()
	UHealthBar* HealthBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UHealthBar> HealthBarWidgetClass;

protected:
	
	virtual void BeginPlay() override;
	
	
};
