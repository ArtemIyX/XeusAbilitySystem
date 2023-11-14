// Developed by OIC

#include "Data/Effects/XeusProgressEffect.h"

#include "Net/UnrealNetwork.h"

UXeusProgressEffect::UXeusProgressEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentProgress = 0.0f;
	NeedProgress = 10.0f;
	ProgressRate = 1.0f;
	ProgressAmount = 1.0f;
	bInProgress = false;
	bStackable = false;
}

void UXeusProgressEffect::StartTimer()
{
	FTimerManager& manager = GetWorld()->GetTimerManager();
	if (manager.TimerExists(ProgressTimerHandle))
		manager.ClearTimer(ProgressTimerHandle);
	
	manager.SetTimer(ProgressTimerHandle,
	                 this, &UXeusProgressEffect::TimerWork, ProgressRate, true);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UXeusProgressEffect::PauseTimer()
{
	auto& manager = GetWorld()->GetTimerManager();
	if (manager.IsTimerActive(ProgressTimerHandle))
	{
		manager.PauseTimer(ProgressTimerHandle);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UXeusProgressEffect::UnPauseTimer()
{
	auto& manager = GetWorld()->GetTimerManager();
	if (manager.IsTimerPaused(ProgressTimerHandle))
	{
		manager.UnPauseTimer(ProgressTimerHandle);
	}
}

void UXeusProgressEffect::TimerWork_Implementation()
{
	SetCurrentProgress(GetCurrentProgress() + GetProgressRate());
}


void UXeusProgressEffect::Work_Implementation()
{
	bInProgress = true;
	OnInProgressChanged.Broadcast(this, bInProgress);
	StartTimer();
}

void UXeusProgressEffect::SetCurrentProgress(float Value)
{
	this->CurrentProgress = FMath::Clamp(Value, 0.0f, NeedProgress);
	OnCurrentProgressChanged.Broadcast(this, CurrentProgress);
	if (CurrentProgress >= NeedProgress)
	{
		SetIsInProgress(false);
		EndWork();
	}
}

void UXeusProgressEffect::SetNeedProgress(float Value)
{
	this->NeedProgress = FMath::Clamp(Value, 1.0f, Value);
	OnNeedProgressChanged.Broadcast(this, NeedProgress);
}

void UXeusProgressEffect::SetProgressRate(float Value)
{
	this->ProgressRate = FMath::Clamp(Value, 0.001f, Value);
	StartTimer();
	OnProgressRateChanged.Broadcast(this, ProgressRate);
}

void UXeusProgressEffect::SetProgressAmount(float Value)
{
	this->ProgressAmount = FMath::Clamp(Value, 0.001f, Value);
	OnProgressAmountChanged.Broadcast(this, ProgressAmount);
}

void UXeusProgressEffect::SetIsInProgress(bool Value)
{
	bInProgress = Value;
	if (bInProgress)
	{
		UnPauseTimer();
	}
	else
	{
		PauseTimer();
	}
	OnInProgressChanged.Broadcast(this, bInProgress);
}

float UXeusProgressEffect::GetCurrentProgress() const
{
	return CurrentProgress;
}

float UXeusProgressEffect::GetNeedProgress() const
{
	return NeedProgress;
}

float UXeusProgressEffect::GetProgressRate() const
{
	return ProgressRate;
}

float UXeusProgressEffect::GetProgressAmount() const
{
	return ProgressAmount;
}

bool UXeusProgressEffect::GetIsInProgress() const
{
	return bInProgress;
}

float UXeusProgressEffect::GetPercent() const
{
	return CurrentProgress / NeedProgress;
}
