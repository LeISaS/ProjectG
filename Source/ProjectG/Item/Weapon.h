// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECTG_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Combat")
	class UBoxComponent* CombatCollision;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	float AttackPoint;

	class APlayerCharacter* Player;

protected:
	virtual void BeginPlay() override;

public:
	//Combat
	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Damage - ApplyDamage TODO
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	AController* WeaponInstigator;

	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }

	//노티파이 설정
	UFUNCTION(BlueprintCallable)
	void ActivateCollision();
	UFUNCTION(BlueprintCallable)
	void DeactivateCollsiion();

	bool bBossKnockbackCheck;

	int32 PlayerKnockBackLevel;

	int32 EnemyKnockBackLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "bool")
	bool bChargingHit;

	bool bDamageCheck;

};
