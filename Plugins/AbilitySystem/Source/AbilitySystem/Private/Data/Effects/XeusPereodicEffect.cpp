// Developed by OIC


#include "Data/Effects/XeusPereodicEffect.h"

UXeusPereodicEffect::UXeusPereodicEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Rate = 1.0f;
	Value = 1.0f;
}

void UXeusPereodicEffect::PeriodTick_Implementation()
{
	// Tick here
}

void UXeusPereodicEffect::Work_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UXeusPereodicEffect::PeriodTick, Rate, true);
}

void UXeusPereodicEffect::EndWork_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	Super::EndWork_Implementation();
}
