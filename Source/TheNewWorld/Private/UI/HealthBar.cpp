// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthBar.h"
#include "Characters/BaseCharacter.h"
#include "HealthSystem/HealthComponent.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(OnHit, 0.f, 1, EUMGSequencePlayMode::PingPong, 5.f);

}

void UHealthBar::NativeTick(const FGeometry& Geometry, float InDeltaTime)
{
	Super::NativeTick(Geometry, InDeltaTime);
}

float UHealthBar::UpdateHealth()
{
	if (!CharacterREF->IsLocallyControlled()) return 0.f;
	if (!CharacterREF->HealthComponent) return 0.f;
	if (CharacterREF->HealthComponent->bIsDead) return 0.f;
	return CharacterREF->HealthComponent->CurrentHealth / CharacterREF->HealthComponent->MaxHealth;
}
