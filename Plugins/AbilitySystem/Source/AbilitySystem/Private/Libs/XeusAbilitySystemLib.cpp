// Developed by OIC


#include "Libs/XeusAbilitySystemLib.h"

#include "Components/XeusAbilitySystemComponent.h"


TArray<FAttributeData> UXeusAbilitySystemLib::GetAttributeData(UXeusAbilitySystemComponent* AbilitySystemComponentRef)
{
	TArray<FAttributeData> res;
	if (AbilitySystemComponentRef)
	{
		TArray<UXeusAttribute*> Attributes = AbilitySystemComponentRef->GetAttributes();
		for (int32 i = 0; i < Attributes.Num(); ++i)
		{
			res.Add({
				Attributes[i], Attributes[i]->GetDebugName(), Attributes[i]->GetCurrentValue(),
				Attributes[i]->GetMaxValue()
			});
		}
	}
	return res;
}

TArray<FEffectData> UXeusAbilitySystemLib::GetEffectData(UXeusAbilitySystemComponent* AbilitySystemComponentRef)
{
	TArray<FEffectData> res;
	if (AbilitySystemComponentRef)
	{
		TArray<UXeusEffect*> Effects = AbilitySystemComponentRef->GetEffects();
		for (int32 i = 0; i < Effects.Num(); ++i)
		{
			res.Add({Effects[i], Effects[i]->GetDebugName()});
		}
	}
	return res;
}
