#include "CChangeViewTarget.h"
#include "Global.h"
#include "Camera/CameraActor.h"

ACChangeViewTarget::ACChangeViewTarget()
{

}

void ACChangeViewTarget::BeginPlay()
{
	Super::BeginPlay();
	
	
	UKismetSystemLibrary::K2_SetTimer(this, "ChangeCamera", 2.f, true);
}

void ACChangeViewTarget::ChangeCamera()
{
	CheckFalse(Cameras.Num() > 0);
	
	/*APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (!!cameraManager)
	{
		cameraManager->SetViewTarget(Cameras[Index]);
		Index++;

		Index %= Cameras.Num();
	}*/

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!!controller)
	{
		controller->SetViewTargetWithBlend(Cameras[Index], 2.f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.f);
		Index++;

		Index %= Cameras.Num();
	}
}



