#include "Ball.h"
#include "Components/SphereComponent.h"
#include "Engine.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Engine/EngineTypes.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BallCollision = CreateDefaultSubobject<USphereComponent>("Ball Collider");
	BallCollision->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	BallCollision->OnComponentBeginOverlap.AddDynamic(this, &ABall::BeginOverlap);
	AddActorLocalRotation(FRotator(0, rand() % 360, 0));
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalOffset(GetActorForwardVector() * MovementSpeed);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * MovementSpeed, FColor::Blue, true);
}


void ABall::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision event"));
		TArray<FHitResult> AllResults;

		auto Start = GetActorLocation();
		auto End = OtherComp->GetComponentLocation();
		auto CollisionRadius = FVector::Dist(Start, End) * 1.1f;
		GetWorld()->SweepMultiByObjectType(
			AllResults,
			Start,
			End,
			FQuat::Identity,
			0,
			FCollisionShape::MakeSphere(CollisionRadius),
			FCollisionQueryParams::FCollisionQueryParams(false)
		);

		// Finally check which hit result is the one from this event
		for (auto HitResult : AllResults)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit normal: %s"), *HitResult.Normal.ToString());
			if (OtherComp->GetUniqueID() == HitResult.GetComponent()->GetUniqueID()) {
				FVector NewMoveVector;
				if (FMath::Abs(HitResult.Normal.X) == 1)
				{
					NewMoveVector = GetActorRightVector().MirrorByVector(HitResult.Normal);

				}
				else
				{
					NewMoveVector = GetActorForwardVector().MirrorByVector(HitResult.Normal);
				}
				SetActorRotation(NewMoveVector.Rotation());
				break;
			}
		}
}