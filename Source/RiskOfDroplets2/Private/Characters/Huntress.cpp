// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Huntress.h"

#include "BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Projectiles/StrafeProjectile.h"
#include "Projectiles/LaserGlaiveProjectile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"


// Set default values
AHuntress::AHuntress()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it
	PrimaryActorTick.bCanEverTick = false;

	// Setup our damage system
	GetDamageSystem()->MaxHealth = 250.0f;
	GetDamageSystem()->CurrentHealth = GetDamageSystem()->MaxHealth;

	BlinkDistance = 1500.0f;
	BlinkDuration = 0.2f;

	// Can use abilities
	bCanUsePrimarySkill = true;
	bCanUseSecondarySkill = true;
	bCanUseUtilitySkill = true;
	bCanUseSpecialSkill = true;

	// Cooldowns
	PrimarySkillCooldown = 0.5f;
	SecondarySkillCooldown = 7.0f;
	UtilitySkillCooldown = 7.0f;
	SpecialSkillCooldown = 12.0f;

	// How far away we can still target enemies
	AutoAimDistance = 1200.0f;
}

void AHuntress::BeginPlay()
{
	Super::BeginPlay();

	// Set a repeating timer for our auto-targeting
	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &AHuntress::GetNearestTarget, 0.5f, true);	// TODO: Handle stopping timer?
}

void AHuntress::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHuntress::UsePrimarySkill()
{
	if (!bCanUsePrimarySkill)
	{
		UE_LOG(LogTemp, Log, TEXT("Primary skill on cooldown!"));
		return;
	}
	if (!CurrentTarget.Get())
	{
		UE_LOG(LogTemp, Log, TEXT("No target available!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Using Huntress' Primary Skill!"));

	if (PrimarySkillProjectile)
	{
		const TObjectPtr<AStrafeProjectile> StrafeProjectile = GetWorld()->SpawnActor<AStrafeProjectile>(PrimarySkillProjectile, GetActorLocation(), GetActorRotation());
		StrafeProjectile.Get()->SetTarget(CurrentTarget);

		// Go on cooldown 
		// TODO: Allow attack speed to increase cooldown?
		bCanUsePrimarySkill = false;
		GetWorld()->GetTimerManager().SetTimer(PrimarySkillTimerHandle, this, &ABaseCharacter::ResetPrimarySkillCooldown, PrimarySkillCooldown, true);
	}
}

void AHuntress::UseSecondarySkill()
{
	if (!bCanUseSecondarySkill)
	{
		UE_LOG(LogTemp, Log, TEXT("Secondary skill on cooldown!"));
		return;
	}
	if (!CurrentTarget.Get())
	{
		UE_LOG(LogTemp, Log, TEXT("No target available!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Using Huntress' Secondary Skill!"));

	if (SecondarySkillProjectile)
	{
		const TObjectPtr<ALaserGlaiveProjectile> LaserGlaiveProjectile = GetWorld()->SpawnActor<ALaserGlaiveProjectile>(SecondarySkillProjectile, GetActorLocation(), GetActorRotation());
		LaserGlaiveProjectile.Get()->SetTarget(CurrentTarget);

		// Go on cooldown 
		// TODO: Allow attack speed to increase cooldown?
		bCanUseSecondarySkill = false;
		GetWorld()->GetTimerManager().SetTimer(SecondarySkillTimerHandle, this, &ABaseCharacter::ResetSecondarySkillCooldown, SecondarySkillCooldown, true);
	}
}

void AHuntress::UseUtilitySkill()
{
	if (!bCanUseUtilitySkill)
	{
		UE_LOG(LogTemp, Log, TEXT("Utility skill on cooldown!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Using Huntress' Utility Skill!"));
	bCanUseUtilitySkill = false;
	StartBlink();
}

void AHuntress::UseSpecialSkill()
{
	if (!bCanUseSpecialSkill)
	{
		UE_LOG(LogTemp, Log, TEXT("Special skill on cooldown!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Using Huntress' Special Skill!"));

	bCanUseSpecialSkill = false;
	GetWorld()->GetTimerManager().SetTimer(SpecialSkillTimerHandle, this, &ABaseCharacter::ResetSpecialSkillCooldown, SpecialSkillCooldown, true);
}

void AHuntress::GetNearestTarget()
{
	// Reset target?
	CurrentTarget = nullptr;

	// Get the player controller and the camera manager
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		//UE_LOG(LogTemp, Log, TEXT("Calculating nearest enemy!"));

		TArray<APawn*> EnemiesInRange;

		// Get Player's location
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// Define the end point of the box sweep
		FVector PlayerLocation = GetActorLocation() + (CameraRotation.Vector() * CameraRotationScaler) + Offset;
		FVector EndLocation = PlayerLocation + (CameraRotation.Vector() * AutoAimDistance);

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
		//// Visualize the sphere sweep by drawing multiple spheres along the sweep path
		//const int32 NumSteps = 10;  // Number of debug spheres to draw
		//for (int32 i = 0; i <= NumSteps; i++)
		//{
		//	// Calculate the position along the sweep path
		//	float Alpha = (float)i / (float)NumSteps;
		//	FVector DebugLocation = FMath::Lerp(PlayerLocation, EndLocation, Alpha);
		//
		//	DrawDebugBox(
		//		GetWorld(),
		//		DebugLocation,
		//		FVector(ExtentX, ExtentY, ExtentZ),
		//		CameraRotation.Quaternion(),
		//		FColor::Green,		// Sphere color
		//		false,				// Is Persistant
		//		1.0f,				// Duration
		//		0,					// Depth priority
		//		1.0f				// Line thickness
		//	);
		//}
	}
}

void AHuntress::StartBlink()
{
	// Think it's like a half-second lerp from starting position to new position (Camera forward)
	// 1. Huntress also becomes invisible as she "teleports"
	// 2. Might be invuln during time?
	// 3. DOESN'T phase through things but slides across surfaces
	// 4. Resets velocity at end I think

	// Get the player controller and the camera manager
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)	// TODO: If can Blink
	{
		// Get Player's location
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// Initialize our blink
		BlinkStartLocation = GetActorLocation();
		BlinkTargetLocation = GetActorLocation() + (CameraRotation.Vector() * BlinkDistance);
		BlinkTimePassed = 0.0f;
		GetWorld()->GetTimerManager().SetTimer(BlinkTimerHandle, this, &AHuntress::BlinkTick, GetWorld()->GetDeltaSeconds(), true);
	}
}

void AHuntress::BlinkTick()
{
	UE_LOG(LogTemp, Log, TEXT("Blinking!"));
	BlinkTimePassed += GetWorld()->GetDeltaSeconds();

	const float Progress = FMath::Clamp(BlinkTimePassed / BlinkDuration, 0.0f, 1.0f);
	const FVector NewLocation = FMath::Lerp(BlinkStartLocation, BlinkTargetLocation, Progress);

	bool bHit = SetActorLocation(NewLocation, true);
	if (bHit)
	{
		// TODO: Have Huntress "Slide" across terrain smoothly. Will need to calc new target destination and use the Hit.Normals I guess?
	}

	if (Progress >= 1.0f)
	{
		OnBlinkEnd();
	}
}

void AHuntress::OnBlinkEnd()
{
	UE_LOG(LogTemp, Log, TEXT("Finished blinking"));
	// TODO: IsNot Blinking, continue movement, etc.
	GetWorld()->GetTimerManager().ClearTimer(BlinkTimerHandle);

	if (GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}

	// Go on cooldown
	GetWorld()->GetTimerManager().SetTimer(UtilitySkillTimerHandle, this, &ABaseCharacter::ResetUtilitySkillCooldown, UtilitySkillCooldown, true);
}
