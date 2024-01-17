// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TransferMeta.generated.h"

struct FDGMA_AncientStruct;
struct FDGMA_TurretStruct;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTransferMeta : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DGMA_API ITransferMeta
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void TransferMetaAncient(FDGMA_AncientStruct AncientPack) = 0;
	
	UFUNCTION()
	virtual void TransferMetaTurret(FDGMA_TurretStruct TurretPack) = 0;
};
