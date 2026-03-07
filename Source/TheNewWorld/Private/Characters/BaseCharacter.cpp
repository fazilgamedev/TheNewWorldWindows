// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "WeaponSystem/WeaponMaster.h"
#include "WeaponSystem/WeaponPickup.h"
#include "PickupSystem/PickupMaster.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "PickupSystem/Interfaces/InteractInterface.h"
#include "AnimInstances/ArmsAnimInst.h"
#include "AnimInstances/BodyAnimInst.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Materials/MaterialInstance.h"
#include "Camera/CSB_Fire.h"
#include "Camera/CSB_Hit.h"
#include "HealthSystem/HealthComponent.h"
#include "UI/PlayerHUD.h"
#include "UI/Crosshair.h"
#include "UI/HealthBar.h"
#include "UI/WeaponSlots.h"
#include "Components/ProgressBar.h"
#include "Animation/AnimSequence.h"
#include "Components/TextBlock.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	WeaponFP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponFP"));
	WeaponMagFP = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMagFP"));
	WeaponTP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponTP"));
	WeaponMagTP = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMagTP"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));


	Camera->SetupAttachment(GetRootComponent());
	Camera->SetFieldOfView(120.f);
	Camera->bUsePawnControlRotation = true;

	Materials.Init(nullptr, 4);

	Arms->SetupAttachment(Camera);
	Arms->SetOnlyOwnerSee(true);
	Arms->SetCastShadow(false);

	WeaponFP->SetupAttachment(Arms);
	WeaponFP->SetOnlyOwnerSee(true);
	WeaponFP->SetCastShadow(false);

	WeaponMagFP->SetupAttachment(WeaponFP);
	WeaponMagFP->SetOnlyOwnerSee(true);
	WeaponMagFP->SetCastShadow(false);

	WeaponTP->SetupAttachment(GetMesh());
	WeaponTP->SetOwnerNoSee(true);
	WeaponTP->SetCastHiddenShadow(true);

	WeaponMagTP->SetupAttachment(WeaponTP);
	WeaponMagTP->SetOwnerNoSee(true);
	WeaponMagTP->SetCastHiddenShadow(true);

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetCastHiddenShadow(true);

	Loadout.Weapons.Init(nullptr, 2);
	Loadout.CurrentWeaponINDEX = 0;

	HealthComponent->OnDeath.AddDynamic(this, &ABaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddDynamic(this, &ABaseCharacter::OnHealthChanged);
	HealthComponent->MaxHealth = 100.f;

	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay() 
{
	Super::BeginPlay();

	PCREF = Cast<APlayerController>(GetController());


	if(IsLocallyControlled()){
		if(Materials[0] && Materials[1]){
			Arms->SetMaterial(0, Materials[0]);
			Arms->SetMaterial(1, Materials[1]);
		}
		if (PCREF) {
			if (PCREF->PlayerCameraManager) {
				PCREF->PlayerCameraManager->ViewPitchMin = -85.f;
				PCREF->PlayerCameraManager->ViewPitchMax = 70.f;
			}
			HUDREF = Cast<APlayerHUD>(PCREF->GetHUD());
		}
		ArmsAnimInst = Cast<UArmsAnimInst>(Arms->GetAnimInstance());
	}else{
		if(Materials[2] && Materials[3]){
			GetMesh()->SetMaterial(0, Materials[2]);
			GetMesh()->SetMaterial(1, Materials[3]);
		}
	}

	BodyAnimInst = Cast<UBodyAnimInst>(GetMesh()->GetAnimInstance());

	SwitchUnarmed();

	OnHealthChanged();

	OnAmmoCountChanged();

	OnRep_Loadout();

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsLocallyControlled()){
		float Speed = GetVelocity().Size2D();
		float TargetFreq = 0.f;
		FVector TargetAmp = FVector::ZeroVector;
		if(Speed > 400.f){
			TargetFreq = 13.96f;
			TargetAmp = FVector(2.f, 2.f, 2.f);
		}else if(Speed > 3.f){
			TargetFreq = 10.47f;
			TargetAmp = FVector(.5f, .5f, .5f);
		}else{
			TargetFreq = 3.14;
			TargetAmp = FVector(.12f, .12f, .12f);
		}
		CurrentAmp = FMath::VInterpTo(CurrentAmp, TargetAmp, DeltaTime, 5.f);
		CurrentPhase += DeltaTime * TargetFreq;
		if(CurrentPhase > 6.28f) CurrentPhase -= 6.28f;
		WalkVector = FVector(FMath::Sin(CurrentPhase) * CurrentAmp.X, FMath::Cos(CurrentPhase) * CurrentAmp.Y, FMath::Sin(CurrentPhase * 2.f) * CurrentAmp.Z);
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveFront", this, &ABaseCharacter::MoveFront);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ABaseCharacter::LookRight);
	PlayerInputComponent->BindAxis("ADS", this, &ABaseCharacter::ADS);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABaseCharacter::Interacting);
	PlayerInputComponent->BindAction("Primary", IE_Pressed, this, &ABaseCharacter::SwitchPrimary);
	PlayerInputComponent->BindAction("Secondary", IE_Pressed, this, &ABaseCharacter::SwitchSecondary);
	PlayerInputComponent->BindAction("Unarmed", IE_Pressed, this, &ABaseCharacter::SwitchUnarmed);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABaseCharacter::StartAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ABaseCharacter::StopAttack);
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, Loadout);
	DOREPLIFETIME(ABaseCharacter, bCanAttack);
}

float ABaseCharacter::GetCurrentHealth_Implementation()
{
    return HealthComponent->CurrentHealth;
}

float ABaseCharacter::GetMaxHealth_Implementation()
{
    return HealthComponent->MaxHealth;
}

float ABaseCharacter::Heal_Implementation(float Amount)
{
    return HealthComponent->Heal(Amount);
}

bool ABaseCharacter::TakeDamage_Implementation(FDamageInfo DamageInfo)
{
    return HealthComponent->TakeDamage(DamageInfo);
}

void ABaseCharacter::MoveFront(float Value)
{
	if(Value != 0.f) AddMovementInput(GetActorForwardVector(), Value);
	if(ArmsAnimInst) FrontMove = ArmsAnimInst->AimAlpha > .5f ? Value * .2f : Value;
	else FrontMove = Value;
}

void ABaseCharacter::MoveRight(float Value)
{	
	AddMovementInput(GetActorRightVector(), Value);
	SideMove = Value * 2.f;
}

void ABaseCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * V_Sensitivity);
}

void ABaseCharacter::LookRight(float Value)
{
	AddControllerYawInput(Value * H_Sensitivity);
}

void ABaseCharacter::OnRep_Loadout()
{
	SetCurrentWeaponMesh();
	OnAmmoCountChanged();
	StopAttack();
}

void ABaseCharacter::SR_Interact_Implementation(AActor *Target, ABaseCharacter *Interactor)
{
	IInteractInterface::Execute_Interact(Target, Interactor);
}

void ABaseCharacter::MC_SetWeaponAtINDEX_Implementation(UWeaponMaster *Weapon, int32 INDEX)
{
	if(Loadout.Weapons.IsValidIndex(INDEX)) Loadout.Weapons[INDEX] = Weapon;
	OnRep_Loadout();
}

void ABaseCharacter::SR_SetWeaponAtINDEX_Implementation(UWeaponMaster *Weapon, int32 INDEX)
{
	MC_SetWeaponAtINDEX(Weapon, INDEX);
}

void ABaseCharacter::MC_SwitchWeapons_Implementation(int32 INDEX)
{
	if(Loadout.CurrentWeaponINDEX == INDEX) return;
	Loadout.CurrentWeaponINDEX = INDEX;
	OnRep_Loadout();
	if(HUDREF && HUDREF->CrosshairWidget) HUDREF->CrosshairWidget->PlayOnCrosshairFire(false);
}

void ABaseCharacter::SR_SwitchWeapons_Implementation(int32 INDEX)
{
	MC_SwitchWeapons(INDEX);
}

void ABaseCharacter::Interacting()
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors, APickupMaster::StaticClass());
	if(Actors.Num() > 0 && Actors[0] && Actors[0]->GetClass()->ImplementsInterface(UInteractInterface::StaticClass())){
		StopAttack();
		if(HasAuthority()) 	IInteractInterface::Execute_Interact(Actors[0], this);
		else SR_Interact(Actors[0], this);
	}
}

void ABaseCharacter::SwitchPrimary()
{
	SwitchWeapons(0);
}

void ABaseCharacter::SwitchSecondary()
{
	SwitchWeapons(1);
}

void ABaseCharacter::SwitchUnarmed()
{
	SwitchWeapons(-1);
}

void ABaseCharacter::OnRep_bCanAttack()
{
	if (HUDREF && HUDREF->CrosshairWidget) {
		if (bCanAttack) HUDREF->CrosshairWidget->PlayOnCrosshairFire(true);
		else HUDREF->CrosshairWidget->PlayOnCrosshairFire(false);
	}
}

void ABaseCharacter::Recoil()
{
	AddControllerPitchInput(-GetCurrentWeapon()->Recoil_Vertical);
	AddControllerYawInput(FMath::RandRange(-GetCurrentWeapon()->Recoil_Horizontal_Left, GetCurrentWeapon()->Recoil_Horizontal_Right));
}

void ABaseCharacter::MC_StartFire_Implementation()
{
	if (!GetCurrentWeapon()) return;
	UWeaponMaster* Weapon = GetCurrentWeapon();
	if (Weapon->GetCurrentAmmoCount() <= 0) {
		StopAttack();
		return;
	}
	Weapon->SetCurrentAmmoCount(Weapon->GetCurrentAmmoCount() - 1);
	Weapon->OnRep_CurrentMagCount();
	AActor* HitActor = nullptr;
	FHitResult FireHitResult;

	FVector S = Camera->GetComponentLocation();
	FVector E = S + GetBaseAimRotation().Vector() * Weapon->Range;
	FCollisionQueryParams P;
	P.AddIgnoredActor(this);
	P.bTraceComplex = true;
	if (GetWorld()->LineTraceSingleByChannel(FireHitResult, S, E, ECollisionChannel::ECC_Visibility, P)) {
		HitActor = FireHitResult.GetActor();
		if (!HitActor) return;
		if (HitActor->GetClass()->ImplementsInterface(UHealthInterface::StaticClass())) {
			IHealthInterface::Execute_TakeDamage(HitActor, Weapon->DamageInfo);
		}
	}

	if (IsLocallyControlled()) {
		WeaponFP->PlayAnimation(Weapon->FireAnim, false);
		if (ArmsAnimInst) ArmsAnimInst->Firing();
		Recoil();
		if (PCREF) PCREF->ClientStartCameraShake(UCSB_Fire::StaticClass(), 1.3f);
		if (HitActor && HitActor->ActorHasTag(TEXT("Player")) && HUDREF && HUDREF->CrosshairWidget) {
			HUDREF->CrosshairWidget->PlayOnHitMarker();
		}
	}
	else WeaponTP->PlayAnimation(Weapon->FireAnim, false);
	if (!HitActor) return;
	FRotator HitRot = FireHitResult.ImpactNormal.Rotation();
	FVector HitLoc = FireHitResult.Location;
	if (HitActor->ActorHasTag(TEXT("Stone"))) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetCurrentWeapon()->EFX[0], FTransform(HitRot, HitLoc));
	else if (HitActor->ActorHasTag(TEXT("Metal"))) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetCurrentWeapon()->EFX[1], FTransform(HitRot, HitLoc));
	else if (HitActor->ActorHasTag(TEXT("Wood"))) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetCurrentWeapon()->EFX[2], FTransform(HitRot, HitLoc));
	else if (HitActor->ActorHasTag(TEXT("Player"))) { if (IsLocallyControlled()) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetCurrentWeapon()->EFX[3], FTransform(HitRot, HitLoc)); }
	else UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetCurrentWeapon()->EFX[0], FTransform(HitRot, HitLoc));
}

void ABaseCharacter::SR_StartFire_Implementation()
{
	MC_StartFire();
}

void ABaseCharacter::MC_StopAttack_Implementation()
{
	if (!bCanAttack) return;
	bCanAttack = false;
	OnRep_bCanAttack();
	GetWorldTimerManager().ClearTimer(AttackHandle);
	AttackHandle.Invalidate();
}

void ABaseCharacter::SR_StopAttack_Implementation()
{
	MC_StopAttack();
}

void ABaseCharacter::MC_OnAmmoCountChanged_Implementation()
{
	if (HUDREF && HUDREF->WeaponSlotsWidget) HUDREF->WeaponSlotsWidget->UpdateAmmoCounter(GetCurrentWeapon() ? FString::FromInt(GetCurrentWeapon()->GetCurrentAmmoCount()) : TEXT(" - / - "));
}

void ABaseCharacter::SR_OnAmmoCountChanged_Implementation()
{
	MC_OnAmmoCountChanged();
}

void ABaseCharacter::OnDeath()
{
	GetMesh()->PlayAnimation(DeathAnim, false);
}

void ABaseCharacter::MC_OnHealthChanged_Implementation()
{
	if (IsLocallyControlled() && HUDREF && HUDREF->HealthBarWidget) 
		if (GetClass()->ImplementsInterface(UHealthInterface::StaticClass())) {
			HUDREF->HealthBarWidget->HealthBar->SetPercent(IHealthInterface::Execute_GetCurrentHealth(this) / IHealthInterface::Execute_GetMaxHealth(this));
			HUDREF->HealthBarWidget->PlayAnimation(HUDREF->HealthBarWidget->OnHit, 0.f, 1, EUMGSequencePlayMode::PingPong, 1.5f);
			if (PCREF) PCREF->ClientStartCameraShake(UCSB_Hit::StaticClass(), 1.3f);
		}
}

void ABaseCharacter::SR_OnHealthChanged_Implementation()
{
	MC_OnHealthChanged();
}

void ABaseCharacter::OnHealthChanged()
{
	if (HasAuthority()) MC_OnHealthChanged();
	else SR_OnHealthChanged();
}

UWeaponMaster *ABaseCharacter::GetWeaponAtINDEX(int32 INDEX)
{
    return Loadout.Weapons.IsValidIndex(INDEX) ? Loadout.Weapons[INDEX] : nullptr;
}

void ABaseCharacter::SetWeaponAtINDEX(UWeaponMaster *Weapon, int32 INDEX)
{
	if(!Loadout.Weapons.IsValidIndex(INDEX)) return;
	if(HasAuthority()) MC_SetWeaponAtINDEX(Weapon, INDEX);
	else SR_SetWeaponAtINDEX(Weapon, INDEX);
}

UWeaponMaster *ABaseCharacter::GetCurrentWeapon()
{
    return GetWeaponAtINDEX(Loadout.CurrentWeaponINDEX);
}

void ABaseCharacter::SetCurrentWeapon(UWeaponMaster *Weapon)
{
	SetWeaponAtINDEX(Weapon, Loadout.CurrentWeaponINDEX);
}

void ABaseCharacter::SetCurrentWeaponMesh()
{
	if(!WeaponTP || !WeaponFP || !WeaponMagFP || !WeaponMagTP) return;
	UWeaponMaster* Weapon = GetCurrentWeapon();
	if(Weapon && Weapon->WeaponMesh){
		WeaponTP->SetSkeletalMesh(Weapon->WeaponMesh, true);
		WeaponTP->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Weapon->SocketToAttach);
		WeaponFP->SetSkeletalMesh(Weapon->WeaponMesh, true);
		WeaponFP->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale, Weapon->SocketToAttach);		
		if(Weapon->MagazineMesh){
			WeaponMagTP->SetStaticMesh(Weapon->MagazineMesh);
			WeaponMagFP->SetStaticMesh(Weapon->MagazineMesh);
		}else{
			WeaponMagTP->SetStaticMesh(nullptr);
			WeaponMagFP->SetStaticMesh(nullptr);
		}
	}else{
		WeaponTP->SetSkeletalMesh(nullptr);
		WeaponFP->SetSkeletalMesh(nullptr);
		WeaponMagTP->SetStaticMesh(nullptr);
		WeaponMagFP->SetStaticMesh(nullptr);
	}
}

void ABaseCharacter::SwitchWeapons(int32 INDEX)
{
	if(HasAuthority()) MC_SwitchWeapons(INDEX);
	else if(PCREF && IsLocallyControlled()) SR_SwitchWeapons(INDEX);
}

void ABaseCharacter::SpawnWeapon(TSubclassOf<UWeaponMaster> WeaponToSpawn)
{
	if(!WeaponToSpawn) return;
	if(!Loadout.Weapons.IsValidIndex(Loadout.CurrentWeaponINDEX)) SwitchWeapons(0);
	UWeaponMaster* OldWeapon = GetCurrentWeapon();
	if(OldWeapon){
		OldWeapon->OnAmmoCountChanged.RemoveDynamic(this, &ABaseCharacter::OnAmmoCountChanged);
		AWeaponPickup* DroppedPickup = GetWorld()->SpawnActor<AWeaponPickup>(OldWeapon->PickupClass, GetActorLocation() + GetActorForwardVector() * 100.f, FRotator(30.f, 0.f, 0.f));
		if(DroppedPickup) DroppedPickup->CurrentMagCount = OldWeapon->GetCurrentAmmoCount();
		OldWeapon->DestroyComponent();
		SetCurrentWeapon(nullptr);
	}
	UWeaponMaster* NewComp = Cast<UWeaponMaster>(AddComponentByClass(WeaponToSpawn, true, FTransform::Identity, false));
	NewComp->OnAmmoCountChanged.AddDynamic(this, &ABaseCharacter::OnAmmoCountChanged);
	SetCurrentWeapon(NewComp);
}

void ABaseCharacter::ADS(float Value)
{
	if(!GetCurrentWeapon()) return;
	if(IsLocallyControlled() && Value > .5f && HUDREF && HUDREF->CrosshairWidget) HUDREF->CrosshairWidget->PlayOnAim(true);
	else if(HUDREF) HUDREF->CrosshairWidget->PlayOnAim(false);
	if (ArmsAnimInst) {
		ArmsAnimInst->AimAlpha = FMath::FInterpTo(ArmsAnimInst->AimAlpha, Value, GetWorld()->GetDeltaSeconds(), 10.f);
		Camera->SetFieldOfView(FMath::Lerp(120.f, GetCurrentWeapon()->ADSFOV, ArmsAnimInst->AimAlpha));
	}
}

void ABaseCharacter::FireWeapon()
{
	if (HasAuthority()) MC_StartFire();
	else SR_StartFire();
}

void ABaseCharacter::StartAttack()
{
	if (GetCurrentWeapon()) {
		UWeaponMaster* Weapon = GetCurrentWeapon();
		if (Weapon->GetCurrentAmmoCount() <= 0) {
			StopAttack();
			return;
		}
		bCanAttack = true;
		OnRep_bCanAttack();
		FireWeapon();
		GetWorldTimerManager().SetTimer(AttackHandle, this, &ABaseCharacter::FireWeapon, Weapon->FireRate, true);
	}
	else {

	}
}

void ABaseCharacter::StopAttack()
{
	if (HasAuthority()) MC_StopAttack();
	else SR_StopAttack();
}

void ABaseCharacter::OnAmmoCountChanged()
{
	if (HasAuthority()) MC_OnAmmoCountChanged();
	else SR_OnAmmoCountChanged();
}