// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanks/Components/HealthComponent.h"
#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	this->Health = this->DefaultHealth;

	this->GameModeRef = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(this->GetWorld()));

	this->GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	
}


void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage == 0 || this->Health <= 0) return;

	this->Health = FMath::Clamp(this->Health - Damage, 0.f, this->DefaultHealth);

	if (this->Health <= 0)
	{
		if (! this->GameModeRef) return;

		this->GameModeRef->ActorDied(this->GetOwner());
	}
}