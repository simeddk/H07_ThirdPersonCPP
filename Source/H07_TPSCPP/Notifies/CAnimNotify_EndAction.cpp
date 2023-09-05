#include "CAnimNotify_EndAction.h"
#include "Global.h"
#include "Components/CActionComponent.h"
#include "Action/CDoAction.h"
#include "Action/CActionData.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "EndAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionDataSpawned* actionData = actionComp->GetCurrent();
	CheckNull(actionData);

	ACDoAction* doAction = actionData->GetDoAction();
	CheckNull(doAction);

	doAction->End_DoAction();
}

