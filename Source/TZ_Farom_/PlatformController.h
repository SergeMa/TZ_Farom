// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlatformController.generated.h"

/**
 * 
 */
UCLASS()
class TZ_FAROM__API APlatformController : public APlayerController
{
	GENERATED_BODY()

protected:
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
    float MovementSpeed = 20.f;

    UPROPERTY(VisibleAnywhere)
    AActor* ControlledActor;

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_MoveActor(float HorizontalInput);
};
