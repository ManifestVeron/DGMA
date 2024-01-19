// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Turret/DGMA_TurretPlace.h"

#include "Core/Char/DGMA_PlayerState.h"
#include "Core/DataBase/DGMA_AncientStruct.h"
#include "Core/DataBase/DGMA_TurretStruct.h"

void ADGMA_TurretPlace::TransferMetaAncient(FDGMA_AncientStruct AncientPack){ /* not use */}

void ADGMA_TurretPlace::TransferMetaTurret(FDGMA_TurretStruct TurretPack)
{
	Team = TurretPack.Team;
	Price = TurretPack.Price;
	Damage = TurretPack.Damage;
	
	MULTICAST_SetMaterial(TurretPack.MaterialInstance);

	DelegateForBeforeDestroy.BindUFunction(this, "AddMoneyBeforeDead");
	OnDestroyed.Add(DelegateForBeforeDestroy);
}

void ADGMA_TurretPlace::MULTICAST_SetMaterial_Implementation(UMaterialInstance* MaterialInstance)
{
	SetMaterial(MaterialInstance);
}

void ADGMA_TurretPlace::BeginPlay()
{
	Super::BeginPlay();
	
	DGMA_PlayerState = Cast<ADGMA_PlayerState>(GetOwner());
}

void ADGMA_TurretPlace::AddMoneyBeforeDead()
{
	DGMA_PlayerState->Money += Price/5;

	DelegateForBeforeDestroy.Clear();
	OnDestroyed.Remove(DelegateForBeforeDestroy);
}
