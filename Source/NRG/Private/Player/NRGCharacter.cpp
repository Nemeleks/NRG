// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NRGCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/HealthComponent.h"
#include "Components/SatietyComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interface/Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/ActorInfoWidget.h"
#include "Vulkan/Include/vulkan/vulkan_core.h"

#define DEBUGMESSAGE(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));}


// Sets default values
ANRGCharacter::ANRGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent= CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    if (GetLocalRole() == ROLE_Authority)
    {
	    HealthComponent->OnDeath.AddDynamic(this, &ThisClass::OnDeath);
    }
	
	SatietyComponent = CreateDefaultSubobject<USatietyComponent>(TEXT("SatietyComponent"));
    if (GetLocalRole() == ROLE_Authority)
    {
	    SatietyComponent->OnHungry.AddDynamic(this, &ThisClass::OnHungry);
    }

	HitCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollider"));
	HitCollider->SetupAttachment(GetRootComponent());
	HitCollider->SetIsReplicated(true);
	HitCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnDamageCause);
    

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);

	InfoWidgetSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InfoWidgetSpawnPoint"));
	InfoWidgetSpawnPoint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ANRGCharacter::BeginPlay()
{
	Super::BeginPlay();
	bIsAttacking = false;
	bCanCauseDamage = false;
	if (InfoWidgetClass && WidgetComponent)
	{
		WidgetComponent->SetWidgetClass(InfoWidgetClass);
		WidgetComponent->SetWidget(nullptr);
	}
	if(HitCollider)
	{
		HitCollider->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "LeftWirst");
	}
}

void ANRGCharacter::OnDamageCause(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto Character = Cast<ANRGCharacter>(OtherActor);
	
	if (Character && Character != this && bCanCauseDamage )
	{
		EnableDamage(false);
		Server_ApplyDamage(Character);
	}
}

// Called every frame
void ANRGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceInfo();
}

// Called to bind functionality to input
void ANRGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ANRGCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnAround", this, &ANRGCharacter::TurnAround);
	PlayerInputComponent->BindAction("Interact",IE_Pressed, this, &ANRGCharacter::Interact);
	PlayerInputComponent->BindAction("Punch",IE_Pressed, this, &ANRGCharacter::StartPunch);

}

void ANRGCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANRGCharacter, bIsAttacking);
	DOREPLIFETIME(ANRGCharacter, bCanCauseDamage);
}

float ANRGCharacter::GetMovementDirection() const
{
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	return FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);
}

TArray<FString> ANRGCharacter::GetActorInfo() const
{
	TArray<FString> ActorInfo;
	ActorInfo.Add(PlayerName);
	const float CurrentHP = HealthComponent->GetCurrentHealth();
	const FString HealthPoints = "HP: " + FString::SanitizeFloat(CurrentHP);
	ActorInfo.Add(HealthPoints);
	const float CurrentSP = SatietyComponent->GetCurrentSatiety();
	const FString SatietyPoints = "SP: " + FString::SanitizeFloat(CurrentSP);
	ActorInfo.Add(SatietyPoints);
	return ActorInfo;
}

void ANRGCharacter::OnDeath()
{
	Destroy();
}

void ANRGCharacter::OnHungry()
{
	const FDamageEvent DamageEvent;
	TakeDamage(2.f, DamageEvent, GetController(), this);
}

void ANRGCharacter::Server_Attack_Implementation(bool IsAttacking)
{
	bIsAttacking = true;
	GetWorld()->GetTimerManager().SetTimer(PunchTimerHandle, this, &ThisClass::StopPunch, AttackAnimRate, false);
}

void ANRGCharacter::Server_ApplyDamage_Implementation(AActor* Target)
{
	if (const auto Enemy = Cast<ANRGCharacter>(Target))
	{
		if (const auto Health = Cast<UHealthComponent>(Enemy->GetComponentByClass(UHealthComponent::StaticClass())))
		{
			Health->SetCurrentHealth(Health->GetCurrentHealth() - Damage);
		}
	}
}

void ANRGCharacter::ApplyDamage_Implementation(AActor* Target)
{
	Server_ApplyDamage(Target);
}

void ANRGCharacter::Server_EnableDamage_Implementation(bool CanCauseDamage)
{
	bCanCauseDamage = CanCauseDamage;
}

void ANRGCharacter::EnableDamage(bool CanCauseDamage)
{
	Server_EnableDamage(CanCauseDamage);
}

void ANRGCharacter::MoveForward(float Amount)
{
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ANRGCharacter::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector(), Amount);
}

void ANRGCharacter::LookUp(float Amount)
{
	AddControllerPitchInput(Amount);
}

void ANRGCharacter::TurnAround(float Amount)
{
	AddControllerYawInput(Amount);
}

void ANRGCharacter::Interact_Implementation()
{
	if (ActorToInteract)
	{
		ActorToInteract->OnUse(this);
	}
}

void ANRGCharacter::StartPunch()
{
	if (!bIsAttacking)
	{
		Server_EnableDamage(true);
		Server_Attack(bIsAttacking);
	}
}

void ANRGCharacter::StopPunch()
{
	bIsAttacking = false;
	EnableDamage(false);
	GetWorld()->GetTimerManager().ClearTimer(PunchTimerHandle);
}

void ANRGCharacter::TraceInfo()
{
	if (InfoWidgetClass && WidgetComponent)
	{
		FHitResult HitResult;
		const auto TraceStart = GetActorLocation();
		const auto Direction = GetControlRotation().Vector();
		const auto TraceEnd = TraceStart+Direction*TraceLenght;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		if(GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart, TraceEnd, ECollisionChannel::ECC_Camera, Params))
		{
			if (!InfoWidget)
			{
				if (const auto Actor = Cast<IActorInfo>(HitResult.Actor))
				{
					const auto Info = Actor->GetActorInfo();
					InfoWidget = CreateWidget<UActorInfoWidget>(GetWorld(), InfoWidgetClass);
					InfoWidget->Init(Info);
					WidgetComponent->SetWidget(InfoWidget);
					WidgetComponent->SetWorldLocation(Actor->GetWidgetSpawnPoint());
				}
			}
			if (!ActorToInteract)
			{
				if (const auto Interactable = Cast<IInteractable>(HitResult.Actor))
				{
					ActorToInteract = Interactable;
				}
			}
		}
		else
		{
			if (InfoWidget)
			{
				InfoWidget = nullptr;
				WidgetComponent->SetWidget(nullptr);
			}
			if (ActorToInteract)
			{
				ActorToInteract = nullptr;
			}
		}
	}
}

FVector ANRGCharacter::GetWidgetSpawnPoint() const
{
	if (InfoWidgetSpawnPoint)
	{
		return InfoWidgetSpawnPoint->GetComponentLocation();
	}
	return FVector::ZeroVector;
}