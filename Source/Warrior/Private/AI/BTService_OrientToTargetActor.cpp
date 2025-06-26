// Vince Petrelli All Rights Reserved


#include "AI/BTService_OrientToTargetActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_OrientToTargetActor::UBTService_OrientToTargetActor()
{
	NodeName = TEXT("Native Orient Rotation To Target Actor");

	// 서비스 노드의 플래그를 초기화합니다.
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	RotationInterpSpeed = 5.f; // 타겟 액터를 향해 회전하는 속도를 설정합니다.
	Interval = 0.f; // 타겟 액터를 업데이트하는 간격을 설정합니다.
	RandomDeviation = 0.f; // 타겟 액터를 업데이트하는 간격의 랜덤 편차를 설정합니다.

	// 이 서비스가 실행될 때마다 타겟 액터를 업데이트합니다.
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_OrientToTargetActor, InTargetActorKey), AActor::StaticClass());
}


// 에셋에서 이 서비스를 초기화합니다.
void UBTService_OrientToTargetActor::InitializeFromAsset(UBehaviorTree& Asset) 
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset()) // 블랙보드 에셋이 있는 경우
	{
		// InTargetActorKey의 키 이름을 설정합니다.
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
	}

}

// 이 서비스의 정적 설명을 반환합니다.
FString UBTService_OrientToTargetActor::GetStaticDescription() const 

{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Orient rotation to %s Key %s"), *KeyDescription, *GetStaticServiceDescription());
}

 

void UBTService_OrientToTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) // 이 서비스 노드가 매 프레임마다 호출됩니다.
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 타겟 액터를 블랙보드에서 가져옵니다.
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	// 타겟 액터가 유효한지 확인합니다.
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	// 타겟 액터가 유효하고, AI 컨트롤러가 소유한 폰이 있는 경우에만 회전을 수행합니다.
	if (OwningPawn && TargetActor)
	{
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation()); // 타겟 액터를 향한 회전값을 계산합니다.
		const FRotator TargetRot = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed); // 현재 회전값에서 타겟 회전값으로 보간합니다.

		OwningPawn->SetActorRotation(TargetRot);
	}
}