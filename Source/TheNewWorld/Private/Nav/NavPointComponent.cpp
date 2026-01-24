// Fill out your copyright notice in the Description page of Project Settings.


#include "Nav/NavPointComponent.h"
#include "Characters/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UNavPointComponent::UNavPointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UNavPointComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	ABaseCharacter* LocalChar = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(LocalChar) LocalChar->NavPoint = GetOwner()->GetActorLocation();
	

}


// Called every frame
void UNavPointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

