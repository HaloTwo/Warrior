// Vince Petrelli All Rights Reserved


#include "Controllers/WarriorHeroController.h"

AWarriorHeroController::AWarriorHeroController(const FObjectInitializer& ObjectInitializer)
{
	HeroTeamId = FGenericTeamId(0);
}

FGenericTeamId AWarriorHeroController::GetGenericTeamId() const
{
	return HeroTeamId;
}
