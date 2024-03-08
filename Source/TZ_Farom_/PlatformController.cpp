#include "PlatformController.h"
#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"

void APlatformController::BeginPlay()
{
    Super::BeginPlay();
    if (IsLocalPlayerController())
    {
        UE_LOG(LogTemp, Warning, TEXT("LocalController"));
        InputComponent->BindAxis("MovementRightLeft");
    }
}

void APlatformController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (ControlledActor && InputComponent)
    {
        float HorizontalInput = InputComponent->GetAxisValue("MovementRightLeft");
        ControlledActor->AddActorWorldOffset(ControlledActor->GetActorRightVector() * HorizontalInput * MovementSpeed);
        FVector NewLocation = ControlledActor->GetActorLocation();
        NewLocation.X = FMath::Clamp(NewLocation.X, 80.f, 800.f);
        ControlledActor->SetActorLocation(NewLocation);
        Server_MoveActor(HorizontalInput);
    }
    else
    {
        try
        {
            ControlledActor = GetPawn();
        }
        catch (const std::exception&)
        {
            UE_LOG(LogTemp, Error, TEXT("Error in getting pawn"));
        }
    }
}

void APlatformController::Server_MoveActor_Implementation(float HorizontalInput)
{
    ControlledActor->AddActorWorldOffset(ControlledActor->GetActorRightVector() * HorizontalInput * MovementSpeed);
    FVector NewLocation = ControlledActor->GetActorLocation();
    NewLocation.X = FMath::Clamp(NewLocation.X, 0.f, 900.f);
    ControlledActor->SetActorLocation(NewLocation);
}

bool APlatformController::Server_MoveActor_Validate(float HorizontalInput)
{
    return true;
}