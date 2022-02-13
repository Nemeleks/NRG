// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ActorInfo.h"
#include "Interface/Interactable.h"
#include "ConsumableBase.generated.h"

UCLASS()
class NRG_API AConsumableBase : public AActor, public IActorInfo, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AConsumableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	USceneComponent* InfoWidgetSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Info")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Info")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Info")
	FText InteractText = FText::FromString("Press <E> To Interact.");
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnUse(AActor* InteractInstigator) override;

	virtual TArray<FString> GetActorInfo() const override;

	virtual FVector GetWidgetSpawnPoint() const override;
	
};
