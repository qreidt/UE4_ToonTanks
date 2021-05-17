// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Controllers/PlayerControllerBase.h"

void ATankGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    this->HandleGameStart();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
    if (DeadActor == this->PlayerTank) {
        
        this->PlayerTank->HandleDestruction();

        this->HandleGamveOver(false);

        if (! this->PlayerControllerRef) return;

        this->PlayerControllerRef->SetPlayerEnabledState(false);

        return;
    }

    if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor)) {
        DestroyedTurret->HandleDestruction();

        if (--TargetTurrets == 0) {
            this->HandleGamveOver(true);
        }
    }
}

void ATankGameModeBase::HandleGameStart()
{

    this->TargetTurrets = this->GetTargetTurretCount();

    this->PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

    this->PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));


    this->GameStart();

    if (! this->PlayerControllerRef) return;

    this->PlayerControllerRef->SetPlayerEnabledState(false);

    FTimerHandle PlayerEnableHandle;
    FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(
        this->PlayerControllerRef,
        &APlayerControllerBase::SetPlayerEnabledState,
        true
    );

    this->GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, this->StartDelay, false);
}

void ATankGameModeBase::HandleGamveOver(bool PlayerWon)
{
    this->GameOver(PlayerWon);
}

int ATankGameModeBase::GetTargetTurretCount()
{
    TArray<AActor*> TurretActors;
    UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), APawnTurret::StaticClass(), TurretActors);

    return TurretActors.Num();
}