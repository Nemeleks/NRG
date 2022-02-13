// Fill out your copyright notice in the Description page of Project Settings.


#include "Consumable/ConsumableBase.h"


// Sets default values
AConsumableBase::AConsumableBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	InfoWidgetSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InfoWidgetSpawnPoint"));
	InfoWidgetSpawnPoint->SetupAttachment(RootComponent);
	
	bReplicates = true;
}

// Called when the game starts or when spawned
void AConsumableBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AConsumableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AConsumableBase::OnUse(AActor* InteractInstigator)
{
}


TArray<FString> AConsumableBase::GetActorInfo() const
{
	TArray<FString> Info;
	return Info;
}

FVector AConsumableBase::GetWidgetSpawnPoint() const
{
	if (InfoWidgetSpawnPoint)
	{
		return InfoWidgetSpawnPoint->GetComponentLocation();
	}
	return FVector::ZeroVector;
}

