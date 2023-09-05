#include "CNameWidget.h"
#include "Components/TextBlock.h"

void UCNameWidget::SetNameText_Implementation(const FString& InControllerName, const FString& InCharacterName)
{
	ControllerName->SetText(FText::FromString(InControllerName));
	CharacterName->SetText(FText::FromString(InCharacterName.Replace(L"BP_CEnemy_", L"")));
}
