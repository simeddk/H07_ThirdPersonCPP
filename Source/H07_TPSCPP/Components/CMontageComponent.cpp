#include "CMontageComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMontageComponent::UCMontageComponent()
{

}


void UCMontageComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DataTable == nullptr)
	{
		CLog::Log("DataTable Asset is not set");
		return;
	}

	TArray<FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);

	for (int32 i = 0 ; i < (int32)EStateType::Max; i++)
	{
		for (FMontageData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;
				continue;
			}
		}
	}
	
}

void UCMontageComponent::PlayRoll()
{
	PlayAnimMontage(EStateType::Roll);
}

void UCMontageComponent::PlayBackStep()
{
	PlayAnimMontage(EStateType::BackStep);
}

void UCMontageComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontageComponent::PlayDead()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontageComponent::PlayAnimMontage(EStateType InStateType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());
	CheckNull(character);

	const FMontageData* data = Datas[(int32)InStateType];
	if (!!data)
	{
		if (!!data->AnimMontage)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRate, data->StartSection);
		}
	}
}
