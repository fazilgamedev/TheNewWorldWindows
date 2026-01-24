// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthSystem/Interfaces/HealthInterface.h"
#include "BaseCharacter.generated.h"

class UCameraComponent;
class UMaterialInstance;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class UWeaponMaster;
class UArmsAnimInst;
class UBodyAnimInst;
class APlayerContoller;
class UHealthComponent;
class APlayerHUD;

USTRUCT(BlueprintType)
struct FLoadout
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UWeaponMaster*> Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentWeaponINDEX;

};

UCLASS()
class THENEWWORLD_API ABaseCharacter : public ACharacter, public IHealthInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Arms;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* WeaponTP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMagTP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* WeaponFP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMagFP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float H_Sensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float V_Sensitivity;

	UPROPERTY()
	float FrontMove;

	UPROPERTY()
	float SideMove;

	UPROPERTY()
	FVector WalkVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;

	UPROPERTY()
	FVector NavPoint;

	UPROPERTY()
    int32 WallAlpha = 0;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float GetCurrentHealth_Implementation() override;

	virtual float GetMaxHealth_Implementation() override;

	virtual float Heal_Implementation(float Amount) override;

	virtual bool TakeDamage_Implementation(FDamageInfo DamageInfo) override;

private:

	UPROPERTY()
	APlayerController* PCREF;

	UPROPERTY()
	APlayerHUD* HUDREF;

	UPROPERTY()
	FTimerHandle AttackHandle;

	UPROPERTY()
	UArmsAnimInst* ArmsAnimInst;

	UPROPERTY()
	UBodyAnimInst* BodyAnimInst;

	UPROPERTY()
	float CurrentPhase;

	UPROPERTY()
	FVector CurrentAmp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	USoundBase* HitSound;

///////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	void MoveFront(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void LookUp(float Value);

	UFUNCTION()
	void LookRight(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	TArray<UMaterialInstance*> Materials;

	UPROPERTY(ReplicatedUsing = OnRep_Loadout)
	FLoadout Loadout;

	UFUNCTION()
	void OnRep_Loadout();

	UFUNCTION(Server, Reliable)
	void SR_Interact(AActor* Target, ABaseCharacter* Interactor);

	UFUNCTION(NetMulticast, Reliable)
	void MC_SetWeaponAtINDEX(UWeaponMaster* Weapon, int32 INDEX);

	UFUNCTION(Server, Reliable)
	void SR_SetWeaponAtINDEX(UWeaponMaster* Weapon, int32 INDEX);

	UFUNCTION(NetMulticast, Reliable)
	void MC_SwitchWeapons(int32 INDEX);
	
	UFUNCTION(Server, Reliable)
	void SR_SwitchWeapons(int32 INDEX);

	UFUNCTION()
	void Interacting();

	UFUNCTION()
	void SwitchPrimary();

	UFUNCTION()
	void SwitchSecondary();

	UFUNCTION()
	void SwitchUnarmed();

	UPROPERTY(ReplicatedUsing = OnRep_bCanAttack)
	bool bCanAttack;

	UFUNCTION()
	void OnRep_bCanAttack();

	UFUNCTION(Server, Reliable)
	void SR_StartAttack();

	UFUNCTION(Server, Reliable)
	void SR_StopAttack();
	
	UFUNCTION()
	void Trace();

	UFUNCTION(NetMulticast, Reliable)
	void MC_Fire(FVector HitLoc, FRotator HitRot, AActor* HitActor);

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void Recoil();

	UFUNCTION()
	void Death();

	UFUNCTION()
	void DamageResponse(EDamageResponse DamageResponse);


public:

	UFUNCTION()
	UWeaponMaster* GetWeaponAtINDEX(int32 INDEX);

	UFUNCTION()
	void SetWeaponAtINDEX(UWeaponMaster* Weapon, int32 INDEX);

	UFUNCTION()
	UWeaponMaster* GetCurrentWeapon();

	UFUNCTION()
	void SetCurrentWeapon(UWeaponMaster* Weapon);

	UFUNCTION()
	void SetCurrentWeaponMesh();

	UFUNCTION()
	void SwitchWeapons(int32 INDEX);

	UFUNCTION()
	void SpawnWeapon(TSubclassOf<UWeaponMaster> WeaponToSpawn);
	
	UFUNCTION()
	void ADS(float Value);

	UFUNCTION()
	void StartAttack();

	UFUNCTION()
	void StopAttack();


	
};
