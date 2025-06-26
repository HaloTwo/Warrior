// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "WarriorAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability", meta = (ApplyLevel = "1"))
	void GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
    // InSpecHandlesToRemove에 포함된 능력 세트에 해당하는 능력을 제거합니다.
    void RemoveGrantederoWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	// 능력을 활성화하려고 시도합니다. 성공하면 true를 반환합니다.
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
};
