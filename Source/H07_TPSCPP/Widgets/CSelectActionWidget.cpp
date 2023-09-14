#include "CSelectActionWidget.h"
#include "Global.h"
#include "Components/GridPanel.h"
#include "Components/Border.h"
#include "CActionItemWidget.h"

void UCSelectActionWidget::NativeConstruct()
{
	TArray<UWidget*> children = Grid->GetAllChildren();
	for (const auto& child : children)
	{
		UCActionItemWidget* item = Cast<UCActionItemWidget>(child);
		Items.Add(child->GetName(), item);
	}

	Super::NativeConstruct();
}

void UCSelectActionWidget::Pressed(FString InName)
{
	//Todo. 실제 무기 교체
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
