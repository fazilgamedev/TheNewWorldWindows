// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSlots.generated.h"

class ABaseCharacter;
class UTextBlock;

/**
 * 
 */
UCLASS()
class THENEWWORLD_API UWeaponSlots : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& Geometry, float InDeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	ABaseCharacter* CharacterREF;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* AmmoCounter;

	UFUNCTION(NetMulticast, Reliable)
	void UpdateAmmoCounter(const FString& AmmoCount);


};
