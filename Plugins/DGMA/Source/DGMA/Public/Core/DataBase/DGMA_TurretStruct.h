// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DGMA_TurretStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDGMA_TurretStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* MaterialInstance;

	FDGMA_TurretStruct():Team(0),Price(0),Health(100.0f),Damage(5.0f),Icon(nullptr),MaterialInstance(nullptr)
	{}

	FDGMA_TurretStruct(const FDGMA_TurretStruct& turret_pack)
	{
		Name = turret_pack.Name;
		Team = turret_pack.Team;
		Price = turret_pack.Price;
		Health = turret_pack.Health;
		Damage = turret_pack.Damage;
		Icon = turret_pack.Icon;
		MaterialInstance = turret_pack.MaterialInstance;
	}

	FDGMA_TurretStruct& operator= (const FDGMA_TurretStruct& torpedo_packet);
};

