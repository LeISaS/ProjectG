// Fill out your copyright notice in the Description page of Project Settings.


#include "UntitledPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "../Character/PlayerCharacter.h"


void AUntitledPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bClickTitleMenu = false;

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
	}
	EnemyHealthBar->AddToViewport();
	EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);

	if (WInGameManual)
	{
		InGameManualBar = CreateWidget<UUserWidget>(this, WInGameManual);
	}

	InGameManualBar->AddToViewport();
	InGameManualBar->SetVisibility(ESlateVisibility::Hidden);
}

void AUntitledPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}
void AUntitledPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AUntitledPlayerController::DisplayInGameManual()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (InGameManualBar)
	{
		PC->bShowMouseCursor = true;
		bInGameManual = true;
		InGameManualBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AUntitledPlayerController::RemoveDisplayInGameManual()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (InGameManualBar)
	{
		PC->bShowMouseCursor = false;
		bInGameManual = false;
		InGameManualBar->SetVisibility(ESlateVisibility::Hidden);
	}

}
