// Copyright PixelSpawn 2018

#include "SpawnPoint.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
USpawnPoint::USpawnPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void USpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	auto NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnClass, GetComponentTransform()); // spawns actor but does not run Begin play or construction blueprints

	if (!NewActor) { return; }
	NewActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

	UGameplayStatics::FinishSpawningActor(NewActor, GetComponentTransform()); // Finalizes spawn actor, running Begin Play etc
}

// Called every frame
void USpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

