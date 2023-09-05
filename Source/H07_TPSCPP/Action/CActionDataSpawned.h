// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CActionDataSpawned.generated.h"

//-----------------------------------------------------------------------------
//Struct - EquipmentData
//-----------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1.f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;
};

//-----------------------------------------------------------------------------
//Struct - DoActionData
//-----------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FDoActionData : public FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Power = 1.f;

	UPROPERTY(EditAnywhere)
		float HitStop;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShake> ShakeClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACMagicBall> ProjectileClass;
};

//-----------------------------------------------------------------------------
//DataAsset Spawnd Object Class
//-----------------------------------------------------------------------------
UCLASS()
class H07_TPSCPP_API UCActionDataSpawned : public UObject
{
	GENERATED_BODY()

public:
	friend class UCActionData;

public:
	FORCEINLINE class ACEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class ACDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE FLinearColor GetEquipmentColor() { return EquipmentColor; }

private:
	class ACEquipment* Equipment;
	class ACAttachment* Attachment;
	class ACDoAction* DoAction;
	FLinearColor EquipmentColor;
};
