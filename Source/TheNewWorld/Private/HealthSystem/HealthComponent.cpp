// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthSystem/HealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bIsDead = false;

	// ...
}

void UHealthComponent::OnRep_CurrentHealth(float OldHealth)
{

}

void UHealthComponent::OnRep_bIsDead()
{
	if(bIsDead) OnDeath.Broadcast();
}

void UHealthComponent::OnRep_OldDamageInfo()
{
	OnDamageResponse.Broadcast(OldDamageInfo.DamageResponse);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
	DOREPLIFETIME(UHealthComponent, bIsDead);
	DOREPLIFETIME(UHealthComponent, OldDamageInfo);

}

float UHealthComponent::Heal(float Amount)
{
	if (bIsDead) return 0.f;
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);
	return CurrentHealth;
}

bool UHealthComponent::TakeDamage(FDamageInfo DamageInfo)
{
	if(!GetOwner()->HasAuthority() || bIsDead) return false;
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageInfo.Amount, 0.f, MaxHealth);
	OldDamageInfo = DamageInfo;
	if(!GetWorld()->IsNetMode(NM_DedicatedServer)){
		OnRep_OldDamageInfo();
	}
	if(CurrentHealth <= 0.f){
		bIsDead = true;
		OnRep_bIsDead();
	}
	OnRep_CurrentHealth(CurrentHealth + DamageInfo.Amount);
	return true;
}
