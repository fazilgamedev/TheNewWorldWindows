// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/WeaponMaster.h"
#include "WeaponSystem/WeaponPickup.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Animation/AnimSequence.h"


// Sets default values for this component's properties
UWeaponMaster::UWeaponMaster()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	SetIsReplicatedByDefault(true);

	WeaponMesh = nullptr;
	MagazineMesh = nullptr;
	PickupClass = AWeaponPickup::StaticClass();
	SocketToAttach = TEXT("");
	FireAnim = nullptr;
	EFX.Init(nullptr, 4);
	WeaponName = EWeaponName::None;
	
}


// Called when the game starts
void UWeaponMaster::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeaponMaster::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

