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
 * A progressive effect that performs its work periodically and accumulates a charge.
 * When the maximum charge has accumulated, it asks for removal
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
	/**
	 * @brief Current value of progress. (Time to heal, time to loot etc...)
	 */
	UPROPERTY(BlueprintReadOnly, meta=(ClampMin=0.0))
	float CurrentProgress;

	/**
	 * @brief Progress value when effect will finish work
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1.0))
	float NeedProgress;

	/**
	 * @brief Tick rate
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0.001))
	float ProgressRate;

	/**
	 * @brief Amount of progress for 1 tick
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0.001))
	float ProgressAmount;

	/**
	 * @brief Is in progress (Effect can be paused)
	 */
	UPROPERTY(BlueprintReadOnly)
	bool bInProgress;

private:
	/**
	 * @brief Start tick rate (it will refresh timer)
	 */
	void StartTimer();

	/**
	 * @brief Pause ticks
	 */
	void PauseTimer();

	/**
	 * @brief Unpause ticks
	 */
	void UnPauseTimer();

protected:
	/**
	 * @brief Change progress of timer
	 * You should override this for custom logic
	 */
	UFUNCTION(BlueprintNativeEvent)
	void TimerWork();

	/**
	 * @brief Starts tick timer
	 */
	virtual void Work_Implementation() override;

public:
	/**
	 * @brief Change progress directly
	 * @param Value New progress value
	 */
	UFUNCTION(BlueprintCallable)
	void SetCurrentProgress(float Value);

	/**
	 * @brief Set progress target (max value)
	 * @param Value New progress target
	 */
	UFUNCTION(BlueprintCallable)
	void SetNeedProgress(float Value);

	/**
	 * @brief Set tick rate
	 * @param Value New tick rate
	 */
	UFUNCTION(BlueprintCallable)
	void SetProgressRate(float Value);

	/**
	 * @brief Set progress amount for 1 tick
	 * @param Value New amount
	 */
	UFUNCTION(BlueprintCallable)
	void SetProgressAmount(float Value);

	/**
	 * @brief Pause or unpause progress
	 * @param Value 
	 */
	UFUNCTION(BlueprintCallable)
	void SetIsInProgress(bool Value);

	/**
	 * @brief Get current progress
	 * @return Progress
	 */
	UFUNCTION(BlueprintPure)
	float GetCurrentProgress() const;

	/**
	 * @brief Get progress target
	 * @return Max progress value
	 */
	UFUNCTION(BlueprintPure)
	float GetNeedProgress() const;

	/**
	 * @brief Get tick rate
	 * @return Tick rate
	 */
	UFUNCTION(BlueprintPure)
	float GetProgressRate() const;
	
	/**
	 * @brief Get work amount for 1 tick
	 * @return Progress amount
	 */
	UFUNCTION(BlueprintPure)
	float GetProgressAmount() const;

	/**
	 * @brief Check if effect is not paused
	 * @return True if timer is ticking, false otherwise
	 */
	UFUNCTION(BlueprintPure)
	bool GetIsInProgress() const;

	/**
	 * @brief Get ratio of current progress and max progress
	 * @return Percent of work completed
	 */
	UFUNCTION(BlueprintPure)
	float GetPercent() const;


	/**
	 * @brief Called when progress changed
	 */
	UPROPERTY(BlueprintAssignable)
	FXeusProgressEffectValueDelegate OnCurrentProgressChanged;

	/**
	 * @brief Called when target changed
	 */
	UPROPERTY(BlueprintAssignable)
	FXeusProgressEffectValueDelegate OnNeedProgressChanged;

	/**
	 * @brief Called when tick rate changed
	 */
	UPROPERTY(BlueprintAssignable)
	FXeusProgressEffectValueDelegate OnProgressRateChanged;

	/**
	 * @brief Called when progress amount for 1 tick changed
	 */
	UPROPERTY(BlueprintAssignable)
	FXeusProgressEffectValueDelegate OnProgressAmountChanged;

	/**
	 * @brief Called when timer paused or unpaused
	 */
	UPROPERTY(BlueprintAssignable)
	FXeusProgressEffectBoolDelegate OnInProgressChanged;
};
