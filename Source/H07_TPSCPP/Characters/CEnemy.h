#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ICharacter.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class H07_TPSCPP_API ACEnemy : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void Hitted();
	void Dead();

	UFUNCTION()
		void End_Dead();

public:
	virtual void ChangeColor(FLinearColor InColor) override;

private:
	UFUNCTION()
		void RestoreColor();

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	UPROPERTY(EditAnywhere)
		float LaunchValue = 25.f;

private: //Scene Component
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;

protected: //Actor Component
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCActionComponent* Action;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontageComponent* Montage;
	
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;
	
	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

private:
	class ACharacter* Attacker;
	float DamageValue;
	class AActor* Causer;

	class UMaterialInstanceDynamic* LowerBodyMaterial;
	class UMaterialInstanceDynamic* UpperBodyMaterial;

};
