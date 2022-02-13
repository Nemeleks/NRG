// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SatietyComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USatietyComponent::USatietyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USatietyComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);

	MaxSatiety = 100.f;
	CurrentSatiety = MaxSatiety;

	GetWorld()->GetTimerManager().SetTimer(SatietyTimerHandle, this, &USatietyComponent::SatietyDecrease, SatietyDecreaseRate, true);
	
}


// Called every frame
void USatietyComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USatietyComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USatietyComponent, CurrentSatiety);
}

void USatietyComponent::SetCurrentSatiety(float SatietyValue)
{
	CurrentSatiety = FMath::Clamp( SatietyValue, 0.f, MaxSatiety);
	if (CurrentSatiety <= 0)
	{
		OnHungry.Broadcast();
	}
}


void USatietyComponent::SatietyDecrease()
{
	SetCurrentSatiety(CurrentSatiety - SatietyDecreaseValue);
}

