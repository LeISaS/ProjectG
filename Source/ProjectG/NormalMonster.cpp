// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalMonster.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/PlayerCharacter.h"
#include "AIController.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ANormalMonster::ANormalMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->SetCollisionProfileName("FSM_Enemy");

	AgroSphere->InitSphereRadius(600.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.f);

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetMesh(), FName("EnemySocket"));

	Health = 100.f;
	MaxHealth = 100.f;

	Damage = 10.f;
	DeathDelay = 3.f;
	AttackMinTime = 0.5f;
	AttackMaxTime = 1.0f;
 	NormalMonsterMovementStatus = ENormalMonsterMovementStatus::EMS_IDLE;

	bAttackInterp = false;
	bAttacking = false; 
	bHasValidTarget = false;
	bOverlappingCombatSphere = false;
	bKnockBackStart = false;
	
	bDamageCheck = false;
}

// Called when the game starts or when spawned
void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &ANormalMonster::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &ANormalMonster::AgroSphereOnOverlapEnd);
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &ANormalMonster::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &ANormalMonster::CombatSphereOnOverlapEnd);
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &ANormalMonster::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &ANormalMonster::CombatOnOverlapEnd);


	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Overlap);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


}

// Called every frame
void ANormalMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	switch (NormalMonsterMovementStatus)
	{
	case ENormalMonsterMovementStatus::EMS_IDLE:
		if (AIController)
		{
			AIController->StopMovement();
		}
		break;
	case ENormalMonsterMovementStatus::EMS_MOVETOTARGET:
		break;
	case ENormalMonsterMovementStatus::EMS_ATTACKING:
		if (PlayerCombatTarget)
		{
			this->MoveToTarget(PlayerCombatTarget);
		}
		break;
	case ENormalMonsterMovementStatus::EMS_Dead:
		if (AIController)
		{
			AIController->StopMovement();
		}
		

		break;
	case ENormalMonsterMovementStatus::EMS_MAX:
		break;
	default:
		break;
	}



	switch (NormalMonsterStatus)
	{
	case ENormalMonsterStatus::ENMS_AGRO:
		break;
	case ENormalMonsterStatus::ENMS_COMBAT:
		break;
	case ENormalMonsterStatus::ENMS_MAX:
		break;
	default:
		break;
	}

	switch (NormalMonsterKnockBackStatus)
	{
	case ENormalMonsterKnockBackStatus::EMKS_IDLE:
		break;
	case ENormalMonsterKnockBackStatus::EMKS_KNOCKBACK:
		if (Health > 0.f && !bKnockBackStart)
		{
			//KnockBackStart();
		}
		break;
	case ENormalMonsterKnockBackStatus::EMKS_MAX:
		break;
	default:
		break;
	}

	if (bAttackInterp&&Alive())
	{
		FVector CurrentLocation = GetActorLocation();

		InterpSpeed = FMath::RandRange(0.f, 1.0f);

		FVector Interp = FMath::VInterpTo(CurrentLocation, GetPlayerLocation(), DeltaTime, InterpSpeed);

		SetActorLocation(Interp);
	}

}

FVector ANormalMonster::GetPlayerLocation()
{
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	return PlayerLocation;
}

// Called to bind functionality to input
void ANormalMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ANormalMonster::MoveToTarget(APlayerCharacter* Target)
{
	SetNormalMovementStatus(ENormalMonsterMovementStatus::EMS_MOVETOTARGET);

	if (AIController&& Alive())
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);

		MoveRequest.SetAcceptanceRadius(10.0f);

		FNavPathSharedPtr NavPath;
		AIController->MoveTo(MoveRequest, &NavPath);
	}
}


//Combat OverlapBegin
void ANormalMonster::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		SetNormalStatus(ENormalMonsterStatus::ENMS_AGRO);

		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

		if (Player && !bAttacking)
		{
			UE_LOG(LogTemp,Log,TEXT("AgroSphereOverlapBegin"));
			MoveToTarget(Player);
		}
	}
}

void ANormalMonster::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (Player&&Alive())
		{
			bHasValidTarget = false;

			if (Player->NormalMonster == this)
			{
				Player->SetCombatTarget(nullptr);
			}
			Player->SetHasNormalCombatTarget(false);

			SetNormalMovementStatus(ENormalMonsterMovementStatus::EMS_IDLE);

			Player->UpdateCombatTarget();

			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
	
}

void ANormalMonster::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		SetNormalStatus(ENormalMonsterStatus::ENMS_COMBAT);
		if (Player)
		{
			MoveToTarget(Player);
			bHasValidTarget = true;
			
			Player->SetHasNormalCombatTarget(true);
			Player->SetNormalCombatTarget(this);
			Player->UpdateCombatTarget();

			PlayerCombatTarget = Player;
			bOverlappingCombatSphere = true;
			
			//AttackTimer;
			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &ANormalMonster::Attack, AttackTime);
		}

		
	}
}

void ANormalMonster::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherComp)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		SetNormalStatus(ENormalMonsterStatus::ENMS_AGRO);

		if (Player && !bAttacking && Alive())
		{
			bOverlappingCombatSphere = false;
			MoveToTarget(Player);
			PlayerCombatTarget = nullptr;

			if (Player->NormalMonster == this)
			{
				Player->SetNormalCombatTarget(nullptr);
				Player->bHasNormalCombatTarget = false;
				Player->UpdateCombatTarget();
			}

			//Add HUD
			GetWorldTimerManager().ClearTimer(AttackTimer);
		}

	}

}

void ANormalMonster::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (Player)
		{	
			if (DamageTypeClass && !bDamageCheck)
			{
				bDamageCheck = true;
				UE_LOG(LogTemp, Log, TEXT("DamageTypeClass"));
				UGameplayStatics::ApplyDamage(Player, Damage, AIController, this, DamageTypeClass);
			}
		}
	}
}

void ANormalMonster::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}




bool ANormalMonster::Alive()
{
	return GetNormalMonsterMovementStatus() != ENormalMonsterMovementStatus::EMS_Dead;
}

void ANormalMonster::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ANormalMonster::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ANormalMonster::Attack()
{
	if (Alive() && bHasValidTarget)
	{
		bDamageCheck = false;
		if (AIController)
		{
			AIController->StopMovement();
			SetNormalMovementStatus(ENormalMonsterMovementStatus::EMS_ATTACKING);
		}
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!bAttacking && CombatMontage && AnimInstance&& Alive())
	{
		bAttacking = true;
		
		int32 Section = FMath::RandRange(0, 1);

		switch (Section)
		{
		case 0:
			AnimInstance->Montage_Play(CombatMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
			break;
		case 1:
			AnimInstance->Montage_Play(CombatMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
			break;
		}


	}
}


void ANormalMonster::AttackEnd(AActor* OtherActor)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	SetNormalMovementStatus(ENormalMonsterMovementStatus::EMS_IDLE);
	bAttacking = false;
	MoveToTarget(Player);

}

float ANormalMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if (Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount;
		Die(DamageCauser);
	}
	else
	{
		Health -= DamageAmount;
	}
	return DamageAmount;
}

void ANormalMonster::Die(AActor* Causer)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	SetNormalMovementStatus(ENormalMonsterMovementStatus::EMS_Dead);
	SetNormalMonsterKnockBackStatus(ENormalMonsterKnockBackStatus::EMKS_IDLE);
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);

	}
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bAttacking = false;

	APlayerCharacter* Player = Cast<APlayerCharacter>(Causer);

	if (Player)
	{
		Player->UpdateCombatTarget();
	}
}

void ANormalMonster::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &ANormalMonster::Disappear, DeathDelay);
}


void ANormalMonster::Disappear()
{
	Destroy();
}

void ANormalMonster::AttackInterpStart()
{
	if (Alive() && bHasValidTarget)
	{
		bAttackInterp = true;
	}
}

void ANormalMonster::AttackInterpEnd()
{
	if (Alive() && bHasValidTarget)
	{
		bAttackInterp = false;
	}
}



float ANormalMonster::PlayHighPriorityMontage(UAnimMontage* Montage, FName StartSectionName, float InPlayRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance->Montage_IsPlaying(Montage))
	{
		UAnimMontage* HighMontage = Montage;

		return PlayAnimMontage(HighMontage, InPlayRate, StartSectionName);
	}

	return 0.0f;
}

void ANormalMonster::KnockBackStart()
{
	if (NormalMonsterMovementStatus == ENormalMonsterMovementStatus::EMS_Dead)
		return;

	SetNormalMonsterKnockBackStatus(ENormalMonsterKnockBackStatus::EMKS_KNOCKBACK);
	bAttacking = false;
	bKnockBackStart = true;

	PlayHighPriorityMontage(CombatMontage, "KnockBack", 1.0f);
	
}

void ANormalMonster::KnocKBackEnd()
{
	SetNormalMovementStatus(ENormalMonsterMovementStatus::EMS_IDLE);
	bKnockBackStart = false;
	
}

