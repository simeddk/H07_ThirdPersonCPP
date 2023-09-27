#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHealthWidget_Player.generated.h"

UCLASS()
class H07_TPSCPP_API UCHealthWidget_Player : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintNativeEvent)
		void Update(float InCurrentHealth, float InMaxHealth);
	void Update_Implementation(float InCurrentHealth, float InMaxHealth);

private:
	UPROPERTY(meta = (BindWidget))
		class UImage* CircleGuage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentHealthText;

	//UPROPERTY(meta = (BindWidgetAnimation))
	//	class UWidgetAnimation* HittedAnim;

private:
	class UMaterialInstanceDynamic* Material;
};
