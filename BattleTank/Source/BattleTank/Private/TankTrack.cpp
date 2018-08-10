// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "SprungWheel.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"

UTankTrack::UTankTrack() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::SetThrottle(float Throttle)
{
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -1, 1);
	DriveTrack(CurrentThrottle);
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<ASprungWheel*> Wheels;

	return TArray<ASprungWheel*>();
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
	auto Wheels = GetWheels();
	float ForcePerWheel = ForceApplied / Wheels.Num();

	for (ASprungWheel* Wheel : Wheels) {
		Wheel->AddDrivingForce(ForcePerWheel);
	}
}


