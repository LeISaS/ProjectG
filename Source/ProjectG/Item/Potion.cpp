// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/world.h"
#include "../Character/PlayerCharacter.h"
// Sets default values
APotion::APotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	bRotate = false;
	RotationRate = 45.f;
	
}

// Called when the game starts or when spawned
void APotion::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APotion::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APotion::OnOverlapEnd);

}

// Called every frame
void APotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotate)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);
	}
}

void APotion::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Potion OnOverlapBegin()"));

	if (OtherActor)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		
		if (PlayerCharacter)
		{
			PlayerCharacter->IncrementHealthCount(1);

			if (PlayerCharacter->MaxPotions < PlayerCharacter->Potions)
			{
				PlayerCharacter->Potions = PlayerCharacter->MaxPotions;
			}

			//particles 

			//Destroy
			Destroy();
		}
	}
}

void APotion::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp,Log,TEXT("Potion OnOverlapEnd()"))
}

