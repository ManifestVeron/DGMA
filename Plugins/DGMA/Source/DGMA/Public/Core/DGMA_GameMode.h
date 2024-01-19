// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UInterface/TransferMeta.h"
#include "DGMA_GameMode.generated.h"

class ADGMA_Char;
class ADGMA_PlayerController;
/**
 * 
 */
UCLASS()
class DGMA_API ADGMA_GameMode : public AGameModeBase, public ITransferMeta
{
	GENERATED_BODY()

public:

	void SpawnPlayer(FName TagPlayerController, ADGMA_PlayerController* PlayerController, TSubclassOf<ADGMA_Char> Char) const;

	// Interface
	UFUNCTION(BlueprintCallable)
	virtual void TransferMetaAncient(FDGMA_AncientStruct AncientPack) override;
	virtual void TransferMetaTurret(FDGMA_TurretStruct TurretPack) override;
};
