#include "CBTTaskNode_Action.h"
#include "Global.h"
#include "Characters/AI/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(enemy, EBTNodeResult::Failed);

	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(enemy);
	CheckNullResult(actionComp, EBTNodeResult::Failed);

	actionComp->DoAction();

	RunningTime = 0.f;

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	RunningTime += DeltaSeconds;

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(enemy);

	UCStateComponent* stateComp = CHelpers::GetComponent<UCStateComponent>(enemy);
	CheckNull(stateComp);

	if (stateComp->IsIdleMode() && RunningTime > Delay)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
