// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/StrafeProjectile.h"

AStrafeProjectile::AStrafeProjectile()
{
	GetProjectileMovement()->bIsHomingProjectile = true;
	GetProjectileMovement()->ProjectileGravityScale = 0.0f;	// No gravity
	GetProjectileMovement()->HomingAccelerationMagnitude = 500.0f;

	DamageInfo.Amount = 100.0f;
}

void AStrafeProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetBoxCollision()->OnComponentBeginOverlap.AddDynamic(this, &AStrafeProjectile::OnBeginOverlap);
}

void AStrafeProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other->GetRootComponent() == GetProjectileMovement()->HomingTargetComponent)
	{
		// TODO: Apply Damage and whatever status effects to target
		UE_LOG(LogTemp, Log, TEXT("SUCCESS: Collided with Target!!"));
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SKIP: Collided with something that is not the target!!"));
	}

}

void AStrafeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStrafeProjectile::SetTarget(const AActor* Target)
{
	GetProjectileMovement()->HomingTargetComponent = Target->GetRootComponent();
}
