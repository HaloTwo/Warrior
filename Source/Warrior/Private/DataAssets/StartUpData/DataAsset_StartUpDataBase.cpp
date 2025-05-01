// Vince Petrelli All Rights Reserved


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilityStstemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	// AbilitySystemComponent가 유효한지 확인
	check(InASCToGive);

	// 시작 시 즉시 발동되는 능력 부여
	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);

	// 반응형 능력 (ex. 피해 입었을 때 자동 발동 등) 부여
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);
}



void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilitiesToGive, UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	// 부여할 능력 리스트가 비어있는 경우
	if (InAbilitiesToGive.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No abilities to grant."));
		return;
	}

	// 각 능력에 대해 처리
	for (const TSubclassOf<UWarriorGameplayAbility>& Ability : InAbilitiesToGive)
	{
		// 능력 클래스가 유효하지 않으면 건너뜀
		if (!Ability)
		{
			UE_LOG(LogTemp, Error, TEXT("Found null ability class. Skipping."));
			continue;
		}

		// 능력 스펙 생성 (레벨 및 소스 설정)
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		// 능력 부여
		InASCToGive->GiveAbility(AbilitySpec);
		UE_LOG(LogTemp, Log, TEXT("Granted ability: %s"), *Ability->GetName());
	}
}


