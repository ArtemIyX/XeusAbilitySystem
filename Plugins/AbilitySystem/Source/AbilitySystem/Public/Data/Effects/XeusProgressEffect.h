// Developed by OIC

#pragma once

#include "CoreMinimal.h"
#include "Data/XeusEffect.h"

#include "XeusProgressEffect.generated.h"

class UXeusProgressEffect;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FXeusProgressEffectValueDelegate,
                                             UXeusProgressEffect*, Effect, float, Value);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FXeusProgressEffectBoolDelegate,
                                             UXeusProgressEffect*, Effect, float, Value);

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup=(XeusAbilitySystem))
class ABILITYSYSTEM_API UXeusProgressEffect : public UXeusEffect
{
	GENERATED_BODY()

public:
	UXeusProgressEffect(const FObjectInitializer& ObjectInitializer);

private:
	FTimerHandle ProgressTimerHandle;

protected:
	UPROPERTY(BlueprintReadOnly, meta=(ClampMin=0.0))
	float CurrentProgress;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1.0))
	float NeedProgress;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0.001))
	float ProgressRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0.001))
	float ProgressAmount;

	UPROPERTY(BlueprintReadOnly)
	bool bInProgress;

protected:
	UFUNCTION(BlueprintCallable)
	void StartTimer();
	UFUNCTION(BlueprintCallable)
	void PauseTimer();
	UFUNCTION(BlueprintCallable)
	void UnPauseTimer();
	UFUNCTION(BlueprintCallable)
	void TimerWork();

	virtual void Work_Implementation() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetCurrentProgress(float Value);
	UFUNCTION(BlueprintCallable)
	void SetNeedProgress(float Value);
	UFUNCTION(BlueprintCallable)
	void SetProgressRate(float Value);
	UFUNCTION(BlueprintCallable)
	void SetProgressAmount(float Value);
	UFUNCTION(BlueprintCallable)
	void SetIsInProgress(bool Value);

	UFUNCTION(BlueprintPure)
	float GetCurrentProgress() const;
	UFUNCTION(BlueprintPure)
	float GetNeedProgress() const;
	UFUNCTION(BlueprintPure)
	float GetProgressRate() const;
	UFUNCTION(BlueprintPure)
	float GetProgressAmount() const;
	UFUNCTION(BlueprintPure)
	bool GetIsInProgress() const;


	UPROPERTY(BlueprintAssignable)
	FXeusProgressEffectValueDelegate OnCurrentProgressChanged;
	UPROPERTY(BlueprintAssignable)
	FXeusProgressEffectValueDelegate OnNeedProgressChanged;
	UPROPERTY(BlueprintAssignable)
	FXeusProgressEffectValueDelegate OnProgressRateChanged;
	UPROPERTY(BlueprintAssignable)
	FXeusProgressEffectValueDelegate OnProgressAmountChanged;
	UPROPERTY(BlueprintAssignable)
	FXeusProgressEffectBoolDelegate OnInProgressChanged;
};
