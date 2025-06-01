#include "Components/Combat/PawnCombatComponent.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"

#include "WarriorDebugHelper.h"

/// 생성된 무기를 태그와 함께 등록하고 선택적으로 장착 상태로 설정
void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AWarriorWeaponBase* InWeaponToRegister, bool bResgiterAsEquippedWeapon)
{
	// 중복 등록 방지 - 같은 태그의 무기가 이미 있으면 에러
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("A named named %s has already been added as carried weapon"), *InWeaponTagToRegister.ToString());

	// 무기 객체 유효성 검사
	check(InWeaponToRegister);

	// 무기를 맵에 추가
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	// 장착 옵션이 true면 현재 장착 무기로 설정
	if (bResgiterAsEquippedWeapon) CurrentEquippedWeaponTag = InWeaponTagToRegister;

	// 디버그용 등록 완료 메시지 출력
	const FString WeaponString = FString::Printf(TEXT("A Weapon named: %s has been registered using the tag %s"), *InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString());
	Debug::Print(WeaponString);
}

// 태그를 사용해 보유 중인 무기 객체를 검색하여 반환
AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	// 맵에 해당 태그가 존재하는지 확인
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		// 태그로 무기 포인터를 안전하게 검색 후 있으면 무기 객체 반환
		if (AWarriorWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}

	return nullptr;
}

// 현재 장착된 무기 객체를 반환
AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	// 현재 장착된 무기 태그가 유효하지 않으면 nullptr 반환
	if (!CurrentEquippedWeaponTag.IsValid()) return nullptr;

	// 장착 태그로 실제 무기 객체 검색 후 반환
	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollsion(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{

	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		AWarriorWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();
		
		check(WeaponToToggle);

		if (bShouldEnable)
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else 
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}	
	}

	//if (AWarriorWeaponBase* EquippedWeapon = GetCharacterCurrentEquippedWeapon())
	//{
	//	switch (ToggleDamageType)
	//	{
	//	case EToggleDamageType::CurrentEquippedWeapon:
	//		AWarriorWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();
	//		break;

	//	case EToggleDamageType::RightHand:
	//		EquippedWeapon->SetDamageEnabled(bShouldEnable);
	//		break;

	//	default:
	//		break;
	//	}
	//}
}
