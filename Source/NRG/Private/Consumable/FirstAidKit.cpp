// Fill out your copyright notice in the Description page of Project Settings.


#include "Consumable/FirstAidKit.h"

#include "Components/HealthComponent.h"
#include "Player/NRGCharacter.h"


// Sets default values
AFirstAidKit::AFirstAidKit()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFirstAidKit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFirstAidKit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFirstAidKit::OnUse(AActor* InteractInstigator)
{
	if (const auto Character = Cast<ANRGCharacter>(InteractInstigator))
	{
		if (const auto Health = Cast<UHealthComponent>(Character->GetComponentByClass(UHealthComponent::StaticClass())))
		{
			Health->SetCurrentHealth(Health->GetCurrentHealth() + HealthToRestore);
			Destroy();
		}
	}
}

TArray<FString> AFirstAidKit::GetActorInfo() const
{
	TArray<FString> Info;
	Info.Add(Name.ToString());
	Info.Add("Restore " + FString::SanitizeFloat(HealthToRestore) + " Health.");
	Info.Add(Description.ToString());
	Info.Add(InteractText.ToString());
	return Info;
}

