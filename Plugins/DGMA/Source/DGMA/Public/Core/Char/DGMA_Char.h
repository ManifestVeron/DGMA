// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/DataBase/DGMA_TurretStruct.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DGMA_Char.generated.h"

class ADGMA_PlayerState;
class ADGMA_PlayerController;
class ADGMA_TurretPlace;
class ADGMA_TurretGhost;
class ADGMA_Ancient;
class UDGMADataAsset;
class ITransferMeta;

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogDGMA_Char, Log, All);

UCLASS(config=Game)
class ADGMA_Char : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "DGMA_Char")
	int32 Team;

	UPROPERTY()
	ADGMA_PlayerState* DGMA_PlayerState;
	
protected:
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Replicated, Category = "DGMA_Char")
	class UTextRenderComponent* TextRenderComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "DGMA_Char")
	TSubclassOf<ADGMA_Ancient> DGMA_Ancient = nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "DGMA_Char")
	TSubclassOf<ADGMA_TurretGhost> DGMA_TurretGhost = nullptr;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "DGMA_Char")
	TSubclassOf<ADGMA_TurretPlace> DGMA_TurretPlace = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "DGMA_Char")
	UDGMADataAsset* DGMA_DataAssetsAncient;
	
	UPROPERTY(BlueprintReadWrite, Category = "DGMA_Char")
	bool bIsBuilding = false;

	UPROPERTY(BlueprintReadWrite, Category = "DGMA_Char")
	FDGMA_TurretStruct SelectedTurret;

	UPROPERTY()
	ADGMA_PlayerController* DGMA_PlayerController;

private:

	UPROPERTY(ReplicatedUsing = OnRep_SetCooldown)
	int32 Cooldown = 5;
	
	UPROPERTY()
	ADGMA_TurretGhost* TurretGhost;

	UPROPERTY()
	ADGMA_TurretPlace* TurretPlace;
	
	bool bIsSpawnedGhost = true;
	bool bIsNoCooldown = true;
	
	FTimerHandle TimerHandleForCooldown;

public:
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
private:

	ADGMA_Char();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	virtual void PossessedBy(AController* NewController) override;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	
	UFUNCTION(Server,Reliable)
	void SERVER_SpawnGhost();

	UFUNCTION(Server,Reliable)
	void SERVER_MovingGhost();

	UFUNCTION(Server,Reliable)
	void SERVER_SpawnSelectedTurret();

	UFUNCTION(Server,Reliable)
	void SERVER_Cooldown();
	
	void CooldownStart();

	UFUNCTION()
	void OnRep_SetCooldown() const;
};