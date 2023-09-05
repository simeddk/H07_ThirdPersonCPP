#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CActionDataSpawned.h"
#include "CActionData.generated.h"

UCLASS()
class H07_TPSCPP_API UCActionData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	void BeginPlay(class ACharacter* InOwnerCharacter, UCActionDataSpawned** OutSpawned);

private:
	FString GetCustomLabelName(class ACharacter* InOwnerCharacter, FString InMiddleName);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipment")
		TSubclassOf<class ACEquipment> EquipmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipment")
		FEquipmentData EquipmentData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipment")
		FLinearColor EquipmentColor = FLinearColor(0, 0, 0);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attachment")
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DoAction")
		TSubclassOf<class ACDoAction> DoActionClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DoAction")
		TArray<FDoActionData> DoActionDatas;

};
