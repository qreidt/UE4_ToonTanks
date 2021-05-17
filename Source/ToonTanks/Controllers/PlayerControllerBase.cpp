// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanks/Controllers/PlayerControllerBase.h"


void APlayerControllerBase::SetPlayerEnabledState(bool PlayerEnabledState)
{

    if (PlayerEnabledState) {
        this->GetPawn()->EnableInput(this);

    } else {
        this->GetPawn()->DisableInput(this);
    }

    this->bShowMouseCursor = PlayerEnabledState;

}