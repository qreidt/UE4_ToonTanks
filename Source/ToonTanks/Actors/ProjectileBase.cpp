// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanks/Actors/ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	this->ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	this->RootComponent = this->ProjectileMesh;

	this->ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	this->ProjectileMovement->InitialSpeed = this->MovementSpeed;
	this->ProjectileMovement->MaxSpeed = this->MovementSpeed;

	this->TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	this->TrailParticle->SetupAttachment(this->RootComponent);

	this->InitialLifeSpan = 3.f;

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(this, this->LaunchSound, this->GetActorLocation());
	
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = this->GetOwner();

	if (! MyOwner) return;

	if (! OtherActor || OtherActor == this && OtherActor == MyOwner) return;

	// Deal Damage
	UGameplayStatics::ApplyDamage(OtherActor, this->Damage, MyOwner->GetInstigatorController(), this, this->DamageType);
	
	// Spawn Particles
	UGameplayStatics::SpawnEmitterAtLocation(this, this->HitParticle, this->GetActorLocation());

	// Play Sound
	UGameplayStatics::PlaySoundAtLocation(this, this->HitSound, this->GetActorLocation());

	// Shake Camera
	this->GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(this->HitShake);

	// Destroy Projectile
	this->Destroy();
}

