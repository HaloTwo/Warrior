// Vince Petrelli All Rights Reserved
#include "Controllers/WarriorAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "WarriorDebugHelper.h"

AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
    // CrowdFollowingComponent가 제대로 설정되었는지 확인 (디버그용)
    if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
    {
        Debug::Print(TEXT("CrowdFollowingComponent valid"), FColor::Green);
    }

    // AI 시각 감지 설정 컴포넌트 생성
    AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("EnemySenseConfig_Sight"));

    // 시각 감지 대상 설정 (적만 감지하도록 설정)
    AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;    // 적 감지 활성화
    AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false; // 아군 감지 비활성화
    AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;   // 중립 감지 비활성화

    // 시각 감지 범위 및 각도 설정
    AISenseConfig_Sight->SightRadius = 5000.0f;                    // 시야 반경 (5000 유닛)
    AISenseConfig_Sight->LoseSightRadius = 0.0f;                   // 시야 잃는 반경 (0이면 SightRadius와 동일)
    AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.0f;    // 시야각 (360도 = 전방향)

    // AI 인식 컴포넌트 생성 및 설정
    EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
    EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);                           // 시각 감지 설정 적용
    EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());         // 주요 감지 방식을 시각으로 설정

    // 적 감지/감지 해제 시 호출될 델리게이트 바인딩
    EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);
}

void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{


}
