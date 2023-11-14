// Developed by OIC

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Data/XeusAttribute.h"
#include "Data/XeusEffect.h"

#include "XeusAbilitySystemComponent.generated.h"

class UXeusAbilitySystemComponent;
class UXeusAttribute;
class AXeusAbility;
class UXeusEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilitySystemActionDelegate,
                                            UXeusAbilitySystemComponent*, AbilitySystemComponent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAbilitySystemAttributeValueDelegate,
                                               UXeusAbilitySystemComponent*, AbilitySystemComponent,
                                               UXeusAttribute*, Attribute,
                                               float, Value);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAbilitySystemAttributeDelegate,
                                             UXeusAbilitySystemComponent*, AbilitySystemComponent,
                                             UXeusAttribute*, Attribute);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAbilitySystemXeusEffectActionDelegate,
                                             UXeusAbilitySystemComponent*, AbilitySystemComponent,
                                             UXeusEffect*, Effect);


/**
 * Main ability system component
 * You should add it to actor if you want to have attributes or effects
 */
UCLASS(ClassGroup=(XeusAbilitySystem), meta=(BlueprintSpawnableComponent))
class ABILITYSYSTEM_API UXeusAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UXeusAbilitySystemComponent();

protected:
	/**
	 * @brief Container of current attributes
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<UXeusAttribute*> Attributes;

	/**
	 * @brief Container of current effects
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<UXeusEffect*> Effects;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma region Effects_Funcs

	/**
	 * @brief Called at begin play to initialize all default effects
	 * @see BeginPlay
	 * @see InitialEffects
	 */
	UFUNCTION()
	void InitEffects();

	/**
	 * @brief Called when some effect wants to be removed
	 * It will notify all effects and call broadcast event
	 * @param Effect 
	 */
	UFUNCTION()
	void Effect_NeedRemove(UXeusEffect* Effect);

	/**
	 * @brief Try to stack effect by class
	 * @param InClass Effect class
	 * @return Pointer to effect if stacked, nullptr otherwise 
	 */
	UFUNCTION()
	UXeusEffect* StackEffect(TSubclassOf<UXeusEffect> InClass);

	/**
	 * @brief Add new effect and NotifyBeginWork
	 * @param InEffect Effect instance
	 */
	UFUNCTION()
	void PushEffect(UXeusEffect* InEffect);

	/**
	 * @brief Destroy effect by class and remove from container
	 * Called when some effect wants to be removed
	 * @param InClass Effect class
	 * @return True if removed
	 */
	UFUNCTION()
	bool RemoveEffect(TSubclassOf<UXeusEffect> InClass);

#pragma endregion
#pragma region Attributes_Funcs

	/**
	 * @brief Called at begin play to initialize all default attributes
	 * @see BeginPlay
	 * @see InitialAttributes
	 */
	UFUNCTION()
	void InitAttributes();

	/**
	 * @brief Called when current value of any attribute changed to broadcast event
	 * @param Attribute Attribute instance
	 * @param Value New value of attribute
	 */
	UFUNCTION()
	void ValueChangedHandle(UXeusAttribute* Attribute, float Value);

	/**
	 * @brief Called when min value of any attribute changed to broadcast event
	 * @param Attribute Attribute instance
	 * @param Value New min value of attribute
	 */
	UFUNCTION()
	void MinValueChangedHandle(UXeusAttribute* Attribute, float Value);

	/**
	 * @brief Called when max value of any attribute changed to broadcast event
	 * @param Attribute Attribute instance
	 * @param Value New max value of attribute
	 */
	UFUNCTION()
	void MaxValueChangedHandle(UXeusAttribute* Attribute, float Value);

	/**
	 * @brief Called when value of any attribute clamped to max value
	 * @param Attribute Attribute instance
	 */
	UFUNCTION()
	void MaxHandle(UXeusAttribute* Attribute);

	/**
	 * @brief Called when value of any attribute clamped to min value
	 * @param Attribute Attribute instance
	 */
	UFUNCTION()
	void MinHandle(UXeusAttribute* Attribute);

#pragma endregion

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

#pragma endregion
#pragma region Effects

protected:
	/**
	 * @brief Service function. Will be called at end play to clear effects memory
	 * Do not call it directly
	 */
	UFUNCTION()
	void RemoveAllEffects();

public:
	/**
	 * @brief Initial effects that will be initialized from begin play
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilitySystem|Effects")
	TArray<TSubclassOf<UXeusEffect>> InitialEffects;

	/**
	 * @brief Add effect by class
	 * @param InClass Effect class
	 * @param bSuccess True if created and added
	 * @param OutEffect Pointer to new instance of effect
	 */
	UFUNCTION(BlueprintCallable, DisplayName="Add Effect")
	void BP_AddEffect(TSubclassOf<UXeusEffect> InClass, bool& bSuccess, UXeusEffect*& OutEffect);

	/**
	 * @brief Add effect by class. Will try to stack and push effect.
	 * @param InClass Effect class
	 * @return Pointer to instance of effect
	 */
	UFUNCTION()
	UXeusEffect* AddEffectImpl(TSubclassOf<UXeusEffect> InClass);

	/**
	 * @brief Template function of AddEffectImpl
	 * @see AddEffectImpl
	 * @tparam T Effect class
	 * @return Pointer to instance of effect
	 */
	template <class T>
	T* AddEffectImplT()
	{
		return Cast<T>(AddEffectImpl(T::StaticClass()));
	}

	/**
	 * @deprecated 
	 * @brief Add effect by class with custom settings
	 * @param InClass Effect class
	 * @param Settings Pointer to settings struct
	 * @return Pointer to instance of effect
	 */
	UXeusEffect* AddEffectWithSettingsImpl(TSubclassOf<UXeusEffect> InClass, FXeusEffectSettings* Settings);

	/**
	 * @deprecated
	 * @brief Template function of AddEffectWithSettingsImpl
	 * @tparam T Effect class
	 * @param Settings Pointer to effect initialization settings struct 
	 * @return Pointer to instance of effect
	 */
	template <class T>
	T* AddEffectWithSettingsImplT(FXeusEffectSettings* Settings)
	{
		return Cast<T>(AddEffectWithSettingsImpl(T::StaticClass(), Settings));
	}

	/**
	 * @brief Ends work of effect by class
	 * @param InClass Effect class
	 * @return True if stopped, false otherwise
	 */
	UFUNCTION(BlueprintCallable)
	bool StopEffect(TSubclassOf<UXeusEffect> InClass);


	/**
	 * @brief Ends work of effect by instance
	 * @param InEffectInstance Effect instance
	 * @return True if stopped, false otherwise
	 */
	UFUNCTION(BlueprintCallable)
	bool StopEffectInstance(UXeusEffect* InEffectInstance);

	/**
	 * @brief Template function of StopEffect
	 * @see StopEffect
	 * @tparam T Effect class
	 * @return True if stopped, false otherwise
	 */
	template <class T>
	bool StopEffectT()
	{
		return StopEffect(T::StaticClass());
	}

	/**
	 * @brief Stop all effect by same class
	 * If the effects do not stack, there may be several of them. 
	 * @param InClass Effect class
	 */
	UFUNCTION(BlueprintCallable)
	void StopAllEffectsByClass(TSubclassOf<UXeusEffect> InClass);

	/**
	 * @brief Stop all existing effects 
	 */
	UFUNCTION(BlueprintCallable)
	void StopAllEffects();

	/**
	 * @brief Check if we have effect by class
	 * @param InClass Effect class
	 * @return True if container has effect, false otherwise
	 */
	UFUNCTION(BlueprintPure)
	bool HasEffect(TSubclassOf<UXeusEffect> InClass) const;

	/**
	 * @brief Template function of HasEffect
	 * @see HasEffect
	 * @tparam T Effect class
	 * @return True if container has effect, false otherwise
	 */
	template <class T>
	bool HasEffectT() const
	{
		return GetEffectByClass(T::StaticClass()) != nullptr;
	}

	/**
	 * @brief Try to get effect by class
	 * @param InClass Effect class
	 * @return Pointer to instance of effect if found, nullptr otherwise
	 */
	UFUNCTION(BlueprintPure)
	UXeusEffect* GetEffectByClass(TSubclassOf<UXeusEffect> InClass) const;

	/**
	 * @brief Template function of GetEffectByClass
	 * @see GetEffectByClass
	 * @tparam T Effect class
	 * @return Pointer to instance of effect if found, nullptr otherwise
	 */
	template <class T>
	UXeusEffect* GetEffect() const
	{
		UClass* effClass = T::StaticClass();
		for (int32 i = 0; i < Effects.Num(); ++i)
			if (IsValid(Effects[i]) && Effects[i]->IsA(effClass))
				return Effects[i];
		return nullptr;
	}

	/**
	 * @brief Template function of GetEffect
	 * @tparam T Effect class
	 * @return Pointer to instance of effect if found, nullptr otherwise
	 */
	template <class T>
	T* GetEffectT() const
	{
		return Cast<T>(GetEffectByClass(T::StaticClass()));
	}

	/**
	 * @brief Get all effects with same class
	 * If the effects do not stack, there may be several of them. 
	 * @param InClass Effects class
	 * @return Array of pointers to effects with same class
	 */
	UFUNCTION(BlueprintCallable)
	TArray<UXeusEffect*> GetAllEffectsByClass(TSubclassOf<UXeusEffect> InClass) const;

	/**
	 * @brief Template function of GetAllEffectsByClass
	 * @tparam T Effects class
	 * @return Array of pointers to effects with same class
	 */
	template <class T>
	TArray<T*> GetAllEffectsT() const
	{
		TArray<UXeusEffect*> effects = GetAllEffectsByClass(T::StaticClass());
		TArray<T*> res;
		for (int32 i = 0; i < effects.Num(); ++i)
		{
			res.Add(Cast<T>(effects[i]));
		}
		return res;
	}

	/**
	 * @brief Get all effects
	 * @return Effects container
	 */
	UFUNCTION(BlueprintPure)
	TArray<UXeusEffect*> GetEffects() const;

	/**
	 * @brief Called when some effect started working
	 * @see PushEffect
	 */
	UPROPERTY(BlueprintAssignable)
	FAbilitySystemXeusEffectActionDelegate OnEffectStartedWork;

	/**
	 * @brief Called when some effect finishing working
	 * @see Effect_NeedRemove
	 */
	UPROPERTY(BlueprintAssignable)
	FAbilitySystemXeusEffectActionDelegate OnEffectEndWork;


#pragma endregion
#pragma region Attributes

	/**
	 * @brief Initial attributes that will be initialized from begin play
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilitySystem|Attributes")
	TArray<TSubclassOf<UXeusAttribute>> InitialAttributes;

	/**
	 * @brief Check if we have attribute by class
	 * @param InClass Attribute class
	 * @return True if have, false otherwise
	 */
	UFUNCTION(BlueprintPure)
	bool HasAttribute(TSubclassOf<UXeusAttribute> InClass) const;

	/**
	 * @brief Get attribute by class
	 * @param InClass Attribute class
	 * @return Pointer to attribute instance if found, nullptr otherwise 
	 */
	UFUNCTION(BlueprintPure)
	UXeusAttribute* GetAttributeByClass(TSubclassOf<UXeusAttribute> InClass) const;

	/**
	 * @brief Template function of GetAttributeByClass
	 * @tparam T Attribute class
	 * @return Pointer to attribute instance if found, nullptr otherwise
	 */
	template <class T>
	UXeusAttribute* GetAttribute() const
	{
		UClass* attClass = T::StaticClass();
		for (int32 i = 0; i < Attributes.Num(); ++i)
			if (IsValid(Attributes[i]) && Attributes[i]->IsA(attClass))
				return Attributes[i];

		return nullptr;
	}

	/**
	 * @brief Template function of GetAttribute
	 * @tparam T Attribute class
	 * @return Pointer to attribute instance if found, nullptr otherwise
	 */
	template <class T>
	T* GetAttributeT() const
	{
		UClass* attClass = T::StaticClass();
		for (int32 i = 0; i < Attributes.Num(); ++i)
			if (IsValid(Attributes[i]) && Attributes[i]->IsA(attClass))
				return Cast<T>(Attributes[i]);

		return nullptr;
	}

	/**
	 * @brief Get all attributes
	 * @return Attributes container
	 */
	UFUNCTION(BlueprintPure)
	TArray<UXeusAttribute*> GetAttributes() const;

	/**
	 * @brief Add attribute by class (should be unique)
	 * @param InClass Attribute class (unique)
	 * @param bSuccess True if added
	 * @param Attribute Pointer to attribute instance if added, nullptr otherwise
	 */
	UFUNCTION(BlueprintCallable, DisplayName="Add Attribute")
	void BP_AddAttribute(TSubclassOf<UXeusAttribute> InClass, bool& bSuccess, UXeusAttribute*& Attribute);

	/**
	 * @brief Add attribute by class (should be unique)
	 * @param InClass Attribute class (unique)
	*/
	UXeusAttribute* AddAttributeImpl(TSubclassOf<UXeusAttribute> InClass);

	/**
	 * @brief Remove attribute by class
	 * @param InClass Attribute class
	 * @return True if removed
	 */
	UFUNCTION(BlueprintCallable)
	bool RemoveAttribute(TSubclassOf<UXeusAttribute> InClass);

	/**
	 * @brief Remove all attributes from container (clear memory)
	 * @see EndPlay
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveAllAttributes();

	/**
	 * @brief Called when value of any attribute changed
	 */
	UPROPERTY(BlueprintAssignable)
	FAbilitySystemAttributeValueDelegate OnValueChanged;

	/**
	 * @brief Called when max value of any attribute changed
	 */
	UPROPERTY(BlueprintAssignable)
	FAbilitySystemAttributeValueDelegate OnMaxValueChanged;

	/**
	 * @brief Called when min value of any attribute changed
	 */
	UPROPERTY(BlueprintAssignable)
	FAbilitySystemAttributeValueDelegate OnMinValueChanged;

	/**
	 * @brief Called when value of any attribute clamped to min value
	 */
	UPROPERTY(BlueprintAssignable)
	FAbilitySystemAttributeDelegate OnMinValue;

	/**
	 * @brief Called when value of any attribute clamped to max value
	 */
	UPROPERTY(BlueprintAssignable)
	FAbilitySystemAttributeDelegate OnMaxValue;


#pragma endregion
};
