#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UENUM(BlueprintType)
enum class EMoveSpeedType : uint8
{
	Sneak, Walk, Run, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class H07_TPSCPP_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; }

	FORCEINLINE float GetSneakSpeed()	{ return Speeds[(int32)EMoveSpeedType::Sneak]; }
	FORCEINLINE float GetWalkSpeed()	{ return Speeds[(int32)EMoveSpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed()		{ return Speeds[(int32)EMoveSpeedType::Run]; }
	
	FORCEINLINE bool IsCanMove() { return bCanMove; }
	void SetMove();
	void SetStop();
	void SetSpeed(EMoveSpeedType InType);


	void IncreaseHealth(float InAmount);
	void DecreaseHealth(float InAmount);

		
private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speeds[(int32)EMoveSpeedType::Max] = { 200.f, 400.f, 600.f };
	
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100.f;

private:
	bool bCanMove = true;
	float CurrentHealth;
};
