// Vince Petrelli All Rights Reserved
#pragma once
#include "CoreMinimal.h"
#include "Animinstance/WarriorBaseAnimInstance.h"
#include "WarriorCharacterAnimInstance.generated.h"

// 전방 선언: AWarriorBaseCharacter 클래스를 참조하지만 전체 헤더는 포함하지 않음
class AWarriorBaseCharacter;
class UCharacterMovementComponent;

/**
 * WarriorCharacterAnimInstance 클래스
 *
 * 전사 캐릭터의 애니메이션을 제어하는 애니메이션 인스턴스 클래스입니다.
 * UWarriorBaseAnimInstance를 상속받아 구현되며, 캐릭터의 상태에 따라
 * 적절한 애니메이션을 재생하는 역할을 담당합니다.
 */
UCLASS()
class WARRIOR_API UWarriorCharacterAnimInstance : public UWarriorBaseAnimInstance
{
	GENERATED_BODY() // 언리얼 엔진 리플렉션 시스템을 위한 매크로

public:

	 //애니메이션 인스턴스가 생성될 때 호출되며, 필요한 초기 설정을 수행합니다.
	 //UAnimInstance 클래스의 동일한 함수를 오버라이드합니다.
	virtual void NativeInitializeAnimation() override;

	 //매 프레임마다 호출되어 애니메이션 상태를 업데이트합니다.
	 //이 함수는 별도의 스레드에서 안전하게 실행될 수 있도록 설계되어야 합니다.
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

protected:

	UPROPERTY()
	AWarriorBaseCharacter* OwningCharacter;

	//캐릭터의 움직임을 제어
	UPROPERTY()
	UCharacterMovementComponent* owningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocaomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocaomotionData")
	bool bHasAcceleration;
};