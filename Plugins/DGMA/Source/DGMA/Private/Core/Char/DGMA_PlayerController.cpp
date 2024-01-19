// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Char/DGMA_PlayerController.h"

#include "Core/DGMA_GameMode.h"
#include "Core/Char/DGMA_PlayerState.h"
#include "Kismet/GameplayStatics.h"

void ADGMA_PlayerController::SERVER_SpawnPlayer_Implementation(FName TagTeam, int32 id_Team, APlayerController* PlayerController)
{
	Team = id_Team;
	
	Cast<ADGMA_GameMode>(UGameplayStatics::GetGameMode(GetWorld()))->SpawnPlayer(TagTeam, this, DGMA_Char);

	check(PlayerController);
	InitEnhancedInpute(PlayerController);
}
