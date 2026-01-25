
// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CSB_Hit.h"

UCSB_Hit::UCSB_Hit()
{
	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Roll.Amplitude = FMath::FloatSelect(FMath::RandBool(), -.7f, .7f);
	RotOscillation.Roll.Frequency = 80.f;
}
