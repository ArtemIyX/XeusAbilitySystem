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
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UXeusAbilitySystemLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static TArray<FAttributeData> GetAttributeData(UXeusAbilitySystemComponent* AbilitySystemComponentRef);
	UFUNCTION(BlueprintCallable)
	static TArray<FEffectData> GetEffectData(UXeusAbilitySystemComponent* AbilitySystemComponentRef);
};
