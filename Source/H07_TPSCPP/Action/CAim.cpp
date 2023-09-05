#include "CAim.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CHUD.h"
#include "GameFramework/GameModeBase.h"

UCAim::UCAim()
{
	CHelpers::GetAsset(&Curve, "/Game/Action/Curve_FOV");
}

void UCAim::BeginPlay(ACharacter* InOnwerCharacter)
{
	//Get OwnerCharacter & Components
	OwnerCharacter = InOnwerCharacter;
	SpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);

	//Set Timeline Properties
	OnFOVDelegate.BindUFunction(this, "OnFOV");
	FOVTimeline.AddInterpFloat(Curve, OnFOVDelegate);
	FOVTimeline.SetPlayRate(20.f);

	//Get HUD
	UWorld* world = OwnerCharacter->GetWorld();
	CheckNull(world);

	APlayerController* controller = world->GetFirstPlayerController();
	CheckNull(controller);

	HUD = controller->GetHUD<ACHUD>();
}

void UCAim::Tick(float DeltaTime)
{
	FOVTimeline.TickTimeline(DeltaTime);
}

void UCAim::On()
{
	CheckFalse(IsAvaliable());
	CheckTrue(bAiming);

	bAiming = true;
	HUD->See();

	SpringArm->TargetArmLength = 100.f;
	SpringArm->SocketOffset = FVector(0, 30, 10);
	SpringArm->bEnableCameraLag = false;

	FOVTimeline.PlayFromStart();
}

void UCAim::Off()
{
	CheckFalse(IsAvaliable());
	CheckFalse(bAiming);

	bAiming = false;
	HUD->NoSee();

	SpringArm->TargetArmLength = 200.f;
	SpringArm->SocketOffset = FVector(0, 0, 0);
	SpringArm->bEnableCameraLag = true;

	FOVTimeline.ReverseFromEnd();
}

void UCAim::OnFOV(float Output)
{
	Camera->FieldOfView = Output;
}
