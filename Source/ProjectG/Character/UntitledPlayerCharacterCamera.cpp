// Fill out your copyright notice in the Description page of Project Settings.

#include "UntitledPlayerCharacterCamera.h"

// Need Implimentation
#include "Math/Vector.h"
#include "Math/Quat.h"
#include "Math/Rotator.h"
#include "Kismet/KismetMathLibrary.h"

// Dedug Helper
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UUntitledPlayerCharacterCamera::UUntitledPlayerCharacterCamera()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bHasTarget = false;
	Target = nullptr;
}


// Called when the game starts
void UUntitledPlayerCharacterCamera::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UUntitledPlayerCharacterCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AActor* UUntitledPlayerCharacterCamera::LockOnTrace()
{
	FVector RotationForward = this->GetComponentQuat().GetForwardVector();

	FHitResult Hit;
	FCollisionQueryParams CollisionQuery;

	FVector TraceStart = this->GetComponentLocation();
	FVector TraceEnd = TraceStart + RotationForward * 5000.f;

	FCollisionShape box;
	box.SetBox(FVector(400.0f, 400.0f, 400.0f));

	this->GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, this->GetComponentQuat(),ECollisionChannel::ECC_GameTraceChannel2, box,CollisionQuery);
	
	if (Hit.GetActor() == nullptr)
	{
		return nullptr;
	}
	bHasTarget = true;
	Target = Hit.GetActor();
	
	return Target;
}

void UUntitledPlayerCharacterCamera::UnLockTrace()
{
	bHasTarget = false;
	Target = nullptr;
}