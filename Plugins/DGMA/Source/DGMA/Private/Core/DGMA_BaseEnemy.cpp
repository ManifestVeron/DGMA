// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DGMA_BaseEnemy.h"

#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADGMA_BaseEnemy::ADGMA_BaseEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root Component");
	SetRootComponent(Root);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision");
	BoxCollision->SetupAttachment(GetRootComponent());
	
	Health = 100.0f;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ADGMA_BaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADGMA_BaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ADGMA_BaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Health -= DamageAmount;
	if (Health <= 0.0f)
	{
		Destroy();
	}
	return DamageAmount;
}

void ADGMA_BaseEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADGMA_BaseEnemy, Team);
	DOREPLIFETIME(ADGMA_BaseEnemy, Health);
}