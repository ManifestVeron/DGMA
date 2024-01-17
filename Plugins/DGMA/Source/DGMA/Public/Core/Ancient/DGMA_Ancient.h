// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DGMA_BaseEnemy.h"
#include "Core/DataBase/DGMA_AncientStruct.h"
#include "Core/UInterface/TransferMeta.h"
#include "DGMA_Ancient.generated.h"

/**
 * 
 */
UCLASS()
class DGMA_API ADGMA_Ancient : public ADGMA_BaseEnemy, public ITransferMeta
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
	UStaticMeshComponent* Mesh;

	FDGMA_AncientStruct AncientStruct;

public:

	// Interface
	virtual void TransferMetaAncient(FDGMA_AncientStruct AncientPack) override;
	virtual void TransferMetaTurret(FDGMA_TurretStruct TurretPack) override;
	
protected:

	ADGMA_Ancient();
	

	// Pawn Overriding
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

private:

	UFUNCTION(Server,Reliable)
	void SERVER_Regeneration();

	UFUNCTION(NetMulticast,Reliable)
	void MULTICAST_Regeneration();
	
	void Regeneration();
	
	UFUNCTION(NetMulticast,Reliable)
	void SetMeta(FDGMA_AncientStruct AncientPack);
	
};
