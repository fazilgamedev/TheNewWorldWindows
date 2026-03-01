// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/WeaponPickup.h"
#include "Characters/BaseCharacter.h"
#include "WeaponSystem/WeaponMaster.h"
#include "Net/UnrealNetwork.h"

AWeaponPickup::AWeaponPickup()
{
    WeaponToSpawn = UWeaponMaster::StaticClass();
}

void AWeaponPickup::BeginPlay()
{
    Super::BeginPlay();

    SetActorRotation(FRotator(90.f, 0.f, 0.f));
}

void AWeaponPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AWeaponPickup, CurrentMagCount);
}

void AWeaponPickup::Interact_Implementation(ABaseCharacter *Interactor)
{
    SR_Interact(Interactor, WeaponToSpawn);
}

void AWeaponPickup::SR_Interact_Implementation(ABaseCharacter *Interactor, TSubclassOf<UWeaponMaster> Weapon)
{
    Interactor->SpawnWeapon(Weapon);
    UWeaponMaster* NewWeapon = Interactor->GetCurrentWeapon();
    if (NewWeapon) {
        NewWeapon->CurrentAmmoCount = CurrentMagCount;
        Interactor->OnAmmoCountChanged();
    }
    Destroy();
}