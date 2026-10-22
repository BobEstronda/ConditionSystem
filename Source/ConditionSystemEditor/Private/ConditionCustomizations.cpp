// Fill out your copyright notice in the Description page of Project Settings.


#include "ConditionCustomizations.h"
#include "DynamicCall/Public/DynamicCallTypes.h"
#include "ConditionTypes.h"

//#include "DetailLayoutBuilder.h"
//#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
//#include "IDetailPropertyRow.h"
#include "IPropertyUtilities.h"
#include "PropertyCustomizationHelpers.h"
//#include "Widgets/Input/SSearchableComboBox.h"
//#include "Widgets/Layout/SBox.h"
//#include "Widgets/SBoxPanel.h"
#include "Widgets/SNullWidget.h"
//#include "Widgets/Text/STextBlock.h"

namespace
{
	// Ponteiro cru para a struct editada (primeiro objeto selecionado).
	template <typename T>
	T* GetStructPtr(const TSharedPtr<IPropertyHandle>& H)
	{
		if (!H.IsValid()) return nullptr;
		TArray<void*> Raw;
		H->AccessRawData(Raw);
		return Raw.Num() > 0 ? static_cast<T*>(Raw[0]) : nullptr;
	}

	// Pede refresh do painel (adiado para o proximo tick, seguro de chamar dentro de callbacks).
	FSimpleDelegate MakeRefreshDelegate(IPropertyTypeCustomizationUtils& CU)
	{
		TWeakPtr<IPropertyUtilities> Weak = CU.GetPropertyUtilities();
		return FSimpleDelegate::CreateLambda([Weak]()
			{
				if (TSharedPtr<IPropertyUtilities> U = Weak.Pin())
				{
					//U->RequestRefresh();
					U->RequestForceRefresh();
				}
			});
	}

	// Cria o widget certo (checkbox, numero, texto, seletor de asset...) conforme o tipo do literal.
	TSharedRef<SWidget> MakeLiteralWidget(const TSharedPtr<IPropertyHandle>& LiteralH, IPropertyTypeCustomizationUtils& CU)
	{
		const FDynamicLiteral* L = GetStructPtr<FDynamicLiteral>(LiteralH);
		const EDynamicValueType T = L ? L->Type : EDynamicValueType::Float;

		switch (T)
		{
		case EDynamicValueType::Bool:
			return LiteralH->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDynamicLiteral, bValue))->CreatePropertyValueWidget();
		case EDynamicValueType::Int:
			return LiteralH->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDynamicLiteral, IntValue))->CreatePropertyValueWidget();
		case EDynamicValueType::Float:
			return LiteralH->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDynamicLiteral, FloatValue))->CreatePropertyValueWidget();
		case EDynamicValueType::String:
			return LiteralH->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDynamicLiteral, StringValue))->CreatePropertyValueWidget();
		case EDynamicValueType::Name:
			return LiteralH->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDynamicLiteral, NameValue))->CreatePropertyValueWidget();
		case EDynamicValueType::Object:
		{
			const UClass* Allowed = (L && L->ObjectClass) ? L->ObjectClass.Get() : UObject::StaticClass();
			return SNew(SObjectPropertyEntryBox)
				.PropertyHandle(LiteralH->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDynamicLiteral, ObjectValue)))
				.AllowedClass(Allowed)
				.ThumbnailPool(CU.GetThumbnailPool());
		}
		}
		return SNullWidget::NullWidget;
	}
}

void FConditionOperandCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> Handle, FDetailWidgetRow& Row, IPropertyTypeCustomizationUtils& Utils)
{
	TSharedPtr<IPropertyHandle> H = Handle;
	TSharedPtr<IPropertyHandle> ModeH = Handle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FConditionOperand, Mode));
	TSharedPtr<IPropertyHandle> LiteralH = Handle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FConditionOperand, Literal));

	ModeH->SetOnPropertyValueChanged(MakeRefreshDelegate(Utils));

	Row.NameContent()
		[
			Handle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.MinDesiredWidth(320.f)
		[
			SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(0.f, 0.f, 4.f, 0.f)
				[
					ModeH->CreatePropertyValueWidget()
				]

				+ SHorizontalBox::Slot()
				.FillWidth(1.f)
				[
					SNew(SBox)
						.Visibility_Lambda([H]()
							{
								const FConditionOperand* O = GetStructPtr<FConditionOperand>(H);
								return (O && O->Mode == EConditionOperandMode::Literal) ? EVisibility::Visible : EVisibility::Collapsed;
							})
						[
							MakeLiteralWidget(LiteralH, Utils)
						]
				]
		];
}

void FConditionOperandCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> Handle, IDetailChildrenBuilder& Children, IPropertyTypeCustomizationUtils& Utils)
{
	TSharedPtr<IPropertyHandle> H = Handle;

	Children.AddProperty(Handle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FConditionOperand, Accessor)).ToSharedRef())
		.Visibility(TAttribute<EVisibility>::CreateLambda([H]()
			{
				const FConditionOperand* O = GetStructPtr<FConditionOperand>(H);
				return (O && O->Mode == EConditionOperandMode::Accessor) ? EVisibility::Visible : EVisibility::Collapsed;
			}));
}
