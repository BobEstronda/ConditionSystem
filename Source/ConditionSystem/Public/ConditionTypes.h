// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/Object.h"
#include "DynamicCallTypes.h"
#include "ConditionTypes.generated.h"

/**
 * 
 */
//UCLASS()
//class CONDITIONSYSTEM_API UConditionTypes : public UObject
//{
//	GENERATED_BODY()
//	
//};

class AActor;


UENUM(BlueprintType)
enum class EConditionCompare : uint8
{
	Equal UMETA(DisplayName = "=="),
	NotEqual UMETA(DisplayName = "!="),
	Greater UMETA(DisplayName = ">"),
	GreaterEqual UMETA(DisplayName = ">="),
	Less UMETA(DisplayName = "<"),
	LessEqual UMETA(DisplayName = "<=")
};


UENUM(BlueprintType)
enum class EConditionOperandMode : uint8
{
	Literal UMETA(DisplayName = "Fixed value"),
	Accessor UMETA(DisplayName = "Dynamic value")
};


// Operand: fixed OR dynamic value
USTRUCT(BlueprintType)
struct CONDITIONSYSTEM_API FConditionOperand
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Operand", meta = (DisplayName = "Mode"))
	EConditionOperandMode Mode = EConditionOperandMode::Literal;

	UPROPERTY(EditAnywhere, Category = "Operand")
	FDynamicLiteral Literal;
	//FConditionLiteral Literal;

	UPROPERTY(EditAnywhere, Category = "Operand")
	FDynamicPath Accessor;
	//FConditionAccessor Accessor;

	bool Evaluate(AActor* Ctx, /*FConditionValue&*/ FDynamicValue& Out) const;
	bool ResolveType( /*EConditionValueType&*/ EDynamicValueType& OutType, UClass*& OutClass) const;
};