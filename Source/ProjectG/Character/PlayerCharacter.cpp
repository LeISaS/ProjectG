// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "../Item/Weapon.h"
#include "../Boss/DummyBossTest.h"
#include "../Core/UntitledPlayerController.h"
#include "UntitledPlayerCharacterCamera.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Engine/Classes/Animation/AnimMontage.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "../NormalMonster.h"
// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*Camera*/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UUntitledPlayerCharacterCamera>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	BaseTurnRate = 110.f;
	BaseLookUpRate = 100.f;

	GetCapsuleComponent()->SetCapsuleSize(34.f, 88.f);

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;    //Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); // at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 550.f;
	GetCharacterMovement()->AirControl = 0.2f;



	/*******************************************************/

	/***************Bool Type**********************/

	bShiftKeyDown = false;
	bLMBDown = false;
	bSpaceKeyDown = false;
	bRolling = false;
	bAttacking = false;
	bMoveForward = false;
	bMoveRight = false;
	bAttackButtonCheck = false;
	bRMBDown = false;
	bCharging = false;
	bHasCombatTarget = false;
	bPotionClickDown = false;
	bKnockBackStart = false;

	bCkeyDown = false;

	bDead = false;

	bInGameManual = false;
	
	bGraping = false;
	bCharinghit = false;
	bXboxCheck = false;
	bChargingCheck = false;
	/**************Player Speed Status*********************/
	WalkSpeed = 200.f;
	RunSpeed = 400.f;
	SprintSpeed = 600.f;

	RollSpeed = 1.f;
	AttackSpeed = 1.5f;
	DeadSpeed = 1.f;
	KnockbackSpeed = 1.f;
	ComboCount = 0;

	InterpKnockBackSpeed = 5.0f;
	InterpRollingSpeed = 10.0f;


	/**************Player Status*********************/
	MaxHealthPoint = 100.f;
	HealthPoint = 100.f;

	MaxStaminaPoint = 100.f;
	StaminaPoint = 100.f;

	MinActionStamina = 10.f;
	SprintStaminaCost = 13.f;
	StaminaRecovery = 25.f;

	RollStaminaCost = 25.f;
	NormalAttackStaminaCost = 25.f;

	ChargingStaminaCost = 6.f;
	ChargeReadyStaminaCost = 25.f;

	ChargingTimer = 0.f;
	FirstChargeTime = 2.f;
	SecondChargeTime = 4.f;
	ChargingComboMax = 4.1f;

	KnockBackNormalAttackA = 1;
	KnockBackNormalAttackB = 1;
	KnockBackChargeAttackN = 1;
	KnockBackChargeAttackA = 2;
	KnockBackChargeAttackB = 3;


	/**************Player Status Damage*********************/
	NormalAttackADamage = 100.f;
	NormalAttackBDamage = 100.f;
	ChargeAttackNDamage = 150.f;
	ChargeAttackADamage = 370.f;
	ChargeAttackBDamage = 686.f;

	NormalAttackAHitStopTime = 0.16f;
	NormalAttackBHitStopTime = 0.28f;
	ChargeAttackNHitStopTime = 0.16f;
	ChargeAttackAHitStopTime = 0.28f;
	ChargeAttackBHitStopTime = 0.38f;

	DefaultHitStop = 0.0f;

	/**************Player  Stamina Timer*********************/
	StaminaNoActionTimer = 3.f;
	bStaminaNoAction = false;


	//Lock On System
	LockOnTarget = nullptr;
	bHasLockOnTarget = false;

	// Potion System
	Potions = 5;
	MaxPotions = 10;
	HealthAmount = 75.f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UntitledPlayerController = Cast<AUntitledPlayerController>(GetController());
	SetMovementStatus(EMovementStatus::EMS_WALK);
	SetPlayerStatus(EPlayerStatus::EPS_NORMAL);
	SetStaminaStatus(EStaminaStatus::ESS_NORMAL);

	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (WeaponClass)
	{
		FTransform WeaponTransform;
		WeaponTransform.SetLocation(FVector::ZeroVector);
		WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));

		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, WeaponTransform, SpawnParams);
		if (Weapon)
		{
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("PlayerWeaponSocket"));

			Weapon->SetInstigator(this->GetController());
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerStatus == EPlayerStatus::EPS_DEAD) return;

	float SprintStaminaCostDelta = SprintStaminaCost * DeltaTime;

	float StaminaRecoveryDelta = StaminaRecovery * DeltaTime;
	//통상적인 스태미너 -> 추후 변경 (2차안)
	StaminaDecrease(SprintStaminaCostDelta, StaminaRecoveryDelta);

	if (PlayerStatus == EPlayerStatus::EPS_KNOCKBACK )
	{
		if (HealthPoint>0.f)
		{
			KnockBackStart();
			ShiftKeyUp();
		}
	}

	if (!bKnockBackStart)
	{
		if (bCharging)
		{
			ChargingTimer += DeltaTime;
			ChargingAttack(ChargingTimer);
		}
		else
		{
			ChargingTimer = 0.f;
		}
	}

	if (bHasLockOnTarget)
	{
		if (bHasLockOnTarget)
		{
			// Camera Rotation [ Controller Rotation ]
			FRotator FindRotator = UKismetMathLibrary::FindLookAtRotation(FollowCamera->GetComponentLocation(), LockOnTarget->GetActorLocation());
			if (LockOnTarget != nullptr)
			{
				FRotator InterpRotator = FMath::RInterpTo(this->GetController()->GetControlRotation(), FindRotator, DeltaTime, 3.f);
				FRotator ResultRotator(InterpRotator.Pitch, InterpRotator.Yaw, this->GetController()->GetControlRotation().Roll);
				this->GetController()->SetControlRotation(ResultRotator);

				if (MovementStatus == EMovementStatus::EMS_WALK || MovementStatus == EMovementStatus::EMS_RUN && !bAttacking) 
				{
				FRotator CharacterFindRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockOnTarget->GetActorLocation());
				FRotator CharacterInterpRotator = FMath::RInterpTo(this->GetActorRotation(), CharacterFindRotator, DeltaTime, 5.f);
				FRotator CharacterResultRotator = FRotator(0, CharacterInterpRotator.Yaw, 0);
				this->SetActorRotation(CharacterResultRotator);
				}
			}
		}
	}

	//플레이어 공격시 회전
	if (SmoothPlayerRotator.Yaw != 0)
	{
		SmoothRotatorTimer += DeltaTime;

		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), SmoothPlayerRotator, DeltaTime, 5.0f);
		SetActorRotation(InterpRotation);

		if (SmoothRotatorTimer > 1)
		{
			SmoothRotatorTimer = 0;
			SmoothPlayerRotator.Yaw = 0;
		}
	}

	//보스 바라보는 넉백 회전
	if (bKnockBackStart || bDead )
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpKnockBackSpeed);
		SetActorRotation(InterpRotation);
	}

	if (bRolling)
	{
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), RInterpMovement, DeltaTime, InterpRollingSpeed);

		SetActorRotation(InterpRotation);
	}

}
void APlayerCharacter::StaminaNoAction()
{
	if (bStaminaNoAction)
	{
		ShiftKeyUp();
		GetWorldTimerManager().SetTimer(TimerHandler, this, &APlayerCharacter::StaminaNoActionBoolCheck, StaminaNoActionTimer);
	}
}

void APlayerCharacter::StaminaNoActionBoolCheck()
{
	bStaminaNoAction = false;
}

void APlayerCharacter::StaminaDecrease(float SprintStaminaCostDelta, float StaminaRecoveryDelta)
{


	if (StaminaPoint <= 0.f)
	{
		StaminaPoint = 0.f;
		bStaminaNoAction = true;
		StaminaNoAction();
	}
	else if (StaminaPoint >= MaxStaminaPoint)
	{
		StaminaPoint = MaxStaminaPoint;
	}


	if (PlayerStatus == EPlayerStatus::EPS_NORMAL)
	{
		switch (StaminaStatus)
		{
		case EStaminaStatus::ESS_NORMAL:
			if (bShiftKeyDown)
			{
				if (StaminaPoint - SprintStaminaCostDelta <= MinActionStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_BELOWMINIMUM);
					StaminaPoint -= SprintStaminaCostDelta;
				}
				else
				{
					StaminaPoint -= SprintStaminaCostDelta;
				}
				if (bMoveForward || bMoveRight)
				{
					SetMovementStatus(EMovementStatus::EMS_SPRINT);
				}
				else
				{
					SetMovementStatus(EMovementStatus::EMS_WALK);
				}
				if (bHasLockOnTarget) {
					GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); // at this rotation rate
				}
			}
			else //Shift key up 
			{
				if (StaminaPoint + StaminaRecoveryDelta >= MaxStaminaPoint)
				{
					StaminaPoint = MaxStaminaPoint;
				}
				else
				{
					StaminaPoint += StaminaRecoveryDelta;
				}
				if (bHasLockOnTarget) {
					GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.f, 0.0f); // at this rotation rate
				}
			}
			break;

		case EStaminaStatus::ESS_BELOWMINIMUM:
			if (bShiftKeyDown)
			{
				if (StaminaPoint - SprintStaminaCostDelta <= 0.f)
				{
					SetStaminaStatus(EStaminaStatus::ESS_EXHAUSTED);
					StaminaPoint = 0;
					SetMovementStatus(EMovementStatus::EMS_WALK);
				}
				else
				{
					StaminaPoint -= SprintStaminaCostDelta;
					if (bMoveForward || bMoveRight)
					{
						SetMovementStatus(EMovementStatus::EMS_SPRINT);
					}
					else
					{
						SetMovementStatus(EMovementStatus::EMS_WALK);
					}
				}
			}
			else   //shift key up
			{
				if (StaminaPoint + StaminaRecoveryDelta >= MinActionStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_NORMAL);
					StaminaPoint += StaminaRecoveryDelta;
				}
				else
				{
					StaminaPoint += StaminaRecoveryDelta;
				}
				SetMovementStatus(EMovementStatus::EMS_WALK);
			}
			break;

		case EStaminaStatus::ESS_EXHAUSTED:
			if (bShiftKeyDown)
			{
				//StaminaPoint = 0.f;
				SetStaminaStatus(EStaminaStatus::ESS_EXHAUSTEDRECOVERING);
				StaminaPoint += StaminaRecoveryDelta;
			}
			else //shift key up
			{
				StaminaPoint += StaminaRecoveryDelta;
			}
			SetMovementStatus(EMovementStatus::EMS_WALK);
			break;

		case EStaminaStatus::ESS_EXHAUSTEDRECOVERING:
			if (StaminaPoint + StaminaRecoveryDelta >= MinActionStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_NORMAL);
				StaminaPoint += StaminaRecoveryDelta;
			}
			else
			{
				StaminaPoint += StaminaRecoveryDelta;
			}
			SetMovementStatus(EMovementStatus::EMS_WALK);
			break;

		default:
			;

		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/*===========================Action===============================*/
	// sprint Toggle
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::ShiftKeyDown);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::ShiftKeyUp);

	PlayerInputComponent->BindAction("RunXbox", IE_Repeat, this, &APlayerCharacter::ShiftKeyDown);
	PlayerInputComponent->BindAction("RunXbox", IE_Released, this, &APlayerCharacter::ShiftKeyUp);

	// Character Roll
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &APlayerCharacter::SpaceKeyDown);
	PlayerInputComponent->BindAction("Roll", IE_Released, this, &APlayerCharacter::SpaceKeyUp);

	PlayerInputComponent->BindAction("RollXbox", IE_Released, this, &APlayerCharacter::XboxSpaceKeyDown);
	PlayerInputComponent->BindAction("RollXbox", IE_Released, this, &APlayerCharacter::SpaceKeyUp);

	// WalkToggle
	PlayerInputComponent->BindAction("WalkToggle", IE_Pressed, this, &APlayerCharacter::CkeyDown);
	// 
	// Character Normal Attack
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &APlayerCharacter::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &APlayerCharacter::LMBUp);

	// Character Charge Attack
	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &APlayerCharacter::RMBDown);
	PlayerInputComponent->BindAction("RMB", IE_Released, this, &APlayerCharacter::RMBUp);

	// Target Lock On
	PlayerInputComponent->BindAction("TargetLockOn", IE_Pressed, this, &APlayerCharacter::RKeyDown);

	// Potion
	PlayerInputComponent->BindAction("Potion", IE_Pressed, this, &APlayerCharacter::PotionClickDown);
	PlayerInputComponent->BindAction("Potion", IE_Released, this, &APlayerCharacter::PotionClickUp);

	//InGameManualBar
	PlayerInputComponent->BindAction("InGameManualBar", IE_Pressed, this, &APlayerCharacter::DisplayInGameManual);

	/*=============================Axis===============================*/
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookupAtRate);
}

void APlayerCharacter::DisplayInGameManual()
{
	bInGameManual =!bInGameManual;
	if (bInGameManual)
	{
		UntitledPlayerController->DisplayInGameManual();
	}
	else
	{
		UntitledPlayerController->RemoveDisplayInGameManual();
	}
}



bool APlayerCharacter::CanMove(float value)
{
	if (Controller)
	{
		return (value != 0.0f) &&
			(!bAttacking) && (!bRolling) && (!bKnockBackStart) &&
			(PlayerStatus != EPlayerStatus::EPS_DEAD) &&!bGraping;
	}
	return false;
}


void APlayerCharacter::MoveForward(float value)
{

	if (PlayerStatus == EPlayerStatus::EPS_DEAD)
		return;

	bMoveForward = false;
	if (CanMove(value) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
		bMoveRight = true;

	}
}
void APlayerCharacter::MoveRight(float value)
{
	if (PlayerStatus == EPlayerStatus::EPS_DEAD)
		return;

	bMoveForward = false;
	if (CanMove(value))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);

		bMoveRight = true;

	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	if (!bHasLockOnTarget) {
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}
void APlayerCharacter::LookupAtRate(float Rate)
{
	if (!bHasLockOnTarget && !bInGameManual) {
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

/**Shift Key**/
//스태미너 추가시 수정
void APlayerCharacter::ShiftKeyDown()
{
	if (PlayerStatus == EPlayerStatus::EPS_DEAD) return; 

	if (!bKnockBackStart && !bStaminaNoAction&& !bInGameManual&& !bGraping && KeyInputCheck())
	{
		bXboxCheck = true;
		bShiftKeyDown = true;
		bCkeyDown = false;
		SetMovementStatus(EMovementStatus::EMS_SPRINT);
	}
}
void APlayerCharacter::ShiftKeyUp()
{
	bShiftKeyDown = false;

	if (bCkeyDown) SetMovementStatus(EMovementStatus::EMS_WALK);
	else SetMovementStatus(EMovementStatus::EMS_RUN);

	GetWorldTimerManager().SetTimer(XboxTimerHandler, this, &APlayerCharacter::XboxTimerCheck, 0.1f);

}
void APlayerCharacter::XboxTimerCheck()
{
	bXboxCheck = false;
}
/**C Key **/

void APlayerCharacter::CkeyDown()
{
	if (PlayerStatus == EPlayerStatus::EPS_DEAD) return;
	if (bShiftKeyDown)
	{
		bShiftKeyDown = false;
	}

	if (!bKnockBackStart)
	{
		bCkeyDown = !bCkeyDown;
		if (bCkeyDown)
		{
			SetMovementStatus(EMovementStatus::EMS_WALK);
		}
		else
		{
			SetMovementStatus(EMovementStatus::EMS_RUN);
		}
	}
}

/**EMovementStatus*/
void APlayerCharacter::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;

	if (Status == EMovementStatus::EMS_SPRINT)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else if (Status == EMovementStatus::EMS_RUN)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else if (Status == EMovementStatus::EMS_WALK)
	{
		bCkeyDown = true;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

/******************************************/
/******************Attack******************/
/******************************************/
void APlayerCharacter::LMBDown()
{
	if (PlayerStatus == EPlayerStatus::EPS_DEAD) return;
	Weapon->bDamageCheck = false;

	if (!bKnockBackStart &&!bCharging &&!bGraping  && !bChargingCheck)
	{
		bLMBDown = true;
	
		if (PlayerStatus == EPlayerStatus::EPS_DEAD || !(StaminaPoint >= MinActionStamina)) return;

		SetPlayerStatus(EPlayerStatus::EPS_ATTACK);
		if (bAttacking == false)
		{
			Attack();
		}
		else if (bAttacking == true)
		{
			bAttackButtonCheck = true;
		}
	}
}
void APlayerCharacter::LMBUp()
{
	bLMBDown = false;
}

void APlayerCharacter::Attack()
{
	if (!bRolling && !bCharging && PlayerStatus != EPlayerStatus::EPS_DEAD && !bKnockBackStart&&!bInGameManual)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (!GetCharacterMovement()->IsFalling())
		{
			if (!GetCharacterMovement()->GetLastInputVector().IsZero())
			{
				//SmoothPlayerRotator = KeyboardInputRotation();
			}
		}

		if (!AnimInstance || !CombatMontage) return;

		bAttacking = true;

		const char* comboList[] = { "Combo01", "Combo02" };

		//애니메이션이 실행중이지 않을때
		if (StaminaPoint >= MinActionStamina)
		{
			if (!(AnimInstance->Montage_IsPlaying(CombatMontage)))
			{
				SmoothPlayerRotator = KeyboardInputRotation();
				AnimInstance->Montage_Play(CombatMontage, 1.2f);
				StaminaPoint -= NormalAttackStaminaCost;

				Weapon->AttackPoint = NormalAttackADamage;
				DefaultHitStop = NormalAttackAHitStopTime;

				Weapon->PlayerKnockBackLevel = KnockBackNormalAttackA;
				
			}
			//애니메이션이 실행 중일때
			else if (AnimInstance->Montage_IsPlaying(CombatMontage))
			{
				if (!bHasLockOnTarget) 
				{
					SmoothPlayerRotator = KeyboardInputRotation();
				}
				AnimInstance->Montage_Play(CombatMontage, 1.2f);
				AnimInstance->Montage_JumpToSection(FName(comboList[ComboCount]), CombatMontage);
				StaminaPoint -= NormalAttackStaminaCost;

				Weapon->AttackPoint = NormalAttackBDamage;
				DefaultHitStop = NormalAttackBHitStopTime;

				Weapon->PlayerKnockBackLevel = KnockBackNormalAttackB;
			}
		}

	}
}
void APlayerCharacter::AttackEnd()
{

	bAttacking = false;
	bAttackButtonCheck = false;
	SetPlayerStatus(EPlayerStatus::EPS_NORMAL);
}
void APlayerCharacter::AttackInputChecking()
{

	if (ComboCount >= 1)
	{
		ComboCount = 0;
	}

	if (bAttackButtonCheck)
	{
		ComboCount += 1;
		bAttackButtonCheck = false;
		Attack();
	}
}

void APlayerCharacter::AttackCancel()
{
	SetPlayerStatus(EPlayerStatus::EPS_NORMAL);
	bAttacking = false;
	//if (bSpaceKeyDown) bRolling;

	float InputForwardValue = GetInputAxisValue(FName("MoveForward"));
	float InputRightValue = GetInputAxisValue(FName("MoveRight"));
	UE_LOG(LogTemp, Log, TEXT("InputForwardValue : %f"), InputForwardValue);
	UE_LOG(LogTemp, Log, TEXT("InputRightValue : %f"), InputRightValue);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (InputForwardValue != 0.0f || InputRightValue != 0.0f)
	{
		if (AnimInstance)
		{
			//AnimInstance->Montage_JumpToSectionsEnd(comboList[ComboCount], CombatMontage);
			AnimInstance->Montage_Stop(0.05f, CombatMontage);
		}
	}
}

void APlayerCharacter::KnockBackCancel()
{
	SetPlayerStatus(EPlayerStatus::EPS_NORMAL);
	bKnockBackStart = false;
	//if (bSpaceKeyDown) bRolling;

	float InputForwardValue = GetInputAxisValue(FName("MoveForward"));
	float InputRightValue = GetInputAxisValue(FName("MoveRight"));
	UE_LOG(LogTemp, Log, TEXT("InputForwardValue : %f"), InputForwardValue);
	UE_LOG(LogTemp, Log, TEXT("InputRightValue : %f"), InputRightValue);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (InputForwardValue != 0.0f || InputRightValue != 0.0f)
	{
		if (AnimInstance)
		{
			//AnimInstance->Montage_JumpToSectionsEnd(comboList[ComboCount], CombatMontage);
			AnimInstance->Montage_Stop(0.2f, CombatMontage);
		}
	}
}

FRotator APlayerCharacter::KeyboardInputRotation()
{
	float ForwardInput = GetInputAxisValue(FName("MoveForward"));
	float RightInput = GetInputAxisValue(FName("MoveRight"));

	FRotator LookAt = GetControlRotation();

	float YawRotation = 0.0f;

	if (RightInput == 0.0f)
	{
		YawRotation = ForwardInput >= 0.0f? LookAt.Yaw :LookAt.Yaw - 180.0f;
	}
	else
	{
		YawRotation = LookAt.Yaw + (ForwardInput* RightInput * -45.0f) + (RightInput * 90.0f);
	}

	UE_LOG(LogTemp, Warning, TEXT("%f"), YawRotation);

	return FRotator(0.0f, YawRotation, 0.0f);;
}




/****************************************/
/******************Roll******************/
/****************************************/
void APlayerCharacter::SpaceKeyDown()
{
	if (PlayerStatus == EPlayerStatus::EPS_DEAD) return;
	if (!bKnockBackStart&&!bInGameManual&&!bGraping&&!Weapon->bChargingHit &&KeyInputCheck())
	{
		bSpaceKeyDown = true;
		bAttackButtonCheck = false;
		RollStart();
		RollDeltaTime += GetWorld()->GetDeltaSeconds();
	}
}

void APlayerCharacter::XboxSpaceKeyDown()
{
	if (PlayerStatus == EPlayerStatus::EPS_DEAD) return;
	if (!bKnockBackStart && !bInGameManual && !bGraping && !Weapon->bChargingHit && !bXboxCheck && !bStaminaNoAction && KeyInputCheck())
	{
		bSpaceKeyDown = true;
		bAttackButtonCheck = false;
		RollStart();
		RollDeltaTime += GetWorld()->GetDeltaSeconds();
	}

}

void APlayerCharacter::SpaceKeyUp()
{
	bSpaceKeyDown = false;
}

void APlayerCharacter::RollStart()
{
	if (StaminaPoint >= MinActionStamina)
	{
		if (!bRolling && !bAttacking && !bCharging && PlayerStatus != EPlayerStatus::EPS_DEAD && !bKnockBackStart)
		{
			SetPlayerStatus(EPlayerStatus::EPS_ROLL);

			if (!GetCharacterMovement()->IsFalling())
			{
				if (!GetCharacterMovement()->GetLastInputVector().IsZero())
				{
					bRolling = true;

					RInterpMovement = RollMovementInput();

					PlayHighPriorityMontage(CombatMontage, FName("Roll"), 1.0f);

					StaminaPoint -= RollStaminaCost;

				}
			}

		}
		if (StaminaPoint <= 0)
			StaminaPoint = 0;
	}
}
void APlayerCharacter::RollEnd()
{
	bRolling = false;
	SetPlayerStatus(EPlayerStatus::EPS_NORMAL);
	RollDeltaTime = 0.f;
	if (bSpaceKeyDown) bRolling;
}
FRotator APlayerCharacter::RollMovementInput()
{
	FVector LastMovementInput = GetLastMovementInputVector();
	FRotator MovementRotation = LastMovementInput.Rotation();

	return MovementRotation;
}


void APlayerCharacter::RollinginvincibilityStart()
{
	binvincibility = true;
	UE_LOG(LogTemp, Log, TEXT("RollinginvincibilityStart"));
}

void APlayerCharacter::RollinginvincibilityEnd()
{
	binvincibility = false;
	UE_LOG(LogTemp, Log, TEXT("RollinginvincibilityEnd"));
}


void APlayerCharacter::RollCancel()
{
	SetPlayerStatus(EPlayerStatus::EPS_NORMAL);
	bRolling = false;

	if (bSpaceKeyDown) bRolling;

	float InputForwardValue = GetInputAxisValue(FName("MoveForward"));
	float InputRightValue = GetInputAxisValue(FName("MoveRight"));
	UE_LOG(LogTemp, Log, TEXT("InputForwardValue : %f"), InputForwardValue);
	UE_LOG(LogTemp, Log, TEXT("InputRightValue : %f"), InputRightValue);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (InputForwardValue != 0.0f || InputRightValue != 0.0f)
	{
		if (AnimInstance)
		{
			AnimInstance->Montage_Stop(0.2f, CombatMontage);
		}
	}
}

float APlayerCharacter::PlayHighPriorityMontage(UAnimMontage* Montage, FName StartSectionName, float InPlayRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance->Montage_IsPlaying(Montage))
	{
		UAnimMontage* HighMontage = Montage;

		return PlayAnimMontage(HighMontage, InPlayRate, StartSectionName);
	}

	return 1;
}


/*****************************************/
/******************RMB******************/
/*****************************************/
void APlayerCharacter::RMBDown()
{

	if (PlayerStatus == EPlayerStatus::EPS_DEAD) return;
	if (!bKnockBackStart && !bAttacking&& !bInGameManual&&!bGraping)
	{
		ShiftKeyUp();
		UE_LOG(LogTemp, Log, TEXT("RMBDOWN"));
		bRMBDown = true;
		StaminaPoint -= ChargeReadyStaminaCost;
		bCharging = true;
		bChargingCheck = true;
		Weapon->bChargingHit = true;
	}

}
void APlayerCharacter::RMBUp()
{
	UE_LOG(LogTemp, Log, TEXT("RMBUP"));
	bRMBDown = false;
	bCharging = false;
	bChargingOne = false;
	bChargingTwo = false;
}


/*****************************************/
/***************Camera LockOn*************/
/*****************************************/
void APlayerCharacter::RKeyDown()
{
	if (PlayerStatus == EPlayerStatus::EPS_DEAD&&!bInGameManual) return;

	if (bHasLockOnTarget)
	{
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); // at this rotation rate
		FollowCamera->UnLockTrace();
		bHasLockOnTarget = false;

	}
	else
	{
		LockOnTarget = FollowCamera->LockOnTrace();
		if (LockOnTarget != nullptr)
		{
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.f, 0.0f); // at this rotation rate
			bHasLockOnTarget = true;
		}
	}
	return;
}

/*****************************************/
/*************Charging Attack*************/
/*****************************************/
void APlayerCharacter::ChargingAttack(float value)
{
	Weapon->bDamageCheck = false;
	float ChargeTime = GetWorld()->GetDeltaSeconds();
	float ChargingStaminaCostDelta = ChargeTime * ChargingStaminaCost;

	if (StaminaPoint <= 0)
	{
		StaminaPoint = 0;
		RMBUp();
	}

	if (StaminaPoint >= 0)
	{
		SetPlayerStatus(EPlayerStatus::EPS_CHARGING);

		if (!bRolling && !bAttacking && PlayerStatus != EPlayerStatus::EPS_DEAD&&!bInGameManual)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			if (value >= 0.f && value < FirstChargeTime)
				//차징 0단계
			{
				UE_LOG(LogTemp, Log, TEXT("Charging_0"));
				StaminaPoint -= ChargingStaminaCostDelta;
				Weapon->AttackPoint = ChargeAttackNDamage;
				DefaultHitStop = ChargeAttackNHitStopTime;

				Weapon->PlayerKnockBackLevel = KnockBackChargeAttackN;
			}
			else if ((value >= FirstChargeTime && value < SecondChargeTime))
			{
				//차징 1단계
				UE_LOG(LogTemp, Log, TEXT("Charging_1"));
				bChargingOne = true;
				StaminaPoint -= ChargingStaminaCostDelta;
				Weapon->AttackPoint = ChargeAttackADamage;
				DefaultHitStop = ChargeAttackAHitStopTime;

				Weapon->bBossKnockbackCheck = true;

				Weapon->PlayerKnockBackLevel = KnockBackChargeAttackA;
			}
			else if ((value >= SecondChargeTime))
				//차징 2단계
			{
				UE_LOG(LogTemp, Log, TEXT("Charging_2"));
				ChargingTimer = ChargingComboMax;
				bChargingTwo = true;
				StaminaPoint -= ChargingStaminaCostDelta;
				Weapon->AttackPoint = ChargeAttackBDamage;
				DefaultHitStop = ChargeAttackBHitStopTime;

				Weapon->PlayerKnockBackLevel = KnockBackChargeAttackB;
			}
		}
	}
}
void APlayerCharacter::ChargingEnd()
{

	UE_LOG(LogTemp, Log, TEXT("ChargingEnd"));
	SetPlayerStatus(EPlayerStatus::EPS_NORMAL);
	bCharging = false;
	Weapon->bBossKnockbackCheck = false;
	Weapon->bChargingHit = false;
	bChargingCheck = false;
	//if (bRMBDown) bCharging;
}

void APlayerCharacter::ChargingCancelStart()
{
	
}

void APlayerCharacter::ChargingCancelEnd()
{

}

bool APlayerCharacter::KeyInputCheck()
{

	float InputForwardValue = GetInputAxisValue(FName("MoveForward"));
	float InputRightValue = GetInputAxisValue(FName("MoveRight"));

	if (InputForwardValue != 0.0f || InputRightValue != 0.0f)
	{
		return true;
	}


	return false;
}

/*****************************************/
/******************KnockBack***********/
/*****************************************/
void APlayerCharacter::KnockBackStart()
{
	if (PlayerStatus == EPlayerStatus::EPS_DEAD) return;

	Weapon->DeactivateCollsiion();

	Weapon->bChargingHit = false;
	SetPlayerStatus(EPlayerStatus::EPS_NORMAL);
	bKnockBackStart = true;
	bAttacking = false;
	bRolling = false;
	bChargingCheck = false;
	binvincibility = false;
	RMBUp();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && PlayerStatus != EPlayerStatus::EPS_DEAD)
	{
		AnimInstance->Montage_Play(CombatMontage);
		AnimInstance->Montage_JumpToSection(FName("KnockBack"), CombatMontage);
	}
	UE_LOG(LogTemp, Log, TEXT("KnockBackStart"));
}

void APlayerCharacter::KnockBackEnd()
{
	SetPlayerStatus(EPlayerStatus::EPS_NORMAL);
	bKnockBackStart = false;
}

FRotator APlayerCharacter::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

/*****************************************/
/******************Player Hp**************/
/*****************************************/
void APlayerCharacter::DecrementHealth(float Amount)
{
	if (HealthPoint - Amount <= 0.f)
	{
		HealthPoint -= Amount;
		Dead();
	}
	else
	{
		HealthPoint -= Amount;
	}
}
float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ADummyBossTest* Enemy = Cast<ADummyBossTest>(DamageCauser);
	ANormalMonster* Normal_Enemy = Cast<ANormalMonster>(DamageCauser);
	if (HealthPoint - DamageAmount <= 0.f)
	{

		HealthPoint -= DamageAmount;
		bKnockBackStart = false;
		Dead();

		//죽음 이미지 추가

		
		if (DamageCauser)
		{
			if (Enemy)
			{
				Enemy->bHasValidTarget = false;
			}
		}

		if (DamageCauser)
		{
			if (Normal_Enemy)
			{
				Normal_Enemy->bHasValidTarget = false;
			}
		}
	}
	else
	{
		HealthPoint -= DamageAmount;
	}

	return DamageAmount;
}

/// <summary>
/// HUD
/// </summary>
/// 
void APlayerCharacter::UpdateHUD(bool SetHUD)
{
	if (SetHUD)
	{
		UntitledPlayerController->DisplayEnemyHealthBar();
	}
	else
	{
		UntitledPlayerController->RemoveEnemyHealthBar();
	}
}






/*****************************************/
/******************Dead******************/
/*****************************************/
//Dead는 추후 애니메이션 추가시 추가 및 체력 추가 후 작업
void APlayerCharacter::Dead()
{
	bDead = true;
	SetPlayerStatus(EPlayerStatus::EPS_DEAD);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, DeadSpeed);
		AnimInstance->Montage_JumpToSection(FName("Death"),CombatMontage);
	}

}
void APlayerCharacter::DeadEnd()
{

	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	DeathNative();
}
void APlayerCharacter::DeathNative_Implementation()
{

}

void APlayerCharacter::ChapterOneClearUI_Implementation()
{

}

/**********************Potion********************************/
void APlayerCharacter::IncrementHealth()
{
	if (Potions > 0)
	{
		if (HealthPoint + HealthAmount > MaxHealthPoint)
		{
			HealthPoint = MaxHealthPoint;
		}
		else
		{
			//포션 애니 추가
			HealthPoint += HealthAmount;
		}
		GetMesh()->GetAnimInstance()->Montage_Play(PotionMontage);

		Potions--;
	}
}

void APlayerCharacter::IncrementHealthCount(int32 Amount)
{
	Potions += Amount;
}


void APlayerCharacter::PotionClickDown()
{
	if (PlayerStatus == EPlayerStatus::EPS_DEAD) return;

	if (!bInGameManual && !bKnockBackStart &&!bAttacking&&!bChargingCheck)
	{
		bPotionClickDown = true;
		IncrementHealth();
		
	}
}

void APlayerCharacter::PotionClickUp()
{
	bPotionClickDown = false;
}

void APlayerCharacter::GrapStart()
{
	if (PlayerStatus == EPlayerStatus::EPS_DEAD) return;
	bGraping = true;
	SetPlayerStatus(EPlayerStatus::EPS_NORMAL);

	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && PlayerStatus != EPlayerStatus::EPS_DEAD)
	{
		AnimInstance->Montage_Play(CombatMontage);
		AnimInstance->Montage_JumpToSection(FName("caught"), CombatMontage);
	}


}

void APlayerCharacter::GrapEnd()
{
	UE_LOG(LogTemp,Log,TEXT("GrapEnd_2"));
	bGraping = false;

	SetPlayerStatus(EPlayerStatus::EPS_KNOCKBACK);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	

}

void APlayerCharacter::UpdateCombatTarget()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);
	
	if (OverlappingActors.Num() == 0)
	{
		//add Remove HUD
		return;
	}

	ANormalMonster* ClosetEnemy = Cast<ANormalMonster>(OverlappingActors[0]);
	if (ClosetEnemy)
	{
		FVector Location = GetActorLocation();
		float MinDistance = (ClosetEnemy->GetActorLocation() - Location).Size();

		for (auto Actor : OverlappingActors)
		{
			ANormalMonster* Enemy = Cast<ANormalMonster>(Actor);

			if (Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation() - GetActorLocation()).Size();
				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;
					ClosetEnemy = Enemy;
				}
			}
		}

		//AddHUD
		SetNormalCombatTarget(ClosetEnemy);
		bHasNormalCombatTarget = true;
	}
}