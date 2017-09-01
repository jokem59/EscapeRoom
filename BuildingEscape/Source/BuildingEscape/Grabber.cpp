// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FString ObjectName = GetOwner()->GetName();
	FString ObjectPos = GetOwner()->GetTransform().GetLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *ObjectName, *ObjectPos)

	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// Physics Handle is found
	}
	else
	{
		// Not found
		UE_LOG(LogTemp, Error, TEXT("%s missing Physics Handle"), *ObjectName)
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		// Input Component is found
		UE_LOG(LogTemp, Warning, TEXT("%s found Input Component"), *ObjectName)
		/// Bind input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Input Component"), *ObjectName)
	}
	
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"))
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key released"))
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	// Get the player viewpoint each tick
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s. Rotation: %s."), 
	//	*PlayerViewPointLocation.ToString(), 
	//	*PlayerViewPointRotation.ToString())

	/// Draw a red trace in the world to visual
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	DrawDebugLine(GetWorld(), 
		PlayerViewPointLocation,
		LineTraceEnd, 
		FColor(255,0,0),
		false,
		0.f,
		0,
		5.f
		);
	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Ray-cast/Line-cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
		UE_LOG(LogTemp, Warning, TEXT("Line Trace hit: %s"), *ActorHit->GetName())
}

