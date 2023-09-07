#include "CDoAction_MagicBall.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "CMagicBall.h"
#include "CAim.h"

void ACDoAction_MagicBall::BeginPlay()
{
	Super::BeginPlay();

	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);

	ActionComp = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	CheckNull(ActionComp);
	ActionComp->OnActionTypeChanged.AddDynamic(this, &ACDoAction_MagicBall::AbortByActionTypeChanged);
}

void ACDoAction_MagicBall::Tick(float Deltatime)
{
	Super::Tick(Deltatime);

	CheckNull(Aim);
	Aim->Tick(Deltatime);
}

void ACDoAction_MagicBall::DoAction()
{
	CheckFalse(Datas.Num() > 0);

	if (Aim->IsAvaliable())
		CheckFalse(Aim->IsAiming());

	CheckFalse(StateComp->IsIdleMode());
	StateComp->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();
}

void ACDoAction_MagicBall::Begin_DoAction()
{
	CheckNull(Datas[0].ProjectileClass);

	FVector location;
	FRotator rotation;

	//Todo. Interfaceȭ
	OwnerCharacter->GetController()->GetPlayerViewPoint(location, rotation);
	FVector direction = rotation.Vector();
	FVector handSocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation("hand_r");
	location = location + direction * ((handSocketLocation - location) | direction);
	//---

	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotation));

	ACMagicBall* magicBall = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACMagicBall>
		(
			Datas[0].ProjectileClass,
			transform,
			OwnerCharacter,
			OwnerCharacter,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
	magicBall->OnMagicBallBeginOverlap.AddDynamic(this, &ACDoAction_MagicBall::OnMagicBallBeginOverlap);
	magicBall->FinishSpawning(transform);
}

void ACDoAction_MagicBall::End_DoAction()
{
	StateComp->SetIdleMode();
	StatusComp->SetMove();
}

void ACDoAction_MagicBall::OnAim()
{
	CheckNull(Aim);
	Aim->On();
}

void ACDoAction_MagicBall::OffAim()
{
	CheckNull(Aim);
	Aim->Off();
}

void ACDoAction_MagicBall::OnMagicBallBeginOverlap(FHitResult InResult)
{
	FDamageEvent damageEvent;
	InResult.Actor->TakeDamage(Datas[0].Power, damageEvent, OwnerCharacter->GetController(), this);
}


void ACDoAction_MagicBall::AbortByActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	CheckNull(Aim);
	CheckFalse(Aim->IsAvaliable());
	CheckFalse(Aim->IsAiming());

	Aim->Off();
}