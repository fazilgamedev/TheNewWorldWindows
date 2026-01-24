// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponSystem/WeaponEnums.h"
#include "HealthSystem/DamageInfo.h"
#include "WeaponMaster.generated.h"

class USkeletalMesh;
class AWeaponPickup;
class UParticleSystem;
class UAnimSequence;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class THENEWWORLD_API UWeaponMaster : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponMaster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* MagazineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeaponPickup> PickupClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketToAttach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageInfo DamageInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ADSFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* FireAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UParticleSystem*> EFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VerticalRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HorizontalRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SideRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FrontRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RightRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UpRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Recoil_Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Recoil_Horizontal_Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Recoil_Horizontal_Right;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
