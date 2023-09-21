#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteractable.h"
#include "CInteractDoor.generated.h"

enum class EDoorType { Done, Opening, Closing };

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
	void Opening(float InDeltaTime);
	void Closing(float InDeltaTime);

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

	UPROPERTY(EditAnywhere)
		float Speed = 100.f;

private:
	bool bClosed = true; //문이 열려 있는지 닫혀 있는지
	EDoorType Type;

	float SignedDotEachForward; //앞에서 문을 열었는지, 뒤에서 열었는지
	float SignedMaxDegree; //활짝 열려야 할 각도(앞뒤 방향이 적용이 된)
	
	float CurrentYaw; //점점 열리거나 닫히는 Yaw 값
};
