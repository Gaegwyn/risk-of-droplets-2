// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/BaseProjectile.h"
#include "LaserGlaiveProjectile.generated.h"

/**
 * 
 */
UCLASS()
class RISKOFDROPLETS2_API ALaserGlaiveProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:
	// Set default values for this actor's properties
	ALaserGlaiveProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Callbacks
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTarget(const class AActor* Target);

	// Each time we bounce to another target we increase our damage delt by this
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float BounceDamageIncrease;

	UPROPERTY(EditAnywhere, Category = "Debugging")
	float SphereRadius;

private:

	void GetNextBestTarget();

	// Targets that have already been hit by the glaive. They have a lower priority
	UPROPERTY(VisibleAnywhere)
	TArray<const AActor*> PreviousTargets;

	// The maximum amount of times the laser glaive can bounce to another viable enemy
	UPROPERTY(EditAnywhere)
	int32 MaxBounces;

	UPROPERTY(VisibleAnywhere)
	int32 CurrBounces;
};
