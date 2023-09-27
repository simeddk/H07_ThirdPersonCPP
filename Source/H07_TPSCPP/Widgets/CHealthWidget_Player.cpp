#include "CHealthWidget_Player.h"
#include "Global.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Characters/CPlayer.h"
#include "Components/CStatusComponent.h"

void UCHealthWidget_Player::NativeConstruct()
{
	CheckNull(CircleGuage);
	Material = CircleGuage->GetDynamicMaterial();
	Material->SetScalarParameterValue("Ratio", 1.f);

	ACPlayer* player = Cast<ACPlayer>(GetOwningPlayer()->GetPawn());
	CheckNull(player);

	UCStatusComponent* statusComp = CHelpers::GetComponent<UCStatusComponent>(player);
	CheckNull(statusComp);

	CheckNull(CurrentHealthText);
	FString currentHealthStr = FString::FromInt((int32)statusComp->GetMaxHealth());
	CurrentHealthText->SetText(FText::FromString(currentHealthStr));

	Super::NativeConstruct();
}

void UCHealthWidget_Player::Update_Implementation(float InCurrentHealth, float InMaxHealth)
{
	CheckNull(Material);
	Material->SetScalarParameterValue("Ratio", InCurrentHealth / InMaxHealth);

	CheckNull(CurrentHealthText);
	FString currentHealthStr = FString::FromInt((int32)InCurrentHealth);
	CurrentHealthText->SetText(FText::FromString(currentHealthStr));

	//PlayAnimation()
}