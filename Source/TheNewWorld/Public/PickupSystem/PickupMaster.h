// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSystem/Interfaces/InteractInterface.h"
#include "PickupMaster.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class THENEWWORLD_API APickupMaster : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupMaster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UStaticMeshComponent* PickupModel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	USphereComponent* PickupRadius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(class ABaseCharacter* Interactor) override;
	
	
};
