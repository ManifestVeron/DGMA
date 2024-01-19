// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DGMA_GameMode.h"

#include "Core/Char/DGMA_Char.h"
#include "Core/Char/DGMA_PlayerController.h"
#include "Core/DataBase/DGMA_AncientStruct.h"
#include "Core/DataBase/DGMA_TurretStruct.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ADGMA_GameMode::TransferMetaTurret(FDGMA_TurretStruct TurretPack) { /* not use */ }

// END GAME
void ADGMA_GameMode::TransferMetaAncient(FDGMA_AncientStruct AncientPack)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Green,
						FString::Printf(TEXT("TEAM %i WIN"), AncientPack.Team));
	
	for (int player = 0; player < GetNumPlayers(); player++)
	{
		UGameplayStatics::GetPlayerController(GetWorld(),player)->GetPawn()->Destroy();
	}
}

void ADGMA_GameMode::SpawnPlayer(FName TagPlayerController, ADGMA_PlayerController* LocalPlayerController, TSubclassOf<ADGMA_Char> Char) const
{
	if(IsValid(Char))
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),APlayerStart::StaticClass(),TagPlayerController,OutActors);

		const int32 Elem = FMath::RandRange(0, OutActors.Num()-1);
		
		if (OutActors[Elem])
		{
			ADGMA_Char* LinkChar = GetWorld()->SpawnActor<ADGMA_Char>(Char,OutActors[Elem]->GetTransform());
			LinkChar->Team = LocalPlayerController->Team;
			
			LocalPlayerController->Possess(LinkChar);
		}
		else UE_LOG(LogTemp,Error,TEXT("Add player starts with team tags to the game"));
	}
	else UE_LOG(LogTemp,Error,TEXT("Add DGMA_Char in DGMA_PlayerController"));
}
