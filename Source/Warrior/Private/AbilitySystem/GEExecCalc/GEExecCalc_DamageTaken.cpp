#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/WarriorAttributeSet.h"
#include "WarriorGameplayTags.h"

#include "WarriorDebugHelper.h"

// 데미지 계산에 필요한 어트리뷰트들을 캡처하는 구조체
struct FWarriorDamageCapture
{
	// 공격력 어트리뷰트 캡처 정의 선언
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	// 방어력 어트리뷰트 캡처 정의 선언
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)

	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)


	// 생성자에서 캡처 정의들을 초기화
	FWarriorDamageCapture()
	{
		// 공격력: Source(공격자)에서 캡처, 스냅샷 안 함
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false)
		// 방어력: Target(피격자)에서 캡처, 스냅샷 안 함
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefensePower, Target, false)

		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DamageTaken, Target, false)
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

	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DamageTakenDef);
}

// GAS 데미지 계산의 핵심 실행 함수
void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 현재 적용되는 GameplayEffect의 정보를 가져옴
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	/*컨텍스트에서 가져올 수 있는 추가 정보들 (현재 미사용)
	EffectSpec.GetContext().GetSourceObject();    // 효과를 발생시킨 오브젝트
	EffectSpec.GetContext().GetAbility();         // 효과를 발생시킨 어빌리티
	EffectSpec.GetContext().GetInstigator();      // 효과를 시작한 액터
	EffectSpec.GetContext().GetEffectCauser();*/  // 효과를 직접 일으킨 액터

	// 어트리뷰트 평가를 위한 파라미터 설정
	FAggregatorEvaluateParameters EvaluateParameters;
	// Source(공격자)의 태그들을 평가 파라미터에 설정
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	// Target(피격자)의 태그들을 평가 파라미터에 설정
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	// 공격자의 공격력을 캡처해서 가져오기
	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	//Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);

	// SetByCaller로 전달받을 데미지 관련 값들 초기화
	float BaseDamage = 0.f;                    // 기본 데미지
	int32 UsedLightAttckComboCount = 0;        // 라이트 공격 콤보 카운트
	int32 UsedHeavyAttackComboCount = 0;       // 헤비 공격 콤보 카운트

	// SetByCaller로 전달된 태그-값 쌍들을 순회하며 데이터 추출
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		// 기본 데미지 태그인지 확인하고 값 저장
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
	/*		Debug::Print(TEXT("BaseDamage"), BaseDamage);*/
		}
		// 라이트 공격 타입 태그인지 확인하고 콤보 카운트 저장
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttckComboCount = TagMagnitude.Value;
			/*Debug::Print(TEXT("UsedLightAttckComboCount"), UsedLightAttckComboCount);*/
		}
		// 헤비 공격 타입 태그인지 확인하고 콤보 카운트 저장
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
			/*Debug::Print(TEXT("UsedHeavyAttackComboCount"), UsedHeavyAttackComboCount);*/
		}
	}

	// 피격자의 방어력을 캡처해서 가져오기
	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);

	//방어력
	//Debug::Print(TEXT("TargetDefensePower"), TargetDefensePower);

	if (UsedLightAttckComboCount != 0)
	{
		const float DamageIncreasePercentLight = (UsedLightAttckComboCount - 1) * 0.05 + 1.f;

		BaseDamage *= DamageIncreasePercentLight;
	/*	Debug::Print(TEXT("ScaledBaseDamageLight"), BaseDamage);*/
	}

	if (UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = UsedHeavyAttackComboCount * 0.15f + 1.f;

		BaseDamage *= DamageIncreasePercentHeavy;
		//Debug::Print(TEXT("ScaledBaseDamageHeavy"), BaseDamage);
	}

	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	//Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone);

	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetWarriorDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}