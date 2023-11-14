// Developed by OIC


#include "Data/XeusEffect.h"
#include "Algo/IndexOf.h"

FXeusEffectModifier::FXeusEffectModifier()
{
	UniquedId = NAME_None;
	Value = 1.0f;
}

FXeusEffectModifier::FXeusEffectModifier(FName Id, float Val)
	: UniquedId(Id)
	, Value(Val) { }

UXeusEffect::UXeusEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystem = nullptr;
	bDisplayable = false;
}

UXeusEffect* UXeusEffect::CreateEffect(TSubclassOf<UXeusEffect> InClass, UObject* Outer, AActor* Owner)
{
	UXeusEffect* Effect = NewObject<UXeusEffect>(Outer, InClass);
	//Effect->SetOwner(Owner);
	return Effect;
}

void UXeusEffect::EndWork_Implementation()
{
	OnNeedRemove.Broadcast(this);
}

void UXeusEffect::Work_Implementation()
{
	EndWork();
}

void UXeusEffect::Setup(FXeusEffectSettings* Settings) { }

void UXeusEffect::OnRep_Modifiers() { }

bool UXeusEffect::GetIsStackable() const
{
	return bStackable;
}

void UXeusEffect::Stack(TSubclassOf<UXeusEffect> InClass) { }

void UXeusEffect::NotifyBeginWork(UXeusAbilitySystemComponent* InAbilitySystem)
{
	check(InAbilitySystem);
	this->AbilitySystem = InAbilitySystem;
	Work();
}

void UXeusEffect::NotifyEndWork()
{
	EndWork();
}

void UXeusEffect::EffectAdded(UXeusEffect* InEffect) { }

void UXeusEffect::EffectRemoving(UXeusEffect* InEffect) { }

FString UXeusEffect::GetDebugName() const
{
	return GetClass()->GetName();
}

TSoftObjectPtr<UTexture2D> UXeusEffect::GetIcon() const
{
	return Icon;
}

FLinearColor UXeusEffect::GetColor() const
{
	return EffectColor;
}

bool UXeusEffect::GetIsDisplayable() const
{
	return bDisplayable;
}

float UXeusEffect::GetTotalModifier() const
{
	float res = 1.0f;
	for (const auto& mod : Modifiers)
		res *= mod.Value;
	return res;
}

int32 UXeusEffect::FindModifier(FName UniqueId) const
{
	return Algo::IndexOfByPredicate(Modifiers,
		[UniqueId](const FXeusEffectModifier& Mod) {
			return Mod.UniquedId == UniqueId;
		});
}

bool UXeusEffect::HasModifier(FName UniqueId) const
{
	return FindModifier(UniqueId) != -1;
}

void UXeusEffect::ApplyModifier(FXeusEffectModifier Mod)
{
	const int32 index = FindModifier(Mod.UniquedId);
	if (index != -1)
		Modifiers[index].Value = Mod.Value;
	else
		Modifiers.Add(Mod);
}

void UXeusEffect::RemoveModifier(FName UniqueId)
{
	const int32 index = FindModifier(UniqueId);
	if (index != -1)
		Modifiers.RemoveAt(index);
}
