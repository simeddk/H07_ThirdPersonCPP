#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Doors/IInteractable.h"
#include "ICharacter.h"
#include "CPlayer.generated.h"

UCLASS()
class H07_TPSCPP_API ACPlayer : public ACharacter, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

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
	void OnInteract();

	UFUNCTION() void OnFist();
	UFUNCTION() void OnOneHand();
	UFUNCTION() void OnTwoHand();
	UFUNCTION() void OnMagicBall();
	UFUNCTION() void OnWarp();
	UFUNCTION() void OnStorm();

	void OnAction();
	void OnAim();
	void OffAim();

	void OnSelectAction();
	void OffSelectAction();

public:
	virtual void ChangeColor(FLinearColor InColor) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	FORCEINLINE class UCSelectActionWidget* GetSelectActionWidget() { return SelectActionWidget; }

private:
	void Hitted();
	void Dead();

	UFUNCTION()
		void End_Dead();

private:
	UFUNCTION()
		void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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

	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;

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
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		uint8 TeamID = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UCSelectActionWidget> SelectActionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UCHealthWidget_Player> HealthWidgetClass;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
		class UCSelectActionWidget* SelectActionWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Widget")
		class UCHealthWidget_Player* HealthWidget;

private:
	class UMaterialInstanceDynamic* DynamicMaterial;

	float DamageValue;
	class ACharacter* Attacker;
	class AActor* Causer;

	IIInteractable* InteractableActor;
};
