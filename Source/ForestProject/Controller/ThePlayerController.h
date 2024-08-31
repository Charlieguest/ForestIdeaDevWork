

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "ForestProject/Controller/Interfaces/IAInterface.h"
#include "ThePlayerController.generated.h"

class UInputComponent;
class AThePlayerCharacter;
class ATheGameMode;

UCLASS(Abstract)
class FORESTPROJECT_API AThePlayerController : public APlayerController, public IIAInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TObjectPtr<ACharacter> _PlayerCharacter;

	UPROPERTY()
	TObjectPtr<ATheGameMode> _TheGameMode;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* _PlayerMappingContext;

public:

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Move;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Sprint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Look;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Jump;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Crouch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Interact;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _AimingWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _ShootWeapon;

	void MoveInput(const FInputActionInstance& Instance);
	void LookInput(const FInputActionInstance& Instance);
	void InteractInput(const FInputActionInstance& Instance);
	void SprintInput(const FInputActionInstance& Instance);
	void StopSprintingInput(const FInputActionInstance& Instance);
	void CrouchInput(const FInputActionInstance& Instance);
	void StandInput(const FInputActionInstance& Instance);
	void JumpInput(const FInputActionInstance& Instance);
	void AimWeapon(const FInputActionInstance& Instance);
	void StopAimingWeapon(const FInputActionInstance& Instance);
	void ShootWeapon(const FInputActionInstance& Instance);

	void PossessPlayer();
};
