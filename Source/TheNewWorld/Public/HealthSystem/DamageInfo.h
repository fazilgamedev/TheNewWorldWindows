// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInfo.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8 {
	None,
	Melee,
	Projectile,
	Explosion,
	Environmental
};

UENUM(BlueprintType)
enum class EDamageResponse : uint8 {
	None,
	HitReaction,
	Stagger,
	Stun,
	Knockback
};

USTRUCT(BlueprintType)
struct FDamageInfo {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageResponse DamageResponse;

	FDamageInfo() : Amount(0.f), DamageType(EDamageType::None), DamageResponse(EDamageResponse::None) {}
};

