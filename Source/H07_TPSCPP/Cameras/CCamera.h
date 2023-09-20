#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "CCamera.generated.h"

UCLASS()
class H07_TPSCPP_API ACCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	ACCamera();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		void StartTimeline();

private:
	UFUNCTION()
		void OnProgress(float Output);

	UFUNCTION(BlueprintCallable)
		void OnFinish();

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		class UCurveFloat* Curve;

	UPROPERTY(EditAnywhere)
		float PlayRate = 0.25f;

	UPROPERTY(EditAnywhere)
		bool bDeactive;

private:
	FTimeline Timeline;
	class ACCameraSpline* CameraSpline;
};
