// Vince Petrelli All Rights Reserved


#include "AI/BTTask_RotateToFaceTarget.h"

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget() 
{
	NodeName = TEXT("Native Rotate to Face Target Actor");
	AnglePrecision = 10.f;
	RotationInterpSpeed = 5.f;
	
	bNotifyTick = true; // 노드가 Tick 이벤트를 받도록 설정
	bNotifyTaskFinished = true; // 노드가 완료되었을 때 알림을 받도록 설정
	bCreateNodeInstance = false; // 노드 인스턴스를 생성하지 않도록 설정

	// 블랙보드 키를 설정합니다
	INIT_TASK_NODE_NOTIFY_FLAGS();

	// InTargetToFaceKey를 초기화합니다
	InTargetToFaceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetToFaceKey), AActor::StaticClass());
}

void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	// 블랙보드 자산을 가져옵니다
	if (UBlackboardData* BBAsset = GetBlackboardAsset()) 
	{
		InTargetToFaceKey.ResolveSelectedKey(*BBAsset); // 타겟을 바라보는 키를 선택합니다
	}
}

uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
	return sizeof(FRotateToFaceTargetTaskMemory); // 인스턴스 메모리 크기를 반환합니다
}

FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{
	const FString KeyDescription = InTargetToFaceKey.SelectedKeyName.ToString();

	// 정밀도와 키 이름을 포함한 설명 문자열을 반환합니다
	return FString::Printf(TEXT("Smoothly rotates to face %s Key until the angle precision: %s is reached"), *KeyDescription, *FString::SanitizeFloat(AnglePrecision));
}