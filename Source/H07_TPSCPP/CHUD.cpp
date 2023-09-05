#include "CHUD.h"
#include "Global.h"
#include "Engine/Texture2D.h"
#include "Engine/Canvas.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

ACHUD::ACHUD()
{
	CHelpers::GetAsset(&CrossHairTexture, "/Game/Player/FirstPersonCrosshair");
}


void ACHUD::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	StateComp = CHelpers::GetComponent<UCStateComponent>(player);
	CheckNull(StateComp);

	StateTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStateType"), true);
}

void ACHUD::DrawHUD()
{
	Super::DrawHUD();
	
	//Visible Player StateType
	FString typeStr = "State : ";
	if (!!StateTypeEnum)
	{
		FName typeName = StateTypeEnum->GetNameByValue((int64)StateComp->GetType());
		typeStr.Append(typeName.ToString());
	}
	DrawText(typeStr, FLinearColor::Red, 10, Canvas->ClipY - 50, nullptr, 1.5f);

	//Visible Aim(R-Button)
	CheckFalse(bVisible);
	const FVector2D center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	const FVector2D imageHalfSize(CrossHairTexture->GetSurfaceWidth() * 0.5, CrossHairTexture->GetSurfaceHeight() * 0.5f);
	const FVector2D imagePosition(center.X - imageHalfSize.X, center.Y - imageHalfSize.Y);

	FCanvasTileItem imageItem(imagePosition, CrossHairTexture->Resource, FLinearColor::White);
	imageItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(imageItem);

	
}

