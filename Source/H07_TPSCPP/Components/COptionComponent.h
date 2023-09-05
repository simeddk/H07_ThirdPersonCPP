#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "COptionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class H07_TPSCPP_API UCOptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCOptionComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetHorizontalLookSpeed() { return HorizontalLookSpeed; }
	FORCEINLINE float GetVerticalLookSpeed() { return VerticalLookSpeed; }

	FORCEINLINE float GetZoomSpeed() { return ZoomSpeed; }
	FORCEINLINE float GetZoomMin() { return ZoomRange.X; }
	FORCEINLINE float GetZoomMax() { return ZoomRange.Y; }


private:
	UPROPERTY(EditAnywhere, Category = "Mouse")
		float HorizontalLookSpeed = 90.f;

	UPROPERTY(EditAnywhere, Category = "Mouse")
		float VerticalLookSpeed = 45.f;

	UPROPERTY(EditAnywhere, Category = "Mouse")
		float ZoomSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Mouse")
		FVector2D ZoomRange = FVector2D(50, 500);
};
