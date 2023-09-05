#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlapSignature, class ACharacter*, InAttacker, class AActor*, InCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentEndOverlapSignature, class ACharacter*, InAttacker, class AActor*, InCauser, class ACharacter*, InOtherCharacter);

UCLASS()
class H07_TPSCPP_API ACAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAttachment();

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSocketName);

	UFUNCTION(BlueprintCallable)
		void AttachToSceneComponent(class USceneComponent* InComponent, FName InSocketName);

public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnEquip();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUnequip();

public:
	void OnCollisions();
	void OffCollisions();

private:
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(BlueprintAssignable)
		FAttachmentBeginOverlapSignature OnAttachmentBeginOverlap;

	UPROPERTY(BlueprintAssignable)
		FAttachmentEndOverlapSignature OnAttachmentEndOverlap;


private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* StateComp;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* StatusComp;

private:
	TArray<class UShapeComponent*> ShapeComponents;
};
