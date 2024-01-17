// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Char/DGMA_PlayerState.h"

#include "Net/UnrealNetwork.h"

void ADGMA_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADGMA_PlayerState, Money);
}
