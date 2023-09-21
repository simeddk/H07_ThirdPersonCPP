#include "CInteractDoor.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

ACInteractDoor::ACInteractDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateSceneComponent(this, &Root, "Root");
	CHelpers::CreateSceneComponent(this, &Box, "Box", Root);
	CHelpers::CreateSceneComponent(this, &DoorFrame, "DoorFrame", Root);
	CHelpers::CreateSceneComponent(this, &Door, "Door", DoorFrame);

	UStaticMesh* frameMeshAsset;
	CHelpers::GetAsset<UStaticMesh>(&frameMeshAsset, "/Game/DoorMesh/Props/SM_DoorFrame");
	DoorFrame->SetStaticMesh(frameMeshAsset);

	UStaticMesh* doorMeshAsset;
	CHelpers::GetAsset<UStaticMesh>(&doorMeshAsset, "/Game/DoorMesh/Props/SM_Door");
	Door->SetStaticMesh(doorMeshAsset);
	Door->SetRelativeLocation(FVector(0, 45, 0));

	Box->SetRelativeLocation(FVector(0, 0, 100));
	Box->InitBoxExtent(FVector(150, 55, 100));
}

void ACInteractDoor::BeginPlay()
{
	Super::BeginPlay();
	
	Box->SetHiddenInGame(bHiddenInGame);
}

void ACInteractDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (Type)
	{
		case EDoorType::Opening:	Opening(DeltaTime);		break;
		case EDoorType::Closing:	Closing(DeltaTime);		break;
	}
}

void ACInteractDoor::Interact(ACharacter* InCharacter)
{
	//플레이어와 문의 내적
	float dot = GetActorForwardVector() | InCharacter->GetActorForwardVector();
	SignedDotEachForward = FMath::Sign(dot);
	SignedMaxDegree = MaxDegree * SignedDotEachForward;

	//플레이어 -> 문의 방향
	FVector direction = GetActorLocation() - InCharacter->GetActorLocation();
	direction.Normalize();

	//플레이어가 문을 바라보면 +1, 등 지면 -1
	float signedLookAtDoor = FMath::Sign(direction | InCharacter->GetActorForwardVector());
	
	//플레이어가 문을 바라보고 있을 때만 동작
	CheckFalse(signedLookAtDoor > 0);

	//문이 현재 열려 있는지, 닫혀 있는지
	if (bClosed)
	{
		bClosed = false;
		Type = EDoorType::Opening;

		return;
	}

	bClosed = true;
	Type = EDoorType::Closing;

}

void ACInteractDoor::Opening(float InDeltaTime)
{
	CurrentYaw = SignedDotEachForward * Speed * InDeltaTime;

	if (FMath::IsNearlyEqual(Door->GetRelativeRotation().Yaw, SignedMaxDegree, 2.f))
	{
		Type = EDoorType::Done;
		Door->SetRelativeRotation(FRotator(0, SignedMaxDegree, 0));
	}

	Door->AddRelativeRotation(FRotator(0, CurrentYaw, 0));
}

void ACInteractDoor::Closing(float InDeltaTime)
{
	CurrentYaw = (Door->GetRelativeRotation().Yaw > 0.f ? -Speed : +Speed) * InDeltaTime;

	if (FMath::IsNearlyZero(Door->GetRelativeRotation().Yaw, 2.f))
	{
		Type = EDoorType::Done;
		Door->SetRelativeRotation(FRotator::ZeroRotator);
	}

	Door->AddRelativeRotation(FRotator(0, CurrentYaw, 0));
}

