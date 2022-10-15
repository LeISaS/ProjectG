// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "../Character/PlayerCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "../Boss/DummyBossTest.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
#include "../NormalMonster.h"

// Sets default values
AWeapon::AWeapon() 
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Combat"));
	CombatCollision->SetupAttachment(SkeletalMesh, FName("Weapon"));

	AttackPoint = 100.f;
	bDamageCheck = false;
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel7, ECollisionResponse::ECR_Overlap);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Overlap);

	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	

	SkeletalMesh->SetSimulatePhysics(false);
	PlayerKnockBackLevel = 0;
	EnemyKnockBackLevel = 0;
}

void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ADummyBossTest* Enemy = Cast<ADummyBossTest>(OtherActor);
		ANormalMonster* NormalMonster = Cast<ANormalMonster>(OtherActor);
		if (Enemy)
		{
			if (DamageTypeClass && !bDamageCheck)
			{
				bDamageCheck = true;
				UGameplayStatics::ApplyDamage(Enemy, AttackPoint, WeaponInstigator, this, DamageTypeClass);	
				UE_LOG(LogTemp, Log, TEXT("AttackPoint : %f"), AttackPoint);
			
				if (PlayerKnockBackLevel>Enemy->DefaultKnockBackLevel)
				{
					Enemy->SetDummyBossKnockBackState(EDummyBossKnockBackState::EKBS_AIRKNOCKBACK);
				}
				else if(PlayerKnockBackLevel== Enemy->DefaultKnockBackLevel)
				{
					Enemy->SetDummyBossKnockBackState(EDummyBossKnockBackState::EKBS_KNOCKBACK);
				}
				else
				{
					Enemy->SetDummyBossKnockBackState(EDummyBossKnockBackState::EKBS_IDLE);
				}

				const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("EffectSocket");
				if (WeaponSocket)
				{
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);
					/*UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Enemy->NiagarahitEffect, SocketLocation, Enemy->NiagarahitEffectRotator,
						Enemy->NiagarahitEffectScale, true, true, ENCPoolMethod::None, true);*/

					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, SocketLocation, FRotator(0.f),
						Enemy->HitparticlesScales, true, EPSCPoolMethod::None, true);
					UE_LOG(LogTemp, Log, TEXT("Effect Test"));
				}
			}
		}

		if (NormalMonster)
		{
			if (DamageTypeClass&&!bDamageCheck)
			{
				bDamageCheck = true;
				UGameplayStatics::ApplyDamage(NormalMonster, AttackPoint, WeaponInstigator, this, DamageTypeClass);
				UE_LOG(LogTemp, Log, TEXT("AttackPoint : %f"), AttackPoint);

				if (PlayerKnockBackLevel > NormalMonster->DefaultKnockBackLevel)
				{
					NormalMonster->SetNormalMonsterKnockBackStatus(ENormalMonsterKnockBackStatus::EMKS_KNOCKBACK);
				}

				const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("EffectSocket");
				if (WeaponSocket)
				{
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);

					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NormalMonster->HitParticles, SocketLocation, FRotator(0.f),
						NormalMonster->HitparticlesScales, true, EPSCPoolMethod::None, true);
				}
			}
		}
	}
}

void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AWeapon::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AWeapon::DeactivateCollsiion()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
