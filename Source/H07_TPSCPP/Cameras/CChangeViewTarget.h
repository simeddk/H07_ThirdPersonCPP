#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CChangeViewTarget.generated.h"

UCLASS()
class H07_TPSCPP_API ACChangeViewTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	ACChangeViewTarget();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void ChangeCamera();

private:
	UPROPERTY(EditInstanceOnly)
		TArray<class ACameraActor*> Cameras;

	uint32 Index;
};
