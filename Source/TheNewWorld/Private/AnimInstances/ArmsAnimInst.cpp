// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/ArmsAnimInst.h"
#include "Characters/BaseCharacter.h"
#include "WeaponSystem/WeaponMaster.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UArmsAnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CharacterREF = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (!CharacterREF) return;
	else OldRotation = CharacterREF->GetControlRotation();
}

void UArmsAnimInst::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

    if(!CharacterREF) CharacterREF = Cast<ABaseCharacter>(TryGetPawnOwner());
    if (!CharacterREF) return;

	Speed = CharacterREF->GetVelocity().Size2D();

	Direction = CalculateDirection(CharacterREF->GetVelocity(), CharacterREF->GetActorRotation());

	// float P = CharacterREF->GetControlRotation().Pitch;

	// Pitch = P > 180.f ? 360 - P : -P;

	CurrentWeapon = CharacterREF->GetCurrentWeapon();

	DirectionalRotation(SideMove, CharacterREF->SideMove, DeltaTime, 1.f, 6.f, 11.f);
	DirectionalRotation(FrontMove, CharacterREF->FrontMove, DeltaTime, 1.f, 4.f, 10.f);

	WalkAnim(DeltaTime);

	HandSway(DeltaTime);


	if (CurrentWeapon) {

		LHIKAlpha = 1.f;

		WeaponName = CurrentWeapon->WeaponName;

		SetSightTransform();

		SetRelativeHandTransform();

		SetLHIKTransform();

		if (!RecoilTransform.Equals(FTransform::Identity) || !RecoilResetTransform.Equals(FTransform::Identity)) {

			InterpResetRecoil(DeltaTime);

			InterpRecoil(DeltaTime);

		}
	}
	else {

		LHIKAlpha = 0.f;
		AimAlpha = 0.f;
		WeaponName = EWeaponName::None;
	}
}

void UArmsAnimInst::SetSightTransform()
{
	FTransform Temp = CharacterREF->Camera->GetComponentTransform().GetRelativeTransform(CharacterREF->Arms->GetComponentTransform()/*GetSocketTransform(TEXT("head"), ERelativeTransformSpace::RTS_World)*/);
	SightTransform = FTransform(Temp.GetRotation(), Temp.GetRotation().GetForwardVector() * CurrentWeapon->AimOffset + Temp.GetLocation()); // If dynamic offset of sight is needed take the comments off but make sure to lower the veritcal sensitivity cause if you don't you might see clipping 
}

void UArmsAnimInst::SetRelativeHandTransform()
{
	RelativeHandTransform = CharacterREF->WeaponFP->GetSocketTransform(TEXT("Aim"), ERelativeTransformSpace::RTS_World).GetRelativeTransform(CharacterREF->Arms->GetSocketTransform(TEXT("hand_r"), ERelativeTransformSpace::RTS_World));
}

void UArmsAnimInst::SetLHIKTransform()
{
	LHIKTransform = CharacterREF->WeaponFP->GetSocketTransform(TEXT("FP_LHIK"), ERelativeTransformSpace::RTS_World).GetRelativeTransform(CharacterREF->Arms->GetSocketTransform(TEXT("hand_r"), ERelativeTransformSpace::RTS_World));
}

void UArmsAnimInst::WalkAnim(float DeltaTime)
{
	SwayVector = FMath::VInterpTo(SwayVector, CharacterREF->WalkVector, DeltaTime, 10.f);
}

void UArmsAnimInst::HandSway(float DeltaTime)
{
	FRotator CurrentRotation = CharacterREF->GetControlRotation();
	// Clamp if needed
	TurnRotation = FMath::RInterpTo(TurnRotation, CurrentRotation - OldRotation, DeltaTime, 10.f);
	TurnRotation.Roll = TurnRotation.Pitch * -1.f;
	TurnRotation.Yaw = FMath::Clamp(TurnRotation.Yaw, -9.f, 9.f);
	TurnRotation.Roll = FMath::Clamp(TurnRotation.Roll, -9.f, 9.f);
	TurnLocation.X = TurnRotation.Yaw / 18.f;
	TurnLocation.Z = TurnRotation.Roll / 18.f;
	OldRotation = CurrentRotation;
}

void UArmsAnimInst::DirectionalRotation(float & DirVar, float MoveVar, float DeltaTime, float U, float V, float W)
{
	DirVar = FMath::FInterpTo(DirVar, UKismetMathLibrary::MapRangeClamped(MoveVar, -U, U, -V, V), DeltaTime, W);
}

void UArmsAnimInst::InterpRecoil(float DeltaTime)
{
	RecoilResetTransform = UKismetMathLibrary::TInterpTo(RecoilResetTransform, FTransform(), DeltaTime, 10.f);
}

void UArmsAnimInst::InterpResetRecoil(float DeltaTime)
{
	RecoilTransform = UKismetMathLibrary::TInterpTo(RecoilTransform, RecoilResetTransform, DeltaTime, 10.f);
}

void UArmsAnimInst::Firing()
{
	FVector RecoilLocation = RecoilResetTransform.GetLocation();
	RecoilLocation += FVector(FMath::RandRange(CurrentWeapon->RightRecoil * -1.f, CurrentWeapon->RightRecoil), FMath::RandRange(CurrentWeapon->FrontRecoil * -1.f - 1.f, CurrentWeapon->FrontRecoil * -1.f), FMath::RandRange(CurrentWeapon->UpRecoil * -1.f, CurrentWeapon->UpRecoil));
	FRotator RocoilRotation = RecoilResetTransform.GetRotation().Rotator();
	RocoilRotation += FRotator(FMath::RandRange(CurrentWeapon->HorizontalRecoil * -1.f, CurrentWeapon->HorizontalRecoil), FMath::RandRange(CurrentWeapon->SideRecoil * -1.f, CurrentWeapon->SideRecoil), FMath::RandRange(CurrentWeapon->VerticalRecoil * -1.f - 1.f, CurrentWeapon->VerticalRecoil * -1.f));
	RecoilResetTransform.SetRotation(RocoilRotation.Quaternion());
	RecoilResetTransform.SetLocation(RecoilLocation);
}


