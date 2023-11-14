// Developed by OIC

#pragma once

#include "CoreMinimal.h"
#include "Data/XeusEffect.h"
#include "UObject/Object.h"
#include "XeusPereodicEffect.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UXeusPereodicEffect : public UXeusEffect
{
	GENERATED_BODY()
public:
	UXeusPereodicEffect(const FObjectInitializer& ObjectInitializer);
protected:
	FTimerHandle TimerHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Rate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value;
public:
	UFUNCTION(BlueprintNativeEvent)
	void PeriodTick();
	
	virtual void Work_Implementation() override;
	virtual void EndWork_Implementation() override;
};
