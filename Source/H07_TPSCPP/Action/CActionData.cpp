#include "CActionData.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Action/CEquipment.h"
#include "Action/CAttachment.h"
#include "Action/CDoAction.h"

void UCActionData::BeginPlay(ACharacter* InOwnerCharacter, UCActionDataSpawned** OutSpawned)
{
	FTransform transform;

	ACAttachment* attachment = nullptr;
	if (!!AttachmentClass)
	{
		attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, InOwnerCharacter);
		attachment->SetActorLabel(GetCustomLabelName(InOwnerCharacter, "Attachment"));
		UGameplayStatics::FinishSpawningActor(attachment, transform);
	}

	ACEquipment* equipment = nullptr;
	if (!!EquipmentClass)
	{
		equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);
		equipment->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		equipment->SetData(EquipmentData);
		equipment->SetColor(EquipmentColor);
		equipment->SetActorLabel(GetCustomLabelName(InOwnerCharacter, "Equipment"));
		UGameplayStatics::FinishSpawningActor(equipment, transform);

		if (!!attachment)
		{
			equipment->OnEquipmentDelegate.AddDynamic(attachment, &ACAttachment::OnEquip);
			equipment->OnUnequipmentDelegate.AddDynamic(attachment, &ACAttachment::OnUnequip);
		}
	}

	ACDoAction* doAction = nullptr;
	if (!!DoActionClass)
	{
		doAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(DoActionClass, transform, InOwnerCharacter);
		doAction->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		doAction->SetDatas(DoActionDatas);
		doAction->SetActorLabel(GetCustomLabelName(InOwnerCharacter, "DoAction"));
		UGameplayStatics::FinishSpawningActor(doAction, transform);

		if (!!equipment)
		{
			doAction->SetEquippedThis(equipment->IsEquppedThis());
		}

		if (!!attachment)
		{
			attachment->OnAttachmentBeginOverlap.AddDynamic(doAction, &ACDoAction::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(doAction, &ACDoAction::OnAttachmentEndOverlap);
		}
	}

	*OutSpawned = NewObject<UCActionDataSpawned>();
	(*OutSpawned)->Attachment = attachment;
	(*OutSpawned)->Equipment = equipment;
	(*OutSpawned)->DoAction = doAction;
	(*OutSpawned)->EquipmentColor = EquipmentColor;

}

FString UCActionData::GetCustomLabelName(ACharacter* InOwnerCharacter, FString InMiddleName)
{
	FString labelName;
	labelName.Append(InOwnerCharacter->GetActorLabel());
	labelName.Append("_");
	labelName.Append(InMiddleName);
	labelName.Append("_");
	labelName.Append(GetName().Replace(L"DA_", L""));

	return labelName;
}
