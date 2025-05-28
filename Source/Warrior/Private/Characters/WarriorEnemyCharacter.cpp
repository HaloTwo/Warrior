// Vince Petrelli All Rights Reserved


#include "Characters/WarriorEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "WarriorDebugHelper.h"

AWarriorEnemyCharacter::AWarriorEnemyCharacter()
{
	// AI 컨트롤러 자동 빙의 설정 - 월드에 배치되거나 스폰될 때 자동으로 AI가 조종
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 컨트롤러(AI)의 회전값을 캐릭터에 직접 적용하지 않음 - AI가 바라보는 방향과 캐릭터 방향을 분리
	bUseControllerRotationPitch = false;  // 상하 회전(고개 끄덕임) 사용 안함
	bUseControllerRotationRoll = false;   // 좌우 기울임 사용 안함  
	bUseControllerRotationYaw = false;    // 좌우 회전(고개 돌리기) 사용 안함

	// 캐릭터 무브먼트 컴포넌트 설정
	GetCharacterMovement()->bUseControllerDesiredRotation = false;  // 컨트롤러가 원하는 회전 방향 사용 안함
	GetCharacterMovement()->bOrientRotationToMovement = true;       // 이동 방향으로 캐릭터가 자동 회전 (자연스러운 이동)
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);  // 회전 속도 설정 (초당 180도로 좌우 회전)
	GetCharacterMovement()->MaxWalkSpeed = 300.f;                   // 최대 걷기 속도 300 유닛/초
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;    // 걸을 때 제동력 1000 (빠르게 멈춤)

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));
}


void AWarriorEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void AWarriorEnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull()) return;


	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this]()
			{
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					LoadedData->GiveToAbilityStstemComponent(WarriorAbilitySystemComponent);

					Debug::Print(TEXT("Enemy Start Up Data Loaded 맞아 ?"), FColor::Green);
				}
			}
		)
	);
}
