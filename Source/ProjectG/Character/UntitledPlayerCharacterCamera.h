// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "UntitledPlayerCharacterCamera.generated.h"

UCLASS()
class PROJECTG_API UUntitledPlayerCharacterCamera : public UCameraComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUntitledPlayerCharacterCamera();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* LockOnTrace();
	void UnLockTrace();
	bool bHasTarget;
	AActor* Target;
};
