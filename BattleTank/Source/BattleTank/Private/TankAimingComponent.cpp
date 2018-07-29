// Copyright PixelSpawn 2018

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
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
	if (!ensure(BarrelToSet && TurretToSet)) { return; }
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
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

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel && ProjectileBlueprint)) { return; }
	bool isReloaded = ((GetWorld()->GetTimeSeconds() - LastFireTime) > ReloadTimeInSeconds);
	if (isReloaded) {

		// Spawn a projectile at the socket location of the barrel
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint,
															  Barrel->GetSocketLocation(FName("Projectile")),
															  Barrel->GetSocketRotation(FName("Projectile"))
															  );
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = GetWorld()->GetTimeSeconds();
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; }

	// Work out difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}