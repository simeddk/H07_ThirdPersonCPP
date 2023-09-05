#pragma once

#include "CoreMinimal.h"
#include "Action/CDoAction.h"
#include "Components/CActionComponent.h"
#include "CDoAction_MagicBall.generated.h"

UCLASS()
class H07_TPSCPP_API ACDoAction_MagicBall : public ACDoAction
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float Deltatime) override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;
	
	virtual void OnAim() override;
	virtual void OffAim() override;

private:
	UFUNCTION()
		void OnMagicBallBeginOverlap(FHitResult InResult);

	UFUNCTION()
		void AbortByActionTypeChanged(EActionType InPrevType, EActionType InNewType);

private:
	UPROPERTY()
		class UCAim* Aim;

	class UCActionComponent* ActionComp;
};
