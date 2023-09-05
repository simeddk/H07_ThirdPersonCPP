#include "CActionComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Action/CActionData.h"
#include "Action/CActionDataSpawned.h"
#include "Action/CEquipment.h"
#include "Action/CAttachment.h"
#include "Action/CDoAction.h"

UCActionComponent::UCActionComponent()
{

}


void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* ownerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(ownerCharacter);

	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(ownerCharacter, &Datas[i]);
	}
}

void UCActionComponent::SetUnaremdMode()
{
	if (!!Datas[(int32)Type] && !!Datas[(int32)Type]->GetEquipment())
		Datas[(int32)Type]->GetEquipment()->Unequip();

	Datas[(int32)EActionType::Unaremd]->GetEquipment()->Equip();

	ChangeType(EActionType::Unaremd);
}

void UCActionComponent::SetFistMode()
{
	SetMode(EActionType::Fist);
}

void UCActionComponent::SetOneHandMode()
{
	SetMode(EActionType::OneHand);
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetWarpMode()
{
	SetMode(EActionType::Warp);
}

void UCActionComponent::SetMagicBallMode()
{
	SetMode(EActionType::MagicBall);
}

void UCActionComponent::SetStormMode()
{
	SetMode(EActionType::Storm);
}

void UCActionComponent::DoAction()
{
	CheckTrue(IsUnaremdMode());

	if (!!Datas[(int32)Type] && !!Datas[(int32)Type]->GetDoAction())
	{
		ACDoAction* doAction = Datas[(int32)Type]->GetDoAction();
		
		if (!!doAction)
			doAction->DoAction();
	}
}

void UCActionComponent::DoAim(bool bAiming)
{
	CheckTrue(IsUnaremdMode());

	if (!!Datas[(int32)Type] && !!Datas[(int32)Type]->GetDoAction())
	{
		ACDoAction* doAction = Datas[(int32)Type]->GetDoAction();

		if (!!doAction)
			bAiming ? doAction->OnAim() : doAction->OffAim();
	}
}

void UCActionComponent::Dead()
{
	OffAllCollisions();
}

void UCActionComponent::SetMode(EActionType InNewType)
{
	if (Type == InNewType)
	{
		SetUnaremdMode();
		return;
	}

	else if (IsUnaremdMode() == false)
	{
		if (!!Datas[(int32)Type] && !!Datas[(int32)Type]->GetEquipment())
			Datas[(int32)Type]->GetEquipment()->Unequip();
	}

	if (!!Datas[(int32)InNewType] && !!Datas[(int32)InNewType]->GetEquipment())
		Datas[(int32)InNewType]->GetEquipment()->Equip();

	ChangeType(InNewType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType prevType = Type;
	Type = InNewType;

	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prevType, InNewType);
}

void UCActionComponent::OffAllCollisions()
{
	for (const auto& data : Datas)
	{
		if (data == nullptr)
			continue;

		if (data->GetAttachment() == nullptr)
			continue;

		data->GetAttachment()->OffCollisions();
	}
}
