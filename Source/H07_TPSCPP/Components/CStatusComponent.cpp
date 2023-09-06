#include "CStatusComponent.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

UCStatusComponent::UCStatusComponent()
{

}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void UCStatusComponent::SetMove()
{
	bCanMove = true;
}

void UCStatusComponent::SetStop()
{
	bCanMove = false;
}

void UCStatusComponent::SetSpeed(EMoveSpeedType InType)
{
	UCharacterMovementComponent* movementComp = CHelpers::GetComponent<UCharacterMovementComponent>(GetOwner());
	CheckNull(movementComp);

	movementComp->MaxWalkSpeed = Speeds[(int32)InType];
}

void UCStatusComponent::IncreaseHealth(float InAmount)
{
	CurrentHealth += InAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
}

void UCStatusComponent::DecreaseHealth(float InAmount)
{
	CurrentHealth -= InAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
}

