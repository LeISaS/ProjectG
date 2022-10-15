// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyBossTest.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "../Character/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/App.h"
#include "Math/UnrealMathUtility.h"
#include "../Core/ProjectGGameModeBase.h"
#include "../Item/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "../Core/UntitledPlayerController.h"

// Sets default values
ADummyBossTest::ADummyBossTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetupAttachment(GetRootComponent());

	NearSphere = CreateDefaultSubobject<USphereComponent>(TEXT("NearSphere"));
	NearSphere->SetupAttachment(GetRootComponent());

	CommonSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CommonSphere"));
	CommonSphere->SetupAttachment(GetRootComponent());

	ApproachCommonSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ApproachCommonSphere"));
	ApproachCommonSphere->SetupAttachment(GetRootComponent());

	FarSphere = CreateDefaultSubobject<USphereComponent>(TEXT("FarSphere"));
	FarSphere->SetupAttachment(GetRootComponent());

	ApproachFarSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ApproachFarSphere"));
	ApproachFarSphere->SetupAttachment(GetRootComponent());

	CombatCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetMesh(),FName("WeaponSocket"));

	GrapCollision = CreateDefaultSubobject<USphereComponent>(TEXT("GrapCollision"));
	GrapCollision->SetupAttachment(GetMesh(),FName("Grap"));

	JumpCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("JumpCollision"));
	JumpCollision->SetupAttachment(GetMesh());

	TwoPhaseCollision = CreateDefaultSubobject<USphereComponent>(TEXT("TwoPhaseCollision"));
	TwoPhaseCollision->SetupAttachment(GetMesh());
	TwoPhaseCollision->InitSphereRadius(800.f);


	/************************************************************/

	bHasValidTarget = false;
	bAttacking = false;
	bApproachFarInterp = false;
	bTwoPhase = false;
	bKnockBackStart = false;

	NearATKOneSpeed1 = 1.f;
	NearATKOneSpeed2 = 1.f;

	CommonATKOneSpeed1 = 1.f;
	CommonATKOneSpeed2 = 1.f;
	CommonATKOneSpeed3 = 1.f;

	FarATKOneSpeed1 = 1.f;
	FarATKOneSpeed2 = 1.f;

	ApproachAttackTime = 0.f;
	JumpDownSpeed = 1.f;
	InterpSpeed = 5.0f;

	HitStopTimer = 0.5f;

	//FSMWeight Init
#pragma region FSMWeight
	//가중치 
	WeightNear_N_ATK1_1 = 20;
	WeightNear_C_ATK1_1 = 10;
	WeightNear_C_ATK1_2 = 20;
	WeightNear_C_ATK1_3 = 10;
	WeightNear_C_ATK2_1 = 20;
	WeightNear_C_ATK2_2 = 10;

	WeightCommon_C_ATK1_1 = 15;
	WeightCommon_C_ATK1_2 = 10;
	WeightCommon_C_ATK1_3 = 15;
	WeightCommon_C_ATK2_1 = 10;
	WeightCommon_C_ATK2_2 = 15;

	WeightFar_F_ATK1_1 = 10;
	WeightFar_F_ATK1_2 = 15;

	//가중치 공격 bool
	//근접 공격 bool 1페이즈
	bNearHeavy_N_ATK1_1 = false;
	bNearHeavy_C_ATK1_1 = true;
	bNearHeavy_C_ATK1_2 = false;
	bNearHeavy_C_ATK1_3 = true;
	//근접 공격 bool 2페이즈
	bNearHeavy_C_ATK2_1 = false;
	bNearHeavy_C_ATK2_2 = true;

	//평거리 공격 bool 1페이즈
	bCommonHeavy_C_ATK1_1 = true;
	bCommonHeavy_C_ATK1_2 = false;
	bCommonHeavy_C_ATK1_3 = true;
	//평거리 공격 bool 2페이즈
	bCommonHeavy_C_ATK2_1 = false;
	bCommonHeavy_C_ATK2_2 = true;

	//원거리 공격 bool 1페이즈
	bFarHeavy_F_ATK1_1 = true;
	bFarHeavy_F_ATK1_2 = false; 
#pragma endregion

	/****************KnockBackLevel**********************/
	DefaultKnockBackLevel = 0;
	N_ATK1_1KnockBackLevel = 2;
	N_ATK1_2KnockBackLevel = 2;
	C_ATK1_1KnockBackLevel = 2;
	C_ATK1_1_1KnockBackLevel = 2;

	C_ATK1_2KnockBackLevel = 2;

	C_ATK1_3KnockBackLevel = 2;
	C_ATK1_3_1KnockBackLevel = 2;

	F_ATK1_1KnockBackLevel = 2;
	F_ATK1_2KnockBackLevel = 2;

	PhaseTrigger_1KnockBackLevel = 2;
	PhaseTrigger_2KnockBackLevel = 2;

	C_ATK2_1KnockBackLevel = 2;

	C_ATK2_2KnockBackLevel = 2;
	C_ATK2_2_1KnockBackLevel  = 2;

	/**********************Damage*******************************/
	N_ATK1_1Damage = 10.f;
	C_ATK1_1Damage = 10.f;
	C_ATK1_2Damage = 10.f;
	C_ATK1_3Damage = 10.f;
	C_ATK2_1Damage = 10.f;
	C_ATK2_2Damage = 10.f;
	F_ATK1_1Damage = 10.f;
	F_ATK1_2Damage = 10.f;

	TwoPhase_1Damage = 10.f;
	TwoPhase_2Damage = 10.f;


	TwoPhaseCount = 0;

	/*************************************************************/

	Health = 75.f;
	MaxHealth = 100.f;

	DefaultDamage = 10.f;
	TwoPhaseHealthPercent = 50.f;

	DummyBossMovementStatus = EDummyBossMovementStatus::EDBM_IDLE;
	//DummyBossStatus = EDummyBossStatus::EDBS_IDLE;

	bHasValidTarget = false;
	bDamageCheck = false;
}



// Called when the game starts or when spawned
void ADummyBossTest::BeginPlay()
{

	Super::BeginPlay();


	AIController = Cast<AAIController>(GetController());

	//SetDummyBossStatus(EDummyBossStatus::EDBS_IDLE);
	SetDummyBossMovementStatus(EDummyBossMovementStatus::EDBM_IDLE);
	SetDummyBossPhaseStatus(EDummyBossPhaseStatus::EDBPS_ONEPHASE);
	SetDummyBossKnockBackState(EDummyBossKnockBackState::EKBS_IDLE);


	ApproachFarSphere->OnComponentBeginOverlap.AddDynamic(this, &ADummyBossTest::ApproachFarSphereOnOverlapBegin);
	ApproachFarSphere->OnComponentEndOverlap.AddDynamic(this, &ADummyBossTest::ApproachFarSphereOnOverlapEnd);	
	FarSphere->OnComponentBeginOverlap.AddDynamic(this, &ADummyBossTest::FarSphereOnOverlapBegin);
	FarSphere->OnComponentEndOverlap.AddDynamic(this, &ADummyBossTest::FarSphereOnOverlapEnd);
	ApproachCommonSphere->OnComponentBeginOverlap.AddDynamic(this, &ADummyBossTest::ApproachCommonOnOverlapBegin);
	ApproachCommonSphere->OnComponentEndOverlap.AddDynamic(this, &ADummyBossTest::ApproachCommonOnOverlapEnd);
	CommonSphere->OnComponentBeginOverlap.AddDynamic(this, &ADummyBossTest::CommonOnOverlapBegin);
	CommonSphere->OnComponentEndOverlap.AddDynamic(this, &ADummyBossTest::CommonOnOverlapEnd);
	NearSphere->OnComponentBeginOverlap.AddDynamic(this, &ADummyBossTest::NearOnOverlapBegin);
	NearSphere->OnComponentEndOverlap.AddDynamic(this, &ADummyBossTest::NearOnOverlapEnd);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &ADummyBossTest::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &ADummyBossTest::CombatOnOverlapEnd);
	
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Overlap);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GrapCollision->OnComponentBeginOverlap.AddDynamic(this, &ADummyBossTest::GrapOnOverlapBegin);
	GrapCollision->OnComponentEndOverlap.AddDynamic(this, &ADummyBossTest::GrapOnOverlapEnd);

	GrapCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GrapCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
	GrapCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GrapCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	GrapCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Overlap);

	JumpCollision->OnComponentBeginOverlap.AddDynamic(this, &ADummyBossTest::JumpOnOverlapBegin);
	JumpCollision->OnComponentEndOverlap.AddDynamic(this, &ADummyBossTest::JumpOnOverlapEnd);

	JumpCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	JumpCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
	JumpCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	JumpCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Overlap);

	TwoPhaseCollision->OnComponentBeginOverlap.AddDynamic(this, &ADummyBossTest::TwoPhaseOnOverlapBegin);
	TwoPhaseCollision->OnComponentEndOverlap.AddDynamic(this, &ADummyBossTest::TwoPhaseOnOverlapEnd);

	TwoPhaseCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TwoPhaseCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
	TwoPhaseCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TwoPhaseCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	TwoPhaseCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Overlap);


#pragma region FSMWeightBeginPlay
	//근접 공격 가중치 1페이즈 0~3
	FSMWeight.Add(WeightNear_N_ATK1_1);
	FSMWeight.Add(WeightNear_C_ATK1_1);
	FSMWeight.Add(WeightNear_C_ATK1_2);
	FSMWeight.Add(WeightNear_C_ATK1_3);
	//근접 공격 가중치 2페이즈 4~5			(0~5)
	FSMWeight.Add(WeightNear_C_ATK2_1);
	FSMWeight.Add(WeightNear_C_ATK2_2);
	//평거리 공격 가중치 1페이즈 6~8
	FSMWeight.Add(WeightCommon_C_ATK1_1);
	FSMWeight.Add(WeightCommon_C_ATK1_2);
	FSMWeight.Add(WeightCommon_C_ATK1_3);
	//평거리 공격 가중치 2페이즈 9~10		(6~10)
	FSMWeight.Add(WeightCommon_C_ATK2_1);
	FSMWeight.Add(WeightCommon_C_ATK2_2);
	//원거리 공격 가중치 1페이즈 11~12
	FSMWeight.Add(WeightFar_F_ATK1_1);
	FSMWeight.Add(WeightFar_F_ATK1_2);

	//근접 공격 bool 1페이즈(0~3)
	FSMIsHeavy.Add(bNearHeavy_N_ATK1_1);
	FSMIsHeavy.Add(bNearHeavy_C_ATK1_1);
	FSMIsHeavy.Add(bNearHeavy_C_ATK1_2);
	FSMIsHeavy.Add(bNearHeavy_C_ATK1_3);
	//근접 공격 bool 2페이즈 4~5			(0~5)
	FSMIsHeavy.Add(bNearHeavy_C_ATK2_1);
	FSMIsHeavy.Add(bNearHeavy_C_ATK2_2);
	//평거리 공격 bool 1페이즈 6~8
	FSMIsHeavy.Add(bCommonHeavy_C_ATK1_1);
	FSMIsHeavy.Add(bCommonHeavy_C_ATK1_2);
	FSMIsHeavy.Add(bCommonHeavy_C_ATK1_3);
	//평거리 공격 가중치 2페이즈 9~10		(6~10)
	FSMIsHeavy.Add(bCommonHeavy_C_ATK2_1);
	FSMIsHeavy.Add(bCommonHeavy_C_ATK2_2);
	//원거리 공격 가중치 1페이즈 11~12
	FSMIsHeavy.Add(bFarHeavy_F_ATK1_1);
	FSMIsHeavy.Add(bFarHeavy_F_ATK1_2);
#pragma endregion

	TwoPhasePercent = Health * TwoPhaseHealthPercent / 100.0f;

}

// Called every frame
void ADummyBossTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bKnockBackStart&&Health <= TwoPhasePercent && DummyBossPhaseStatus!=EDummyBossPhaseStatus::EDBPS_TWOPHASE) //2페이즈시작
	{
		SetDummyBossPhaseStatus(EDummyBossPhaseStatus::EDBPS_TWOPHASE);
		AttackEndCheck();

		bWalkLeft = false;
		bWalkRight = false;
		bTwoPhase = true;
		bAttacking = false;
		bKnockBackStart = false;

		

		UE_LOG(LogTemp, Log, TEXT("2page"));

		bool bAnyMontagePlaying = GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
		
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (bAnyMontagePlaying)
		{
			if (AnimInstance)
			{
				AnimInstance->Montage_Stop(0.05f);
			}

			UE_LOG(LogTemp, Log, TEXT("Montage IS Playing"));
		}
		if (AIController)
		{
			AIController->StopMovement();
		}

		PlayHighPriorityMontage(StateMontage, "TwoPageStart", 1.0f);
	}

	switch (DummyBossMovementStatus)
	{
	case EDummyBossMovementStatus::EDBM_IDLE:
		if (AIController)
		{
			AIController->StopMovement();
		}
		break;
	case EDummyBossMovementStatus::EDBM_ATTACK:
		break;
	case EDummyBossMovementStatus::EDBM_MOVETARGET:
		if (Main)
		{
			this->MoveToTarget(Main);
		}
		break;
	case EDummyBossMovementStatus::EDBM_MAX:
		break;
	default:
		break;
	}


	switch (DummyBossStatus)
	{
	case EDummyBossStatus::EDBS_NEAR:
		if (!bAttacking)
		{
			NearAttackStart();
		}
		break;
	case EDummyBossStatus::EDBS_COMMON:
		if (!bAttacking)
		{
			CommonAttackStart();
		}
		break;
	case EDummyBossStatus::EDBS_APPCOMMON:
		break;
	case EDummyBossStatus::EDBS_FAR:
	
	case EDummyBossStatus::EDBS_APPFAR:
		if (!bAttacking)
		{
			ApproachAttackTime += DeltaTime;

			if (ApproachAttackTime >= 2.f)
			{
				ApproachFarAttackStart();
				ApproachAttackTime = 0.f;
			}
		}
		break;

	case EDummyBossStatus::EDBS_DEAD:
		break;
	case EDummyBossStatus::EDBS_MAX:
		break;
	default:
		UE_LOG(LogTemp, Log, TEXT("Out Of Rang State"));
		break;
	}	

	if (bApproachFarInterp)
	{
		FVector CurrentLocation = GetActorLocation();

		InterpSpeed = FMath::RandRange(0.f, 1.0f);

		FVector Interp = FMath::VInterpTo(CurrentLocation, GetPlayerLocation(), DeltaTime, InterpSpeed);

		SetActorLocation(Interp);
	}


	switch (DummyBossKnockBackState)
	{
	case EDummyBossKnockBackState::EKBS_IDLE:
		break;
	case EDummyBossKnockBackState::EKBS_KNOCKBACK:
		if (Health > 0.f &&!bKnockBackStart)
		{

			KnockBackStart();
		}

		break;
	case EDummyBossKnockBackState::EKBS_AIRKNOCKBACK:
		if (AIController)
		{
			AIController->StopMovement();
		}
		if (Health > 0.f&& !bKnockBackStart)
		{
			KnockBackStart();
		}
		break;
	case EDummyBossKnockBackState::EKBS_MAX:
		break;
	default:
		
		break;
	}

	if (bHeavyState)
	{
		PlayHeavyFSMMontage(bHeavyState);
	}

	//왼쪽 오른쪽움직임 추가
	if (bWalkRight || bWalkLeft)
	{
		if (AIController)
		{
			FVector Forward = GetActorForwardVector();

			int Direction = bWalkRight ? 1 : -1;

			FRotator DestRotation(0.f, 90.f * Direction, 0.f);
			FVector Dest = DestRotation.RotateVector(Forward);

			GetCharacterMovement()->MoveSmooth(Dest * 200.0f, DeltaTime);
		}
	}
}

// Called to bind functionality to input
void ADummyBossTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADummyBossTest::MoveToTarget(APlayerCharacter* Target)
{

	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);

		MoveRequest.SetAcceptanceRadius(10.0f);

		FNavPathSharedPtr NavPath;
		AIController->MoveTo(MoveRequest, &NavPath);
	}
	
}

void ADummyBossTest::AnimModeChange()
{
	if (AIController)
	{
		AIController->StopMovement();
		SetDummyBossMovementStatus(EDummyBossMovementStatus::EDBM_ATTACK);
	}
}

/**************************************************/
/**ApproachFarSphere - 맨끝 거리 체크 구**/
/*************************************************/
void ADummyBossTest::ApproachFarSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"),*OtherActor->GetName());
		Main = Cast<APlayerCharacter>(OtherActor);
		if (Main && Alive())
		{
			SetDummyBossMovementStatus(EDummyBossMovementStatus::EDBM_MOVETARGET);
			SetDummyBossStatus(EDummyBossStatus::EDBS_APPFAR);

			bHasValidTarget = true;

			Main->SetHasCombatTarget(true);
			Main->SetCombatTarget(this);

			//HUD
			Main->UpdateHUD(true);
		}
	}
}

void ADummyBossTest::ApproachFarSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		
		if (PlayerCharacter)
		{
			SetDummyBossMovementStatus(EDummyBossMovementStatus::EDBM_IDLE);
			//SetDummyBossStatus(EDummyBossStatus::EDBS_IDLE);


			if (PlayerCharacter->CombatTarget == this)
			{
				PlayerCharacter->SetCombatTarget(nullptr);
			}

			bHasValidTarget = false;

			PlayerCharacter->UpdateHUD(false);
			//HUDUpdate
			
			
			ApproachAttackTime = 0.f;
		}
	}
}

void ADummyBossTest::ApproachFarAttackStart()
{
	if (Alive() && bHasValidTarget)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && !bKnockBackStart&&!bAttacking)
		{
			bHeavyState = PlayWeightFSMMontage(11, 12);

		}
	}
}
 
void ADummyBossTest::ApproachFarAttackInterpStart()
{
	if (Alive() && bHasValidTarget)
	{
		bApproachFarInterp =true;
	}
}

void ADummyBossTest::ApproachFarAttackInterpEnd()
{
	if (Alive() && bHasValidTarget)
	{
		bApproachFarInterp = false;
	}
}

/**************************************************/
/********FarSphere - 맨끝 거리 앞구*********/
/**************************************************/

void ADummyBossTest::FarSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			SetDummyBossStatus(EDummyBossStatus::EDBS_FAR);
			
		}

	}
}

void ADummyBossTest::FarSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && Alive())
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			SetDummyBossStatus(EDummyBossStatus::EDBS_APPFAR);

		}
	}
}

void ADummyBossTest::FarAttackStart()
{
	if (Alive() && bHasValidTarget)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		UE_LOG(LogTemp, Log, TEXT("FarAttackStart()"));

		if (AnimInstance && !bKnockBackStart&&!bAttacking)
		{
			bHeavyState = PlayWeightFSMMontage(11, 12);
		}
	}
}

void ADummyBossTest::GrapDeAttachment_Implementation()
{

}

void ADummyBossTest::GrapStart()
{
	if (DummyBossStatus == EDummyBossStatus::EDBS_DEAD) return;

	SetDummyBossMovementStatus(EDummyBossMovementStatus::EDBM_ATTACK);

	UE_LOG(LogTemp, Log, TEXT("GrapStartBoss"));
	if (DummyBossStatus == EDummyBossStatus::EDBS_DEAD)
	{
		PlayHighPriorityMontage(NearATKMontage, FName("N_ATK1_1_Success"), 1.0f);

	}

}



/*****************************************************************/
/********ApproachCommonSphere - 중간 거리 구*******/
/****************************************************************/




void ADummyBossTest::ApproachCommonOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		
		if (PlayerCharacter)
		{
			SetDummyBossStatus(EDummyBossStatus::EDBS_APPCOMMON);
		}
		
	}

}

void ADummyBossTest::ApproachCommonOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && Alive())
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			SetDummyBossStatus(EDummyBossStatus::EDBS_FAR);

		}
	}
}

void ADummyBossTest::ApproachCommonAttackStart()
{
	if (Alive() && bHasValidTarget)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		UE_LOG(LogTemp, Log, TEXT("AppCommonAttackStart()"));

		if (AnimInstance)
		{

		}
		
	}

}


/*****************************************************************/
/********CommonSphere -  평 거리 구*******/
/****************************************************************/

void ADummyBossTest::CommonOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			SetDummyBossStatus(EDummyBossStatus::EDBS_COMMON);

		}

	}
}

void ADummyBossTest::CommonOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && Alive())
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			SetDummyBossStatus(EDummyBossStatus::EDBS_APPCOMMON);
		}
	}

}

void ADummyBossTest::CommonAttackStart()
{
	if (Alive() && bHasValidTarget)
	{

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		
		if (AnimInstance && !bKnockBackStart &&!bAttacking)
		{

			if (DummyBossPhaseStatus == EDummyBossPhaseStatus::EDBPS_ONEPHASE)
			{
				bHeavyState = PlayWeightFSMMontage(6, 8);
			}
			else
			{
				bHeavyState = PlayWeightFSMMontage(6, 10);
			}
		}
	}
}
/*****************************************************************/
/********NearSphere -  근접 거리 구*******/
/****************************************************************/

void ADummyBossTest::NearOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			SetDummyBossStatus(EDummyBossStatus::EDBS_NEAR);
		}
	}
}

void ADummyBossTest::NearOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && Alive())
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			SetDummyBossStatus(EDummyBossStatus::EDBS_COMMON);
			SetDummyBossMovementStatus(EDummyBossMovementStatus::EDBM_MOVETARGET);
		}
	}
	
}



void ADummyBossTest::NearAttackStart()
{
	if (Alive() && bHasValidTarget)
	{

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && !bAttacking &&!bKnockBackStart)
		{
			if (DummyBossPhaseStatus == EDummyBossPhaseStatus::EDBPS_ONEPHASE)
			{
				bHeavyState = PlayWeightFSMMontage(0, 3);
			}
			else
			{
				bHeavyState = PlayWeightFSMMontage(0, 5);
			}
		}
	}

}

bool ADummyBossTest::Alive()
{
	return GetDummyBossStatus() != EDummyBossStatus::EDBS_DEAD;
}

void ADummyBossTest::JumpOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		if(PlayerCharacter)
		{
			if (DamageTypeClass && !(PlayerCharacter->binvincibility)&&!bDamageCheck)
			{
				UGameplayStatics::ApplyDamage(PlayerCharacter, DefaultDamage, AIController, this, DamageTypeClass);
				PlayerCharacter->SetPlayerStatus(EPlayerStatus::EPS_KNOCKBACK);
			}
		}
	}
}

void ADummyBossTest::JumpOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ADummyBossTest::TwoPhaseOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			if (DamageTypeClass && !(PlayerCharacter->binvincibility)&&!bDamageCheck)
			{
				UGameplayStatics::ApplyDamage(PlayerCharacter, DefaultDamage, AIController, this, DamageTypeClass);
				PlayerCharacter->SetPlayerStatus(EPlayerStatus::EPS_KNOCKBACK);
			}
		}
	}
}

void ADummyBossTest::TwoPhaseOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

/*****************************************************************/
/*****************other function - combat*********************/
/****************************************************************/

float ADummyBossTest::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{

		Health -= DamageAmount;
		Dead();
	}
	else
	{
		Health -= DamageAmount;
		BeginHitStop();
	}
	return DamageAmount;
	
}

void ADummyBossTest::BeginHitStop()
{
	CustomTimeDilation = 0.f;
	Main->CustomTimeDilation = 0.f;

	
	HitStopTimer = Main->DefaultHitStop;
	GetWorld()->GetTimerManager().SetTimer(hitStopTimerHandle, this, &ADummyBossTest::EndHitStop, HitStopTimer, false);
}

void ADummyBossTest::EndHitStop()
{
	CustomTimeDilation = 1.f;
	Main->CustomTimeDilation = 1.f;

}


void ADummyBossTest::Dead()
{
	SetDummyBossStatus(EDummyBossStatus::EDBS_DEAD);

	PlayHighPriorityMontage(StateMontage, "Death", 1.0f);

	NearSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CommonSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ApproachCommonSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FarSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ApproachFarSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GrapCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bAttacking = false; 
	bWalkLeft = false;
	bWalkRight = false;
	bKnockBackStart = false;
	
}

void ADummyBossTest::DeadEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	Main->ChapterOneClearUI();

	Destroy();
	
	//GetWorldTimerManager().SetTimer(DeathTimer, this, &ADummyBossTest::Disappear, 10.f);
}

void ADummyBossTest::Disappear()
{

}

void ADummyBossTest::TwoPhaseStop()
{
	bTwoPhase = false;
	//SetDummyBossKnockBackState(EDummyBossKnockBackState::EKBS_IDLE);
}



//************************CombatActivateCollision*****************//

void ADummyBossTest::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bDamageCheck = false;
}

void ADummyBossTest::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADummyBossTest::JumpActivateCollision()
{
	JumpCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bDamageCheck = false;
}

void ADummyBossTest::JumpDeActivateCollision()
{
	JumpCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADummyBossTest::TwoPhaseActivateCollision()
{
	if (TwoPhaseCount == 0)
	{
		DefaultDamage = TwoPhase_1Damage;
	}
	else
	{
		DefaultDamage = TwoPhase_2Damage;
	}

	TwoPhaseCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bDamageCheck = false;
}
void ADummyBossTest::TwoPhaseDeactivateCollision()
{
	TwoPhaseCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TwoPhaseCollision->InitSphereRadius(1200.f);
	TwoPhaseCount++;
}

void ADummyBossTest::PreventOverlappingCharacter_S()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Ignore);
}

void ADummyBossTest::PreventOverlappingCharacter_E()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Block);
}

void ADummyBossTest::GrapDeAttachNotify()
{
	if (Main->bGraping)
	{
		Main->GrapEnd();
		GrapDeAttachment();
	}
}

void ADummyBossTest::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			if (DamageTypeClass && !(PlayerCharacter->binvincibility) && !bDamageCheck)
			{
				UGameplayStatics::ApplyDamage(PlayerCharacter, DefaultDamage, AIController, this, DamageTypeClass);
				PlayerCharacter->SetPlayerStatus(EPlayerStatus::EPS_KNOCKBACK);
			}
		}

	}
}

void ADummyBossTest::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


//******************GrapActivateCollision*******************//

void ADummyBossTest::GrapActivateCollision()
{
	SetDummyBossKnockBackState(EDummyBossKnockBackState::EKBS_IDLE);

	GrapCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
void ADummyBossTest::GrapDeactivateCollision()
{
	GrapCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ADummyBossTest::GrapOnOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			PlayerCharacter->SetPlayerStatus(EPlayerStatus::EPS_GRAP);

			if (DamageTypeClass && !(PlayerCharacter->binvincibility)&&!bDamageCheck)
			{
				UGameplayStatics::ApplyDamage(PlayerCharacter, DefaultDamage, AIController, this, DamageTypeClass);
			}
		}
	
	}

}

void ADummyBossTest::GrapOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

//*************************Other****************************//

FVector ADummyBossTest::GetPlayerLocation()
{
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	return PlayerLocation;
}

float ADummyBossTest::PlayHighPriorityMontage(UAnimMontage* Montage, FName StartSectionName, float InPlayRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance->Montage_IsPlaying(Montage))
	{
		UAnimMontage* HighMontage = Montage;

		return PlayAnimMontage(HighMontage, InPlayRate, StartSectionName);
	}
	
	return 0.0f;
}

bool ADummyBossTest::PlayWeightFSMMontage(int32 StartNumber, int32 EndNumber)
{
	

	//SetDummyBossMovementStatus(EDummyBossMovementStatus::EDBM_ATTACK);




	//C_ATK2_1 , 2 스피드 변수 추가

	if (DummyBossStatus != EDummyBossStatus::EDBS_DEAD && !bKnockBackStart && !bWalkRight
		&&!bWalkLeft && !bTwoPhase)
	{
		GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
		bDamageCheck = false;
		bAttacking = true;

		int TestFSMWeight = FSMWeightFunction(StartNumber, EndNumber);

		UE_LOG(LogTemp, Log, TEXT("TestFSMWeight : %d "), TestFSMWeight);
		bool TestFSMHeavy = bFsmIsHeavyFunction(TestFSMWeight);

		UE_LOG(LogTemp, Log, TEXT("TestFSMHeavy : %s "), TestFSMHeavy ? TEXT("true") : TEXT("false"));
		switch (TestFSMWeight)
		{
			//근접 공격 1페이즈
		case 0:
			//체크 추가
			PlayHighPriorityMontage(NearATKMontage, "N_ATK1_1", NearATKOneSpeed1);
			DefaultKnockBackLevel = N_ATK1_1KnockBackLevel;
			DefaultDamage = N_ATK1_1Damage;
			break;
		case 1:
			PlayHighPriorityMontage(CommonATKMontage, "C_ATK1_1", CommonATKOneSpeed1);
			DefaultKnockBackLevel = C_ATK1_1KnockBackLevel;
			DefaultDamage = C_ATK1_1Damage;
			break;
		case 2:
			PlayHighPriorityMontage(CommonATKMontage, "C_ATK1_2", CommonATKOneSpeed2);
			DefaultKnockBackLevel = C_ATK1_2KnockBackLevel;
			DefaultDamage = C_ATK1_2Damage;
			break;
		case 3:
			PlayHighPriorityMontage(CommonATKMontage, "C_ATK1_3", CommonATKOneSpeed3);
			DefaultKnockBackLevel= C_ATK1_3KnockBackLevel;
			DefaultDamage = C_ATK1_3Damage;
			break;
			//근접 공격 2페이즈
		case 4:
			PlayHighPriorityMontage(CommonATKMontage, "C_ATK2_1", 1.0f);
			DefaultKnockBackLevel = C_ATK2_1KnockBackLevel;
			DefaultDamage = C_ATK2_1Damage;
			break;
		case 5:
			PlayHighPriorityMontage(CommonATKMontage, "C_ATK2_2", 1.0f);
			DefaultKnockBackLevel = C_ATK2_2KnockBackLevel;
			DefaultDamage = C_ATK2_2Damage;
			break;
			//평거리 공격 1페이즈
		case 6:
			PlayHighPriorityMontage(CommonATKMontage, "C_ATK1_1", CommonATKOneSpeed1);
			DefaultKnockBackLevel = C_ATK1_1KnockBackLevel;
			DefaultDamage = C_ATK1_1Damage;
			break;
		case 7:
			PlayHighPriorityMontage(CommonATKMontage, "C_ATK1_2", CommonATKOneSpeed2);
			DefaultKnockBackLevel = C_ATK1_2KnockBackLevel;
			DefaultDamage = C_ATK1_2Damage;
			break;
		case 8:
			PlayHighPriorityMontage(CommonATKMontage, "C_ATK1_3", CommonATKOneSpeed3);
			DefaultKnockBackLevel = C_ATK1_3KnockBackLevel;
			DefaultDamage = C_ATK1_3Damage;
			break;
			//평거리 공격 2페이즈
		case 9:
			PlayHighPriorityMontage(CommonATKMontage, "C_ATK2_1", 1.0f);
			DefaultKnockBackLevel = C_ATK2_1KnockBackLevel;
			DefaultDamage = C_ATK2_1Damage;
			break;
		case 10:
			PlayHighPriorityMontage(CommonATKMontage, "C_ATK2_2", 1.0f);
			DefaultKnockBackLevel = C_ATK2_2KnockBackLevel;
			DefaultDamage = C_ATK2_2Damage;
			break;
			//원거리 공격 1페이즈
		case 11:
			PlayHighPriorityMontage(FarATKMontage, "F_ATK1_1", FarATKOneSpeed1);
			DefaultKnockBackLevel = F_ATK1_1KnockBackLevel;
			DefaultDamage = F_ATK1_1Damage;
			break;
		case 12:
			PlayHighPriorityMontage(FarATKMontage, "F_ATK1_2", FarATKOneSpeed2);
			DefaultKnockBackLevel = F_ATK1_2KnockBackLevel;
			DefaultDamage = F_ATK1_2Damage;
			break;

		default:
			UE_LOG(LogTemp, Log, TEXT("SelectMontage FSM Out Of Range!!"));
			break;
		}
		return TestFSMHeavy;
	}
	return false;
	
	
}

void ADummyBossTest::PlayHeavyFSMMontage(bool SelectHeavy)
{
	//UE_LOG(LogTemp, Warning, TEXT("Heavy"));
	bool bAnyMontagePlaying = GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
	
	if (SelectHeavy)
	{
		if (!bAnyMontagePlaying)
		{
			bHeavyState = false;
			int randIndex = FMath::RandRange(0, 1);
			switch (randIndex)
			{
			case 0:
				PlayHighPriorityMontage(StateMontage, "WalkRight", 1.0f);
				UE_LOG(LogTemp, Log, TEXT("WalkRight"));
				bWalkRight = true;
				break;
			case 1:
				PlayHighPriorityMontage(StateMontage, "WalkLeft", 1.0f);
				UE_LOG(LogTemp, Log, TEXT("WalkLeft"));
				bWalkLeft = true;
				break;
			default:
				break;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("PlayHeavyFSMMontage false"));
		//바로 시작
	}
}

//가중치 값 계산 


float ADummyBossTest::FSMWeightFunction(int32 Start, int32 End)
{
	int Index = 0;
	double FSMRandom = FMath::Rand() / (double)RAND_MAX;
	double setRate = 0.0f;
	int32 CurTotal = 0;
	for (int i = Start; i <= End; i++)
	{
		CurTotal += FSMWeight[i];
	}

	for (Start; Start <= End; Start++)
	{
		setRate += FSMWeight[Start] / (double)CurTotal;

		if (setRate > FSMRandom)
		{
			Index = Start;
			break;
		}
	}
	return Index;
}

bool ADummyBossTest::bFsmIsHeavyFunction(int32 index)
{
	bool bisReturnHeavy = false ;
	switch (index)
	{
	case 0: bisReturnHeavy = bNearHeavy_N_ATK1_1; break;
	case 1: bisReturnHeavy = bNearHeavy_C_ATK1_1; break;
	case 2: bisReturnHeavy = bNearHeavy_C_ATK1_2; break;
	case 3: bisReturnHeavy = bNearHeavy_C_ATK1_3; break;
	case 4: bisReturnHeavy = bNearHeavy_C_ATK2_1; break;
	case 5: bisReturnHeavy = bNearHeavy_C_ATK2_2; break;
	case 6: bisReturnHeavy = bCommonHeavy_C_ATK1_1; break;
	case 7: bisReturnHeavy = bCommonHeavy_C_ATK1_2; break;
	case 8: bisReturnHeavy = bCommonHeavy_C_ATK1_3; break;
	case 9: bisReturnHeavy = bCommonHeavy_C_ATK2_1; break;
	case 10: bisReturnHeavy = bCommonHeavy_C_ATK2_2; break;
	case 11: bisReturnHeavy = bFarHeavy_F_ATK1_1; break;
	case 12: bisReturnHeavy = bFarHeavy_F_ATK1_2; break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("bFSMIsHeavyFunction out of Range!!"));
		break;
	}

	return bisReturnHeavy;
}

void ADummyBossTest::AttackEndCheck()
{
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_NavWalking;
	SetDummyBossMovementStatus(EDummyBossMovementStatus::EDBM_MOVETARGET);
	if (!bTwoPhase)
	{
		GetWorldTimerManager().SetTimer(TimerHandler, this, &ADummyBossTest::AttackingCheck, 1.5f);
	
	}

	UE_LOG(LogTemp, Log, TEXT("AttackEndCheck"));
}

void ADummyBossTest::AttackingCheck()
{
	bAttacking = false;
}


void ADummyBossTest::HeavyTrigger()
{
	SetDummyBossMovementStatus(EDummyBossMovementStatus::EDBM_MOVETARGET);
	HeavyTriggerCheck();
	//GetWorldTimerManager().SetTimer(TimerHandler, this, &ADummyBossTest::HeavyTriggerCheck, 1.5f);
	
}

void ADummyBossTest::HeavyTriggerCheck()
{
	bHeavyTrigger = false;
	bWalkRight = false;
	bWalkLeft = false;
}

void ADummyBossTest::KnockBackStart()
{
	if (DummyBossStatus == EDummyBossStatus::EDBS_DEAD)
		return;
	if (bTwoPhase)
	{
		SetDummyBossKnockBackState(EDummyBossKnockBackState::EKBS_IDLE);
		return;
	}
	SetDummyBossMovementStatus(EDummyBossMovementStatus::EDBM_IDLE);
	bKnockBackStart = true;
	bApproachFarInterp = false;
	bAttacking = false;

	if (DummyBossKnockBackState == EDummyBossKnockBackState::EKBS_KNOCKBACK)
	{
		PlayHighPriorityMontage(StateMontage, "KnockBack", 1.0f);
		AttackEndCheck();
	}
	else if (DummyBossKnockBackState == EDummyBossKnockBackState::EKBS_AIRKNOCKBACK)
	{
		PlayHighPriorityMontage(StateMontage, "AirKnockBack", 1.0f);
		AttackEndCheck();
	}

}

void ADummyBossTest::KnockBackIaunch_Pimpl()
{
	this->KnockBackLaunch();
}

void ADummyBossTest::KnockBackEnd()
{
	SetDummyBossKnockBackState(EDummyBossKnockBackState::EKBS_IDLE);
	bKnockBackStart = false;
}

FRotator ADummyBossTest::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);

	return LookAtRotationYaw;
}