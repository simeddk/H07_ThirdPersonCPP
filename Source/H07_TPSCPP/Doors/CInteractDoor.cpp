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

}

void ACInteractDoor::Interact(ACharacter* InCharacter)
{
	//플레이어와 문의 내적
	float dot = GetActorForwardVector() | InCharacter->GetActorForwardVector();

	//플레이어 -> 문의 방향
	FVector direction = GetActorLocation() - InCharacter->GetActorLocation();
	direction.Normalize();

	//플레이어가 문을 바라보면 +1, 등 지면 -1
	Direction = FMath::Sign(direction | InCharacter->GetActorForwardVector());
	CLog::Print(Direction);
}

