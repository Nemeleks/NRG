// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SatietyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHungry);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NRG_API USatietyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USatietyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Satiety")
	float MaxSatiety;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Satiety")
	float SatietyDecreaseRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Satiety")
	float SatietyDecreaseValue = 1.f;

	UPROPERTY(Replicated)
	float CurrentSatiety;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category = "Satiety")
	float GetMaxSatiety() const {return MaxSatiety;}

	UFUNCTION(BlueprintPure, Category = "Satiety")
	float GetCurrentSatiety() const {return CurrentSatiety;}

	UFUNCTION(BlueprintCallable, Category = "Satiety")
	void SetCurrentSatiety(float SatietyValue);

	FOnHungry OnHungry;



private:
	FTimerHandle SatietyTimerHandle;

	void SatietyDecrease();
};
