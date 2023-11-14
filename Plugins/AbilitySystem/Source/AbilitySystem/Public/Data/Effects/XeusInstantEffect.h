// Developed by OIC

#pragma once

#include "CoreMinimal.h"
#include "Data/XeusEffect.h"
#include "UObject/Object.h"
#include "XeusInstantEffect.generated.h"

/**
 * A permanent effect that only does the job once.
 * Used for checks (Is player is busy or is chest is opened)
 * You need to delete it yourself
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
