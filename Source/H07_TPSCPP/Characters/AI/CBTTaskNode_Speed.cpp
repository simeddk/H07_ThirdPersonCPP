#include "CBTTaskNode_Speed.h"
#include "Global.h"
#include "Characters/AI/CAIController.h"
#include "Characters/CEnemy_AI.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(enemy, EBTNodeResult::Failed);

	UCStatusComponent* statusComp = CHelpers::GetComponent<UCStatusComponent>(enemy);
	CheckNullResult(statusComp, EBTNodeResult::Failed);

	statusComp->SetSpeed(Type);

	return EBTNodeResult::Succeeded;
}
