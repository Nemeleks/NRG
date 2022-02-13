// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ActorInfoEntryWidget.h"

#include "Components/TextBlock.h"

void UActorInfoEntryWidget::SetInfoText(const FText& Text)
{
	InfoText->SetText(Text);
}
