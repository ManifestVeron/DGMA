// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DGMA_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DGMA_API ADGMA_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "DGMA_PlayerState")
	int32 Money;
	
};
