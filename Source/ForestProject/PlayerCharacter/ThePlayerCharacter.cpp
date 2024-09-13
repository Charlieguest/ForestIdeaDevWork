


#include "ThePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogThePlayerCharacter, Display, All);

// Sets default values
AThePlayerCharacter::AThePlayerCharacter()
{
	_Collision = GetCapsuleComponent();
	_CharacterMovement = GetCharacterMovement();
	_PlayerVelocity = _CharacterMovement->GetLastUpdateVelocity();
	
	_ItemLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("ItemLocation"));
	_ItemLocation->SetupAttachment(_Collision);

	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(_Collision);
	_Camera->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
	_Camera->bUsePawnControlRotation = true;

	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	_CharacterMovement->MaxWalkSpeedCrouched = _CrouchSpeed;
}

void AThePlayerCharacter::IAMove_Implementation(const FInputActionInstance& Instance)
{
	if(Controller != nullptr)
	{
		const FVector2d MoveValue = Instance.GetValue().Get<FVector2d>();

		if(MoveValue.Y != 0.f)
		{
			AddMovementInput(GetActorForwardVector(), MoveValue.Y);
		}
		if(MoveValue.X != 0.f)
		{
			AddMovementInput(GetActorRightVector(), MoveValue.X);
		}
	}
}

void AThePlayerCharacter::IALook_Implementation(const FInputActionInstance& Instance)
{
	if(Controller != nullptr)
	{
		const FVector2d AxisValue = Instance.GetValue().Get<FVector2d>();

		if(AxisValue.Y != 0.f)
		{
			AddControllerPitchInput(AxisValue.Y);
		}
		if(AxisValue.X != 0.f)
		{
			AddControllerYawInput(AxisValue.X);
		}
	}
}

void AThePlayerCharacter::IAInteract_Implementation(const FInputActionInstance& Instance)
{
	FVector arrowLocation = _ItemLocation->GetComponentLocation();
	AActor* playerCharacter = Cast<AThePlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	FHitResult hit(ForceInit);

	const bool isHitting = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), arrowLocation, arrowLocation, 100.0f, TraceTypeQuery3,
		false, ActorsToBeIgnored, EDrawDebugTrace::ForDuration, hit, true);
	
	if(ItemInHand == nullptr)
	{
		ItemInHand = hit.GetActor();
	}
	
	switch(CanHoldItem)
	{
	case true:
		if(isHitting)
		{
			if(ItemInHand != nullptr)
			{
				ItemInHand->AttachToComponent(_ItemLocation, FAttachmentTransformRules::SnapToTargetIncludingScale);
				CanHoldItem = false;
				bool BoolValue = Instance.GetValue().Get<bool>();
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Interact!"));
			}
		}
		break;
		
	case false:
		if(ItemInHand != nullptr)
		{
			ItemInHand->K2_DetachFromActor(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative);
			ItemInHand = nullptr;
			CanHoldItem = true;
		}
		break;
	}
}

void AThePlayerCharacter::IASprint_Implementation(const FInputActionInstance& Instance)
{
	_CharacterMovement->MaxWalkSpeed = _SprintSpeed;
}

void AThePlayerCharacter::IAStopSprinting_Implementation(const FInputActionInstance& Instance)
{
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
}

void AThePlayerCharacter::IACrouch_Implementation(const FInputActionInstance& Instance)
{
	Crouch();
	//UE_LOG(LogThePlayerCharacter, Display, TEXT("ThePlayerCharacter Working"));
}

void AThePlayerCharacter::IAStand_Implementation(const FInputActionInstance& Instance)
{
	UnCrouch();
}

void AThePlayerCharacter::IAJump_Implementation(const FInputActionInstance& Instance)
{
	Jump();
}

void AThePlayerCharacter::IAAimingWeapon_Implementation(const FInputActionInstance& Instance)
{
	_Camera->SetFieldOfView(_AimingFOV);
}

void AThePlayerCharacter::IAStopAimingWeapon_Implementation(const FInputActionInstance& Instance)
{
	_Camera->SetFieldOfView(_NormalFOV);
}

void AThePlayerCharacter::IAShootingWeapon_Implementation(const FInputActionInstance& Instance)
{
	bool BoolValue = Instance.GetValue().Get<bool>();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Fire!"));
}

// Called when the game starts or when spawned
void AThePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void AThePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

