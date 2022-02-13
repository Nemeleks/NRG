// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ActorInfo.h"
#include "EnvironmentActor.generated.h"

UCLASS()
class NRG_API AEnvironmentActor : public AActor, public IActorInfo
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnvironmentActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	USceneComponent* InfoWidgetSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	FText Description;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual TArray<FString> GetActorInfo() const override;

	virtual FVector GetWidgetSpawnPoint() const override;
};


