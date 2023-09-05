#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

UCLASS()
class H07_TPSCPP_API ACHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACHUD();

	void DrawHUD() override;

protected:
	void BeginPlay() override;

public:
	FORCEINLINE void See() { bVisible = true; }
	FORCEINLINE void NoSee() { bVisible = false; }
	
private:
	UPROPERTY(EditDefaultsOnly)
		class UTexture2D* CrossHairTexture;

	bool bVisible;

	class UCStateComponent* StateComp;
	UEnum* StateTypeEnum;
};
