// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <Runtime/MediaAssets/Public/MediaPlayer.h>
#include "UntitledPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTG_API AUntitledPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> WEnemyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* EnemyHealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> WInGameManual;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* InGameManualBar;

public:
	bool bEnemyHealthBarVisible;
	bool bInGameManual;
	bool bClearBarVisible;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	bool bClickTitleMenu;

	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();
	
	void DisplayInGameManual();
	UFUNCTION(BlueprintCallable)
	void RemoveDisplayInGameManual();

	


protected:
	virtual void BeginPlay() override;
};
