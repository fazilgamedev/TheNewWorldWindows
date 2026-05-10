// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponEnums.generated.h"

UENUM(BlueprintType)
enum class EWeaponName : uint8
{
	None UMETA(DisplayName="None"),
	AK47 UMETA(DisplayName="AK47"),
	M4 UMETA(DisplayName="M4"),
	AKS74U UMETA(DisplayName="AKS74U"),
	ASVAL UMETA(DisplayName = "ASVAL")
};
