#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CStatusComponent.h"
#include "Widgets/CNameWidget.h"
#include "Widgets/CHealthWidget.h"

ACEnemy::ACEnemy()
{
	//-------------------------------------------------------------------------
	//Create Scene Component
	//-------------------------------------------------------------------------
	CHelpers::CreateSceneComponent(this, &NameWidget, "NameWidget", GetMesh());
	CHelpers::CreateSceneComponent(this, &HealthWidget, "HealthWidget", GetMesh());

	//-------------------------------------------------------------------------
	//Create Actor Component
	//-------------------------------------------------------------------------
	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Montage, "Montage");
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &State, "State");

	//-------------------------------------------------------------------------
	//Component Settings
	//-------------------------------------------------------------------------
	// -> MeshComp
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* meshAsset;
	CHelpers::GetAsset(&meshAsset, "/Game/Character/ZLorp/Mesh/ZLorp");
	GetMesh()->SetSkeletalMesh(meshAsset);

	TSubclassOf<UAnimInstance> animInstanceClass;
	CHelpers::GetClass(&animInstanceClass, "/Game/Enemies/ABP_CEnemy");
	GetMesh()->SetAnimInstanceClass(animInstanceClass);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();

	//-> WidgetComp
	TSubclassOf<UCNameWidget> nameWidgetClass;
	CHelpers::GetClass<UCNameWidget>(&nameWidgetClass, "/Game/Widgets/WB_Name");
	NameWidget->SetWidgetClass(nameWidgetClass);
	NameWidget->SetRelativeLocation(FVector(0, 0, 240));
	NameWidget->SetDrawSize(FVector2D(240, 30));
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCHealthWidget> healthWidgetClass;
	CHelpers::GetClass<UCHealthWidget>(&healthWidgetClass, "/Game/Widgets/WB_Health");
	HealthWidget->SetWidgetClass(healthWidgetClass);
	HealthWidget->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidget->SetDrawSize(FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACEnemy::BeginPlay()
{
	LowerBodyMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), nullptr);
	UpperBodyMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), nullptr);

	GetMesh()->SetMaterial(0, LowerBodyMaterial);
	GetMesh()->SetMaterial(1, UpperBodyMaterial);

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	Super::BeginPlay();

	NameWidget->InitWidget();
	UCNameWidget* nameWidgetObject = Cast<UCNameWidget>(NameWidget->GetUserWidgetObject());
	if (!!nameWidgetObject)
		nameWidgetObject->SetNameText(GetController()->GetName(), GetName());
	
	HealthWidget->InitWidget();
	UCHealthWidget* healthWidgetObject = Cast<UCHealthWidget>(HealthWidget->GetUserWidgetObject());
	if (!!healthWidgetObject)
		healthWidgetObject->Update(Status->GetCurrentHealth(), Status->GetMaxHealth());

	NameWidget->SetVisibility(bVisibleNameWidget);
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	Causer = DamageCauser;

	Action->AbortByDamaged();

	Status->DecreaseHealth(DamageValue);

	//Dead
	if (Status->GetCurrentHealth() <= 0)
	{
		State->SetDeadMode();

		return DamageValue;
	}

	//Hitted
	State->SetHittedMode();

	return DamageValue;
}

void ACEnemy::Hitted()
{
	//Adjust HeathWidget
	UCHealthWidget* healthWidgetObject = Cast<UCHealthWidget>(HealthWidget->GetUserWidgetObject());
	if (!!healthWidgetObject)
		healthWidgetObject->Update(Status->GetCurrentHealth(), Status->GetMaxHealth());

	//Play Hitted Montage
	Montage->PlayHitted();

	//Look At Attacker
	FVector start = GetActorLocation();
	FVector target = Attacker->GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));

	//Hit Back
	FVector direction = (start - target).GetSafeNormal();
	LaunchCharacter(direction * LaunchValue * DamageValue, true, false);

	//Change Hitted Color
	ChangeColor(FLinearColor::Red);
	UKismetSystemLibrary::K2_SetTimer(this, "RestoreColor", 0.2f, false);
	
}

void ACEnemy::Dead()
{
	//Widget Visiblilty False
	NameWidget->SetVisibility(false);
	HealthWidget->SetVisibility(false);

	//All Attachment Collision Disable
	Action->Dead();

	//Ragdoll
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->GlobalAnimRateScale = 0.f;

	//AddForce
	FVector start = Attacker->GetActorLocation();
	FVector target = GetActorLocation();
	FVector direction = (target - start).GetSafeNormal();
	FVector force = direction * DamageValue * LaunchValue;
	GetMesh()->AddForceAtLocation(force, start);

	//Destroy All
	UKismetSystemLibrary::K2_SetTimer(this, "End_Dead", 5.f, false);
}

void ACEnemy::End_Dead()
{
	Action->End_Dead();

	Destroy();
}

void ACEnemy::ChangeColor(FLinearColor InColor)
{
	if (State->IsHittedMode())
	{
		LowerBodyMaterial->SetVectorParameterValue("BodyColor", InColor);
		UpperBodyMaterial->SetVectorParameterValue("BodyColor", InColor);
		return;
	}

	LowerBodyMaterial->SetVectorParameterValue("Emissive", InColor);
	UpperBodyMaterial->SetVectorParameterValue("Emissive", InColor);
}

void ACEnemy::RestoreColor()
{
	LowerBodyMaterial->SetVectorParameterValue("BodyColor", FLinearColor::Black);
	UpperBodyMaterial->SetVectorParameterValue("BodyColor", FLinearColor::Black);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Hitted:	Hitted();	break;
		case EStateType::Dead:		Dead();		break;
	}
}

