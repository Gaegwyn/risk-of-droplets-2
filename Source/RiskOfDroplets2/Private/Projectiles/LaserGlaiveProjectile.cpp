// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/LaserGlaiveProjectile.h"

#include "BaseEnemy.h"
#include "Engine/OverlapResult.h"

ALaserGlaiveProjectile::ALaserGlaiveProjectile()
{
	GetProjectileMovement()->bIsHomingProjectile = true;
	GetProjectileMovement()->ProjectileGravityScale = 0.0f;	// No gravity
	GetProjectileMovement()->HomingAccelerationMagnitude = 5000.0f;	// Want this to bee-line toward next target

	// Configure the base damage that is applied to targets
	DamageInfo.Amount = 125.0f;
	BounceDamageIncrease = 1.1f;

	// Configure how many times we want this to bounce
	MaxBounces = 6;
	CurrBounces = 1;

	/* Debugging */
	SphereRadius = 5.0f;
}

void ALaserGlaiveProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetBoxCollision()->OnComponentBeginOverlap.AddDynamic(this, &ALaserGlaiveProjectile::OnBeginOverlap);
}

void ALaserGlaiveProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only interact with our target
	if (Other->GetRootComponent() == GetProjectileMovement()->HomingTargetComponent)
	{
		// TODO: Apply Damage and whatever status effects to target
		ABaseEnemy* Enemy = static_cast<ABaseEnemy*>(Other);
		Enemy->TakeDamage(DamageInfo);
		
		// Update bounces and increase base damage
		++CurrBounces;
		DamageInfo.Amount *= BounceDamageIncrease;

		if (CurrBounces <= MaxBounces)
		{
			UE_LOG(LogTemp, Log, TEXT("Calculating next nearest target!!"));
			GetNextBestTarget();	// TODO: This probably shouldn't have a Destroy() in it
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Destroying Laser Glaive!!"));
			Destroy();
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SKIP: Collided with something that is not the target!!"));
	}
}

void ALaserGlaiveProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALaserGlaiveProjectile::SetTarget(const AActor* Target)
{
	// Keep track of which target's we've already tracked
	PreviousTargets.Add(Target);

	// Update Project Component's homing
	GetProjectileMovement()->HomingTargetComponent = Target->GetRootComponent();

	// Set new velocity for new target
	const FVector& NewDirection = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	GetProjectileMovement()->Velocity = NewDirection * GetProjectileMovement()->InitialSpeed;

	// TODO: Set rotation based on new target as well?
}

void ALaserGlaiveProjectile::GetNextBestTarget()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereRadius);

	// Perform a sphere collision to determine any possible new targets
	bool bHit = GetWorld()->OverlapMultiByChannel(OverlapResults, GetActorLocation(), GetActorRotation().Quaternion(), ECC_Pawn, Sphere);
	// Debug a Sphere for visualization
	DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 16, FColor::Green);

	AActor* TargetEnemy = nullptr;
	float HighestPriority = -FLT_MAX;

	// Determine the Highest priority target focusing on closest targets that haven't been hit yet 
	for (const auto& Overlap : OverlapResults)
	{
		AActor* OverlapActor = Overlap.GetActor();
		if (OverlapActor && OverlapActor->IsA(ABaseEnemy::StaticClass()) && OverlapActor->GetRootComponent() != GetProjectileMovement()->HomingTargetComponent)	// Ignore our last hit target
		{
			float Priority = 0.0f;
			float DistanceToEnemy = FVector::Dist(GetActorLocation(), OverlapActor->GetActorLocation());
			Priority += (1.0f / DistanceToEnemy) * 1000.0f;	// Higher priority for closer targets

			// Check if this target has been hit before, if so then deprioritize
			if (PreviousTargets.Contains(OverlapActor))
			{
				Priority -= 10000.0f;
			}

			// Determine if we have a new closest target
			if (Priority >= HighestPriority)
			{
				HighestPriority = Priority;
				TargetEnemy = OverlapActor;
			}
		}
	}

	if (TargetEnemy)
	{
		UE_LOG(LogTemp, Log, TEXT("Setting new target!"));
		SetTarget(TargetEnemy);
	}
	else
	{
		// No other possible enemies, so destroy
		UE_LOG(LogTemp, Log, TEXT("No possible new targets, destroying projectile!"));
		Destroy();
	}
}
