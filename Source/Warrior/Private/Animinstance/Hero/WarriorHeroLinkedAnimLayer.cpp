// Vince Petrelli All Rights Reserved


#include "Animinstance/Hero/WarriorHeroLinkedAnimLayer.h"
#include "Animinstance/Hero/WarriorHeroAnimInstance.h"

UWarriorHeroAnimInstance* UWarriorHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
	return Cast<UWarriorHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
