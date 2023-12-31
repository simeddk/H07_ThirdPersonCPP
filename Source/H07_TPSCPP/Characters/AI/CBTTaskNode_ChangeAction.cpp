#include "CBTTaskNode_ChangeAction.h"
#include "Global.h"
#include "Characters/AI/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_ChangeAction::UCBTTaskNode_ChangeAction()
{
	NodeName = "ChangeAction";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_ChangeAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(enemy, EBTNodeResult::Failed);

	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(enemy);
	CheckNullResult(actionComp, EBTNodeResult::Failed);

	if (Type == EActionType::Warp)
	{
		if (actionComp->IsWarpMode() == false)
			actionComp->SetWarpMode();
	}
	else if (Type == EActionType::MagicBall)
	{
		if (actionComp->IsMagicBallMode() == false)
			actionComp->SetMagicBallMode();
	}

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_ChangeAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(enemy);

	UCStateComponent* stateComp = CHelpers::GetComponent<UCStateComponent>(enemy);
	CheckNull(stateComp);

	if (stateComp->IsIdleMode())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
