// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BodyAnimInst.generated.h"

class ABaseCharacter;
class UWeaponMaster;

/**
 * 
 */
UCLASS()
class THENEWWORLD_API UBodyAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UBodyAnimInst();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float WallAlpha;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform LHIK;

protected:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:

	UPROPERTY()
	ABaseCharacter* CharacterREF;

	UPROPERTY()
	UWeaponMaster* CurrentWeapon;

//////////////////////////////////////////////////////////////////



public:

	
	
};
