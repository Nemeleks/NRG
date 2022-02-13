// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ActorInfo.h"
#include "NRGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;
class USatietyComponent;
class USphereComponent;
class UActorInfoWidget;
class IInteractable;
class UWidgetComponent;

UCLASS()
class NRG_API ANRGCharacter : public ACharacter, public IActorInfo
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANRGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	USatietyComponent* SatietyComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	USceneComponent* InfoWidgetSpawnPoint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	USphereComponent* HitCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackAnimRate = 1.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
	FString PlayerName = "Player";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UActorInfoWidget> InfoWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float TraceLenght = 200.f;

	UFUNCTION()
	void OnDamageCause(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

	virtual TArray<FString> GetActorInfo() const override;

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnHungry();
	
	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool GetIsAttacking() const {return bIsAttacking;}

	virtual FVector GetWidgetSpawnPoint() const override ;

	UPROPERTY(Replicated)
	bool bIsAttacking;

	UPROPERTY(Replicated)
	bool bCanCauseDamage;

	UFUNCTION(Server, Reliable)
	void Server_Attack(bool IsAttacking);
	void Server_Attack_Implementation(bool IsAttacking);

	UFUNCTION(Server, Reliable)
	void Server_ApplyDamage(AActor* Target);
	void Server_ApplyDamage_Implementation(AActor* Target);

	UFUNCTION(Client, Reliable)
	void ApplyDamage(AActor* Target);
	void ApplyDamage_Implementation(AActor* Target);

	UFUNCTION(Server, Reliable)
	void Server_EnableDamage(bool CanCauseDamage);
	void Server_EnableDamage_Implementation(bool CanCauseDamage);

	UFUNCTION(BlueprintCallable)
	void EnableDamage(bool CanCauseDamage);

private:
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void LookUp(float Amount);
	void TurnAround(float Amount);
	void StartPunch();
	void StopPunch();
	
	UFUNCTION(Server, Reliable)
	void Interact();
	void Interact_Implementation();
	


	UPROPERTY()
	UActorInfoWidget* InfoWidget;

	void TraceInfo();
	
	IInteractable* ActorToInteract = nullptr;

	

	FTimerHandle PunchTimerHandle;

};
