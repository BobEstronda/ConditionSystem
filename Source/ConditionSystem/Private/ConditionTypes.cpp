// Fill out your copyright notice in the Description page of Project Settings.


#include "ConditionTypes.h"

#include "DynamicCall/Public/DynamicCallTypes.h"

bool FConditionOperand::Evaluate(AActor* Ctx, /*FConditionValue&*/ FDynamicValue& Out) const
{
	if (Mode == EConditionOperandMode::Literal)
	{
		Out = Literal.ToValue();
		return true;
	}
	//return Accessor.Evaluate(Ctx, Out);

	return Accessor.Invoke(Ctx, &Out);
}


bool FConditionOperand::ResolveType( /*EConditionValueType&*/ EDynamicValueType& OutType, UClass*& OutClass) const
{
	if (Mode == EConditionOperandMode::Literal)
	{
		OutType = Literal.Type;
		OutClass = Literal.ObjectClass;
		return true;
	}
	return Accessor.ResolveType(OutType, OutClass);
}