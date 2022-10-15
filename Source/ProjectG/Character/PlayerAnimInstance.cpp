// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.h"
void UPlayerAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			PlayerCharacter = Cast<APlayerCharacter>(Pawn);
		}
	}
}

void UPlayerAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FRotator CharacterRotation = Pawn->GetActorRotation();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		
		MovementSpeed = LateralSpeed.Size();

		Direction = CalculateDirection(Speed, CharacterRotation);
		

		bAir = Pawn->GetMovementComponent()->IsFalling();

		if (PlayerCharacter == nullptr)
		{
			PlayerCharacter = Cast<APlayerCharacter>(Pawn);
		}
	}
}