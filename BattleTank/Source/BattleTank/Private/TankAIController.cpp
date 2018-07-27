// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "BattleTank.h"
#include "Tank.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ATank* ControlledTank = Cast<ATank>(GetPawn());
	ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (PlayerTank) {
		// TODO Move towards the player

		// Aim towards the player
		ControlledTank->AimAt(PlayerTank->GetActorLocation());

		//ControlledTank->Fire();
	}
}