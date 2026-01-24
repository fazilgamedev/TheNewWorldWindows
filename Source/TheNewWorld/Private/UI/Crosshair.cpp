// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Crosshair.h"
#include "Animation/WidgetAnimation.h"
#include "Characters/BaseCharacter.h"

void UCrosshair::NativeConstruct() 
{
	Super::NativeConstruct();

	PlayAnimation(HitMarker, 0.f, 1, EUMGSequencePlayMode::PingPong, 2.f);

}

void UCrosshair::NativeTick(const FGeometry & Geometry, float InDeltaTime)
{

	Super::NativeTick(Geometry, InDeltaTime);

}

void UCrosshair::PlayOnCrosshairFire(bool bCanAttack)
{
	if(CharacterREF->GetCurrentWeapon()) { 
		if(bCanAttack) PlayAnimation(CrosshairFire, 0.f, 1, EUMGSequencePlayMode::Forward, 2.f);
		else PlayAnimation(CrosshairFire, 0.f, 1, EUMGSequencePlayMode::Reverse, 2.f);
	}
}

void UCrosshair::PlayOnHitMarker()
{
	PlayAnimation(HitMarker, 0.f, 1, EUMGSequencePlayMode::PingPong, 3.f);
}

void UCrosshair::PlayOnAim(bool bCanAim)
{
	if(!bCanAim) PlayAnimation(OnAim, 0.f, 1, EUMGSequencePlayMode::Forward, 3.f);
	else PlayAnimation(OnAim, 0.f, 1, EUMGSequencePlayMode::Reverse, 3.f);
}