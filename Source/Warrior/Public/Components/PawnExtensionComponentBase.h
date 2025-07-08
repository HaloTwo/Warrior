// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIOR_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	T* GetOwningController() const
	// T가 APawn에서 파생된 클래스인지 컴파일 시간에 확인함
	// GetOwner()로 얻은 액터를 지정된 Pawn 타입으로 캐스팅하여 반환
	template<class T>
	T* GetOwningPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' Template Parameter to GetPawn must be derived from APawn");
		return CastChecked<T>(GetOwner());
	}

	// 기본 APawn 타입으로 소유 Pawn을 가져오는 비템플릿 버전
	// 템플릿 버전을 APawn 타입으로 호출하여 간편하게 사용할 수 있게 함
	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	// 소유 Pawn의 컨트롤러를 특정 Controller 타입으로 가져오는 템플릿 함수
	// T가 AController에서 파생된 클래스인지 컴파일 시간에 확인함
	// 먼저 Pawn을 얻은 다음 해당 Pawn의 컨트롤러를 지정된 타입으로 가져옴
	template<class T>
	T* GetowningController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' Template Parameter to GetController must be derived from AController");
		return GetOwningPawn<APawn>()->GetController<T>();
	}
};
