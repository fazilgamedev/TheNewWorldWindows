// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupSystem/PickupMaster.h"
#include "WeaponPickup.generated.h"

class UWeaponMaster;
class ABaseCharacter;

/**
 * 
 */
UCLASS()
class THENEWWORLD_API AWeaponPickup : public APickupMaster
{
	GENERATED_BODY()
	
public:

	AWeaponPickup();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWeaponMaster> WeaponToSpawn;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Interact_Implementation(ABaseCharacter* Interactor) override;

	UFUNCTION(Server, Reliable)
	void SR_Interact(ABaseCharacter* Interactor, TSubclassOf<UWeaponMaster> Weapon);
	
};
