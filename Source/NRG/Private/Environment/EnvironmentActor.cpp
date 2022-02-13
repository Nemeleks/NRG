// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/EnvironmentActor.h"


// Sets default values
AEnvironmentActor::AEnvironmentActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InfoWidgetSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InfoWidgetSpawnPoint"));
	InfoWidgetSpawnPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnvironmentActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnvironmentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<FString> AEnvironmentActor::GetActorInfo() const
{
	TArray<FString> Info;
	Info.Add(Name.ToString());
	Info.Add(Description.ToString());
	return Info;
}

FVector AEnvironmentActor::GetWidgetSpawnPoint() const
{
	if (InfoWidgetSpawnPoint)
	{
		return InfoWidgetSpawnPoint->GetComponentLocation();
	}
	return FVector::ZeroVector;
}
