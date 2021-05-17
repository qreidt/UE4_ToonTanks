// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanks/Pawns/PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "ToonTanks/Actors/ProjectileBase.h"
#include "ToonTanks/Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	this->RootComponent = this->CapsuleComp;

	this->BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	this->BaseMesh->SetupAttachment(this->RootComponent);

	this->TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	this->TurretMesh->SetupAttachment(this->BaseMesh);
	
	this->ProjectileSpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Spawn Point"));
	this->ProjectileSpawnPoint->SetupAttachment(this->TurretMesh);

	this->HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

// Called to bind functionality to input
void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APawnBase::RotateTurretTo(FVector LookAtTarget)
{

	FVector LookAtTargetCleaned = FVector(LookAtTarget.X, LookAtTarget.Y, this->TurretMesh->GetComponentLocation().Z);
	FVector StartLocation = this->TurretMesh->GetComponentLocation();

	FRotator TurretRotation = FVector(LookAtTargetCleaned - StartLocation).Rotation();

	this->TurretMesh->SetWorldRotation(TurretRotation);
}

void APawnBase::Fire()
{
    if (! this->ProjectileClass) return;

	FTransform SpawnPoint = this->ProjectileSpawnPoint->GetComponentTransform();

	AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(
		ProjectileClass, 
		SpawnPoint.GetLocation(), 
		SpawnPoint.GetRotation().Rotator()
	);

	TempProjectile->SetOwner(this);
}

void APawnBase::HandleDestruction()
{
	
	UGameplayStatics::SpawnEmitterAtLocation(this, this->DeathParticle, this->GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(this, this->DeathSound, this->GetActorLocation());

	this->GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(this->DeathShake);
	
}

