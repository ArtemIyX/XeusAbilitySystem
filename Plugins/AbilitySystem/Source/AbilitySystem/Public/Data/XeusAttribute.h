// Developed by OIC

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "XeusAttribute.generated.h"

class UXeusAttribute;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FXeusAttributeValueDelegate, UXeusAttribute*, Attribute, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXeusAttributeActionDelegate, UXeusAttribute*, Attribute);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FXeusAttributeMultDelegate, UXeusAttribute*, Attribute, FName, UniquedId);

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup=(XeusAbilitySystem))
class ABILITYSYSTEM_API UXeusAttribute : public UObject
{
	GENERATED_BODY()
public:
	UXeusAttribute(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	static UXeusAttribute* CreateAttributeFromClass(TSubclassOf<UXeusAttribute> InClass, UObject* Outer);
	
protected:
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly);
	float CurrentValue;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly);
	float MaxValue;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly);
	float MinValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DefaultValue;

	UPROPERTY(BlueprintReadOnly)
	TArray<FAttributeMultiplier> Mults;
protected:

public:
	
	
	TArray<const FAttributeMultiplier*> GetMultsByType(EAttributeMultiplierType InType) const;

	UFUNCTION(BlueprintCallable)
	TArray<FAttributeMultiplier> GetMults(EAttributeMultiplierType InType);

	UFUNCTION(BlueprintCallable)
	float GetMultValue(EAttributeMultiplierType InType) const;

	UFUNCTION(BlueprintCallable)
	bool AddMult(FAttributeMultiplier InMult);

	UFUNCTION(BlueprintCallable)
	bool RemoveMult(FName InId);

	UFUNCTION(BlueprintCallable)
	void GetMultById(FName InId, FAttributeMultiplier& OutMult, bool& bOutSuccess);

	const FAttributeMultiplier* GetMult(FName InId) const;
	
	UFUNCTION(BlueprintCallable)
	virtual void SetCurrentValue(float InValue, bool useMult = false);
	
	UFUNCTION(BlueprintCallable)
	virtual void AddCurrentValue(float InValue);
	
	UFUNCTION(BlueprintCallable)
	virtual void RemoveCurrentValue(float InValue);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetMaxValue(float InValue);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetMinValue(float InValue);
	
	UFUNCTION(BlueprintCallable)
	virtual void EditValue(EAttributeModifyType ModifyType, float Value);

	UFUNCTION(BlueprintPure)
	virtual float GetCurrentValue() const;
	UFUNCTION(BlueprintPure)
	virtual float GetMaxValue() const;
	UFUNCTION(BlueprintPure)
	virtual float GetMinValue() const { return MinValue; }
	UFUNCTION(BlueprintPure)
	virtual float GetPercent() const;
	UFUNCTION(BlueprintPure)
	virtual FString GetDebugName();

	UPROPERTY(BlueprintAssignable)
	FXeusAttributeValueDelegate OnValueChanged;
	UPROPERTY(BlueprintAssignable)
	FXeusAttributeValueDelegate OnMaxValueChanged;
	UPROPERTY(BlueprintAssignable)
	FXeusAttributeValueDelegate OnMinValueChanged;

	UPROPERTY(BlueprintAssignable)
	FXeusAttributeActionDelegate OnMinValue;
	UPROPERTY(BlueprintAssignable)
	FXeusAttributeActionDelegate OnMaxValue;

	UPROPERTY(BlueprintAssignable)
	FXeusAttributeMultDelegate OnMultAdded;
	UPROPERTY(BlueprintAssignable)
	FXeusAttributeActionDelegate OnMultRemoved;
	
};

