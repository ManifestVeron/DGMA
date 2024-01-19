// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DGMA_BaseEnemy.h"
#include "Core/UInterface/TransferMeta.h"
#include "DGMA_TurretPlace.generated.h"

class ADGMA_PlayerState;
/**
 * 
 */
UCLASS()
class DGMA_API ADGMA_TurretPlace : public ADGMA_BaseEnemy, public ITransferMeta
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	int32 Price = 0;

	UPROPERTY()
	ADGMA_PlayerState* DGMA_PlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
	
public:

	virtual void BeginPlay() override;
	
	// Interface
	virtual void TransferMetaAncient(FDGMA_AncientStruct AncientPack) override;
	virtual void TransferMetaTurret(FDGMA_TurretStruct TurretPack) override;

protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetMaterial(UMaterialInstance* MaterialInstance);
	
	UFUNCTION(NetMulticast,Reliable)
	void MULTICAST_SetMaterial(UMaterialInstance* MaterialInstance);

	UFUNCTION()
	void AddMoneyBeforeDead();

private:
	
	FScriptDelegate DelegateForBeforeDestroy;
};
