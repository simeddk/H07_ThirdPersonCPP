#include "CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

void ACDoAction_Melee::DoAction()
{
	Super::DoAction();
	CheckFalse(Datas.Num() > 0);

	//Succeed Combo
	if (bCanCombo == true)
	{
		bCanCombo = false;
		bSucceed = true;

		return;
	}

	//First Attack
	CheckFalse(StateComp->IsIdleMode());
	StateComp->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();
	
	CheckFalse(bSucceed);
	bSucceed = false;
	
	ComboCount++;
	ComboCount = FMath::Clamp(ComboCount, 0, Datas.Num() - 1);

	OwnerCharacter->StopAnimMontage();
	OwnerCharacter->PlayAnimMontage(Datas[ComboCount].AnimMontage, Datas[ComboCount].PlayRate, Datas[ComboCount].StartSection);
	Datas[ComboCount].bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	OwnerCharacter->StopAnimMontage();
	ComboCount = 0;

	StateComp->SetIdleMode();
	StatusComp->SetMove();
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InCauser, InOtherCharacter);

	//Add Hitted Characters
	int32 hittedCharactersNum = HittedCharacters.Num();
	HittedCharacters.AddUnique(InOtherCharacter);

	//Check New Hitted Character
	if (hittedCharactersNum < HittedCharacters.Num())
	{
		//Hit Stop
		float hitStop = Datas[ComboCount].HitStop;
		if (FMath::IsNearlyZero(hitStop) == false)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
			UKismetSystemLibrary::K2_SetTimer(this, "RestoreGlobalTimeDilation", 2e-2f * hitStop, false);
		}

		//Play Particle
		UParticleSystem* particle = Datas[ComboCount].Effect;
		if (!!particle)
		{
			FTransform transform = Datas[ComboCount].EffectTransform;
			transform.AddToTranslation(InOtherCharacter->GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particle, transform);
		}

		//Play CameraShake
		TSubclassOf<UCameraShake> shake = Datas[ComboCount].ShakeClass;
		if (!!shake)
		{
			APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (!!controller)
				controller->PlayerCameraManager->PlayCameraShake(shake);
		}


		//Take Damage
		FDamageEvent damageEvent;
		InOtherCharacter->TakeDamage
		(
			Datas[ComboCount].Power,
			damageEvent,
			InAttacker->GetController(),
			InCauser
		);
	}
	
}

void ACDoAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InCauser, InOtherCharacter);
}

void ACDoAction_Melee::RestoreGlobalTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}
