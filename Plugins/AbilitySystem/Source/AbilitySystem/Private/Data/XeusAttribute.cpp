// Developed by OIC

#include "Data/XeusAttribute.h"
#include "AbilitySystemTypes.h"
#include "Algo/IndexOf.h"
#include "Net/UnrealNetwork.h"

FAttributeMultiplier::FAttributeMultiplier()
	: UniqueId("ID")
	  , Value(1.0f)
	  , Type(EAttributeMultiplierType::Add)
{
}

FAttributeMultiplier::FAttributeMultiplier(FName Id, float InValue, EAttributeMultiplierType InType)
	: UniqueId(Id)
	  , Value(InValue)
	  , Type(InType)
{
}

FAttributeMultiplier::FAttributeMultiplier(const FAttributeMultiplier& Other)
	: FAttributeMultiplier(Other.UniqueId, Other.Value, Other.Type)
{
}

bool FAttributeMultiplier::HasSameId(const FAttributeMultiplier& Other) const
{
	return Other.UniqueId == this->UniqueId;
}

bool FAttributeMultiplier::HasSameValue(const FAttributeMultiplier& Other) const
{
	return Other.Value == this->Value;
}

bool FAttributeMultiplier::HasSameType(const FAttributeMultiplier& Other) const
{
	return Other.Type == this->Type;
}

UXeusAttribute::UXeusAttribute(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxValue = 100.0f;
	MinValue = 0.0f;
	DefaultValue = 100.0f;
	CurrentValue = DefaultValue;
}

UXeusAttribute* UXeusAttribute::CreateAttributeFromClass(TSubclassOf<UXeusAttribute> InClass, UObject* Outer)
{
	UXeusAttribute* Attribute = NewObject<UXeusAttribute>(Outer, InClass);
	//Attribute->SetOwner(Owner);
	return Attribute;
}

TArray<const FAttributeMultiplier*> UXeusAttribute::GetMultsByType(EAttributeMultiplierType InType) const
{
	TArray<const FAttributeMultiplier*> pointers;
	for (int32 i = 0; i < Mults.Num(); ++i)
	{
		if (Mults[i].Type == InType)
		{
			const FAttributeMultiplier* p = &Mults[i];
			pointers.Add(p);
		}
	}
	return pointers;
}

TArray<FAttributeMultiplier> UXeusAttribute::GetMults(EAttributeMultiplierType InType)
{
	return Mults.FilterByPredicate([InType](const FAttributeMultiplier& A)
	{
		return A.Type == InType;
	});
}

float UXeusAttribute::GetMultValue(EAttributeMultiplierType InType) const
{
	float Value = 1.0f;
	if (Mults.Num() == 0)
		return Value;

	const TArray<const FAttributeMultiplier*> pointers = GetMultsByType(InType);
	for (const FAttributeMultiplier* pointer : pointers)
		Value *= pointer->Value;
	return Value;
}

bool UXeusAttribute::AddMult(FAttributeMultiplier InMult)
{
	const FAttributeMultiplier* res = GetMult(InMult.UniqueId);
	if (res != nullptr)
		return false;

	Mults.Add(InMult);
	OnMultAdded.Broadcast(this, InMult.UniqueId);
	return true;
}

bool UXeusAttribute::RemoveMult(FName InId)
{
	const int32 index = Algo::IndexOfByPredicate(Mults,
	                                             [InId](const FAttributeMultiplier& AttributeMultiplier)
	                                             {
		                                             return AttributeMultiplier.UniqueId == InId;
	                                             });
	if (index == -1)
		return false;
	Mults.RemoveAt(index);
	OnMultRemoved.Broadcast(this);
	return true;
}

void UXeusAttribute::GetMultById(FName InId, FAttributeMultiplier& OutMult, bool& bOutSuccess)
{
	const FAttributeMultiplier* res = GetMult(InId);
	if (res == nullptr)
	{
		bOutSuccess = false;
	}
	else
	{
		OutMult = *res;
		bOutSuccess = true;
	}
}

const FAttributeMultiplier* UXeusAttribute::GetMult(FName InId) const
{
	if (Mults.Num() == 0)
		return nullptr;

	for (int32 i = 0; i < Mults.Num(); ++i)
		if (Mults[i].UniqueId == InId)
			return &Mults[i];

	return nullptr;
}

void UXeusAttribute::SetCurrentValue(float InValue, bool useMult)
{
	const float mxValue = GetMaxValue();

	//if greater than max value
	if (InValue >= mxValue)
	{
		CurrentValue = mxValue;
		OnMaxValue.Broadcast(this);
	}
	else
	{
		//If less than min value
		const float mnValue = GetMinValue();
		if (InValue <= mnValue)
		{
			CurrentValue = mnValue;
			OnMinValue.Broadcast(this);
		}
		else //else set value
		{
			//with mult of setter
			if (useMult)
			{
				const float mult = GetMultValue(EAttributeMultiplierType::Set);
				CurrentValue = InValue * mult;
			}
			else
			{
				CurrentValue = InValue;
			}
		}
	}

	//Broadcast current value with getter mult
	OnValueChanged.Broadcast(this, GetCurrentValue());
}

void UXeusAttribute::AddCurrentValue(float InValue)
{
	const float mult = GetMultValue(EAttributeMultiplierType::Add);
	SetCurrentValue(GetCurrentValue() + (FMath::Abs(InValue) * mult));
}

void UXeusAttribute::RemoveCurrentValue(float InValue)
{
	const float mult = GetMultValue(EAttributeMultiplierType::Remove);
	SetCurrentValue(GetCurrentValue() - (FMath::Abs(InValue) * mult));
}

void UXeusAttribute::SetMaxValue(float InValue)
{
	MaxValue = InValue;
	OnMaxValueChanged.Broadcast(this, MaxValue);
}

void UXeusAttribute::SetMinValue(float InValue)
{
	MinValue = InValue;
	OnMinValueChanged.Broadcast(this, MinValue);
}

float UXeusAttribute::GetCurrentValue() const
{
	const float mult = GetMultValue(EAttributeMultiplierType::Get);
	return FMath::Clamp(CurrentValue * mult, GetMinValue(), GetMaxValue());
}

float UXeusAttribute::GetMaxValue() const
{
	const float mult = GetMultValue(EAttributeMultiplierType::MaxValue);
	return MaxValue * mult;
}

float UXeusAttribute::GetPercent() const
{
	return FMath::GetMappedRangeValueUnclamped(FVector2D(GetMinValue(), GetMaxValue()),
	                                           FVector2D(0.0f, 1.0f),
	                                           GetCurrentValue());
}

FString UXeusAttribute::GetDebugName()
{
	return GetClass()->GetName();
}

void UXeusAttribute::EditValue(EAttributeModifyType ModifyType, float Value)
{
	switch (ModifyType)
	{
	default:
		break;
	case EAttributeModifyType::Set:
		SetCurrentValue(Value);
		break;
	case EAttributeModifyType::Add:
		AddCurrentValue(Value);
		break;
	case EAttributeModifyType::Remove:
		RemoveCurrentValue(Value);
		break;
	}
}
