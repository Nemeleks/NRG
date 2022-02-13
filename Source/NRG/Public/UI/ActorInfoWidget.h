// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorInfoWidget.generated.h"

class UActorInfoEntryWidget;
class UVerticalBox;

UCLASS()
class NRG_API UActorInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ActorInfo")
	TSubclassOf<UActorInfoEntryWidget> ActorInfoEntryWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerBox;

public:
	void Init(const TArray<FString>& Entries);
};
