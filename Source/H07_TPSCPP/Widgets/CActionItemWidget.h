#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CActionItemWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FImageButtonPressedSignature);

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

public:
	void SetImageButton(class UTexture2D* InImage);

private:
	class UCSelectActionWidget* GetParentWidget();

public:
	UPROPERTY(BlueprintAssignable)
		FImageButtonPressedSignature OnImageButtonPressed;

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* ImageButton;

};
