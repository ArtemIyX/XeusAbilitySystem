// Developed by OIC

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AbilitySystemTypes.h"

#include "XeusEffect.generated.h"

class UXeusAbilitySystemComponent;
class UXeusEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXeusEffectActionDelegate, UXeusEffect*, Effect);

/**
 * Abstract class for all type of gameplay effects
 * You should override Work() and EndWork() methods
 */
UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup=(XeusAbilitySystem))
class ABILITYSYSTEM_API UXeusEffect : public UObject
{
	GENERATED_BODY()
public:
	UXeusEffect(const FObjectInitializer& ObjectInitializer);

	/**
	 * @brief Creates effects of class
	 * @param InClass Class effect
	 * @param Outer Effect owner (character, gun etc..)
	 * @return New effect
	 */
	UFUNCTION(BlueprintCallable)
	static UXeusEffect* CreateEffect(TSubclassOf<UXeusEffect> InClass, UObject* Outer);
protected:
	/**
	 * @brief Saved ability system component pointer
	 * Set when effect starts work
	 * @see NotifyBeginWork
	 */
	UPROPERTY(BlueprintReadOnly, Transient)
	UXeusAbilitySystemComponent* AbilitySystem;

	/**
	 * @brief Should be displayed in HUD or not
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDisplayable;

	/**
	 * @brief Icon for HUD display
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bDisplayable"))
	TSoftObjectPtr<UTexture2D> Icon;
	/**
	 * @brief Color for HUD display
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bDisplayable"))
	FLinearColor EffectColor;

	/**
	 * @brief Should be stackable or not
	 * If an effect stacks, then when adding identical effects a new one will not be created,
	 * and the old one will receive a stacking notification
	 * @see Stack
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bStackable;

	/**
	 * @deprecated 
	 * @brief All modifiers of effect
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<FXeusEffectModifier> Modifiers;

protected:
	/**
	 * @brief Called when effect need to do its work
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Work();
	
	/**
	 * @brief Called when effect needs to be removed
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndWork();


	/**
	 * @deprecated 
	 * @brief Find modifier by unique ID
	 * @param UniqueId Id of modifier
	 * @return Index in Array of Modifiers
	 * @see Modifiers
	 */
	UFUNCTION(BlueprintCallable)
	int32 FindModifier(FName UniqueId) const;
public:
	/**
	 * @deprecated 
	 * @brief Dynamic initialization of effect
	 * You should pass pointer to child of FXeusEffectSettings
	 * @param Settings Pointer to settings struct
	 */
	virtual void Setup(FXeusEffectSettings* Settings);

	/**
	 * @deprecated 
	 * @brief Multiplies all multipliers into one
	 * @return Product of multipliers
	 */
	UFUNCTION(BlueprintPure)
	float GetTotalModifier() const;

	/**
	 * @deprecated 
	 * @brief Check if modifier with id exists
	 * @param UniqueId Modifier id 
	 * @return True if multiplier exists
	 */
	UFUNCTION(BlueprintPure)
	bool HasModifier(FName UniqueId) const;

	/**
	 * @deprecated 
	 * @brief Add new modifier to effect
	 * @param Mod Modifier settings
	 */
	UFUNCTION(BlueprintCallable)
	void ApplyModifier(FXeusEffectModifier Mod);

	/**
	 * @deprecated 
	 * @brief Remove modifier by unique ID
	 * @param UniqueId Modifier id
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveModifier(FName UniqueId);

	/**
	 * @brief Check if effect is stackable
	 * @return True if effect is stackable
	 * @see bStackable
	 */
	UFUNCTION(BlueprintPure)
	bool GetIsStackable() const;

	/**
	 * @brief Called when we need to stack same effect
	 * @param InClass Effect class (can be child)
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Stack(TSubclassOf<UXeusEffect> InClass);

	/**
	 * @brief Called when effect should start work
	 * It will prepare all data, save AbilitySystem pointer etc..
	 * @param InAbilitySystem The component that created the effect
	 */
	void NotifyBeginWork(UXeusAbilitySystemComponent* InAbilitySystem);

	/**
	 * @brief Called when effect should end work
	 * @see EndWork
	 */
	void NotifyEndWork();


	/**
	 * @brief Called when some effect added to ability component
	 * @param InEffect New effect instance
	 */
	UFUNCTION(BlueprintNativeEvent)
	void EffectAdded(UXeusEffect* InEffect);

	/**
	 * @brief Called when some effect is being removed from ability component
	 * @param InEffect Old effect instance
	 */
	UFUNCTION(BlueprintNativeEvent)
	void EffectRemoving(UXeusEffect* InEffect);

	/**
	 * @deprecated 
	 * @brief Get debug name for developers
	 * @return Custom name
	 */
	UFUNCTION(BlueprintPure)
	virtual FString GetDebugName() const;

	/**
	 * @brief Get icon for HUD display
	 * @return Soft ptr to icon texture
	 */
	UFUNCTION(BlueprintCallable)
	virtual TSoftObjectPtr<UTexture2D> GetIcon() const;

	/**
	 * @brief Get color for HUD display
	 * @return Display linear color
	 */
	UFUNCTION(BlueprintPure)
	virtual FLinearColor GetColor() const;

	/**
	 * @brief Check if effect should be displayed
	 * @return True if should be displayed in HUD
	 */
	UFUNCTION(BlueprintPure)
	virtual bool GetIsDisplayable() const;

	/**
	 * @brief Should be called when effect want to be destroyed
	 * Do not destroy object directly
	 * It must be done through ability system component
	 * @see EndWork
	 */
	FXeusEffectActionDelegate OnNeedRemove;
	
};


