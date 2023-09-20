#include "CCamera.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/SplineComponent.h"
#include "CCameraSpline.h"

ACCamera::ACCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateSceneComponent(this, &Camera, "Camera");
	CHelpers::GetAsset(&Curve, "/Game/Player/Curve_CameraSpline");
}

void ACCamera::BeginPlay()
{
	Super::BeginPlay();
	
	//Get CameraSpline from World
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACCameraSpline::StaticClass(), actors);
	CheckFalse(actors.Num() > 0);
	CameraSpline = Cast<ACCameraSpline>(actors[0]);

	//Timeline Play Event
	FOnTimelineFloat onProgress;
	onProgress.BindUFunction(this, "OnProgress");
	Timeline.AddInterpFloat(Curve, onProgress);

	//Timeline End Evnet
	FOnTimelineEvent onFinish;
	onFinish.BindUFunction(this, "OnFinish");
	Timeline.SetTimelineFinishedFunc(onFinish);

	//Timeline Speed
	Timeline.SetPlayRate(PlayRate);
}

void ACCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Timeline.IsPlaying())
		Timeline.TickTimeline(DeltaTime);
}

void ACCamera::StartTimeline()
{
	CheckTrue(bDeactive);

	Timeline.PlayFromStart();

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(controller);
	controller->SetViewTarget(this);
}

void ACCamera::OnProgress(float Output)
{
	USplineComponent* splineComp = CHelpers::GetComponent<USplineComponent>(CameraSpline);
	CheckNull(splineComp);

	float length = splineComp->GetSplineLength();

	FVector location = splineComp->GetLocationAtDistanceAlongSpline(length * Output, ESplineCoordinateSpace::World);
	FRotator rotation = splineComp->GetRotationAtDistanceAlongSpline(length * Output, ESplineCoordinateSpace::World);

	SetActorLocation(location);
	SetActorRotation(rotation);
}

void ACCamera::OnFinish()
{
	Timeline.Stop();

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(controller);

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	CheckNull(player);

	controller->SetViewTarget(player);
}

