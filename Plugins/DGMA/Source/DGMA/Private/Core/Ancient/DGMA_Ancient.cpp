// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Ancient/DGMA_Ancient.h"

#include "Core/DataBase/DGMA_TurretStruct.h"
#include "Net/UnrealNetwork.h"

void ADGMA_Ancient::TransferMetaTurret(FDGMA_TurretStruct) { /* not use */ }

ADGMA_Ancient::ADGMA_Ancient()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetIsReplicated(true);
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
	Mesh->SetMaterial(0, AncientPack.MI_Main);
}

void ADGMA_Ancient::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADGMA_Ancient::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		SERVER_Regeneration();
	}
}
	

void ADGMA_Ancient::SERVER_Regeneration_Implementation()
{
	MULTICAST_Regeneration();
}

void ADGMA_Ancient::MULTICAST_Regeneration_Implementation()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ADGMA_Ancient::Regeneration, 1.0f, true, 2.0f);
}

void ADGMA_Ancient::Regeneration()
{
	if (Health < 100.0f)
	{
		Health += 5.0f; // Regen
		
		if(Health <= 50.0f)
		{
			Mesh->SetMaterial(0, AncientStruct.MI_HalfHP);
			
			if(Health <= 0)
			{
				GetWorldTimerManager().ClearTimer(TimerHandle);
				
				Destroy();
			}
		}
		else Mesh->SetMaterial(0, AncientStruct.MI_Main);
	}
}

void ADGMA_Ancient::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADGMA_Ancient, Mesh);
}
