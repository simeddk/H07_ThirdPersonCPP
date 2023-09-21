#include "CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float leftDistance;
	Trace(LeftFootName, leftDistance);

	float rightDistance;
	Trace(RightFootName, rightDistance);
}

void UCFeetComponent::Trace(FName InSocket, float& OutDistance)
{
	OutDistance = 0.f;

	FVector socketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(InSocket);

	FVector start = FVector(socketLocation.X, socketLocation.Y, OwnerCharacter->GetActorLocation().Z);
	FVector end = FVector(start.X, start.Y, start.Z - CapsuleHalfHeight - Additional);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);
	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		true,
		ignoreActors,
		DrawDebugType,
		hitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	CheckFalse(hitResult.IsValidBlockingHit());

	OutDistance = 100.f;
	//Todo. 이 컴포넌트를 플레이어에게 붙여줘야 함...
}

