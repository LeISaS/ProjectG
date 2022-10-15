// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_WALK				UMETA(DisplayName = "WALK"),
	EMS_RUN				UMETA(DisplayName = "RUN"),
	EMS_SPRINT			UMETA(DisplayName = "SPRINT"),
	EMS_NOSPRINT	UMETA(DisplayName = "NOSPRINT"),

	EMS_MAX				UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_NORMAL									UMETA(DisplayName = "Normal"),
	ESS_BELOWMINIMUM						UMETA(DisplayName = "BelowMinimum"),
	ESS_EXHAUSTED								UMETA(DisplayName = "Exhausted"),
	ESS_EXHAUSTEDRECOVERING		UMETA(DisplayName = "ExhaustedRecovering"),

	ESS_MAX											UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EPlayerStatus :uint8
{
	EPS_NORMAL					UMETA(DisplayName = "Normal"),
	EPS_ATTACK						UMETA(DisplayName = "Attack"),
	EPS_ROLL							UMETA(DisplayName = "Roll"),
	EPS_CHARGING				UMETA(DisplayName = "Charging"),
	EPS_KNOCKBACK				UMETA(DisplayName = "KnockBack"),
	EPS_DEAD							UMETA(DisplayName = "Dead"),
	EPS_GRAP							UMETA(DisplayName = "Grap"),

	EPS_MAX							UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class PROJECTG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	/********************Camera*****************/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UUntitledPlayerCharacterCamera* FollowCamera;

	//나중에 수정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;


	/********************************Enum*****************************/

	//Movement 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Enums")
		EMovementStatus MovementStatus;

	void SetMovementStatus(EMovementStatus Status);

	//Stamina

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Enums")
		EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	//PlayerStatus

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Enums")
		EPlayerStatus PlayerStatus;

	FORCEINLINE void SetPlayerStatus(EPlayerStatus Status) { PlayerStatus = Status; }

	/************************Player Speed Status*********************/


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStatus | Speed")
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStatus | Speed")
		float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStatus | Speed")
		float SprintSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStatus | Speed")
		float RollSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStatus | Speed")
		float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStatus | Speed")
		float DeadSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStatus | Speed")
		float KnockbackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStatus | InterpSpeed")
		float InterpKnockBackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStatus | InterpSpeed")
		float InterpRollingSpeed;


	/************************Player  Status*********************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Status")
		float MaxHealthPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Status")
		float HealthPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Status")
		float MaxStaminaPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Status")
		float StaminaPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Status")
		float DefaultStaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Status")
		float StaminaRecovery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Status")
		float SprintStaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Status")
		float MinActionStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Status")
		float RollStaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Status")
		float NormalAttackStaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Status")
		float ChargeReadyStaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Status")
		float ChargingStaminaCost;

	int ComboCount;



	/************************Player  Status Damage*********************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Damage")
		float NormalAttackADamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Damage")
		float NormalAttackBDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Damage")
		float ChargeAttackNDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Damage")
		float ChargeAttackADamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Damage")
		float ChargeAttackBDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | HitStopTimer")
		float NormalAttackAHitStopTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | HitStopTimer")
		float NormalAttackBHitStopTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | HitStopTimer")
		float ChargeAttackNHitStopTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | HitStopTimer")
		float ChargeAttackAHitStopTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | HitStopTimer")
		float ChargeAttackBHitStopTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | HitStopTimer")
		float DefaultHitStop;

	/************************Player  Stamina Timer*********************/
	FTimerHandle TimerHandler;
	FTimerHandle XboxTimerHandler;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | StaminaTimer")
		float StaminaNoActionTimer;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | StaminaTimer")
	bool bStaminaNoAction;

	void StaminaNoActionBoolCheck();

	void StaminaNoAction();

	/************************************************************/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
		class UAnimMontage* CombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
		class UAnimMontage* PotionMontage;

	bool CanMove(float value);

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
		class AWeapon* Weapon;


	/**********************Target Setting*******************************/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		bool bHasCombatTarget;

	FORCEINLINE void SetHasCombatTarget(bool bHasTarget) { bHasCombatTarget = bHasTarget; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		class ADummyBossTest* CombatTarget;

	FORCEINLINE void SetCombatTarget(ADummyBossTest* Target) { CombatTarget = Target; }



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat | Normal")
	class ANormalMonster* NormalMonster;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat | Normal")
	bool bHasNormalCombatTarget;

	FORCEINLINE void SetHasNormalCombatTarget(bool bHasNormalTarget) { bHasNormalCombatTarget = bHasNormalTarget; }
	
	FORCEINLINE void SetNormalCombatTarget(ANormalMonster* Target) { NormalMonster = Target; }
	
	
	
	


	/***********************HUD**********************************/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class AUntitledPlayerController* UntitledPlayerController;

	void UpdateHUD(bool SetHUD);

	/************************Potion Item************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Potion")
		int32 Potions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Potion")
		int32 MaxPotions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | Potion")
		float HealthAmount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/*플레이어 스탯 (Hp, Stamina)**/
	void StaminaDecrease(float SprintStaminaCostDelta, float StaminaRecoveryDelta);
	void DecrementHealth(float Amount);

	void IncrementHealth();
	void IncrementHealthCount(int32 Amount);

	/**플레이어 키보드 움직임 ( W A S D ) */
	void MoveForward(float value);
	void MoveRight(float value);
	void TurnAtRate(float Rate);
	void LookupAtRate(float Rate);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		bool bMoveForward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		bool bMoveRight;

	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "bool")
	bool bInGameManual;
	
	void DisplayInGameManual();

	/*플레이어 Run (Left Shift Key)*/
	void ShiftKeyDown();
	void ShiftKeyUp();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		bool bShiftKeyDown;


	/*플레이어 WalkToggle(C Key)*/
	void CkeyDown();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		bool bCkeyDown;

	/**플레이어 LMB (LeftMouseButton)*/
	void LMBDown();
	void LMBUp();

	void Attack();
	UFUNCTION(BlueprintCallable)
		void AttackEnd();
	UFUNCTION(BlueprintCallable)
		void AttackInputChecking();

	UFUNCTION(BlueprintCallable)
		void AttackCancel();

	UFUNCTION(BlueprintCallable)
		void KnockBackCancel();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		bool bLMBDown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		bool bAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		bool bAttackButtonCheck;

	/**플레이어 Roll (SpaceKey)*/
	void SpaceKeyDown();

	void XboxSpaceKeyDown();
	void SpaceKeyUp();

	void RollStart();
	UFUNCTION(BlueprintCallable)
		void RollEnd();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		bool bSpaceKeyDown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		bool bRolling;

	UFUNCTION(BlueprintCallable)
		void RollCancel();

	UFUNCTION(BlueprintCallable)
		void RollinginvincibilityStart();
	UFUNCTION(BlueprintCallable)
		void RollinginvincibilityEnd();

	FRotator RollMovementInput();

	FRotator RInterpMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		bool binvincibility;


	float PlayHighPriorityMontage(UAnimMontage* Montage, FName StartSectionName, float InPlayRate);

	float RollDeltaTime;

	/*플레이어 Dead (No HP)*/
	void Dead();
	UFUNCTION(BlueprintCallable)
	void DeadEnd();

	bool bDead;
	

	UFUNCTION(BlueprintNativeEvent)
	void DeathNative();

	UFUNCTION(BlueprintNativeEvent)
	void ChapterOneClearUI();

	/*플레이어 Target Lock On (R Key)*/
	void RKeyDown();

	AActor* LockOnTarget;
	bool bHasLockOnTarget;

	/*플레이어 Charging(RMB)*/
	void RMBDown();
	void RMBUp();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		bool bRMBDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "bool")
		bool bCharinghit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		bool bCharging;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "bool")
		bool bChargingOne;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "bool")
		bool bChargingTwo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "bool")
		bool bChargingCheck;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerStatus | Timer")
		float ChargingTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStatus | Timer")
		float FirstChargeTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStatus | Timer")
		float SecondChargeTime;



	float ChargingComboMax;

	void ChargingAttack(float Value);

	UFUNCTION(BlueprintCallable)
	void ChargingEnd();


	UFUNCTION(BlueprintCallable)
	void ChargingCancelStart();

	UFUNCTION(BlueprintCallable)
	void ChargingCancelEnd();


	bool KeyInputCheck();

	/********************KnockBack*********************/
	void KnockBackStart();

	UFUNCTION(BlueprintCallable)
	void KnockBackEnd();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
	bool bKnockBackStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | KnockBackLevel")
	int32 KnockBackNormalAttackA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | KnockBackLevel")
	int32 KnockBackNormalAttackB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | KnockBackLevel")
	int32 KnockBackChargeAttackN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | KnockBackLevel")
	int32 KnockBackChargeAttackA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus | KnockBackLevel")
	int32 KnockBackChargeAttackB;


	FRotator GetLookAtRotationYaw(FVector Target);

	UFUNCTION(BlueprintCallable)
	void GrapStart();

	UFUNCTION(BlueprintCallable)
	void GrapEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "bool")
	bool bGraping;

	/*********************Potion********************/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		bool bPotionClickDown;

	void PotionClickDown();
	void PotionClickUp();
	/*********************other********************/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
		float InterpRotationSpeed;
	FRotator KeyboardInputRotation();

	FRotator SmoothPlayerRotator;

	float SmoothRotatorTimer;

	
	void UpdateCombatTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<ANormalMonster> EnemyFilter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bool")
	float bXboxCheck;

	void XboxTimerCheck();
};