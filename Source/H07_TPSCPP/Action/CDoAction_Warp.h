#pragma once

#include "CoreMinimal.h"
#include "Action/CDoAction.h"
#include "CDoAction_Warp.generated.h"

UCLASS()
class H07_TPSCPP_API ACDoAction_Warp : public ACDoAction
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction()  override;

private:
	bool IsPlayerControlled();
	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);

private:
	class UDecalComponent* Decal;
	FVector Location;
};
