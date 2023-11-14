// Developed by OIC

#pragma once

#include "CoreMinimal.h"
#include "Data/XeusEffect.h"
#include "UObject/Object.h"
#include "XeusPereodicEffect.generated.h"

/**
 * A periodic effect that does work every N seconds
 * You need to delete it yourself
 */
UCLASS()
class ABILITYSYSTEM_API UXeusPereodicEffect : public UXeusEffect
{
	GENERATED_BODY()
public:
	UXeusPereodicEffect(const FObjectInitializer& ObjectInitializer);
protected:
	UPROPERTY()
	FTimerHandle TimerHandle;

	/**
	 * @brief Tick rate
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Rate;

	/**
	 * @brief Value that can be used in work (heal/damage amount etc..)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value;
public:
	/**
	 * @brief Work of effect.
	 * You should override this
	 */
	UFUNCTION(BlueprintNativeEvent)
	void PeriodTick();
	
	virtual void Work_Implementation() override;
	virtual void EndWork_Implementation() override;
};
