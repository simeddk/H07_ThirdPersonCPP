#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CActionItemWidget.generated.h"

UCLASS()
class H07_TPSCPP_API UCActionItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
		void OnPressed();

	UFUNCTION(BlueprintCallable)
		void OnHover();

	UFUNCTION(BlueprintCallable)
		void OnUnhover();

private:
	class UCSelectActionWidget* GetParentWidget();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* ImageButton;
	
};
