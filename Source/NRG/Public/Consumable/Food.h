// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConsumableBase.h"
#include "Food.generated.h"

UCLASS()
class NRG_API AFood : public AConsumableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Food")
	float SatietyToRestore = 50.f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnUse(AActor* InteractInstigator) override;

	virtual TArray<FString> GetActorInfo() const override;
};
