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
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup=(XeusAbilitySystem))
class ABILITYSYSTEM_API UXeusEffect : public UObject
{
	GENERATED_BODY()
public:
	UXeusEffect(const FObjectInitializer& ObjectInitializer);
	
	static UXeusEffect* CreateEffect(TSubclassOf<UXeusEffect> InClass, UObject* Outer, AActor* Owner);
protected:
	UPROPERTY(Transient)
	UXeusAbilitySystemComponent* AbilitySystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDisplayable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bDisplayable"))
	TSoftObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bDisplayable"))
	FLinearColor EffectColor;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bStackable;

	UPROPERTY()
	TArray<FXeusEffectModifier> Modifiers;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndWork();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Work();

	UFUNCTION()
	virtual void OnRep_Modifiers();

	UFUNCTION()
	int32 FindModifier(FName UniqueId) const;
public:

	virtual void Setup(FXeusEffectSettings* Settings);

	UFUNCTION(BlueprintPure)
	float GetTotalModifier() const;

	UFUNCTION(BlueprintPure)
	bool HasModifier(FName UniqueId) const;
	
	UFUNCTION(BlueprintCallable)
	void ApplyModifier(FXeusEffectModifier Mod);
	UFUNCTION(BlueprintCallable)
	void RemoveModifier(FName UniqueId);
	
	UFUNCTION(BlueprintPure)
	bool GetIsStackable() const;
	
	UFUNCTION(BlueprintCallable)
	virtual void Stack(TSubclassOf<UXeusEffect> InClass);
	
	void NotifyBeginWork(UXeusAbilitySystemComponent* InAbilitySystem);
	void NotifyEndWork();

	virtual void EffectAdded(UXeusEffect* InEffect);
	virtual void EffectRemoving(UXeusEffect* InEffect);

	UFUNCTION(BlueprintPure)
	virtual FString GetDebugName() const;

	UFUNCTION(BlueprintCallable)
	virtual TSoftObjectPtr<UTexture2D> GetIcon() const;
	UFUNCTION(BlueprintPure)
	virtual FLinearColor GetColor() const;
	
	UFUNCTION(BlueprintPure)
	virtual bool GetIsDisplayable() const;
	
	FXeusEffectActionDelegate OnNeedRemove;
	
};


