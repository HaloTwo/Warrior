// Vince Petrelli All Rights Reserved


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorGameplayTags.h"

#include "WarriorDebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	//버랩된 액터가 이미 존재한다면 리턴
	if (OverlappedActors.Contains(HitActor)) return;

	//버랩된 액터에 추가
	OverlappedActors.AddUnique(HitActor);

	//디버그용으로 히트된 액터를 출력
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = false;
	const bool bIsMyAttackUnblockable = false;

	//플레이어가 블록 중이고 공격이 막을 수 없는 공격이 아니라면
	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		//TODO::check if the block is valid
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	//블록이 유효하다면
	if (bIsValidBlock)
	{
		//TODO::Handle successful block
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			WarriorGameplayTags::Shared_Event_MeleeHit,
			EventData
		);
	}
}