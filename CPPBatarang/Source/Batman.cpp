// Fill out your copyright notice in the Description page of Project Settings.


#include "Batman.h"
#include "Batarang.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ABatman::ABatman()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	springArm = CreateDefaultSubobject<USpringArmComponent>("Springarm");
	camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	camera->SetupAttachment(springArm);
	springArm->SetupAttachment(RootComponent);

}
void ABatman::BeginPlay()
{
	Super::BeginPlay();
	playerController = GetWorld()->GetFirstPlayerController();
	InitInput();
	
}

// Called every frame
void ABatman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABatman::Move(const FInputActionValue& _value)
{
	if (!inputToMove)return;
	float _delta = GetWorld()->DeltaTimeSeconds;
	const FVector _fwd = GetActorForwardVector();
	const FVector2D _movementValue = _value.Get<FVector2D>(); //* _delta * moveSpeed
	const float _forwardMovementValue = _movementValue.X * _delta * moveSpeed;
	//forwardInputValue = _movementValue.X;
	AddMovementInput(_fwd, _forwardMovementValue);

	const FVector _rght = GetActorRightVector();
	//rightInputValue = _movementValue.Y;
	const float _rightMovementValue = _movementValue.Y * _delta * moveSpeed;
	AddMovementInput(_rght, _rightMovementValue);
}

void ABatman::RotateYaw(const FInputActionValue& _value)
{
	const float _delta = GetWorld()->DeltaTimeSeconds;
	const float _rotateValue = _value.Get<float>() * _delta * rotateSpeed;
	//rotateInputValue = _rotateValue;
	AddControllerYawInput(_rotateValue);
}

void ABatman::RotatePitch(const FInputActionValue& _value)
{
	//float _delta = GetWorld()->DeltaTimeSeconds;
	//float _rotateValue = _value.Get<float>() * _delta * rotateSpeed;
	////rotateInputValue = _rotateValue;
	////UE_LOG(LogTemp, Warning, TEXT("Pitchused"));
	//AddControllerPitchInput(-_rotateValue);

	float _delta = GetWorld()->DeltaTimeSeconds;
	const float _rotateValue = _value.Get<float>() * _delta * rotateSpeed;
	//rotateInputValue = _rotateValue;
	FRotator _currentRotation = springArm->GetComponentRotation();
	float _newPitch = FMath::Clamp(_currentRotation.Pitch + _rotateValue, minPitchRotation, maxPitchRotation);

	FRotator _newArmRotation = FRotator(_newPitch, _currentRotation.Yaw, _currentRotation.Roll);
	springArm->SetWorldRotation(_newArmRotation);


}

void ABatman::Launch(const FInputActionValue& _value)
{	
	float floatValue = _value.Get<float>();  // Extract the float value from _value
	UE_LOG(LogTemp, Warning, TEXT("launchvalue, %f"), floatValue);

	if (floatValue >= 0.1)
		canLaunch = true;
	if (floatValue <= 0.1)
		canLaunch = false;
	//Transformed to Unpossess
	/*UWorld* _myWorld = GetWorld();
	UE_LOG(LogTemp, Warning, TEXT("launching Batarang"));
	spawnLocation = GetActorLocation() + (GetActorForwardVector() + FVector(forwardDistance, 0,upDistance)) ;
	batarangToPossess = _myWorld->SpawnActor<ABatarang>(spawnedBatarang, spawnLocation, GetActorRotation());
	if (!_myWorld)return;
	try
	{
		GetWorld()->GetTimerManager().SetTimer(cameraTransitionTimer, this, &ABatman::Possess, 0.5, false, -1);
	}
	catch (const std::exception& e)
	{
		UE_LOG(LogTemp, Error, TEXT("Exception: %s"), UTF8_TO_TCHAR(e.what()));
	}*/
}

void ABatman::SpawnBatarang()
{
	UWorld* _myWorld = GetWorld();
	spawnLocation = GetMesh()->GetSocketLocation("LaunchSocket");
	
		//GetActorLocation() + (GetActorForwardVector() + FVector(forwardDistance, 0, upDistance));
	batarangToPossess = _myWorld->SpawnActor<ABatarang>(spawnedBatarang, spawnLocation, GetActorRotation());
	batarangToPossess->SetOwner(this);
}



void ABatman::SetViewTarget()
{
	playerController->SetViewTargetWithBlend(batarangToPossess);
	UE_LOG(LogTemp, Warning, TEXT("blend done"));
	//GetWorld()->GetTimerManager().SetTimer(possessionTimer, this, &ABatman::Possess, 1, false);
}

void ABatman::Possess()
{
	//GetInstigatorController()->UnPossess(); //depossess actual pawn
	if (!playerController)return;
	playerController->Possess(this);
	UE_LOG(LogTemp, Warning, TEXT("possession done"));
	//SetViewTarget();


}


void ABatman::InitInput()
{
	ULocalPlayer* _myPlayer = GetWorld()->GetFirstPlayerController()->GetLocalPlayer();
	if (!_myPlayer)return;
	UEnhancedInputLocalPlayerSubsystem* _inputSystem = _myPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!_inputSystem)return;
	_inputSystem->AddMappingContext(mappingContext, 0);
}

// Called when the game starts or when spawned


// Called to bind functionality to input
void ABatman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* _myInputCompo = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!_myInputCompo)
	{
	}
	_myInputCompo->BindAction(inputToMove, ETriggerEvent::Triggered, this, &ABatman::Move);
	_myInputCompo->BindAction(inputToMove, ETriggerEvent::Completed, this, &ABatman::Move);

	_myInputCompo->BindAction(inputToRotateYaw, ETriggerEvent::Triggered, this, &ABatman::RotateYaw);
	_myInputCompo->BindAction(inputToRotateYaw, ETriggerEvent::Completed, this, &ABatman::RotateYaw);
	_myInputCompo->BindAction(inputToRotatePitch, ETriggerEvent::Triggered, this, &ABatman::RotatePitch);
	_myInputCompo->BindAction(inputToRotatePitch, ETriggerEvent::Completed, this, &ABatman::RotatePitch);
	
	_myInputCompo->BindAction(inputToLaunch, ETriggerEvent::Triggered, this, &ABatman::Launch);
	_myInputCompo->BindAction(inputToLaunch, ETriggerEvent::Completed, this, &ABatman::Launch);


}

