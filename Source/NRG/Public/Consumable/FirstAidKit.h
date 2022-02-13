// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConsumableBase.h"
#include "FirstAidKit.generated.h"

UCLASS()
class NRG_API AFirstAidKit : public AConsumableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFirstAidKit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float HealthToRestore = 30.f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnUse(AActor* InteractInstigator) override;

	virtual TArray<FString> GetActorInfo() const override;
};
