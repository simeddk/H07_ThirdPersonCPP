#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteractable.h"
#include "CInteractDoor.generated.h"

UCLASS()
class H07_TPSCPP_API ACInteractDoor : public AActor, public IIInteractable
{
	GENERATED_BODY()
	
public:	
	ACInteractDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void Interact(class ACharacter* InCharacter) override;

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

private:
	bool bClose = true;
	bool bOpening = false;
	bool bClosing = false;

	float Direction;
};
