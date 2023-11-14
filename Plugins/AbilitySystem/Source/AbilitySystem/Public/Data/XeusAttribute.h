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
 * Abstract class for all gameplay attributes
 * You should create children of this class
 */
UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup=(XeusAbilitySystem))
class ABILITYSYSTEM_API UXeusAttribute : public UObject
{
	GENERATED_BODY()
public:
	UXeusAttribute(const FObjectInitializer& ObjectInitializer);

	/**
	 * @brief Creates attribute of this class
	 * @param InClass Attribute class
	 * @param Outer Attribute owner (character, gun etc...)
	 * @return New attribute instance
	 */
	UFUNCTION(BlueprintCallable)
	static UXeusAttribute* CreateAttributeFromClass(TSubclassOf<UXeusAttribute> InClass, UObject* Outer);
	
protected:

	/**
	 * @brief Current float value of attribute
	 * Do not change it directly
	 * @see EditValue
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly);
	float CurrentValue;

	/**
	 * @brief Max value of CurrentValue
	 * @see CurrentValue
	 * @see EditValue
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly);
	float MaxValue;

	/**
	 * @brief Min value of CurrentValue
	 * @see CurrentValue
	 * @see EditValue
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly);
	float MinValue;

	/**
	 * @brief Default value for effect
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DefaultValue;

	/**
	 * @deprecated 
	 * @brief Container of multipliers
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<FAttributeMultiplier> Mults;
protected:

public:
	/**
	 * @deprecated 
	 * @brief Get all multipliers by type
	 * @param InType Multiplier type
	 * @return Pointer to mult structs
	 */
	TArray<const FAttributeMultiplier*> GetMultsByType(EAttributeMultiplierType InType) const;

	/**
	 * @deprecated 
	 * @brief Get all multipliers by type
	 * @param InType Multiplier type
	 * @return Copy of multipliers
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FAttributeMultiplier> GetMults(EAttributeMultiplierType InType);

	/**
	 * @deprecated 
	 * @brief Get product value by type
	 * @param InType Multiplier type
	 * @return Product
	 */
	UFUNCTION(BlueprintCallable)
	float GetMultValue(EAttributeMultiplierType InType) const;

	/**
	 * @deprecated 
	 * @brief Add new multiplier for this effect
	 * @param InMult Mult data
	 * @return True if added with no conflicts
	 */
	UFUNCTION(BlueprintCallable)
	bool AddMult(FAttributeMultiplier InMult);

	/**
	 * @deprecated 
	 * @brief Remove existing multiplier from this effect
	 * @param InId Multiplier unique id
	 * @return True if removed
	 */
	UFUNCTION(BlueprintCallable)
	bool RemoveMult(FName InId);

	/**
	 * @deprecated 
	 * @brief Get multiplier by custom id
	 * @param InId Custom unique id
	 * @param OutMult Mult data (copy)
	 * @param bOutSuccess True if found effect
	 */
	UFUNCTION(BlueprintCallable)
	void GetMultById(FName InId, FAttributeMultiplier& OutMult, bool& bOutSuccess);

	/**
	 * @deprecated 
	 * @brief Get multiplier by id
	 * @param InId Custom unique id
	 * @return Pointer to multiplier struct if found, nullptr otherwise
	 */
	const FAttributeMultiplier* GetMult(FName InId) const;

	/**
	 * @deprecated 
	 * @brief Change directly attribute value
	 * @param InValue New value to set
	 * @param useMult Should use multiplier
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetCurrentValue(float InValue, bool useMult = false);

	/**
	 * @brief Add value to attribute value
	 * @param InValue Amount
	 */
	UFUNCTION(BlueprintCallable)
	virtual void AddCurrentValue(float InValue);

	/**
	 * @brief Subtract value from attribute value
	 * @param InValue Amount
	 */
	UFUNCTION(BlueprintCallable)
	virtual void RemoveCurrentValue(float InValue);

	/**
	 * @brief Change max value of attribute
	 * @param InValue New max value
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetMaxValue(float InValue);

	/**
	 * @brief Change min value of attribute
	 * @param InValue New min value 
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetMinValue(float InValue);

	/**
	 * @brief Change value of attribute
	 * @param ModifyType Action type (set, add, remove)
	 * @param Value Amount
	 */
	UFUNCTION(BlueprintCallable)
	virtual void EditValue(EAttributeModifyType ModifyType, float Value);

	/**
	 * @brief Get current value of attribute
	 * @return Current value
	 */
	UFUNCTION(BlueprintPure)
	virtual float GetCurrentValue() const;

	/**
	 * @brief Get maximum value of attribute
	 * @return Max value
	 */
	UFUNCTION(BlueprintPure)
	virtual float GetMaxValue() const;

	/**
	 * @brief Get minimum value of attribute
	 * @return Min value
	 */
	UFUNCTION(BlueprintPure)
	virtual float GetMinValue() const;

	/**
	 * @brief Get ratio of current value and max value
	 * @return Ratio of current value (0.0 - 1.0)
	 */
	UFUNCTION(BlueprintPure)
	virtual float GetPercent() const;

	/**
	 * @deprecated 
	 * @brief Get debug name for developers
	 * @return Debug name
	 */
	UFUNCTION(BlueprintPure)
	virtual FString GetDebugName();

	/**
	 * @brief Called when current value changed
	 */
	UPROPERTY(BlueprintAssignable)
	FXeusAttributeValueDelegate OnValueChanged;

	/**
	 * @brief Called when max value changed
	 */
	UPROPERTY(BlueprintAssignable)
	FXeusAttributeValueDelegate OnMaxValueChanged;

	/**
	 * @brief Called when min value changed
	 */
	UPROPERTY(BlueprintAssignable)
	FXeusAttributeValueDelegate OnMinValueChanged;

	/**
	 * @brief Called when current value was clamped to min value
	 */
	UPROPERTY(BlueprintAssignable)
	FXeusAttributeActionDelegate OnMinValue;

	/**
	 * @brief Called when current value was clamped to max value
	 */
	UPROPERTY(BlueprintAssignable)
	FXeusAttributeActionDelegate OnMaxValue;

	/**
	 * @deprecated 
	 * @brief Called when new multiplier (unique id is passed) was added
	 */
	UPROPERTY(BlueprintAssignable)
	FXeusAttributeMultDelegate OnMultAdded;

	/**
	 * @deprecated 
	 * @brief Called when some multiplier was removed
	 */
	UPROPERTY(BlueprintAssignable)
	FXeusAttributeActionDelegate OnMultRemoved;
	
};

