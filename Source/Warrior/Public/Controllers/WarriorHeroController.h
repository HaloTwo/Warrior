// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "WarriorHeroController.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorHeroController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	AWarriorHeroController(const FObjectInitializer& ObjectInitializer);

	virtual FGenericTeamId GetGenericTeamId() const;

private:
	FGenericTeamId HeroTeamId;
};
