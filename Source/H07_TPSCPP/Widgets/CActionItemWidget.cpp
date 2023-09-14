#include "CActionItemWidget.h"
#include "Global.h"
#include "Components/Button.h"
#include "Characters/CPlayer.h"
#include "CSelectActionWidget.h"

void UCActionItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CheckNull(ImageButton);

	ImageButton->OnPressed.AddDynamic(this, &UCActionItemWidget::OnPressed);
	ImageButton->OnHovered.AddDynamic(this, &UCActionItemWidget::OnHover);
	ImageButton->OnUnhovered.AddDynamic(this, &UCActionItemWidget::OnUnhover);
}

void UCActionItemWidget::OnPressed()
{
	CheckNull(GetParentWidget());
	GetParentWidget()->Pressed(GetName());
}

void UCActionItemWidget::OnHover()
{
	CheckNull(GetParentWidget());
	GetParentWidget()->Hover(GetName());
}

void UCActionItemWidget::OnUnhover()
{
	CheckNull(GetParentWidget());
	GetParentWidget()->Unhover(GetName());
}

UCSelectActionWidget* UCActionItemWidget::GetParentWidget()
{
	ACPlayer* player = Cast<ACPlayer>(GetOwningPlayer()->GetPawn());
	CheckNullResult(player, nullptr);

	return player->GetSelectActionWidget();
}
