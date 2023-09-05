#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_BackStepped.generated.h"

UCLASS()
class H07_TPSCPP_API UCAnimNotify_BackStepped : public UAnimNotify
{
	GENERATED_BODY()

public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
