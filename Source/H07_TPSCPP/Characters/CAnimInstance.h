#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CActionComponent.h"
#include "Components/CFeetComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class H07_TPSCPP_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
		void OnActionTypeChanged(EActionType InPrevType, EActionType InNewType);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float Direction;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		EActionType ActionType;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FFeetData Data;
};
