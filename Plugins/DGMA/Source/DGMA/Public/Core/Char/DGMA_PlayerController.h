// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DGMA_PlayerController.generated.h"

class ADGMA_PlayerState;
class ADGMA_Char;
/**
 * 
 */
UCLASS()
class DGMA_API ADGMA_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "DGMA_Char")
	TSubclassOf<ADGMA_Char> DGMA_Char = nullptr;
	
	int32 Team;

public:
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SERVER_SpawnPlayer(FName TagTeam, int32 id_Team, APlayerController* PlayerController);

	UFUNCTION(BlueprintImplementableEvent)
	void InitEnhancedInpute(APlayerController* LocalPlayerController);
};
