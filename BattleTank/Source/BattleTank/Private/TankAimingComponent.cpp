// Copyright PixelSpawn 2018

#include "TankAimingComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"




// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}




void UTankAimingComponent::Initialise(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	if (!ensure(BarrelToSet && TurretToSet)) {	return;	}
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}


void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!ensure(Barrel)) { return; }

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity // Calculate the OutLaunchVelocity
	( 	
		this,
		OutLaunchVelocity, // Out parameter
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false, // will favour low arc
		0.0f, // CollisionRadius,
		0.0f, // Gravity Override
		ESuggestProjVelocityTraceOption::DoNotTrace,
		FCollisionResponseParams::DefaultResponseParam,
		TArray < AActor * >(), // actors to ignore - empty arry
		false // draw debug
	);
	if (bHaveAimSolution) {
		FVector AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
	else { // If no solution, do nothing
	}
	
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel && Turret)) { return; }
	// Work out difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}

