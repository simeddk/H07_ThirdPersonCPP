#include "CDoAction_Storm.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "CAttachment.h"


void ACDoAction_Storm::BeginPlay()
{
	Super::BeginPlay();
	//Box = Attachment->Box

	for (AActor* child : OwnerCharacter->Children)
	{
		if (child->IsA<ACAttachment>() && child->GetActorLabel().Contains("Storm"))
		{
			Box = CHelpers::GetComponent<UBoxComponent>(child);
			break;
		}
	}
}

void ACDoAction_Storm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDoAction_Storm::DoAction()
{
	Super::DoAction();

	CheckFalse(StateComp->IsIdleMode());
	CheckFalse(false);

	bAcivating = true;

	StateComp->SetActionMode();

	Datas[0].bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
}

void ACDoAction_Storm::Begin_DoAction()
{
	Super::Begin_DoAction();

	//Todo. 위에서 Box에 파티클컴포넌트 스폰시키기
}

void ACDoAction_Storm::End_DoAction()
{
	Super::End_DoAction();
}

void ACDoAction_Storm::Abort()
{
	Super::Abort();
}
