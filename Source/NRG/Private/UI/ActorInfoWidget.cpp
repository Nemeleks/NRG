// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ActorInfoWidget.h"

#include "UI/ActorInfoEntryWidget.h"
#include "Components/VerticalBox.h"

void UActorInfoWidget::Init(const TArray<FString>& Entries)
{
	for (const auto Entry : Entries)
	{
		const auto InfoLine = CreateWidget<UActorInfoEntryWidget>(this, ActorInfoEntryWidgetClass);
		if (InfoLine)
		{
			InfoLine->SetInfoText(FText::FromString(Entry));
			VerBox->AddChildToVerticalBox(InfoLine);
		}
		
	}
}
