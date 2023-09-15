#include "CSelectActionWidget.h"
#include "Global.h"
#include "Components/GridPanel.h"
#include "Components/Border.h"
#include "CActionItemWidget.h"

void UCSelectActionWidget::NativeConstruct()
{
	//Register Item Widget to Map(with string key)
	TArray<UWidget*> children = Grid->GetAllChildren();
	for (const auto& child : children)
	{
		UCActionItemWidget* item = Cast<UCActionItemWidget>(child);
		Items.Add(child->GetName(), item);
	}

	//Set Images from Widget Blueprint
	Super::NativeConstruct();

	//Set Images to Item Button
	for (int32 i = 0; i < Items.Num(); i++)
	{
		FString key = "Item";
		key.Append(FString::FromInt(i + 1));

		Items[key]->SetImageButton(Images[i]);
	}
}

void UCSelectActionWidget::Pressed(FString InName)
{
	if (Items[InName]->OnImageButtonPressed.IsBound())
		Items[InName]->OnImageButtonPressed.Broadcast();

	APlayerController* controller = GetOwningPlayer();
	CheckNull(controller);

	controller->bShowMouseCursor = false;
	controller->SetInputMode(FInputModeGameOnly());

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void UCSelectActionWidget::Hover(FString InName)
{
	UBorder* border = Cast<UBorder>(Items[InName]->GetWidgetFromName("ItemBorder"));
	CheckNull(border);

	border->SetBrushColor(FLinearColor::Red);
}

void UCSelectActionWidget::Unhover(FString InName)
{
	UBorder* border = Cast<UBorder>(Items[InName]->GetWidgetFromName("ItemBorder"));
	CheckNull(border);

	border->SetBrushColor(FLinearColor::White);
}
