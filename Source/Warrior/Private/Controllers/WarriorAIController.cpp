// Vince Petrelli All Rights Reserved
#include "Controllers/WarriorAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	SetGenericTeamId(FGenericTeamId(1)); // 팀 ID 설정 (예: 1번 팀)
}

/**
* 다른 액터에 대한 팀 태도를 결정하는 함수
* AI 인식 시스템에서 적/아군을 구분하는 데 사용됨
*/
ETeamAttitude::Type AWarriorAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
    // 대상 액터를 Pawn으로 캐스팅 (읽기 전용)
    const APawn* PawnToCheck = Cast<const APawn>(&Other);

    // 대상 Pawn의 컨트롤러에서 팀 에이전트 인터페이스를 가져옴
    // GenericTeamAgentInterface는 팀 ID를 관리하는 인터페이스
    const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(PawnToCheck->GetController());

    // 상대방이 팀 시스템을 사용하고 있고, 우리와 다른 팀이라면
    if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
    {
        // 적대적 관계로 판정
        return ETeamAttitude::Hostile;
    }

    // 같은 팀이거나 팀이 없다면 우호적 관계로 판정
    return ETeamAttitude::Friendly;
}


/**
* AI 인식 시스템에서 적을 감지하거나 놓쳤을 때 호출되는 콜백 함수
* OnTargetPerceptionUpdated 델리게이트에 바인딩되어 자동으로 실행됨
*/
void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // 감지가 성공적으로 이루어졌고 유효한 액터인 경우
    if (Stimulus.WasSuccessfullySensed() && Actor)
    {
        if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
        {
            BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
        }
    }
    // 참고: 감지를 잃었을 경우 (!Stimulus.WasSuccessfullySensed())의 처리는 없음
}
