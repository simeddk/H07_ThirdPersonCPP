#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector LeftDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector RightDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector PelvisDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator LeftRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator RightRotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class H07_TPSCPP_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCFeetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	FORCEINLINE const FFeetData& GetDatas() { return Data; }

private:
	void Trace(FName InSocket, float& OutDistance);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		FName LeftFootName = "Foot_L";

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		FName RightFootName = "Foot_R";

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		float Additional = 55.f;

	//Todo. 헤더파일 있어야 하는듯...
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForOneFrame;

private:
	FFeetData Data;

	class ACharacter* OwnerCharacter;
	float CapsuleHalfHeight;
};
