// Fill out your copyright notice in the Description page of Project Settings.


#include "Consumable/Food.h"

#include "Components/SatietyComponent.h"
#include "Player/NRGCharacter.h"


// Sets default values
AFood::AFood()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFood::OnUse(AActor* InteractInstigator)
{
	if (const auto Character = Cast<ANRGCharacter>(InteractInstigator))
	{
		if (const auto Satiety = Cast<USatietyComponent>(Character->GetComponentByClass(USatietyComponent::StaticClass())))
		{
			Satiety->SetCurrentSatiety(Satiety->GetCurrentSatiety() + SatietyToRestore);
			Destroy();
		}
	}
}

TArray<FString> AFood::GetActorInfo() const
{
	TArray<FString> Info;
	Info.Add(Name.ToString());
	Info.Add("Restore " + FString::SanitizeFloat(SatietyToRestore) + " Satiety.");
	Info.Add(Description.ToString());
	Info.Add(InteractText.ToString());
	return Info;
}

