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
	bool bClosed = true; //���� ���� �ִ��� ���� �ִ���
	EDoorType Type;

	float SignedDotEachForward; //�տ��� ���� ��������, �ڿ��� ��������
	float SignedMaxDegree; //Ȱ¦ ������ �� ����(�յ� ������ ������ ��)
	
	float CurrentYaw; //���� �����ų� ������ Yaw ��
};
