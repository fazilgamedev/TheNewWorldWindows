// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CompassBar.generated.h"

class ABaseCharacter;
class UImage;
class UCanvasPanel;
class UCanvasPanelSlot;

/**
 * 
 */
UCLASS()
class THENEWWORLD_API UCompassBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& Geometry, float InDeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	ABaseCharacter* CharacterREF;

	UPROPERTY(meta = (BindWidget))
    UCanvasPanel* InnerCanvas;

    UPROPERTY(meta = (BindWidget))
    UImage* Points;

    UPROPERTY(meta = (BindWidget))
    UImage* Marker;

    UPROPERTY(meta = (BindWidget))
    UImage* Objective;

private:

	UPROPERTY()
    UCanvasPanelSlot* PointSlot;

    UPROPERTY()
    UCanvasPanelSlot* ObjectiveSlot;

	UFUNCTION()
    void UpdateObjectiveMarker();
	
	
};
