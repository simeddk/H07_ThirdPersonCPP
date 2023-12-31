#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CFeetComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Action/CActionData.h"
#include "Widgets/CSelectActionWidget.h"
#include "Widgets/CActionItemWidget.h"
#include "Widgets/CHealthWidget_Player.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//------------------------------------------------------------------
	//Create Scene Component
	//------------------------------------------------------------------
	CHelpers::CreateSceneComponent(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateSceneComponent(this, &Camera, "Camera", SpringArm);

	//------------------------------------------------------------------
	//Create Actor Component
	//------------------------------------------------------------------
	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Montage, "Montage");
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Option, "Option");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Feet, "Feet");

	//------------------------------------------------------------------
	//Component Settings
	//------------------------------------------------------------------
	// -> SkelMeshComp
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	USkeletalMesh* meshAsset;
	CHelpers::GetAsset<USkeletalMesh>(&meshAsset, "/Game/Character/Heraklios/Mesh/Heraklios_By_A__Dizon");
	GetMesh()->SetSkeletalMesh(meshAsset);

	TSubclassOf<UAnimInstance> animInstanceClass;
	CHelpers::GetClass(&animInstanceClass, "/Game/Player/ABP_CPlayer");
	GetMesh()->SetAnimInstanceClass(animInstanceClass);

	// -> SpringArm
	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;

	// -> Movement
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	//------------------------------------------------------------------
	//Widget
	//------------------------------------------------------------------
	CHelpers::GetClass<UCSelectActionWidget>(&SelectActionWidgetClass, "/Game/Widgets/WB_SelectAction");
	CHelpers::GetClass<UCHealthWidget_Player>(&HealthWidgetClass, "/Game/Widgets/WB_Health_Player");
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//Create Dynamic Material
	DynamicMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), nullptr);
	GetMesh()->SetMaterial(0, DynamicMaterial);

	//State Type Changed Event
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	//Create Select Action Widget
	SelectActionWidget = CreateWidget<UCSelectActionWidget>(GetController<APlayerController>(), SelectActionWidgetClass);
	CheckNull(SelectActionWidget);
	SelectActionWidget->AddToViewport();
	SelectActionWidget->SetVisibility(ESlateVisibility::Hidden);

	//Bind Event to Select Action Widget
	SelectActionWidget->GetItem("Item1")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnFist);
	SelectActionWidget->GetItem("Item2")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnOneHand);
	SelectActionWidget->GetItem("Item3")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnTwoHand);
	SelectActionWidget->GetItem("Item4")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnMagicBall);
	SelectActionWidget->GetItem("Item5")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnWarp);
	SelectActionWidget->GetItem("Item6")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnStorm);

	//Capsule Collision Event
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACPlayer::OnCapsuleBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACPlayer::OnCapsuleEndOverlap);

	//Create Health Widget
	HealthWidget = CreateWidget<UCHealthWidget_Player>(GetController<APlayerController>(), HealthWidgetClass);
	CheckNull(HealthWidget);
	HealthWidget->AddToViewport();
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Axis Event
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);

	//Action Event
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, this, &ACPlayer::OffRun);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, this, &ACPlayer::OnRun);
	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);
	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnOneHand);
	PlayerInputComponent->BindAction("TwoHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnTwoHand);
	PlayerInputComponent->BindAction("MagicBall", EInputEvent::IE_Pressed, this, &ACPlayer::OnMagicBall);
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, this, &ACPlayer::OnWarp);
	PlayerInputComponent->BindAction("Storm", EInputEvent::IE_Pressed, this, &ACPlayer::OnStorm);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnAction);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::OffAim);
	PlayerInputComponent->BindAction("SelectAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnSelectAction);
	PlayerInputComponent->BindAction("SelectAction", EInputEvent::IE_Released, this, &ACPlayer::OffSelectAction);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ACPlayer::OnInteract);

}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	Causer = DamageCauser;

	Action->AbortByDamaged();

	Status->DecreaseHealth(DamageValue);
	HealthWidget->Update(Status->GetCurrentHealth(), Status->GetMaxHealth());

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

void ACPlayer::OnMoveForward(float InAxis)
{
	CheckTrue(FMath::IsNearlyZero(InAxis));
	CheckFalse(Status->IsCanMove());

	FRotator rotator = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckTrue(FMath::IsNearlyZero(InAxis));
	CheckFalse(Status->IsCanMove());

	FRotator rotator = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	 float speed = Option->GetHorizontalLookSpeed();
	 AddControllerYawInput(InAxis * speed * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	float speed = Option->GetHorizontalLookSpeed();
	AddControllerPitchInput(InAxis * speed * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnZoom(float InAxis)
{
	float speed = Option->GetZoomSpeed() * InAxis * GetWorld()->GetDeltaSeconds();

	SpringArm->TargetArmLength += speed;
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, Option->GetZoomMin(), Option->GetZoomMax());
}

void ACPlayer::OnRun()
{
	//GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	Status->SetSpeed(EMoveSpeedType::Run);
}

void ACPlayer::OffRun()
{
	//GetCharacterMovement()->MaxWalkSpeed = Status->GetWalkSpeed();
	Status->SetSpeed(EMoveSpeedType::Walk);
}

void ACPlayer::OnEvade()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Status->IsCanMove());

	if (InputComponent->GetAxisValue("MoveForward") < 0.f)
	{
		State->SetBackStepMode();
		return;
	}

	State->SetRollMode();
}

void ACPlayer::OnInteract()
{
	CheckNull(InteractableActor);

	InteractableActor->Interact(this);
}

void ACPlayer::OnFist()
{
	CheckFalse(State->IsIdleMode());

	Action->SetFistMode();
}

void ACPlayer::OnOneHand()
{
	CheckFalse(State->IsIdleMode());

	Action->SetOneHandMode();
}

void ACPlayer::OnTwoHand()
{
	CheckFalse(State->IsIdleMode());

	Action->SetTwoHandMode();
}

void ACPlayer::OnMagicBall()
{
	CheckFalse(State->IsIdleMode());

	Action->SetMagicBallMode();
}

void ACPlayer::OnWarp()
{
	CheckFalse(State->IsIdleMode());

	Action->SetWarpMode();
}

void ACPlayer::OnStorm()
{
	CheckFalse(State->IsIdleMode());

	Action->SetStormMode();
}

void ACPlayer::OnAction()
{
	Action->DoAction();
}

void ACPlayer::OnAim()
{
	Action->DoAim(true);
}

void ACPlayer::OffAim()
{
	Action->DoAim(false);
}

void ACPlayer::OnSelectAction()
{
	CheckFalse(State->IsIdleMode());

	SelectActionWidget->SetVisibility(ESlateVisibility::Visible);

	APlayerController* controller = GetController<APlayerController>();
	CheckNull(controller);

	controller->bShowMouseCursor = true;
	controller->SetInputMode(FInputModeGameAndUI());

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
}

void ACPlayer::OffSelectAction()
{
	SelectActionWidget->SetVisibility(ESlateVisibility::Hidden);

	APlayerController* controller = GetController<APlayerController>();
	CheckNull(controller);

	controller->bShowMouseCursor = false;
	controller->SetInputMode(FInputModeGameOnly());

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void ACPlayer::ChangeColor(FLinearColor InColor)
{
	DynamicMaterial->SetVectorParameterValue("BodyColor", InColor);
}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACPlayer::Hitted()
{
	Montage->PlayHitted();
}

void ACPlayer::Dead()
{
	//Disable Input
	APlayerController* controller = GetController<APlayerController>();
	DisableInput(controller);

	//All Attachment Collision Disable
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Action->Dead();

	//Play Dead Montage
	Montage->PlayDead();

	//Destroy All
	UKismetSystemLibrary::K2_SetTimer(this, "End_Dead", 5.f, false);
}

void ACPlayer::End_Dead()
{
	Action->End_Dead();

	CLog::Print("Game Over", -1, 5.f, FColor::Red);

	GetController<APlayerController>()->ConsoleCommand("RestartLevel");
}

void ACPlayer::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == this);

	InteractableActor = Cast<IIInteractable>(OtherActor);
}

void ACPlayer::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OtherActor == this);

	if (!!InteractableActor)
		InteractableActor = nullptr;
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Roll:		Begin_Roll();		break;
		case EStateType::BackStep:	Begin_BackStep();	break;
		case EStateType::Hitted:	Hitted();			break;
		case EStateType::Dead:		Dead();				break;
	}
}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();
	FVector target = start + GetVelocity().GetSafeNormal2D();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));

	Montage->PlayRoll();
}

void ACPlayer::Begin_BackStep()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	Montage->PlayBackStep();
}

void ACPlayer::End_Roll()
{
	if (Action->GetCurrentDataAsset()->EquipmentData.bPawnControl == true)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	State->SetIdleMode();
}

void ACPlayer::End_BackStep()
{
	if (Action->GetCurrentDataAsset()->EquipmentData.bPawnControl == false)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	State->SetIdleMode();
}

