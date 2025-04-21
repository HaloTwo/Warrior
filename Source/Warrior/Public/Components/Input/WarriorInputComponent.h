// Vince Petrelli All Rights Reserved
#pragma once
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_inputConfig.h"
#include "WarriorInputComponent.generated.h"

/**
 * WarriorInputComponent 클래스
 *
 * 언리얼 엔진의 향상된 입력 시스템(Enhanced Input System)을 위한 커스텀 InputComponent입니다.
 * 이 클래스는 게임플레이 태그를 사용하여 입력 액션을 바인딩할 수 있는 기능을 제공합니다.
 * UEnhancedInputComponent를 상속받아 추가 기능을 구현합니다.
 */
UCLASS()
class WARRIOR_API UWarriorInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY() // 언리얼 엔진 리플렉션 시스템을 위한 매크로

public:
	/**
	 * 게임플레이 태그를 통해 네이티브 입력 액션을 바인딩하는 템플릿 함수
	 *
	 * @param InInputConfig - 입력 설정이 포함된 데이터 애셋
	 * @param InInputTag - 바인딩할 입력 액션을 식별하는 게임플레이 태그
	 * @param TriggerEvent - 어떤 트리거 이벤트에 반응할지 지정 (예: 눌렀을 때, 떼었을 때 등)
	 * @param ContextObject - 콜백 함수가 속한 객체 (보통 this)
	 * @param Func - 입력이 발생했을 때 호출될 콜백 함수
	 *
	 * 템플릿 함수로 구현되어 다양한 타입의 객체와 콜백 함수를 받을 수 있습니다.
	 */
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_inputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);
};

/**
 * BindNativeInputAction 템플릿 함수의 구현부
 *
 * 전달받은 게임플레이 태그에 해당하는 입력 액션을 찾아 바인딩합니다.
 */
template<class UserObject, typename CallbackFunc>
inline void UWarriorInputComponent::BindNativeInputAction(const UDataAsset_inputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	// 입력 설정 애셋이 유효한지 확인하고, 그렇지 않으면 오류 메시지와 함께 실행 중단
	checkf(InInputConfig, TEXT("Input config data asset is null, can not proceed with binding"));

	// 게임플레이 태그를 사용하여 입력 액션 객체를 찾음
	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		// 찾은 입력 액션을 지정된 트리거 이벤트, 컨텍스트 객체, 콜백 함수와 함께 바인딩
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}