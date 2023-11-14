// Developed by OIC


#include "Data/Effects/XeusInstantEffect.h"

UXeusInstantEffect::UXeusInstantEffect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bStackable = false;
}

void UXeusInstantEffect::Work_Implementation()
{
	// Nothing, because we dont wont to delete
}

void UXeusInstantEffect::EndWork_Implementation()
{
	Super::EndWork_Implementation();
}
