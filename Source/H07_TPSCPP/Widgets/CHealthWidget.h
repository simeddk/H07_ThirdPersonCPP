#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHealthWidget.generated.h"

UCLASS()
class H07_TPSCPP_API UCHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
		void Update(float InCurrentHealth, float InMaxHealth);
	void Update_Implementation(float InCurrentHealth, float InMaxHealth);

private:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	
};
