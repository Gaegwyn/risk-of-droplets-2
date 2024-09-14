// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/BaseProjectile.h"

#include "StrafeProjectile.generated.h"

/**
 * 
 */
UCLASS()
class RISKOFDROPLETS2_API AStrafeProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:
	// Set default values for this actor's properties
	AStrafeProjectile();

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
};
