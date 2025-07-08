// Vince Petrelli All Rights Reserved


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	// AbilitySystemComponent가 유효한지 확인
	check(InASCToGive);

	// 시작 시 즉시 발동되는 능력 부여
	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);

	// 반응형 능력 (ex. 피해 입었을 때 자동 발동 등) 부여
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);

	// 시작 시 적용할 게임플레이 이펙트들 (버프/초기 스탯 설정 등) 처리
	if (!StartUpGameplayEffects.IsEmpty())
	{
		// 각 이펙트를 순회하며 적용
		for (const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
		{
			// 유효하지 않은 이펙트 클래스는 스킵
			if (!EffectClass) continue;

			// 이펙트의 기본 객체 가져오기
			UGameplayEffect* EffectCD0 = EffectClass->GetDefaultObject<UGameplayEffect>();

			// ASC에 이펙트 적용 (레벨과 컨텍스트 포함)
			InASCToGive->ApplyGameplayEffectToSelf(
				EffectCD0,
				ApplyLevel,
				InASCToGive->MakeEffectContext()
			);
		}
	}
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


