// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DynamicCallTypes.h"
#include "ConditionTypes.h"
#include "Condition.generated.h"

// Base
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, CollapseCategories, Blueprintable, BlueprintType)
class CONDITIONSYSTEM_API UCondition : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Condition")
	virtual bool Evaluate(AActor* Ctx) const { return true; }
};

// Logical operators
UCLASS(meta = (DisplayName = "AND (all true)"))
class CONDITIONSYSTEM_API UCondition_And : public UCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "Condition")
	TArray<TObjectPtr<UCondition>> Conditions;

	virtual bool Evaluate(AActor* Ctx) const override;
};


UCLASS(meta = (DisplayName = "OR (some true one)"))
class CONDITIONSYSTEM_API UCondition_Or : public UCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "Condition")
	TArray<TObjectPtr<UCondition>> Conditions;

	virtual bool Evaluate(AActor* Ctx) const override;
};


UCLASS(meta = (DisplayName = "NOT (inverse)"))
class CONDITIONSYSTEM_API UCondition_Not : public UCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "Condition")
	TObjectPtr<UCondition> Condition;

	virtual bool Evaluate(AActor* Ctx) const override;
};


// Evaluates a bool value (e.g., Instigator -> [InventoryComponent] -> HasItem(Key)).
// If the value is an object, it counts as "is valid." Numbers: anything other than zero.
UCLASS(meta = (DisplayName = "Check (bool / valid)"))
class CONDITIONSYSTEM_API UCondition_Check : public UCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Condition", meta = (DisplayName = "Value"))
	FDynamicPath Value;
	//FConditionAccessor Value;

	virtual bool Evaluate(AActor* Ctx) const override;

//#if WITH_EDITOR
//	virtual void EditorSync() override;
//#endif
};


// compares two values: ==, !=, >, >=, <, <=. The right side can be fixed or dynamic.
UCLASS(meta = (DisplayName = "Comparar (==, !=, >, >=, <, <=)"))
class CONDITIONSYSTEM_API UCondition_Compare : public UCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Condition", meta = (DisplayName = "Value"))
	FDynamicPath Left;
	//FConditionAccessor Left;

	UPROPERTY(EditAnywhere, Category = "Condition", meta = (DisplayName = "Operator"))
	EConditionCompare Op = EConditionCompare::Equal;

	UPROPERTY(EditAnywhere, Category = "Condition", meta = (DisplayName = "Compare with"))
	FConditionOperand Right;

	virtual bool Evaluate(AActor* Ctx) const override;

//#if WITH_EDITOR
//	virtual void EditorSync() override;
//#endif
};
