#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CNameWidget.generated.h"

UCLASS()
class H07_TPSCPP_API UCNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
		void SetNameText(const FString& InControllerName, const FString& InCharacterName);
	void SetNameText_Implementation(const FString& InControllerName, const FString& InCharacterName);
	
private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ControllerName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CharacterName;
};
