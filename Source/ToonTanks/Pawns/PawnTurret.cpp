// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanks/Pawns/PawnTurret.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "Kismet/GameplayStatics.h"


// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	
    GetWorld()->GetTimerManager().SetTimer(
        this->FireRateTimerHandle, 
        this, 
        &APawnTurret::CheckFireCondition, 
        this->FireRate, 
        true
    );

    this->PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (! this->IsTargetInRange()) return;

    this->RotateTurretTo(this->PlayerPawn->GetActorLocation());
}

bool APawnTurret::IsTargetInRange()
{
    if (! this->PlayerPawn) return false;

    if (! this->PlayerPawn->GetIsPlayerAlive()) return false;

    return this->getDistanceToPlayer() <= this->FireRange;
}

void APawnTurret::CheckFireCondition()
{
    if (this->IsTargetInRange()) {
        // Fire...
        this->Fire();
    }

}

float APawnTurret::getDistanceToPlayer()
{
    if (!this->PlayerPawn) return 0.f;

    return FVector::Dist(this->PlayerPawn->GetActorLocation(), this->GetActorLocation());

}

void APawnTurret::HandleDestruction()
{
    Super::HandleDestruction();

    this->Destroy();
}
