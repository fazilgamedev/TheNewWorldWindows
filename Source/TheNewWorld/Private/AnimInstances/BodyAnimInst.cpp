// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/BodyAnimInst.h"
#include "Characters/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UBodyAnimInst::UBodyAnimInst()
{
}

void UBodyAnimInst::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    CharacterREF = Cast<ABaseCharacter>(TryGetPawnOwner());
    if(!CharacterREF) return;
}

void UBodyAnimInst::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if(!CharacterREF) CharacterREF = Cast<ABaseCharacter>(TryGetPawnOwner());
    if(!CharacterREF) return;

    Speed = CharacterREF->GetVelocity().Size2D();

    Direction = CalculateDirection(CharacterREF->GetVelocity(), CharacterREF->GetActorRotation());

	float P = CharacterREF->GetBaseAimRotation().Pitch;

	Pitch = P > 180.f ? P - 360.f : P;

    WallAlpha = FMath::FInterpTo(WallAlpha, CharacterREF->WallAlpha, DeltaTime, 10.f);

    CurrentWeapon = CharacterREF->GetCurrentWeapon();
    if(!CurrentWeapon) return;

    LHIK = UKismetMathLibrary::TInterpTo(LHIK, CharacterREF->WeaponTP->GetSocketTransform(TEXT("TP_LHIK")).GetRelativeTransform(CharacterREF->GetMesh()->GetSocketTransform(TEXT("hand_r"))), DeltaTime, 10.f);

}
