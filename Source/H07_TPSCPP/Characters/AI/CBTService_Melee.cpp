#include "CBTService_Melee.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/AI/CAIController.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Root_Melee";
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		return;
	}

	//Perceived Player & Get Distance
	float distance = enemy->GetDistanceTo(player);

	//In ActionRange
	if (distance < controller->GetBehaviorRange())
	{
		behaviorComp->SetActionMode();
		return;
	}

	//In Sight
	if (distance < controller->GetSightRadius())
	{
		behaviorComp->SetApproachMode();
		return;
	}

	
}

