// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/WarriorBaseCharacter.h"
#include "WarriorEnemyCharacter.generated.h"

class UEnemyCombatComponent;
class UEnemyUIComponent;
/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorEnemyCharacter : public AWarriorBaseCharacter
{
	GENERATED_BODY()
	
public:
	AWarriorEnemyCharacter();

	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	//~ End IPawnUIInterface Interface

	// ~ Begin PawnCombatInterface InterFace
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	// ~ End PawnCombatInterface InterFace

protected:

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UEnemyUIComponent* EnemyUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* EnemyCombatComponent;

private:
	void InitEnemyStartUpData();


public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
};
