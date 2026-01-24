// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CompassBar.h"
#include "Characters/BaseCharacter.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Camera/CameraComponent.h"

void UCompassBar::NativeConstruct()
{
    Super::NativeConstruct();

    if (Points) PointSlot = Cast<UCanvasPanelSlot>(Points->Slot);

    if (Objective) ObjectiveSlot = Cast<UCanvasPanelSlot>(Objective->Slot);

}

void UCompassBar::NativeTick(const FGeometry &Geometry, float InDeltaTime)
{

    Super::NativeTick(Geometry, InDeltaTime);

    if (!CharacterREF || !CharacterREF->Camera || !PointSlot || !ObjectiveSlot) return;
    float CompassX = (CharacterREF->Camera->GetComponentRotation().Yaw * -10.f) - 2700.f;
    PointSlot->SetPosition(FVector2D(CompassX, 0.f));
    UpdateObjectiveMarker();
}

void UCompassBar::UpdateObjectiveMarker()
{
    const FVector CamLoc = CharacterREF->Camera->GetComponentLocation();
    const FVector CamForward = CharacterREF->Camera->GetForwardVector();
    const FVector TargetLoc = CharacterREF->NavPoint;
    FVector DirToTarget = (TargetLoc - CamLoc).GetSafeNormal();
    const float LookDot = FVector::DotProduct(CamForward, DirToTarget);
    const bool bInFront = LookDot > 0.0f;
    const FRotator LookAtRot = DirToTarget.Rotation();
    const FRotator CameraRot = CharacterREF->Camera->GetComponentRotation();
    float NormalizedYaw = FRotator::NormalizeAxis(LookAtRot.Yaw - CameraRot.Yaw);
    const float ScreenX = NormalizedYaw * 10.0f;
    const float ScreenY = bInFront ? -60.0f : 60.0f;
    ObjectiveSlot->SetPosition(FVector2D(ScreenX, ScreenY));
}
