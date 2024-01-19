// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DGMA_BaseEnemy.generated.h"

UCLASS()
class DGMA_API ADGMA_BaseEnemy : public AActor
{
	GENERATED_BODY()
	
protected:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 Team;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float Health;

protected:

	// Sets default values for this actor's properties
	ADGMA_BaseEnemy();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FInstigatedAnyDamageSignature Damage;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
