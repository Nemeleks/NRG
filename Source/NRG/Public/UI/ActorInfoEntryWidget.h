// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorInfoEntryWidget.generated.h"

class UTextBlock;

UCLASS()
class NRG_API UActorInfoEntryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoText;

public:
	void SetInfoText(const FText& Text);
};
