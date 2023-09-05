#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	Wait, Patrol, Action, Approach, Hitted, Avoid
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChagedSignature, EBehaviorType, InPrevType, EBehaviorType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class H07_TPSCPP_API UCBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCBehaviorComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }


public: //Read from Blackboard::BehaviorKey
	UFUNCTION(BlueprintPure)
		bool IsWaitMode();

	UFUNCTION(BlueprintPure)
		bool IsPatrolMode();

	UFUNCTION(BlueprintPure)
		bool IsActionMode();

	UFUNCTION(BlueprintPure)
		bool IsApproachMode();

	UFUNCTION(BlueprintPure)
		bool IsHittedMode();

	UFUNCTION(BlueprintPure)
		bool IsAvoidMode();

	class ACPlayer* GetTargetPlayer();
	FVector GetWarpLocation();


public: //Write to Blackboard::BehaviorKey
	void SetWaitMode();
	void SetPatrolMode();
	void SetActionMode();
	void SetApproachMode();
	void SetHittedMode();
	void SetAvoidMode();

private:
	void ChangeType(EBehaviorType InNewType); //Write BehaviorKey to Blackboard
	EBehaviorType GetType(); //Read BehaviorKey from Blackboard

public:
	UPROPERTY(BlueprintAssignable)
		FBehaviorTypeChagedSignature OnBehaviorTypeChaged;

private:
	UPROPERTY(EditAnywhere)
		FName BehaviorKey = "BehaviorKey";

	UPROPERTY(EditAnywhere)
		FName PlayerKey = "PlayerKey";

	UPROPERTY(EditAnywhere)
		FName LocationKey = "LocationKey";

private:
	class UBlackboardComponent* Blackboard;
};
