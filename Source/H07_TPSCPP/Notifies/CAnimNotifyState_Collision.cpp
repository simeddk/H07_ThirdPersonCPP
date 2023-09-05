#include "CAnimNotifyState_Collision.h"
#include "Global.h"
#include "Components/CActionComponent.h"
#include "Action/CActionData.h"
#include "Action/CAttachment.h"
#include "Action/CDoAction_Melee.h"

FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionDataSpawned* data = actionComp->GetCurrent();
	CheckNull(data);

	ACAttachment* attachment = data->GetAttachment();
	CheckNull(attachment);

	attachment->OnCollisions();
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionDataSpawned* data = actionComp->GetCurrent();
	CheckNull(data);

	ACAttachment* attachment = data->GetAttachment();
	CheckNull(attachment);

	attachment->OffCollisions();

	ACDoAction_Melee* doAction_melee = Cast<ACDoAction_Melee>(data->GetDoAction());
	CheckNull(doAction_melee);
	doAction_melee->ClearHittedCharacters();
}


