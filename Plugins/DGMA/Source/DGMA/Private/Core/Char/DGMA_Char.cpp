// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Char/DGMA_Char.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/TextRenderComponent.h"
#include "Core/Ancient/DGMA_Ancient.h"
#include "Core/Char/DGMA_PlayerController.h"
#include "Core/Char/DGMA_PlayerState.h"
#include "Core/DataBase/DGMA_AncientStruct.h"
#include "Core/Turret/DGMA_TurretGhost.h"
#include "Core/Turret/DGMA_TurretPlace.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogDGMA_Char);


// Sets default values
ADGMA_Char::ADGMA_Char()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render"));
	TextRenderComponent->SetupAttachment(GetRootComponent());
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

// Called when the game starts or when spawned
void ADGMA_Char::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void ADGMA_Char::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADGMA_Char::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADGMA_Char::Look);

		PlayerInputComponent->BindAction("SpawnSelectedTurret",IE_Pressed,this,&ADGMA_Char::SERVER_SpawnSelectedTurret);
	}
	else
	{
		UE_LOG(LogDGMA_Char, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADGMA_Char::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADGMA_Char::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Called every frame
void ADGMA_Char::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsBuilding)
	{
		if(bIsSpawnedGhost)
		{
			SERVER_SpawnGhost();
			bIsSpawnedGhost = false;
		}
		SERVER_MovingGhost();
	}
	else if(!bIsBuilding && !bIsSpawnedGhost)
	{
		if(IsValid(TurretGhost)) TurretGhost->Destroy();
		bIsSpawnedGhost = true;
	}
}

void ADGMA_Char::SERVER_SpawnGhost_Implementation()
{
	TurretGhost = GetWorld()->SpawnActor<ADGMA_TurretGhost>
	(
		DGMA_TurretGhost,
		GetActorLocation(),
		GetActorRotation()-FRotator(0.0f,90.0f,0.0f)
	);
}

void ADGMA_Char::SERVER_MovingGhost_Implementation()
{
	TArray<AActor*> ActorsToIgnor;
	ActorsToIgnor.Add(TurretGhost);
	FHitResult HitResult;
	const FCollisionQueryParams Params;
	FVector StartTrace = FollowCamera->GetComponentLocation();
	FVector EndTrace = FollowCamera->GetForwardVector()*1000.0f + StartTrace;
	
	if(UKismetSystemLibrary::LineTraceSingle(GetWorld(),StartTrace,EndTrace,ETraceTypeQuery::TraceTypeQuery1,false,ActorsToIgnor,EDrawDebugTrace::None,HitResult,true))
	{
		UKismetSystemLibrary::LineTraceSingle(GetWorld(),HitResult.Location + FVector(0.0f,0.0f,500.0f),HitResult.Location - FVector(0.0f,0.0f,50.0f),ETraceTypeQuery::TraceTypeQuery1,false,ActorsToIgnor,EDrawDebugTrace::None,HitResult,true);
		TurretGhost->SetActorLocation(HitResult.Location);
	}
	else
	{
		UKismetSystemLibrary::LineTraceSingle(GetWorld(),HitResult.TraceEnd,HitResult.TraceEnd - FVector(0.0f,0.0f,500.0f),ETraceTypeQuery::TraceTypeQuery1,false,ActorsToIgnor,EDrawDebugTrace::None,HitResult,true);
		TurretGhost->SetActorLocation(HitResult.Location);
	}
}

void ADGMA_Char::SERVER_SpawnSelectedTurret_Implementation()
{
	if(IsValid(TurretGhost))
	{
		if(bIsBuilding && TurretGhost->bIsSpawn && bIsNoCooldown && DGMA_PlayerState->Money >= SelectedTurret.Price)
		{
			DGMA_PlayerState->Money -= SelectedTurret.Price;
			
			bIsBuilding = false;
			
			FActorSpawnParameters SpawnParameter;
			SpawnParameter.Owner = DGMA_PlayerState;
			SpawnParameter.Instigator = this;
			
			TurretPlace = GetWorld()->SpawnActor<ADGMA_TurretPlace>(DGMA_TurretPlace,TurretGhost->GetTransform(),SpawnParameter);
			
			SelectedTurret.Team = Team;
			
			TurretPlace->TransferMetaTurret(SelectedTurret);
			
			TurretGhost->Destroy();
			
			GetWorldTimerManager().SetTimer(TimerHandleForCooldown,this,&ADGMA_Char::CooldownStart,1.0f,true);
			
			bIsSpawnedGhost = true;
		}
	}
}

void ADGMA_Char::CooldownStart()
{
	SERVER_Cooldown();
}

void ADGMA_Char::SERVER_Cooldown_Implementation()
{
	if(Cooldown > 0)
	{
		bIsNoCooldown = false;
		--Cooldown;
		OnRep_SetCooldown();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandleForCooldown);
		bIsNoCooldown = true;
		Cooldown = 5;
		OnRep_SetCooldown();
	}
}

void ADGMA_Char::OnRep_SetCooldown() const
{
	TextRenderComponent->SetText(UKismetTextLibrary::Conv_IntToText(Cooldown));
}

void ADGMA_Char::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	DGMA_PlayerController = Cast<ADGMA_PlayerController>(NewController);
	DGMA_PlayerState = Cast<ADGMA_PlayerState>(NewController->PlayerState);
	
	Team = DGMA_PlayerController->Team;
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;

	const auto SpawnedAncient = GetWorld()->SpawnActor<ADGMA_Ancient>
	(
		DGMA_Ancient,
		GetActorLocation()-FVector(300.0f,0.0f,0.0f),
		GetActorRotation(),
		SpawnParameters
	);
	
	if(SpawnedAncient && DGMA_DataAssetsAncient)
	{
		SpawnedAncient->TransferMetaAncient(DGMA_DataAssetsAncient->AssetItems[Team - 1]);
	}
	else UE_LOG(LogDGMA_Char,Error,TEXT("Check DGMA_DataAssetsAncient and DGMA_Ancient in Character "));
}

void ADGMA_Char::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADGMA_Char, TextRenderComponent);
	DOREPLIFETIME(ADGMA_Char, Cooldown);
	
}