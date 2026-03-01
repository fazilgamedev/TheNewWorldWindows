// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WeaponSlots.h"
#include "Components/TextBlock.h"

void UWeaponSlots::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWeaponSlots::NativeTick(const FGeometry& Geometry, float InDeltaTime)
{
	Super::NativeTick(Geometry, InDeltaTime);
}

void UWeaponSlots::UpdateAmmoCounter_Implementation(const FString& AmmoCount)
{
	AmmoCounter->SetText(FText::FromString(AmmoCount));
}