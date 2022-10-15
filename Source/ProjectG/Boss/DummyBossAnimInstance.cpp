// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyBossAnimInstance.h"
#include "DummyBossTest.h"

void UDummyBossAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Enemy = Cast<ADummyBossTest>(Pawn);
		}
	}
}

void UDummyBossAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		
		if (Pawn)
		{
			Enemy = Cast<ADummyBossTest>(Pawn);
		}
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
	}
}
