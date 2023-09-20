#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCameraSpline.generated.h"

UCLASS()
class H07_TPSCPP_API ACCameraSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	ACCameraSpline();

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USplineComponent* Spline;

};
