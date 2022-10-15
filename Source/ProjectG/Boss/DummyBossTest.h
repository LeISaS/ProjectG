// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DummyBossTest.generated.h"

UENUM(BlueprintType)
enum class EDummyBossPhaseStatus : uint8
{
	EDBPS_ONEPHASE			UMETA(DeplayName = "OnePhase"),
	EDBPS_TWOPHASE			UMETA(DeplayName = "TwoPhase")
};


UENUM(BlueprintType)
enum class EDummyBossMovementStatus :uint8
{
	EDBM_IDLE					UMETA(DeplayName = "Idle"),
	EDBM_ATTACK				UMETA(DeplayName = "Attacking"),
	EDBM_MOVETARGET	UMETA(DeplayName = "MoveTarget"),

	EDBM_MAX					UMETA(DeplayName = "DefaultMAX")
};


UENUM(BlueprintType)
enum class EDummyBossStatus : uint8
{
	EDBS_NEAR					UMETA(DeplayName = "Near"),
	EDBS_COMMON			UMETA(DeplayName = "Common"),
	EDBS_APPCOMMON		UMETA(DeplayName = "AppCommon"),
	EDBS_FAR						UMETA(DeplayName = "Far"),
	EDBS_APPFAR				UMETA(DeplayName = "AppFar"),

	EDBS_DEAD					UMETA(DeplayName = "Dead"),
	EDBS_MAX						UMETA(DeplayName = "DefaultMax")
};
UENUM(BlueprintType)
enum class EDummyBossKnockBackState :uint8
{
	EKBS_IDLE					UMETA(DeplayName = "Idle"),
	EKBS_KNOCKBACK	UMETA(DeplayName = "Knockback"),
	EKBS_AIRKNOCKBACK	UMETA(DeplayName = "AirKnockBack"),

	EKBS_MAX							UMETA(DeplayName = "DefaultMax")
};



UCLASS()
class PROJECTG_API ADummyBossTest : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADummyBossTest();
	/**********Enum *************/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enum")
	EDummyBossMovementStatus DummyBossMovementStatus;

	FORCEINLINE void SetDummyBossMovementStatus(EDummyBossMovementStatus Status) { DummyBossMovementStatus = Status; }
	FORCEINLINE EDummyBossMovementStatus GetDummyBossMovementStatus() { return DummyBossMovementStatus; }

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Enum")
	EDummyBossStatus DummyBossStatus;

	FORCEINLINE void SetDummyBossStatus(EDummyBossStatus Status) { DummyBossStatus = Status; }
	FORCEINLINE EDummyBossStatus GetDummyBossStatus() { return DummyBossStatus; }

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Enum")
	EDummyBossPhaseStatus DummyBossPhaseStatus;

	FORCEINLINE void SetDummyBossPhaseStatus(EDummyBossPhaseStatus Status) { DummyBossPhaseStatus = Status; }
	FORCEINLINE EDummyBossPhaseStatus GetDummyBossPhaseStatus() { return DummyBossPhaseStatus; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enum")
	EDummyBossKnockBackState DummyBossKnockBackState;

	FORCEINLINE void SetDummyBossKnockBackState(EDummyBossKnockBackState Status) { DummyBossKnockBackState = Status; }
	FORCEINLINE EDummyBossKnockBackState GetDummyBossKnockBackState() { return DummyBossKnockBackState; }


	/**Attack Range Sphere**/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Al")
	 class USphereComponent* NearSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Al")
	USphereComponent* CommonSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Al")
	USphereComponent* ApproachCommonSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Al")
	USphereComponent* FarSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Al")
	USphereComponent* ApproachFarSphere;

	/**************Status********************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Damage")
	float DefaultDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Damage")
	float N_ATK1_1Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Damage")
	float C_ATK1_1Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Damage")
	float C_ATK1_2Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Damage")
	float C_ATK1_3Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Damage")
	float C_ATK2_1Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Damage")
	float C_ATK2_2Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Damage")
	float F_ATK1_1Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Damage")
	float F_ATK1_2Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Damage")
	float TwoPhase_1Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Damage")
	float TwoPhase_2Damage;


	int32 TwoPhaseCount;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float TwoPhaseHealthPercent;

	float TwoPhasePercent;

	/**********************AtkSpeed***************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Speed")
	float JumpDownSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Speed")
	float NearATKOneSpeed1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Speed")
	float NearATKOneSpeed2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Speed")
	float CommonATKOneSpeed1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Speed")
	float CommonATKOneSpeed2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Speed")
	float CommonATKOneSpeed3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Speed")
	float FarATKOneSpeed1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Speed")
	float FarATKOneSpeed2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Speed")
	float InterpSpeed ;
	/****************Timer**********************/

	FTimerHandle TimerHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Time")
	float ApproachAttackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Time")
	float FarAttackTime;

	/****************KnockBackLevel**********************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 DefaultKnockBackLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 N_ATK1_1KnockBackLevel;	//잡기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 N_ATK1_2KnockBackLevel;	//백스핀 블로우


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 C_ATK1_1KnockBackLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 C_ATK1_1_1KnockBackLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 C_ATK1_2KnockBackLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 C_ATK1_3KnockBackLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 C_ATK1_3_1KnockBackLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 F_ATK1_1KnockBackLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 F_ATK1_2KnockBackLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 PhaseTrigger_1KnockBackLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 PhaseTrigger_2KnockBackLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 C_ATK2_1KnockBackLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 C_ATK2_2KnockBackLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | KnockBackLevel")
	int32 C_ATK2_2_1KnockBackLevel;





	//Notify 넉백 레벨 전환 추가//

	/*****************bool*********************/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status | bool")
	bool bHasValidTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status | bool")
	bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status | bool")
	bool bApproachFarInterp;

	/*******************Combat ***********************/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	USphereComponent* CombatCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		USphereComponent* GrapCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class UBoxComponent* JumpCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		USphereComponent* TwoPhaseCollision;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class AUntitledPlayerController* UntitledPlayerController;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	UFUNCTION(BlueprintCallable)
	void GrapActivateCollision();

	UFUNCTION(BlueprintCallable)
	void GrapDeactivateCollision();
	
	UFUNCTION(BlueprintCallable)
	void JumpActivateCollision();

	UFUNCTION(BlueprintCallable)
	void JumpDeActivateCollision();

	UFUNCTION(BlueprintCallable)
	void TwoPhaseActivateCollision();

	UFUNCTION(BlueprintCallable)
	void TwoPhaseDeactivateCollision();

	UFUNCTION(BlueprintCallable)
	void PreventOverlappingCharacter_S();

	UFUNCTION(BlueprintCallable)
	void PreventOverlappingCharacter_E();

	UFUNCTION(BlueprintNativeEvent)
	void GrapDeAttachment();

	UFUNCTION(BlueprintCallable)
	void GrapDeAttachNotify();

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintNativeEvent)
	void GrapOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void GrapOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void GrapStart();

	UFUNCTION()
	void JumpOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void JumpOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void TwoPhaseOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TwoPhaseOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintCallable)
	void AnimModeChange();

	/*******************HUD ***********************/

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* StateMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* NearATKMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* CommonATKMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* FarATKMontage;

	void BeginHitStop();

	void EndHitStop();

	FTimerHandle hitStopTimerHandle;

	float HitStopTimer;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Al")
	class AAIController* AIController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class APlayerCharacter* Target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerCharacter* Main;
	

	/***************************ApproachFar Overlap & Function****************************/
	UFUNCTION()
	virtual void ApproachFarSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void ApproachFarSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ApproachFarAttackStart();

	UFUNCTION(BlueprintCallable)
	void ApproachFarAttackInterpStart();

	UFUNCTION(BlueprintCallable)
	void ApproachFarAttackInterpEnd();

	/***************************Far Overlap & Function****************************/
	UFUNCTION()
	virtual void FarSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void FarSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status | bool")
	bool bFarAttack;

	void FarAttackStart();
	

	/***************************ApproachCommon Overlap & Function****************************/
	UFUNCTION()
	virtual void ApproachCommonOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void ApproachCommonOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void ApproachCommonAttackStart();


	/***************************Common Overlap & Function****************************/
	UFUNCTION()
	virtual void CommonOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void CommonOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CommonAttackStart();
	
	/***************************Near Overlap & Function****************************/
	UFUNCTION()
	virtual void NearOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void NearOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void NearAttackStart();

	/**********************Other Function*************************/

	bool Alive();

	void AttackingCheck();

	FVector GetPlayerLocation();

	//Niagara
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UNiagaraSystem* NiagarahitEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FRotator NiagarahitEffectRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector NiagarahitEffectScale;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector HitparticlesScales;

	float PlayHighPriorityMontage(UAnimMontage* Montage, FName StartSectionName, float InPlayRate);


	bool PlayWeightFSMMontage(int32 StartNumber, int32 EndNumber);

	void PlayHeavyFSMMontage(bool SelectHeavy);

	//밑에부분 나중에 정리

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status | bool")
	bool bTwoPhase;

	UFUNCTION(BlueprintCallable)
	void AttackEndCheck();

	UFUNCTION(BlueprintCallable)
	void HeavyTrigger();

	bool bHeavyTrigger;

	void HeavyTriggerCheck();

	bool bHeavyState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status | bool")
	bool bWalkRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status | bool")
	bool bWalkLeft;

	void KnockBackStart();

	UFUNCTION(BlueprintImplementableEvent)
	void KnockBackLaunch();

	UFUNCTION(BlueprintCallable)
	void KnockBackIaunch_Pimpl();

	UFUNCTION(BlueprintCallable)
	void KnockBackEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status | bool")
	bool bKnockBackStart;

	class AWeapon* Weapon;
	
	void Dead();

	UFUNCTION(BlueprintCallable)
	void DeadEnd();
	
	UFUNCTION(BlueprintCallable)
		void TwoPhaseStop();

	

	FTimerHandle DeathTimer;


	void Disappear();


#pragma region FSMWeight
	//FSMWeight

	TArray<bool> FSMIsHeavy;

	bool bFsmIsHeavyFunction(int32 index);

	//근접 공격 bool 1페이즈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMHeavyBool")
	bool bNearHeavy_N_ATK1_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMHeavyBool")
	bool bNearHeavy_C_ATK1_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMHeavyBool")
	bool bNearHeavy_C_ATK1_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMHeavyBool")
	bool bNearHeavy_C_ATK1_3;

	//근접 공격 bool 2페이즈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMHeavyBool")
	bool bNearHeavy_C_ATK2_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMHeavyBool")
	bool bNearHeavy_C_ATK2_2;

	//평거리 공격 bool 1페이즈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMHeavyBool")
	bool bCommonHeavy_C_ATK1_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMHeavyBool")
	bool bCommonHeavy_C_ATK1_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMHeavyBool")
	bool bCommonHeavy_C_ATK1_3;

	//평거리 공격 bool 2페이즈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMHeavyBool")
	bool bCommonHeavy_C_ATK2_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMHeavyBool")
	bool bCommonHeavy_C_ATK2_2;

	//원거리 공격 bool 1페이즈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMHeavyBool")
	bool bFarHeavy_F_ATK1_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMHeavyBool")
	bool bFarHeavy_F_ATK1_2;


	TArray<int32> FSMWeight;

	//근접 공격 가중치 1페이즈 (0~3)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMWeight")
		int32 WeightNear_N_ATK1_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMWeight")
		int32 WeightNear_C_ATK1_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMWeight")
		int32 WeightNear_C_ATK1_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMWeight")
		int32 WeightNear_C_ATK1_3;

	//근접 공격 가중치 2페이즈(4~5) 0~5
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMWeight")
		int32 WeightNear_C_ATK2_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMWeight")
		int32 WeightNear_C_ATK2_2;

	//평거리 공격 가중치 1페이즈 (6~8)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMWeight")
		int32 WeightCommon_C_ATK1_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMWeight")
		int32 WeightCommon_C_ATK1_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMWeight")
		int32 WeightCommon_C_ATK1_3;

	//평거리 공격 가중치 2페이즈 (9~10)  6~10
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMWeight")
		int32 WeightCommon_C_ATK2_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMWeight")
		int32 WeightCommon_C_ATK2_2;

	//원거리 공격 가중치 1페이즈  (11~12)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMWeight")
		int32 WeightFar_F_ATK1_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | FSMWeight")
		int32 WeightFar_F_ATK1_2;

	float FSMWeightFunction(int32 Start, int32 End);
#pragma endregion

	FRotator GetLookAtRotationYaw(FVector Target);
	
	bool bDamageCheck;

};