// Fill out your copyright notice in the Description page of Project Settings.


#include "BatarangPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABatarangPawn::ABatarangPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create the Mesh Component
	meshCompo = CreateDefaultSubobject<UStaticMeshComponent>("meshCompo");
	RootComponent = meshCompo;

	// Create the Spring Arm Component and attach it to the Root Component
	springArm = CreateDefaultSubobject<USpringArmComponent>("springarm");
	springArm->SetupAttachment(RootComponent); // Attach Spring Arm to the Root Component

	// Create the Camera Component and attach it to the Spring Arm
	camera = CreateDefaultSubobject<UCameraComponent>("camera");
	camera->SetupAttachment(meshCompo); // Attach Camera to the Spring Arm
}

// Called when the game starts or when spawned
void ABatarangPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABatarangPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	deltaSeconds = GetWorld()->DeltaTimeSeconds;
	AutoMove();
	SelfRotate();
	InverseRotationForCamera();

}




// Called to bind functionality to input
void ABatarangPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABatarangPawn::AutoMove()
{
	FVector _direction =  (GetActorForwardVector() * deltaSeconds * moveSpeed);
	SetActorLocation(_direction);
}

void ABatarangPawn::SelfRotate()
{
	float _speed = rotateSpeed * deltaSeconds;
	FRotator _newRotation = GetActorRotation() + FRotator(0, _speed, 0);
	//GetMesh()->SetWorldRotation(_newRotation);
	SetActorRotation(_newRotation);
}

void ABatarangPawn::InverseRotationForCamera()
{
	/*FRotator _compoRotation = meshCompo->GetComponentRotation();
	FRotator _inverseRotation = _compoRotation.GetInverse();*/
	springArm->SetWorldRotation(FRotator (0,0,0));

}

