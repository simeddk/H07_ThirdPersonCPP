#include "CBehaviorComponent.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CPlayer.h"

UCBehaviorComponent::UCBehaviorComponent()
{

}


void UCBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

bool UCBehaviorComponent::IsWaitMode()
{
	return GetType() == EBehaviorType::Wait;
}

bool UCBehaviorComponent::IsPatrolMode()
{
	return GetType() == EBehaviorType::Patrol;
}

bool UCBehaviorComponent::IsActionMode()
{
	return GetType() == EBehaviorType::Action;
}

bool UCBehaviorComponent::IsApproachMode()
{
	return GetType() == EBehaviorType::Approach;
}

bool UCBehaviorComponent::IsHittedMode()
{
	return GetType() == EBehaviorType::Hitted;
}

bool UCBehaviorComponent::IsAvoidMode()
{
	return GetType() == EBehaviorType::Avoid;
}

ACPlayer* UCBehaviorComponent::GetTargetPlayer()
{
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

FVector UCBehaviorComponent::GetWarpLocation()
{
	return Blackboard->GetValueAsVector(LocationKey);
}

void UCBehaviorComponent::SetWaitMode()
{
	ChangeType(EBehaviorType::Wait);
}

void UCBehaviorComponent::SetPatrolMode()
{
	ChangeType(EBehaviorType::Patrol);
}

void UCBehaviorComponent::SetActionMode()
{
	ChangeType(EBehaviorType::Action);
}

void UCBehaviorComponent::SetApproachMode()
{
	ChangeType(EBehaviorType::Approach);
}

void UCBehaviorComponent::SetHittedMode()
{
	ChangeType(EBehaviorType::Hitted);
}

void UCBehaviorComponent::SetAvoidMode()
{
	ChangeType(EBehaviorType::Avoid);
}

void UCBehaviorComponent::ChangeType(EBehaviorType InNewType)
{
	EBehaviorType prevType = InNewType;
	Blackboard->SetValueAsEnum(BehaviorKey, (uint8)InNewType);

	if (OnBehaviorTypeChaged.IsBound())
		OnBehaviorTypeChaged.Broadcast(prevType, InNewType);
}

EBehaviorType UCBehaviorComponent::GetType()
{
	return (EBehaviorType)Blackboard->GetValueAsEnum(BehaviorKey);
}
