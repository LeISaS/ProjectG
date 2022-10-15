// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalMonsterAnimInstance.h"
#include "NormalMonster.h"


void UNormalMonsterAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			NormalMonster = Cast<ANormalMonster>(Pawn);
		}
	}

}


void UNormalMonsterAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			NormalMonster = Cast<ANormalMonster>(Pawn);
		}
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

	}

}
