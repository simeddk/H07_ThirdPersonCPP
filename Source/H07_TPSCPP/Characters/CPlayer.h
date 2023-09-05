#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "ICharacter.h"
#include "CPlayer.generated.h"

UCLASS()
class H07_TPSCPP_API ACPlayer : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private: // Axis Event
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);
	void OnZoom(float InAxis);

private: //Action Event
	void OnRun();
	void OffRun();
	void OnEvade();

	void OnFist();
	void OnOneHand();
	void OnTwoHand();
	void OnMagicBall();
	void OnWarp();
	void OnStorm();

	void OnAction();
	void OnAim();
	void OffAim();

public:
	virtual void ChangeColor(FLinearColor InColor) override;

private: //Scene Component
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

private: //Actor Component
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontageComponent* Montage;

	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void Begin_Roll();
	void Begin_BackStep();

public:
	void End_Roll();
	void End_BackStep();

private:
	class UMaterialInstanceDynamic* DynamicMaterial;
};
