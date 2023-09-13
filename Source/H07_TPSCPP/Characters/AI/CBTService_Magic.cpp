#include "CBTService_Magic.h"
#include "Global.h"
#include "Characters/AI/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"


UCBTService_Magic::UCBTService_Magic()
{
	NodeName = "Root_Magic";
}

void UCBTService_Magic::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	UCBehaviorComponent* behaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behaviorComp);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(enemy);

	UCStateComponent* stateComp = CHelpers::GetComponent<UCStateComponent>(enemy);
	CheckNull(stateComp);

	//Already Dead
	CheckTrue(stateComp->IsDeadMode());

	//Hitted(Taken Damage)
	if (stateComp->IsHittedMode())
	{
		behaviorComp->SetHittedMode();
		return;
	}

	//Get Player from Blackboard
	ACPlayer* player = behaviorComp->GetTargetPlayer();

	//Not Perceived Player
	if (player == nullptr)
	{
		behaviorComp->SetWaitMode();
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		return;
	}

	//Perceived Player
	// -> Look At Player(Focus)
	controller->SetFocus(player);

	// -> Get Distance
	float distance = enemy->GetDistanceTo(player);

	//In Avoid Range
	if (distance < controller->GetBehaviorRange())
	{
		behaviorComp->SetAvoidMode();
		return;
	}

	//In Sight
	if (distance < controller->GetSightRadius())
	{
		behaviorComp->SetActionMode();
		return;
	}
}
