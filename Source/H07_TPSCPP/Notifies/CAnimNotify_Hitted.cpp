#include "CAnimNotify_Hitted.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

FString UCAnimNotify_Hitted::GetNotifyName_Implementation() const
{
	return "Hitted";
}

void UCAnimNotify_Hitted::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCStateComponent* stateComp = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(stateComp);

	stateComp->SetIdleMode();

	UCStatusComponent* statusComp = CHelpers::GetComponent<UCStatusComponent>(MeshComp->GetOwner());
	statusComp->SetMove();
}
