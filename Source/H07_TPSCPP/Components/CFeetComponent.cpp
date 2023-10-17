#include "CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Engine/TriggerVolume.h"
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

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATriggerVolume::StaticClass(), actors);
	CheckFalse(actors.Num() > 0);

	for (const auto& trigger : actors)
	{
		trigger->OnActorBeginOverlap.AddDynamic(this, &UCFeetComponent::ActiveIK);
		trigger->OnActorEndOverlap.AddDynamic(this, &UCFeetComponent::DeactiveIK);
	}
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float leftDistance;
	FRotator leftRotation;
	Trace(LeftFootName, leftDistance, leftRotation);

	float rightDistance;
	FRotator rightRotation;
	Trace(RightFootName, rightDistance, rightRotation);

	float offset = FMath::Min(leftDistance, rightDistance);
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);

	

	Data.LeftDistance.Y = UKismetMathLibrary::FInterpTo(Data.LeftDistance.Y, leftDistance - offset, DeltaTime, InterpSpeed);
	Data.RightDistance.Y = UKismetMathLibrary::FInterpTo(Data.RightDistance.Y, rightDistance - offset, DeltaTime, InterpSpeed);

	CLog::Print("Data.LeftDistance.Y : " + FString::SanitizeFloat(Data.LeftDistance.Y), 1);
	CLog::Print("Data.RightDistance.Y : " + FString::SanitizeFloat(Data.RightDistance.Y), 2);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);;
}

void UCFeetComponent::Trace(FName InSocket, float& OutDistance, FRotator& OutRotation)
{
	OutDistance = 0.f;
	OutRotation = FRotator::ZeroRotator;

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

	float underGroundLength = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
	
	OutDistance = Correction + underGroundLength - Additional;

	FVector impactNormal = hitResult.ImpactNormal;
	float pitch = -UKismetMathLibrary::DegAtan2(impactNormal.X, impactNormal.Z);
	float roll = UKismetMathLibrary::DegAtan2(impactNormal.Y, impactNormal.Z);

	pitch = FMath::Clamp(pitch, -30.f, 30.f);
	roll = FMath::Clamp(roll, -30.f, 30.f);
	OutRotation = FRotator(pitch, 0.f, roll);
	
	DrawDebugDirectionalArrow
	(
		GetWorld(),
		hitResult.ImpactPoint,
		hitResult.ImpactPoint + hitResult.ImpactNormal * 100.f,
		3.f,
		FColor::Purple,
		false,
		-1,
		0,
		1.f
	);
}

void UCFeetComponent::ActiveIK(AActor* OverlappedActor, AActor* OtherActor)
{
	CheckNull(Cast<ACharacter>(OtherActor));
	bIKMode = true;
}

void UCFeetComponent::DeactiveIK(AActor* OverlappedActor, AActor* OtherActor)
{
	CheckNull(Cast<ACharacter>(OtherActor));
	bIKMode = false;
}

