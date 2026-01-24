// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Characters/BaseCharacter.h"
#include "UI/Crosshair.h"
#include "UI/CompassBar.h"
#include "UI/HealthBar.h"

void APlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    CharacterREF = Cast<ABaseCharacter>(GetOwningPawn());

    if(!CharacterREF) return;

    CrosshairWidget = CreateWidget<UCrosshair>(GetWorld(), CrosshairWidgetClass);
    CrosshairWidget->CharacterREF = CharacterREF;
    CrosshairWidget->AddToViewport();

    CompassBarWidget = CreateWidget<UCompassBar>(GetWorld(), CompassBarWidgetClass);
    CompassBarWidget->CharacterREF = CharacterREF;
    CompassBarWidget->AddToViewport();

    HealthBarWidget = CreateWidget<UHealthBar>(GetWorld(), HealthBarWidgetClass);
    HealthBarWidget->CharacterREF = CharacterREF;
    HealthBarWidget->AddToViewport();

}