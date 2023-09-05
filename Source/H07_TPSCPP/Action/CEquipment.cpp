#include "CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Characters/ICharacter.h"
#include "Characters/CPlayer.h"

ACEquipment::ACEquipment()
{

}

void ACEquipment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	StateComp = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	StatusComp = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	
	Super::BeginPlay();
}

void ACEquipment::Equip_Implementation()
{
	StateComp->SetEquipMode();

	Data.bCanMove ? StatusComp->SetMove() : StatusComp->SetStop(); //Todo. 

	if (!!Data.AnimMontage)
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRate, Data.StartSection);
	else
	{
		Begin_Equip();
		End_Equip();
	}

	if (Data.bPawnControl == true)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	IICharacter* characterInterface = Cast<IICharacter>(OwnerCharacter);
	CheckNull(characterInterface);
	characterInterface->ChangeColor(Color);
}

void ACEquipment::Begin_Equip_Implementation()
{
	if (OnEquipmentDelegate.IsBound())
		OnEquipmentDelegate.Broadcast();
}

void ACEquipment::End_Equip_Implementation()
{
	bEquippedThis = true;

	StateComp->SetIdleMode();
	StatusComp->SetMove();

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);
}

void ACEquipment::Unequip_Implementation()
{
	bEquippedThis = false;

	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	if (OnUnequipmentDelegate.IsBound())
		OnUnequipmentDelegate.Broadcast();
}

