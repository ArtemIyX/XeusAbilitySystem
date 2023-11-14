// Developed by OIC

#pragma once

#include "CoreMinimal.h"
#include "Data/XeusEffect.h"
#include "UObject/Object.h"
#include "XeusInstantEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup=(XeusAbilitySystem))
class ABILITYSYSTEM_API UXeusInstantEffect : public UXeusEffect
{
	GENERATED_BODY()
public:
	UXeusInstantEffect(const FObjectInitializer& ObjectInitializer);
public:
	virtual void Work_Implementation() override;
	virtual void EndWork_Implementation() override;
};
