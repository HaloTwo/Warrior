#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/WarriorAttributeSet.h"
#include "WarriorGameplayTags.h"

// 데미지 계산에 필요한 어트리뷰트들을 캡처하는 구조체
struct FWarriorDamageCapture
{
    // 공격력 어트리뷰트 캡처 정의 선언
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
    // 방어력 어트리뷰트 캡처 정의 선언
    DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)

    // 생성자에서 캡처 정의들을 초기화
    FWarriorDamageCapture()
    {
        // 공격력: Source(공격자)에서 캡처, 스냅샷 안 함
        DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false)
            // 방어력: Target(피격자)에서 캡처, 스냅샷 안 함
            DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefensePower, Target, false)
    }
};

// 싱글톤 패턴으로 데미지 캡처 구조체 인스턴스 반환
static const FWarriorDamageCapture& GetWarriorDamageCapture()
{
    // static으로 한 번만 생성되는 인스턴스
    static FWarriorDamageCapture WarriorDamageCapture;
    return WarriorDamageCapture;
}

// 데미지 계산 실행 클래스 생성자
UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
    /*어트리뷰트 캡처를 수동으로 하는 느린 방법 (사용 안 함)*/
    //FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
    //	UWarriorAttributeSet::StaticClass(),
    //	GET_MEMBER_NAME_CHECKED(UWarriorAttributeSet,AttackPower)
    //);
    //FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
    //	AttackPowerProperty,
    //	EGameplayEffectAttributeCaptureSource::Source,
    //	false
    //);
    //RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);

    // 매크로를 사용한 빠른 방법으로 공격력 캡처 정의 추가
    RelevantAttributesToCapture.Add(GetWarriorDamageCapture().AttackPowerDef);
    // 매크로를 사용한 빠른 방법으로 방어력 캡처 정의 추가
    RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DefensePowerDef);
}