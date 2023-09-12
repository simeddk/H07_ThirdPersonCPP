#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Magic.generated.h"

UCLASS()
class H07_TPSCPP_API UCBTService_Magic : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_Magic();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
