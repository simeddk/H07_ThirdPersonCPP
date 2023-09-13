#include "CDoAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CBehaviorComponent.h"
#include "Characters/AI/CAIController.h"
#include "CAttachment.h"

void ACDoAction_Warp::BeginPlay()
{
	Super::BeginPlay();

	for (const auto& child : OwnerCharacter->Children)
	{
		if (child->IsA<ACAttachment>() && child->GetActorLabel().Contains("Warp"))
		{
			Decal = CHelpers::GetComponent<UDecalComponent>(child);
			break;
		}
	}
}

void ACDoAction_Warp::DoAction()
{
	Super::DoAction();

	CheckFalse(*bEquippedThis);
	CheckFalse(StateComp->IsIdleMode());

	if (IsPlayerControlled())
	{
		FRotator rotation;
		CheckFalse(GetCursorLocationAndRotation(Location, rotation));
	}
	else
	{
		//Todo. AIcontroller->behaviorComp->SetLocationKey(from BB) -> Location = (Set)
		ACAIController* aiController = OwnerCharacter->GetController<ACAIController>();
		if (!!aiController)
		{
			UCBehaviorComponent* behaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(aiController);
			Location = behaviorComp->GetWarpLocation();
		}
	}

	Location.Z += OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f;

	StateComp->SetActionMode();
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();
}

bool ACDoAction_Warp::IsPlayerControlled()
{
	return (OwnerCharacter->GetClass() == GetWorld()->GetAuthGameMode()->DefaultPawnClass);
}

void ACDoAction_Warp::Begin_DoAction()
{
	Super::Begin_DoAction();

	FTransform transform = OwnerCharacter->GetTransform();
	transform.AddToTranslation(Datas[0].EffectTransform.GetLocation());
	transform.SetScale3D(Datas[0].EffectTransform.GetScale3D());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Datas[0].Effect, transform);
}

void ACDoAction_Warp::End_DoAction()
{
	Super::End_DoAction();

	OwnerCharacter->SetActorLocation(Location);

	StateComp->SetIdleMode();
	StatusComp->SetMove();
}

void ACDoAction_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(IsPlayerControlled());
	CheckFalse(*bEquippedThis);

	FVector cursorLocation;
	FRotator cursorRotation;
	if (GetCursorLocationAndRotation(cursorLocation, cursorRotation))
	{
		Decal->SetVisibility(true);
		Decal->SetWorldLocation(cursorLocation);
		Decal->SetWorldRotation(cursorRotation);
	}
	else
	{
		Decal->SetVisibility(false);
	}

}

bool ACDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

	FHitResult hitResult;
	if (controller->GetHitResultUnderCursorForObjects(objectTypes, true, hitResult))
	{
		OutLocation = hitResult.Location;
		OutRotation = hitResult.ImpactNormal.Rotation();

		return true;
	}

	return false;
}