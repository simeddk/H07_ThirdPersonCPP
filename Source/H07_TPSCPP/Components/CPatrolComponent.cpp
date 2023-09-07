#include "CPatrolComponent.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Characters/AI/CPatrolPath.h"

UCPatrolComponent::UCPatrolComponent()
{

}


void UCPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCPatrolComponent::UpdateNextIndex()
{
	CheckNull(Path);

	int32 count = Path->GetSplineComponent()->GetNumberOfSplinePoints();

	//Reverse
	if (bReverse)
	{
		//Index is not 0 yet
		if (Index > 0)
		{
			Index--;
			return;
		}

		//Index 0 Reached
		//-> Is Closed Loop True
		if (Path->GetSplineComponent()->IsClosedLoop())
		{
			Index = count - 1;
			return;
		}
		
		//-> Is Closed Loop False
		Index = 1;
		bReverse = false;

		return;
	}

	//Forward
	//Index is not Last yet
	if (Index < count - 1)
	{
		Index++;
		return;
	}

	//Index Reached Last
	//-> Is Closed Loop True
	if (Path->GetSplineComponent()->IsClosedLoop())
	{
		Index = 0;
		return;
	}

	//-> Is Closed Loop False
	Index = count - 2;
	bReverse = true;

}

bool UCPatrolComponent::GetMoveToLocation(FVector& OutLocation)
{
	OutLocation = FVector::ZeroVector;
	CheckNullResult(Path, false);

	OutLocation = Path->GetSplineComponent()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
	return true;
}
