// Fill out your copyright notice in the Description page of Project Settings.


#include "Condition.h"

#include "DynamicCall/Public/DynamicCallTypes.h"

namespace
{
	bool IsNumeric(EDynamicValueType T)
	{
		return T == EDynamicValueType::Bool || T == EDynamicValueType::Int || T == EDynamicValueType::Float;
	}

	bool ApplyOp(int32 Cmp, EConditionCompare Op)
	{
		switch (Op)
		{
		case EConditionCompare::Equal:        return Cmp == 0;
		case EConditionCompare::NotEqual:     return Cmp != 0;
		case EConditionCompare::Greater:      return Cmp > 0;
		case EConditionCompare::GreaterEqual: return Cmp >= 0;
		case EConditionCompare::Less:         return Cmp < 0;
		case EConditionCompare::LessEqual:    return Cmp <= 0;
		}
		return false;
	}

	bool CompareValues(const FDynamicValue& A, EConditionCompare Op, const FDynamicValue& B)
	{
		// bool / int / float: compared as numbers
		if (IsNumeric(A.Type) && IsNumeric(B.Type))
		{
			const bool bNear = FMath::IsNearlyEqual(A.Num, B.Num, 1e-6);
			if (Op == EConditionCompare::Equal)    return bNear;
			if (Op == EConditionCompare::NotEqual) return !bNear;
			return ApplyOp(A.Num < B.Num ? -1 : (A.Num > B.Num ? 1 : 0), Op);
		}

		// string: lexicographical order, case sensitive
		if (A.Type == EDynamicValueType::String && B.Type == EDynamicValueType::String)
		{
			return ApplyOp(A.Str.Compare(B.Str, ESearchCase::CaseSensitive), Op);
		}

		// name / object: == / !=
		if (A.Type == EDynamicValueType::Name && B.Type == EDynamicValueType::Name)
		{
			if (Op == EConditionCompare::Equal)    return A.Name == B.Name;
			if (Op == EConditionCompare::NotEqual) return A.Name != B.Name;
			return false;
		}
		if (A.Type == EDynamicValueType::Object && B.Type == EDynamicValueType::Object)
		{
			if (Op == EConditionCompare::Equal)    return A.Obj == B.Obj;
			if (Op == EConditionCompare::NotEqual) return A.Obj != B.Obj;
			return false;
		}
		return false;
	}
}


bool UCondition_And::Evaluate(AActor* Ctx) const
{
    for (const UCondition* C : Conditions)
    {
        if (C && !C->Evaluate(Ctx)) return false;
    }
    return true;
}

bool UCondition_Or::Evaluate(AActor* Ctx) const
{
    bool bAny = false;
    for (const UCondition* C : Conditions)
    {
        if (!C) continue;
        bAny = true;
        if (C->Evaluate(Ctx)) return true;
    }
    return !bAny;
}

bool UCondition_Not::Evaluate(AActor* Ctx) const
{
    return Condition ? !Condition->Evaluate(Ctx) : true;
}

bool UCondition_Check::Evaluate(AActor* Ctx) const
{
    FDynamicValue V;
    if (!Value.Invoke(Ctx, &V)) return false;

    switch (V.Type)
    {
    case EDynamicValueType::Bool:
    case EDynamicValueType::Int:
    case EDynamicValueType::Float:  return V.Num != 0.0;
    case EDynamicValueType::Object: return IsValid(V.Obj);
    default:                        return false;
    }
}

bool UCondition_Compare::Evaluate(AActor* Ctx) const
{
    FDynamicValue A, B;
    if (!Left.Invoke(Ctx, &A) || !Right.Evaluate(Ctx, B)) return false;
    return CompareValues(A, Op, B);
}
