
#include "Weapon_Revolver.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


// Called when the game starts or when spawned
void AWeapon_Revolver::BeginPlay()
{
	AActor* _PlayerCharacterRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	_PlayerCharacter = Cast<AThePlayerCharacter>(_PlayerCharacterRef);

	_PlayerCharacter->OnItemUse.AddDynamic(this, &AWeapon_Base::UseItem);
}

void AWeapon_Revolver::UseItem_Implementation(AActor* Item)
{
	if(Item == this)
	{
		_CameraLocation = _PlayerCharacter->GetCamera()->GetComponentLocation();
		_CameraRotation = _PlayerCharacter->GetCamera()->GetComponentRotation();
		_CameraDistance = _CameraRotation.Vector();
		
		FHitResult hit(ForceInit);
		
		const bool isHitting = UKismetSystemLibrary::LineTraceSingle(GetWorld(), _CameraLocation, (_CameraLocation + (_CameraDistance * _ItemRange)), TraceTypeQuery4,
			false, _ActorsToIgnore, EDrawDebugTrace::ForDuration, hit, true);

		if(isHitting)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Firing Weapon!"));
		}
	}
	else
	{
		return;
	}
}
