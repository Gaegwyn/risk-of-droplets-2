// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"

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

	// Setup Damage System component
	DamageSystemComponent = CreateDefaultSubobject<UDamageSystemComponent>(TEXT("DamageSystem"));
	DamageSystemComponent->MaxHealth = 100.0f;
	DamageSystemComponent->CurrentHealth = DamageSystemComponent->MaxHealth;

	// Keep track of Walk/Sprint speeds
	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SprintSpeed = WalkSpeed * 1.5f;
	bIsSprinting = false;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
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

void ABaseCharacter::UsePrimarySkill()
{
	//UE_LOG(LogTemp, Log, TEXT("Using BaseCharacter's Primary Skill!"))
}

void ABaseCharacter::UseSecondarySkill()
{
	//UE_LOG(LogTemp, Log, TEXT("Using BaseCharacter's Secondary Skill!"))
}

void ABaseCharacter::UseUtilitySkill()
{
	//UE_LOG(LogTemp, Log, TEXT("Using BaseCharacter's Utility Skill!"))
}

void ABaseCharacter::UseSpecialSkill()
{
	//UE_LOG(LogTemp, Log, TEXT("Using BaseCharacter's Special Skill!"))
}

float ABaseCharacter::GetMaxHealth()
{
	return DamageSystemComponent->GetMaxHealth();
}

float ABaseCharacter::GetCurrentHealth()
{
	return DamageSystemComponent->GetCurrentHealth();
}

void ABaseCharacter::TakeDamage(const FDamageInfo& DamageInfo)
{
	DamageSystemComponent->TakeDamage(DamageInfo);
	// TODO: Check for player death if CurrentHealth <= 0
}

void ABaseCharacter::Heal(const float Value)
{
	DamageSystemComponent->Heal(Value);
}

void ABaseCharacter::ResetPrimarySkillCooldown()
{
	bCanUsePrimarySkill = true;
}

void ABaseCharacter::ResetSecondarySkillCooldown()
{
	bCanUseSecondarySkill = true;
}

void ABaseCharacter::ResetUtilitySkillCooldown()
{
	bCanUseUtilitySkill = true;
}

void ABaseCharacter::ResetSpecialSkillCooldown()
{
	bCanUseSpecialSkill = true;
}

bool ABaseCharacter::IsSprinting() const
{
	return bIsSprinting;
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Log, TEXT("Using BaseCharacter's Move!"))
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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find valid Controller to Move!"))
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Log, TEXT("Using BaseCharacter's Look!"))
	if (Controller != nullptr)
	{
		const FVector2D LookAxisVector = Value.Get<FVector2D>();
		AddControllerPitchInput(LookAxisVector.Y);	//	Up/Down
		AddControllerYawInput(LookAxisVector.X);	//	Left/Right
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Unable to find valid Controller to Look!"))
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
