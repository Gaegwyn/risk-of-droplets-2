// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "BaseEnemy.h"
#include "Engine/World.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InputActionValue.h"

#include "DrawDebugHelpers.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it
	PrimaryActorTick.bCanEverTick = false;

	// Don't rotate when the controller rotates. Let that just affect the camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;				// Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);	// ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 1.0f;	// Full control at max speed of MaxWalkSpeed					
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	// Create a "camera boom" that pulls in toward the player if there is a collision
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f;	// The camera follows at this distance behind the character	
	SpringArmComponent->bUsePawnControlRotation = true;	// Rotate the arm based on the controller

	// Create a camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);	// Attach to end of the boom and let boom adjust to match controller orientation
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to spring arm (camera boom)

	// Keep track of Walk/Sprint speeds
	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SprintSpeed = WalkSpeed * 1.5f;
	bIsSprinting = false;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Inside your class
	FTimerHandle TimerHandle;

	// Ensure the function repeats every 'Interval' seconds
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABaseCharacter::GetEnemiesInRange, 0.5f, true);	// TODO: Handle stopping timer?

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MovementInputAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);

		// Jumping
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &ABaseCharacter::StopSprinting);

		// Primary
		EnhancedInputComponent->BindAction(PrimarySkillInputAction, ETriggerEvent::Triggered, this, &ABaseCharacter::UsePrimarySkill);

		// Secondary
		EnhancedInputComponent->BindAction(SecondarySkillInputAction, ETriggerEvent::Triggered, this, &ABaseCharacter::UseSecondarySkill);

		// Utility
		EnhancedInputComponent->BindAction(UtilitySkillInputAction, ETriggerEvent::Triggered, this, &ABaseCharacter::UseUtilitySkill);

		// Special
		EnhancedInputComponent->BindAction(SpecialSkillInputAction, ETriggerEvent::Triggered, this, &ABaseCharacter::UseSpecialSkill);
	}
}

bool ABaseCharacter::IsSprinting() const
{
	return bIsSprinting;
}


void ABaseCharacter::Move(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MovementVector.Y);
		AddMovementInput(Right, MovementVector.X);
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D LookAxisVector = Value.Get<FVector2D>();
		AddControllerPitchInput(LookAxisVector.Y);	//	Up/Down
		AddControllerYawInput(LookAxisVector.X);	//	Left/Right
	}
}

void ABaseCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	bIsSprinting = true;
}

void ABaseCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsSprinting = false;
}

void ABaseCharacter::UsePrimarySkill()
{
	UE_LOG(LogTemp, Log, TEXT("Using Primary Skill!"))
}

void ABaseCharacter::UseSecondarySkill()
{
	UE_LOG(LogTemp, Log, TEXT("Using Secondary Skill!"))
}

void ABaseCharacter::UseUtilitySkill()
{
	UE_LOG(LogTemp, Log, TEXT("Using Utility Skill!"))
}

void ABaseCharacter::UseSpecialSkill()
{
	UE_LOG(LogTemp, Log, TEXT("Using Special Skill!"))
}

void ABaseCharacter::GetEnemiesInRange()
{
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
			DrawDebugLine(GetWorld(), GetActorLocation(), ClosestEnemy->GetActorLocation(), FColor::Green, false, 1.0f, 0, 1.0f);

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
