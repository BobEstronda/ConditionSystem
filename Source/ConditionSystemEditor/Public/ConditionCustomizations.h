// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "PropertyHandle.h"

/**
 * 
 */
//UCLASS()
//class CONDITIONSYSTEMEDITOR_API UConditionCustomizations : public UObject
//{
//	GENERATED_BODY()
//	
//};

// FConditionOperand: [Valor fixo | Valor dinamico] + widget do literal; o accessor aparece como filho quando dinamico.
class FConditionOperandCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance() { return MakeShared<FConditionOperandCustomization>(); }

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> Handle, FDetailWidgetRow& Row, IPropertyTypeCustomizationUtils& Utils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> Handle, IDetailChildrenBuilder& Children, IPropertyTypeCustomizationUtils& Utils) override;
};
