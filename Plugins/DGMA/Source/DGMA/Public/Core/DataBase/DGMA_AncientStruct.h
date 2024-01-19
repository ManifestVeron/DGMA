// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DGMA_AncientStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDGMA_AncientStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Team;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* MI_Main;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* MI_HalfHP;

	FDGMA_AncientStruct():Team(0),Health(100.0f),MI_Main(nullptr),MI_HalfHP(nullptr)
	{}

	FDGMA_AncientStruct(const FDGMA_AncientStruct& turret_pack)
	{
		Team = turret_pack.Team;
		Health = turret_pack.Health;
		MI_Main = turret_pack.MI_Main;
		MI_HalfHP = turret_pack.MI_HalfHP;
	}

	FDGMA_AncientStruct& operator= (const FDGMA_AncientStruct& torpedo_packet);
};

UCLASS()
class DGMA_API UDGMADataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	TArray<FDGMA_AncientStruct> AssetItems;
};