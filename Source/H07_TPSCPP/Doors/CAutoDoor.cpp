#include "CAutoDoor.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

ACAutoDoor::ACAutoDoor()
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

void ACAutoDoor::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ACAutoDoor::BeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACAutoDoor::EndOverlap);

	Box->SetHiddenInGame(bHiddenInGame);
}

void ACAutoDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator currentRotation = Door->GetRelativeRotation();

	if (bOpen)
	{
		Door->SetRelativeRotation(FMath::Lerp
		(
			currentRotation,
			FRotator(0, SignedMaxDegree, 0),
			StaticAlpha
		));
		return;
	}
	
	Door->SetRelativeRotation(FMath::Lerp
	(
		currentRotation,
		FRotator::ZeroRotator,
		StaticAlpha
	));

}

void ACAutoDoor::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);
	CheckTrue(OtherActor == this);

	FVector doorForward = GetActorForwardVector();
	FVector characterForward = character->GetActorForwardVector();
	float signedDotEachForward = FMath::Sign(doorForward | characterForward);

	SignedMaxDegree = signedDotEachForward* MaxDegree;

	float dotResult =
		UKismetMathLibrary::GetDirectionUnitVector(character->GetActorLocation(), GetActorLocation()) | characterForward;

	if (dotResult > 0.f)
		bOpen = true;
}

void ACAutoDoor::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);
	CheckTrue(OtherActor == this);

	bOpen = false;
}

