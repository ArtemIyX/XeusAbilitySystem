// Developed by OIC


#include "Components//XeusAbilitySystemComponent.h"

#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UXeusAbilitySystemComponent::UXeusAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	InitialAttributes = {};
	Effects = {};
	Attributes = {};
}

void UXeusAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAttributes();
	InitEffects();
}

void UXeusAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	RemoveAllAttributes();
	RemoveAllEffects();
}

void UXeusAbilitySystemComponent::InitEffects()
{
	for (const auto& effectClass : InitialEffects)
	{
		if (effectClass)
		{
			AddEffectImpl(effectClass);
		}
	}
}

void UXeusAbilitySystemComponent::InitAttributes()
{
	for (const auto& attributeClass : InitialAttributes)
	{
		if (attributeClass)
		{
			AddAttributeImpl(attributeClass);
		}
	}
}

void UXeusAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

#pragma region Effects

void UXeusAbilitySystemComponent::Effect_NeedRemove(UXeusEffect* Effect)
{
	for (int32 i = 0; i < Effects.Num(); ++i)
		if (Effects[i] && Effects[i] != Effect)
			Effects[i]->EffectRemoving(Effect);

	OnEffectEndWork.Broadcast(this, Effect);
	RemoveEffect(Effect->GetClass());
}

void UXeusAbilitySystemComponent::BP_AddEffect(TSubclassOf<UXeusEffect> InClass, bool& bSuccess,
                                               UXeusEffect*& OutEffect)
{
	OutEffect = AddEffectImpl(InClass);
	bSuccess = IsValid(OutEffect);
}

// ReSharper disable once CppMemberFunctionMayBeConst
UXeusEffect* UXeusAbilitySystemComponent::StackEffect(TSubclassOf<UXeusEffect> InClass)
{
	if (UXeusEffect* effect = GetEffectByClass(InClass))
	{
		if (effect->GetIsStackable())
		{
			effect->Stack(InClass);
			return effect;
		}
		return nullptr;
	}
	return nullptr;
}

void UXeusAbilitySystemComponent::PushEffect(UXeusEffect* InEffect)
{
	InEffect->OnNeedRemove.AddUniqueDynamic(this, &UXeusAbilitySystemComponent::Effect_NeedRemove);

	for (int32 i = 0; i < Effects.Num(); ++i)
		Effects[i]->EffectAdded(InEffect);

	int32 index = Effects.AddUnique(InEffect);

	OnEffectStartedWork.Broadcast(this, InEffect);
	InEffect->NotifyBeginWork(this);
}

bool UXeusAbilitySystemComponent::RemoveEffect(TSubclassOf<UXeusEffect> InClass)
{
	UXeusEffect* Effect = GetEffectByClass(InClass);
	if (!Effect)
		return false;

	const int32 index = Effects.Find(Effect);
	Effects[index]->ConditionalBeginDestroy();
	Effects[index] = nullptr;
	Effects.RemoveAt(index);

	return true;
}

UXeusEffect* UXeusAbilitySystemComponent::AddEffectImpl(TSubclassOf<UXeusEffect> InClass)
{
	if (UXeusEffect* eff = StackEffect(InClass))
		return eff;

	UXeusEffect* Result = UXeusEffect::CreateEffect(InClass, GetOwner());
	PushEffect(Result);

	return Result;
}

UXeusEffect* UXeusAbilitySystemComponent::AddEffectWithSettingsImpl(TSubclassOf<UXeusEffect> InClass,
                                                                    FXeusEffectSettings* Settings)
{
	check(Settings);

	if (UXeusEffect* eff = StackEffect(InClass))
		return eff;

	UXeusEffect* Result = UXeusEffect::CreateEffect(InClass, GetOwner());
	Result->Setup(Settings);
	PushEffect(Result);

	return Result;
}

bool UXeusAbilitySystemComponent::StopEffect(TSubclassOf<UXeusEffect> InClass)
{
	UXeusEffect* Effect = GetEffectByClass(InClass);
	if (!Effect)
		return false;
	Effect->NotifyEndWork();
	return true;
}

void UXeusAbilitySystemComponent::StopAllEffectsByClass(TSubclassOf<UXeusEffect> InClass)
{
	TArray<UXeusEffect*> res = Effects.FilterByPredicate([InClass](const UXeusEffect* Eff)
	{
		return IsValid(Eff) && Eff->GetClass()->IsChildOf(InClass);
	});
	for (int32 i = 0; i < res.Num(); ++i)
	{
		res[i]->NotifyEndWork();
	}
}

void UXeusAbilitySystemComponent::StopAllEffects()
{
	for (int32 i = 0; i < Effects.Num(); ++i)
	{
		if (IsValid(Effects[i]))
		{
			Effects[i]->NotifyEndWork();
		}
	}
}

bool UXeusAbilitySystemComponent::HasEffect(TSubclassOf<UXeusEffect> InClass) const
{
	return GetEffectByClass(InClass) != nullptr;
}

UXeusEffect* UXeusAbilitySystemComponent::GetEffectByClass(TSubclassOf<UXeusEffect> InClass) const
{
	for (int32 i = 0; i < Effects.Num(); ++i)
		if (IsValid(Effects[i]) && Effects[i]->IsA(InClass))
			return Effects[i];
	return nullptr;
}

TArray<UXeusEffect*> UXeusAbilitySystemComponent::GetAllEffectsByClass(TSubclassOf<UXeusEffect> InClass) const
{
	TArray<UXeusEffect*> result;
	for (int32 i = 0; i < Effects.Num(); ++i)
		if (IsValid(Effects[i]) &&
			(Effects[i]->GetClass()->IsChildOf(InClass) || Effects[i]->IsA(InClass)))
			result.Add(Effects[i]);

	return result;
}

TArray<UXeusEffect*> UXeusAbilitySystemComponent::GetEffects() const
{
	return Effects;
}


#pragma endregion

#pragma region Attributes

bool UXeusAbilitySystemComponent::HasAttribute(TSubclassOf<UXeusAttribute> InClass) const
{
	return GetAttributeByClass(InClass) != nullptr;
}

UXeusAttribute* UXeusAbilitySystemComponent::GetAttributeByClass(TSubclassOf<UXeusAttribute> InClass) const
{
	for (int32 i = 0; i < Attributes.Num(); ++i)
		if (IsValid(Attributes[i]) && Attributes[i]->IsA(InClass))
			return Attributes[i];

	return nullptr;
}


TArray<UXeusAttribute*> UXeusAbilitySystemComponent::GetAttributes() const
{
	return Attributes;
}

void UXeusAbilitySystemComponent::BP_AddAttribute(TSubclassOf<UXeusAttribute> InClass, bool& bSuccess,
                                                  UXeusAttribute*& Attribute)
{
	Attribute = AddAttributeImpl(InClass);
	bSuccess = IsValid(Attribute);
}

UXeusAttribute* UXeusAbilitySystemComponent::AddAttributeImpl(TSubclassOf<UXeusAttribute> InClass)
{
	if (HasAttribute(InClass))
		return nullptr;

	UXeusAttribute* Result = UXeusAttribute::CreateAttributeFromClass(InClass, GetOwner());
	if (Result == nullptr)
		return nullptr;

	Result->OnMinValue.AddUniqueDynamic(this, &UXeusAbilitySystemComponent::MinHandle);
	Result->OnMaxValue.AddUniqueDynamic(this, &UXeusAbilitySystemComponent::MaxHandle);
	Result->OnValueChanged.AddUniqueDynamic(this, &UXeusAbilitySystemComponent::ValueChangedHandle);
	Result->OnMinValueChanged.AddUniqueDynamic(this, &UXeusAbilitySystemComponent::MaxValueChangedHandle);
	Result->OnMaxValueChanged.AddUniqueDynamic(this, &UXeusAbilitySystemComponent::MinValueChangedHandle);

	const int32 index = Attributes.AddUnique(Result);

	return Result;
}

bool UXeusAbilitySystemComponent::RemoveAttribute(TSubclassOf<UXeusAttribute> InClass)
{
	UXeusAttribute* Attribute = GetAttributeByClass(InClass);
	if (!Attribute)
		return false;

	const int32 index = Attributes.Find(Attribute);
	Attributes[index]->ConditionalBeginDestroy();
	Attributes[index] = nullptr;
	Attributes.RemoveAt(index);

	return true;
}

void UXeusAbilitySystemComponent::RemoveAllAttributes()
{
	for (int32 i = 0; i < Attributes.Num(); ++i)
	{
		if (Attributes[i])
		{
			Attributes[i]->ConditionalBeginDestroy();
			Attributes[i] = nullptr;
		}
	}
	Attributes.Empty();
}

void UXeusAbilitySystemComponent::RemoveAllEffects()
{
	for (int32 i = 0; i < Effects.Num(); ++i)
	{
		if (Effects[i])
		{
			Effects[i]->ConditionalBeginDestroy();
			Effects[i] = nullptr;
		}
	}
	Effects.Empty();
}


void UXeusAbilitySystemComponent::ValueChangedHandle(UXeusAttribute* Attribute, float Value)
{
	OnValueChanged.Broadcast(this, Attribute, Value);
}

void UXeusAbilitySystemComponent::MinValueChangedHandle(UXeusAttribute* Attribute, float Value)
{
	OnMinValueChanged.Broadcast(this, Attribute, Value);
}

void UXeusAbilitySystemComponent::MaxValueChangedHandle(UXeusAttribute* Attribute, float Value)
{
	OnMaxValueChanged.Broadcast(this, Attribute, Value);
}

void UXeusAbilitySystemComponent::MaxHandle(UXeusAttribute* Attribute)
{
	OnMaxValue.Broadcast(this, Attribute);
}

void UXeusAbilitySystemComponent::MinHandle(UXeusAttribute* Attribute)
{
	OnMinValue.Broadcast(this, Attribute);;
}

#pragma endregion
