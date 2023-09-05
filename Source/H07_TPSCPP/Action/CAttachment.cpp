#include "CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

ACAttachment::ACAttachment()
{
	CHelpers::CreateSceneComponent(this, &Root, "Root");

}

void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	StateComp = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	StatusComp = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	GetComponents<UShapeComponent>(ShapeComponents);
	if (ShapeComponents.Num() > 0)
	{
		for (const auto& collision : ShapeComponents)
		{
			collision->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::BeginOverlap);
			collision->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::EndOverlap);
		}
	}

	OffCollisions();

	Super::BeginPlay();
}

void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent
	(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSocketName
	);
}

void ACAttachment::AttachToSceneComponent(USceneComponent* InComponent, FName InSocketName)
{
	InComponent->AttachToComponent
	(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSocketName
	);
}

void ACAttachment::OnCollisions()
{
	for (const auto& collision : ShapeComponents)
		collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACAttachment::OffCollisions()
{
	for (const auto& collision : ShapeComponents)
		collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACAttachment::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == OwnerCharacter);
	
	ACharacter* otherCharacter = Cast<ACharacter>(OtherActor);
	CheckNull(otherCharacter);

	if (OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, otherCharacter);
}

void ACAttachment::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* otherCharacter = Cast<ACharacter>(OtherActor);
	CheckNull(otherCharacter);

	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, this, otherCharacter);
}

