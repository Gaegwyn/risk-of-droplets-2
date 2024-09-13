// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Huntress.h"

#include "BaseEnemy.h"
#include "Projectiles/BaseProjectile.h"

#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"


// Set default values
AHuntress::AHuntress()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it
	PrimaryActorTick.bCanEverTick = false;
}

void AHuntress::BeginPlay()
{
	Super::BeginPlay();

	// Inside your class
	FTimerHandle TimerHandle;

	// Ensure the function repeats every 'Interval' seconds
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AHuntress::GetNearestTarget, 0.5f, true);	// TODO: Handle stopping timer?
}

void AHuntress::Tick(float DeltaTime)
{
}

void AHuntress::UsePrimarySkill()
{
	if (!CurrentTarget.Get())
	{
		UE_LOG(LogTemp, Log, TEXT("No target available!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Using Huntress' Primary Skill!"));
}

void AHuntress::UseSecondarySkill()
{
	if (!CurrentTarget.Get())
	{
		UE_LOG(LogTemp, Log, TEXT("No target available!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Using Huntress' Secondary Skill!"));

	// TODO: Try spawning a 'projectile'
	if (SecondarySkillProjectile)
	{
		GetWorld()->SpawnActor<ABaseProjectile>(SecondarySkillProjectile, GetActorLocation(), GetActorRotation());
	}
}

void AHuntress::UseUtilitySkill()
{
	UE_LOG(LogTemp, Log, TEXT("Using Huntress' Utility Skill!"));
}

void AHuntress::UseSpecialSkill()
{
	UE_LOG(LogTemp, Log, TEXT("Using Huntress' Special Skill!"));
}

void AHuntress::GetNearestTarget()
{
	// Reset target?
	CurrentTarget = nullptr;

	// Get the player controller and the camera manager
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		UE_LOG(LogTemp, Log, TEXT("Calculating nearest enemy!"));

		TArray<APawn*> EnemiesInRange;

		// Get Player's location
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// Define the end point of the sphere sweep (for a small vertical Z offset)
		FVector PlayerLocation = GetActorLocation() + (CameraRotation.Vector() * CameraRotationScaler) + Offset;
		FVector EndLocation = PlayerLocation + (CameraRotation.Vector() * 350.0f);

		TArray<FHitResult> HitResults;
		FCollisionShape Box = FCollisionShape::MakeBox(FVector(ExtentX, ExtentY, ExtentZ));

		// Perform a multi-sphere trace (can be based on enemy collision channel)
		bool bHit = GetWorld()->SweepMultiByChannel(
			HitResults,
			PlayerLocation,
			EndLocation,
			CameraRotation.Quaternion(),	//
			ECC_Pawn,						// Collision channel of the enemies
			Box
		);

		// Track the closest enemy
		AActor* ClosestEnemy = nullptr;	// TODO: keep track of this
		float ClosestDistance = FLT_MAX;

		for (auto& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor->IsA(ABaseEnemy::StaticClass())) // Use tags or specific class to identify enemies
			{
				float DistanceToEnemy = FVector::Dist(GetActorLocation(), HitActor->GetActorLocation());

				// Check if this enemy is closer than the previous closest one
				if (DistanceToEnemy < ClosestDistance)
				{
					ClosestDistance = DistanceToEnemy;
					ClosestEnemy = HitActor;
				}

				// Optionally, draw debug lines to visualize hits
				DrawDebugLine(GetWorld(), GetActorLocation(), HitActor->GetActorLocation(), FColor::Red, false, 1.0f, 0, 1.0f);
			}
		}

		if (ClosestEnemy)
		{
			// Keep track of our current target
			CurrentTarget = ClosestEnemy;
			DrawDebugLine(GetWorld(), GetActorLocation(), ClosestEnemy->GetActorLocation(), FColor::Green, false, 1.0f, 0, 1.0f);
		}
		/* Debug BoxSweep*/
		/* {
			// Visualize the sphere sweep by drawing multiple spheres along the sweep path
			const int32 NumSteps = 10;  // Number of debug spheres to draw
			for (int32 i = 0; i <= NumSteps; i++)
			{
				// Calculate the position along the sweep path
				float Alpha = (float)i / (float)NumSteps;
				FVector DebugLocation = FMath::Lerp(PlayerLocation, EndLocation, Alpha);

				DrawDebugBox(
					GetWorld(),
					DebugLocation,
					FVector(ExtentX, ExtentY, ExtentZ),
					CameraRotation.Quaternion(),
					FColor::Green,		// Sphere color
					false,				// Is Persistant
					1.0f,				// Duration
					0,					// Depth priority
					1.0f				// Line thickness
				);
			}
		}*/

	}
}




