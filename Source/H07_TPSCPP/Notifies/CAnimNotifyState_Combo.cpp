#include "CAnimNotifyState_Combo.h"
#include "Global.h"
#include "Components/CActionComponent.h"
#include "Action/CActionData.h"
#include "Action/CDoAction_Melee.h"

FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionDataSpawned* data = actionComp->GetCurrent();
	CheckNull(data);

	ACDoAction_Melee* doAction_melee = Cast<ACDoAction_Melee>(data->GetDoAction());
	CheckNull(doAction_melee);

	doAction_melee->EnableCombo();
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionDataSpawned* data = actionComp->GetCurrent();
	CheckNull(data);

	ACDoAction_Melee* doAction_melee = Cast<ACDoAction_Melee>(data->GetDoAction());
	CheckNull(doAction_melee);

	doAction_melee->DisableCombo();
}


