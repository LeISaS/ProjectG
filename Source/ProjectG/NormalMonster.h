// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NormalMonster.generated.h"

UENUM(BlueprintType)
enum class ENormalMonsterMovementStatus : uint8
{
	EMS_IDLE							UMETA(DeplayName = "Idle"),
	EMS_MOVETOTARGET		UMETA(DeplayName = "MoveToTarget"),
	EMS_ATTACKING				UMETA(DeplayName = "Attacking"),
	EMS_Dead							UMETA(DeplayName = "Dead"),

	EMS_MAX							UMETA(DeplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class ENormalMonsterStatus : uint8
{
	ENMS_AGRO						UMETA(DeplayName = "Agro"),
	ENMS_COMBAT					UMETA(DeplayName = "Combat"),

	ENMS_MAX						UMETA(DeplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class ENormalMonsterKnockBackStatus : uint8
{
	EMKS_IDLE				UMETA(Deplay = "Idle"),
	EMKS_KNOCKBACK	UMETA(Deplay = "KnockBack"),
	EMKS_MAX					UMETA(Deplay = "defaultMax")
};

UCLASS()
class PROJECTG_API ANormalMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANormalMonster();
		

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		ENormalMonsterMovementStatus NormalMonsterMovementStatus;
	
	FORCEINLINE void SetNormalMovementStatus(ENormalMonsterMovementStatus Status) { NormalMonsterMovementStatus = Status; }
	FORCEINLINE ENormalMonsterMovementStatus GetNormalMonsterMovementStatus() { return NormalMonsterMovementStatus; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		ENormalMonsterStatus NormalMonsterStatus;

	FORCEINLINE void SetNormalStatus(ENormalMonsterStatus Status) { NormalMonsterStatus = Status; }
	FORCEINLINE ENormalMonsterStatus GetNormalMonsterStatus() { return NormalMonsterStatus; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		ENormalMonsterKnockBackStatus NormalMonsterKnockBackStatus;

	FORCEINLINE void SetNormalMonsterKnockBackStatus(ENormalMonsterKnockBackStatus Status) { NormalMonsterKnockBackStatus = Status; }
	FORCEINLINE ENormalMonsterKnockBackStatus GetNormalMonsterKnockBackStatus() { return NormalMonsterKnockBackStatus; }


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class USphereComponent* AgroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* CombatSphere;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI | State")
	float Health;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI | State")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | State")
	float Damage;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Combat")
	class UBoxComponent* CombatCollision;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI | Combat")
	class UAnimMontage* CombatMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class APlayerCharacter* PlayerCombatTarget;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bOverlappingCombatSphere;

	//*************************bool***************************//
	bool bAttacking;

	bool bHasValidTarget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	void MoveToTarget(class APlayerCharacter* Target);


	//OtherFunction
	bool Alive();
	

	//Niagara
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		class UNiagaraSystem* NiagarahitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		FRotator NiagarahitEffectRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		FVector NiagarahitEffectScale;

	//effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		FVector HitparticlesScales;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<UDamageType> DamageTypeClass;

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd(AActor* OtherActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AttackMinTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AttackMaxTime;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Die(AActor* Causer);

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	FTimerHandle DeathTimer;
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float DeathDelay;
	void Disappear();

	UFUNCTION(BlueprintCallable)
	void AttackInterpStart();

	UFUNCTION(BlueprintCallable)
	void AttackInterpEnd();

	bool bAttackInterp;

	float InterpSpeed;

	FVector GetPlayerLocation();

	float PlayHighPriorityMontage(UAnimMontage* Montage, FName StartSectionName, float InPlayRate);


	bool bKnockBackStart;

	void KnockBackStart();

	UFUNCTION(BlueprintCallable)
	void KnocKBackEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 DefaultKnockBackLevel;
	
	bool bDamageCheck;



};
