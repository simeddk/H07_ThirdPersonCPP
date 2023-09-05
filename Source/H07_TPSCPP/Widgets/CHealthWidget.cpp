#include "CHealthWidget.h"
#include "Components/ProgressBar.h"

void UCHealthWidget::Update_Implementation(float InCurrentHealth, float InMaxHealth)
{
	float ratio = InCurrentHealth / InMaxHealth;
	HealthBar->SetPercent(ratio);
}
