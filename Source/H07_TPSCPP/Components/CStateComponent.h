#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle, Roll, BackStep, Equip, Action, Hitted, Dead, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChangedSignature, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class H07_TPSCPP_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public: //Get State
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsRollMode() { return Type == EStateType::Roll; }
	
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsBackStepMode() { return Type == EStateType::BackStep; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE EStateType GetType() { return Type; }

public: //Set State
	void SetIdleMode();
	void SetRollMode();
	void SetBackStepMode();
	void SetEquipMode();
	void SetActionMode();
	void SetHittedMode();
	void SetDeadMode();

private:
	void ChangeType(EStateType InNewType);

	void PrintType();
	
public:
	UPROPERTY(BlueprintAssignable)
		FStateTypeChangedSignature OnStateTypeChanged;

private:
	EStateType Type;
};
