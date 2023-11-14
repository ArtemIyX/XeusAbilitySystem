#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemTypes.generated.h"

class UXeusEffect;
class UXeusAttribute;

USTRUCT()
struct FXeusEffectSettings
{
	GENERATED_BODY()
public:
	
};

USTRUCT(BlueprintType)
struct FXeusEffectModifier
{
	GENERATED_BODY()
public:
	FXeusEffectModifier();
	FXeusEffectModifier(FName Id, float Val);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UniquedId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;
};

USTRUCT(BlueprintType)
struct FAttributeData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UXeusAttribute* Ref;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxValue;
};

USTRUCT(BlueprintType)
struct FEffectData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UXeusEffect* Ref;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;
};

UENUM(BlueprintType)
enum class EAttributeModifyType : uint8
{
	Set,
	Add,
	Remove
};

UENUM(BlueprintType)
enum class EAttributeMultiplierType : uint8
{
	Set,
	Add,
	Remove,
	Get,
	MaxValue
};

USTRUCT(BlueprintType)
struct FAttributeMultiplier
{
	GENERATED_BODY()
public:
	FAttributeMultiplier();
	FAttributeMultiplier(FName Id, float InValue, EAttributeMultiplierType InType);
	FAttributeMultiplier(const FAttributeMultiplier& Other);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UniqueId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttributeMultiplierType Type;

	bool HasSameId(const FAttributeMultiplier& Other) const;
	bool HasSameValue(const FAttributeMultiplier& Other) const;
	bool HasSameType(const FAttributeMultiplier& Other) const;
};
