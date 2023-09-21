#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAutoDoor.generated.h"

UCLASS()
class H07_TPSCPP_API ACAutoDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAutoDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* DoorFrame;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Door;

	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

private:
	UPROPERTY(EditAnywhere)
		bool bHiddenInGame;

	UPROPERTY(EditAnywhere)
		float MaxDegree = 120.f;

	UPROPERTY(EditAnywhere, meta = (ClampMax = 1.00, ClampMin = 0.00))
		float StaticAlpha = 0.05f;

private:
	bool bOpen;

	float SignedMaxDegree;
};
