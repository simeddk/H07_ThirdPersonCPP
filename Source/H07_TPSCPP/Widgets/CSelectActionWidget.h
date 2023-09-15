#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSelectActionWidget.generated.h"

UCLASS()
class H07_TPSCPP_API UCSelectActionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE class UCActionItemWidget* GetItem(FString InName) { return Items[InName]; }

	UFUNCTION(BlueprintCallable)
		void Pressed(FString InName);

	UFUNCTION(BlueprintCallable)
		void Hover(FString InName);

	UFUNCTION(BlueprintCallable)
		void Unhover(FString InName);
	
private:
	UPROPERTY(meta = (BindWidget))
		class UGridPanel* Grid;

protected:
	UPROPERTY(BlueprintReadOnly)
		TMap<FString, class UCActionItemWidget*> Items;

	UPROPERTY(BlueprintReadWrite)
		TArray<class UTexture2D*> Images;
};
