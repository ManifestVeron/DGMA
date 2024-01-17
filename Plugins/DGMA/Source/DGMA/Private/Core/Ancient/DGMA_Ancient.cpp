// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Ancient/DGMA_Ancient.h"

#include "Core/DGMA_GameMode.h"
#include "Core/DataBase/DGMA_TurretStruct.h"
#include "Net/UnrealNetwork.h"

void ADGMA_Ancient::TransferMetaTurret(FDGMA_TurretStruct) { /* not use */ }

ADGMA_Ancient::ADGMA_Ancient()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
}

void ADGMA_Ancient::TransferMetaAncient(FDGMA_AncientStruct AncientPack)
{
	SetMeta(AncientPack);
}

void ADGMA_Ancient::SetMeta_Implementation(FDGMA_AncientStruct AncientPack)
{
	AncientStruct = AncientPack;
	Team = AncientPack.Team;
	Mesh->SetMaterial(0, AncientPack.MaterialInstance);
}

void ADGMA_Ancient::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADGMA_Ancient::BeginPlay()
{
	Super::BeginPlay();
}

void ADGMA_Ancient::SERVER_Regeneration_Implementation()
{
	MULTICAST_Regeneration();
}

void ADGMA_Ancient::MULTICAST_Regeneration_Implementation()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ADGMA_Ancient::Regeneration, 1.0f, true, 2.0f);
}

void ADGMA_Ancient::Regeneration()
{
	if (Health < 100.f)
	{
		Health += 5.0f;
	}
}

void ADGMA_Ancient::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADGMA_Ancient, Mesh);
}
