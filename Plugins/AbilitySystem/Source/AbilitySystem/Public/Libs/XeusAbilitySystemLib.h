// Developed by OIC

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AbilitySystemTypes.h"
#include "XeusAbilitySystemLib.generated.h"

class UXeusAttribute;
class UXeusAbilitySystemComponent;
class UXeusEffect;


/**
 * Service lib of ability system
 */
UCLASS(BlueprintType)
class ABILITYSYSTEM_API UXeusAbilitySystemLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief Grab all compact data about attributes from ability component
	 * @param AbilitySystemComponentRef Ability component
	 * @return Compact data about all attributes
	 */
	UFUNCTION(BlueprintCallable)
	static TArray<FAttributeData> GetAttributeData(UXeusAbilitySystemComponent* AbilitySystemComponentRef);

	/**
	 * @brief Grab all compact data about effects from ability component
	 * @param AbilitySystemComponentRef Ability component
	 * @return Compact data about all effects
	 */
	UFUNCTION(BlueprintCallable)
	static TArray<FEffectData> GetEffectData(UXeusAbilitySystemComponent* AbilitySystemComponentRef);
};
