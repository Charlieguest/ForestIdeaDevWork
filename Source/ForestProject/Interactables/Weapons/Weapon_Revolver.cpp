
#include "Weapon_Revolver.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void AWeapon_Revolver::BeginPlay()
{
	AActor* _PlayerCharacterRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	_PlayerCharacter = Cast<AThePlayerCharacter>(_PlayerCharacterRef);
}

void AWeapon_Revolver::FireWeapon_Implementation(AActor* Weapon)
{
	if(Weapon == this)
	{
		UWorld* const world = GetWorld();
		if(world == nullptr || _Projectile == nullptr)
		{
			return;
		}
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		spawnParams.Owner = GetOwner();
		spawnParams.Instigator = GetInstigator();
		world->SpawnActor(_Projectile, &_Arrow->GetComponentTransform(), spawnParams);
	}
}

