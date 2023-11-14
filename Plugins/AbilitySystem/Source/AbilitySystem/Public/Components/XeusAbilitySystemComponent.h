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


UCLASS(ClassGroup=(XeusAbilitySystem), meta=(BlueprintSpawnableComponent))
class ABILITYSYSTEM_API UXeusAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UXeusAbilitySystemComponent();

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<UXeusAttribute*> Attributes;

	UPROPERTY(BlueprintReadOnly)
	TArray<UXeusEffect*> Effects;


protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	#pragma region Effects_Funcs

	void InitEffects();
	UFUNCTION()
	void Effect_NeedRemove(UXeusEffect* Effect);

	UXeusEffect* StackEffect(TSubclassOf<UXeusEffect> InClass);

	void PushEffect(UXeusEffect* InEffect);
	bool RemoveEffect(TSubclassOf<UXeusEffect> InClass);

	#pragma endregion
	#pragma region Attributes_Funcs

	void InitAttributes();
	UFUNCTION()
	void ValueChangedHandle(UXeusAttribute* Attribute, float Value);
	UFUNCTION()
	void MinValueChangedHandle(UXeusAttribute* Attribute, float Value);
	UFUNCTION()
	void MaxValueChangedHandle(UXeusAttribute* Attribute, float Value);

	UFUNCTION()
	void MaxHandle(UXeusAttribute* Attribute);
	UFUNCTION()
	void MinHandle(UXeusAttribute* Attribute);

	#pragma endregion
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*#pragma region Abilities

	UFUNCTION(BlueprintCallable)
	void BP_UseAbility(TSubclassOf<AXeusAbility> InClass, AActor* Instigator = nullptr);

	void UseAbilityImpl(TSubclassOf<AXeusAbility> InClass,
		const struct FAbilityData& Data);

	#pragma endregion*/
	#pragma region Effects

	//Only for end work
	void RemoveAllEffects();
	
	UFUNCTION(BlueprintCallable, DisplayName="Add Effect")
	void BP_AddEffect(TSubclassOf<UXeusEffect> InClass, bool& bSuccess, UXeusEffect*& OutEffect);

	UXeusEffect* AddEffectImpl(TSubclassOf<UXeusEffect> InClass);

	template <class T>
	T* AddEffectImplT()
	{
		return Cast<T>(AddEffectImpl(T::StaticClass()));
	}

	UXeusEffect* AddEffectWithSettingsImpl(TSubclassOf<UXeusEffect> InClass, FXeusEffectSettings* Settings);

	template <class T>
	T* AddEffectWithSettingsImplT(FXeusEffectSettings* Settings)
	{
		return Cast<T>(AddEffectWithSettingsImpl(T::StaticClass(), Settings));
	}

	UFUNCTION(BlueprintCallable)
	bool StopEffect(TSubclassOf<UXeusEffect> InClass);

	template <class T>
	bool StopEffectT()
	{
		return StopEffect(T::StaticClass());
	}

	UFUNCTION(BlueprintCallable)
	void StopAllEffectsByClass(TSubclassOf<UXeusEffect> InClass);

	UFUNCTION(BlueprintCallable)
	void StopAllEffects();

	UFUNCTION(BlueprintPure)
	bool HasEffect(TSubclassOf<UXeusEffect> InClass) const;

	template <class T>
	bool HasEffectT() const
	{
		return GetEffectByClass(T::StaticClass()) != nullptr;
	}

	UFUNCTION(BlueprintPure)
	UXeusEffect* GetEffectByClass(TSubclassOf<UXeusEffect> InClass) const;

	template <class T>
	TArray<T*> GetAllEffectsT() const
	{
		TArray<UXeusEffect*> effects = GetAllEffectsByClass(T::StaticClass());
		TArray<T*> res;
		for(int32 i = 0; i < effects.Num(); ++i)
		{
			res.Add(Cast<T>(effects[i]));
		}
		return res;
	}
	
	UFUNCTION(BlueprintCallable)
	TArray<UXeusEffect*> GetAllEffectsByClass(TSubclassOf<UXeusEffect> InClass) const;

	template <class T>
	UXeusEffect* GetEffect() const
	{
		UClass* effClass = T::StaticClass();
		for (int32 i = 0; i < Effects.Num(); ++i)
			if (IsValid(Effects[i]) && Effects[i]->IsA(effClass))
				return Effects[i];
		return nullptr;
	}

	template <class T>
	T* GetEffectT() const
	{
		return Cast<T>(GetEffectByClass(T::StaticClass()));
	}

	UFUNCTION(BlueprintPure)
	TArray<UXeusEffect*> GetEffects() const;

	UPROPERTY(BlueprintAssignable)
	FAbilitySystemXeusEffectActionDelegate OnEffectStartedWork;
	
	UPROPERTY(BlueprintAssignable)
	FAbilitySystemXeusEffectActionDelegate OnEffectEndWork;

	UPROPERTY(BlueprintAssignable)
	FAbilitySystemXeusEffectActionDelegate Client_OnEffectEndWork;

	UPROPERTY(BlueprintAssignable)
	FAbilitySystemActionDelegate Client_OnEffectsChanged;

	#pragma endregion
	#pragma region Attributes

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilitySystem|Attributes")
	TArray<TSubclassOf<UXeusAttribute>> InitialAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilitySystem|Effects")
	TArray<TSubclassOf<UXeusEffect>> InitialEffects;

	UFUNCTION(BlueprintPure)
	bool HasAttribute(TSubclassOf<UXeusAttribute> InClass) const;

	UFUNCTION(BlueprintPure)
	UXeusAttribute* GetAttributeByClass(TSubclassOf<UXeusAttribute> InClass) const;

	template <class T>
	UXeusAttribute* GetAttribute() const
	{
		UClass* attClass = T::StaticClass();
		for (int32 i = 0; i < Attributes.Num(); ++i)
			if (IsValid(Attributes[i]) && Attributes[i]->IsA(attClass))
				return Attributes[i];

		return nullptr;
	}
	template <class T>
	T* GetAttributeT() const
	{
		UClass* attClass = T::StaticClass();
		for (int32 i = 0; i < Attributes.Num(); ++i)
			if (IsValid(Attributes[i]) && Attributes[i]->IsA(attClass))
				return Cast<T>(Attributes[i]);

		return nullptr;
	}

	UFUNCTION(BlueprintPure)
	TArray<UXeusAttribute*> GetAttributes() const;

	UFUNCTION(BlueprintCallable, DisplayName="Add Attribute")
	void BP_AddAttribute(TSubclassOf<UXeusAttribute> InClass, bool& bSuccess, UXeusAttribute*& Attribute);

	UXeusAttribute* AddAttributeImpl(TSubclassOf<UXeusAttribute> InClass);

	UFUNCTION(BlueprintCallable)
	bool RemoveAttribute(TSubclassOf<UXeusAttribute> InClass);

	UFUNCTION(BlueprintCallable)
	void RemoveAllAttributes();

	UPROPERTY(BlueprintAssignable)
	FAbilitySystemAttributeValueDelegate OnValueChanged;
	UPROPERTY(BlueprintAssignable)
	FAbilitySystemAttributeValueDelegate OnMaxValueChanged;
	UPROPERTY(BlueprintAssignable)
	FAbilitySystemAttributeValueDelegate OnMinValueChanged;

	UPROPERTY(BlueprintAssignable)
	FAbilitySystemAttributeDelegate OnMinValue;
	UPROPERTY(BlueprintAssignable)
	FAbilitySystemAttributeDelegate OnMaxValue;


	#pragma endregion
};


