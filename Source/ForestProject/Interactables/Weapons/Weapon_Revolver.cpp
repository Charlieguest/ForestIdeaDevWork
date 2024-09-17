
#include "Weapon_Revolver.h"
#include "Components/ArrowComponent.h"
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
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Firing!"));
	
	/*
		if(Item == this)
		{
			
		}
		else
		{
			return;
		}
		
		*/
}
