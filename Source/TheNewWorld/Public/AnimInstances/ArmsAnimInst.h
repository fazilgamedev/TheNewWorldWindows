// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponSystem/WeaponEnums.h"
#include "ArmsAnimInst.generated.h"

class ABaseCharacter;
class UWeaponMaster;

/**
 * 
 */
UCLASS()
class THENEWWORLD_API UArmsAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWeaponMaster* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EWeaponName WeaponName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform SightTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AimAlpha;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform RelativeHandTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform LHIKTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float LHIKAlpha;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector SwayVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FRotator TurnRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector TurnLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SideMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float FrontMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform RecoilTransform;

protected:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:

	UPROPERTY()
	ABaseCharacter* CharacterREF;

	UPROPERTY()
	FRotator OldRotation;

	UPROPERTY()
	FTransform RecoilResetTransform;

//////////////////////////////////////////////////////////////////

	UFUNCTION()
	void SetSightTransform();

	UFUNCTION()
	void SetRelativeHandTransform();

	UFUNCTION()
	void SetLHIKTransform();

	UFUNCTION()
	void WalkAnim(float DeltaTime);

	UFUNCTION()
	void HandSway(float DeltaTime);

	UFUNCTION()
	void DirectionalRotation(float &DirVar, float MoveVar, float DeltaTime, float U, float V, float W);

	UFUNCTION()
	void InterpRecoil(float DeltaTime);

	UFUNCTION()
	void InterpResetRecoil(float DeltaTime);

public:

	UFUNCTION()
	void Firing();
	
	
};
