#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/TimelineComponent.h"
#include "CAim.generated.h"

UCLASS()
class H07_TPSCPP_API UCAim : public UObject
{
	GENERATED_BODY()

public:
	UCAim();

	void BeginPlay(class ACharacter* InOnwerCharacter);
	void Tick(float DeltaTime);

public:
	FORCEINLINE bool IsAvaliable() { return (SpringArm != nullptr) && (Camera != nullptr); }
	FORCEINLINE bool IsAiming() { return bAiming; }

	void On();
	void Off();
	
private:
	class ACharacter* OwnerCharacter;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

	bool bAiming;

private:
	UFUNCTION()
		void OnFOV(float Output);

private:
	UPROPERTY()
		class UCurveFloat* Curve;

	FTimeline FOVTimeline;
	FOnTimelineFloat OnFOVDelegate;

	class ACHUD* HUD;
};
