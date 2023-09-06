#include "CBTService_Melee.h"
#include "Global.h"


void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	CLog::Print(OwnerComp.GetName()); //<----
}

