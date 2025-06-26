// Vince Petrelli All Rights Reserved


#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return; // 인풋 태그가 유효하지 않으면 아무 작업도 수행하지 않음

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) // 모든 활성화 가능한 능력에 대해 반복
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) // 현재 능력의 동적 능력 태그가 입력 태그와 정확히 일치하는지 확인
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}


void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
}

void UWarriorAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty()) return;

	for (const FWarriorHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.isValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UWarriorAbilitySystemComponent::RemoveGrantederoWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	InSpecHandlesToRemove.Empty();
}


bool UWarriorAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());

	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

	// FoundAbilitySpecs는 AbilityTagToActivate와 일치하는 모든 능력 사양을 포함합니다.
	if (!FoundAbilitySpecs.IsEmpty())
	{
		// 무작위로 능력 사양을 선택하여 활성화 시도
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		check(SpecToActivate);

		// 선택된 능력 사양이 활성화되지 않은 경우 활성화 시도
		if (!SpecToActivate->IsActive())
		{
			return TryActivateAbility(SpecToActivate->Handle);
		}
	}

	return false;
}
