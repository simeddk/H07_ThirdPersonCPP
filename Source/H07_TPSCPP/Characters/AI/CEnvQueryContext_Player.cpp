#include "CEnvQueryContext_Player.h"
#include "Global.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Characters/AI/CAIController.h"
#include "Components/CBehaviorComponent.h"

void UCEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	if (enemy == nullptr)
	{
		CLog::Log("EQS Owner is not ACEnemy_AI Type!!");
		return;
	}

	ACAIController* controller = Cast<ACAIController>(enemy->GetController());
	if (controller == nullptr)
	{
		CLog::Log("AI Controller is not possessed");
		return;
	}
	
	UCBehaviorComponent* behaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	if (!!behaviorComp)
	{
		if (!!behaviorComp->GetTargetPlayer())
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, behaviorComp->GetTargetPlayer());
		}
	}
}