// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DGMA_BaseEnemy.h"
#include "DGMA_TurretGhost.generated.h"

/**
 * 
 */
UCLASS()
class DGMA_API ADGMA_TurretGhost : public ADGMA_BaseEnemy
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsSpawn;

	ADGMA_TurretGhost();
};
