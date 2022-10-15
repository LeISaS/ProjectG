// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DummyBossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTG_API UDummyBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationsProperties)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class ADummyBossTest* Enemy;

	

	
};
