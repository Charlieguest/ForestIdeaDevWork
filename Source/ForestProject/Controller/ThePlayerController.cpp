﻿#include "ThePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "ForestProject/PlayerCharacter/ThePlayerCharacter.h"
#include "ForestProject/GameMode/TheGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogThePlayerController, Display, All);

void AThePlayerController::BeginPlay()
{
	Super::BeginPlay();

	_TheGameMode = Cast<ATheGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if(_TheGameMode != nullptr)
	{
		_PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		PossessPlayer();
	}
}

void AThePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(_Move, ETriggerEvent::Triggered, this, &AThePlayerController::MoveInput);
		EnhancedInputComponent->BindAction(_Look, ETriggerEvent::Triggered, this, &AThePlayerController::LookInput);
		EnhancedInputComponent->BindAction(_Interact, ETriggerEvent::Triggered, this, &AThePlayerController::InteractInput);
		EnhancedInputComponent->BindAction(_Jump, ETriggerEvent::Triggered, this, &AThePlayerController::JumpInput);
		EnhancedInputComponent->BindAction(_Crouch, ETriggerEvent::Triggered, this, &AThePlayerController::CrouchInput);
		EnhancedInputComponent->BindAction(_Crouch, ETriggerEvent::Completed, this, &AThePlayerController::StandInput);
		EnhancedInputComponent->BindAction(_Sprint, ETriggerEvent::Triggered, this, &AThePlayerController::SprintInput);
		EnhancedInputComponent->BindAction(_Sprint, ETriggerEvent::Completed, this, &AThePlayerController::StopSprintingInput);
		EnhancedInputComponent->BindAction(_AimingWeapon, ETriggerEvent::Triggered, this, &AThePlayerController::AimWeapon);
		EnhancedInputComponent->BindAction(_AimingWeapon, ETriggerEvent::Completed, this, &AThePlayerController::StopAimingWeapon);
		EnhancedInputComponent->BindAction(_ShootWeapon, ETriggerEvent::Triggered, this, &AThePlayerController::ShootWeapon);
	}
}

void AThePlayerController::MoveInput(const FInputActionInstance& Instance)
{
	Execute_IAMove(_PlayerCharacter, Instance);
}

void AThePlayerController::LookInput(const FInputActionInstance& Instance)
{
	Execute_IALook(_PlayerCharacter, Instance);
}

void AThePlayerController::InteractInput(const FInputActionInstance& Instance)
{
	Execute_IAInteract(_PlayerCharacter, Instance);
}

void AThePlayerController::SprintInput(const FInputActionInstance& Instance)
{
	Execute_IASprint(_PlayerCharacter, Instance);
}

void AThePlayerController::StopSprintingInput(const FInputActionInstance& Instance)
{
	Execute_IAStopSprinting(_PlayerCharacter, Instance);
}

void AThePlayerController::CrouchInput(const FInputActionInstance& Instance)
{
	Execute_IACrouch(_PlayerCharacter, Instance);
	//UE_LOG(LogThePlayerController, Display, TEXT("ThePlayerController Working"));
}

void AThePlayerController::StandInput(const FInputActionInstance& Instance)
{
	Execute_IAStand(_PlayerCharacter, Instance);
}

void AThePlayerController::JumpInput(const FInputActionInstance& Instance)
{
	Execute_IAJump(_PlayerCharacter, Instance);
}

void AThePlayerController::AimWeapon(const FInputActionInstance& Instance)
{
	Execute_IAAimingWeapon(_PlayerCharacter, Instance);
}

void AThePlayerController::StopAimingWeapon(const FInputActionInstance& Instance)
{
	Execute_IAStopAimingWeapon(_PlayerCharacter, Instance);
}

void AThePlayerController::ShootWeapon(const FInputActionInstance& Instance)
{
	Execute_IAShootingWeapon(_PlayerCharacter, Instance);
}

void AThePlayerController::PossessPlayer()
{
	this->UnPossess();

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if(_PlayerMappingContext)
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(_PlayerMappingContext,0);
		}
	}
	if(UGameplayStatics::GetPlayerController(this, 0))
	{
		this->Possess((_PlayerCharacter));
	}
}


