// Coded by TVL

#include "Batarang.h"
#include "Batman.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Math/UnrealMathUtility.h"
#include "Components/PointLightComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ABatarang::ABatarang()
{
	PrimaryActorTick.bCanEverTick = true;
	springArm = CreateDefaultSubobject<USpringArmComponent>("springarm");
	pointLightFirst = CreateDefaultSubobject<UPointLightComponent>("light1");
	pointLightSecond = CreateDefaultSubobject<UPointLightComponent>("light2");
	camera = CreateDefaultSubobject<UCameraComponent>("camera");
	meshCompo = CreateDefaultSubobject<UStaticMeshComponent>("mesh");
	meshCompo->SetupAttachment(Mesh);
	camera->SetupAttachment(springArm);
	springArm->SetupAttachment(RootComponent);

	
	pointLightFirst->SetupAttachment(meshCompo);
	pointLightSecond->SetupAttachment(meshCompo);

}

void ABatarang::BeginPlay()
{
	Super::BeginPlay();
	characterMoveCompo = GetCharacterMovement();
	playerController = GetWorld()->GetFirstPlayerController();
	InitBatarang();
	Init();
	InitInput();
	GetComponents<UPointLightComponent>(allLights);
	UpdateLightColor(Blue);
}

void ABatarang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	deltaSeconds = GetWorld()->DeltaTimeSeconds;
	AutoMove();
	SelfRotate();
	ReturnToInitialSpeed();
	ReturnToInitialSelfRotateSpeed();

}

void ABatarang::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* _myInputCompo = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	_myInputCompo->BindAction(inputToMove, ETriggerEvent::Triggered, this, &ABatarang::SpeedUp);
	_myInputCompo->BindAction(inputToMove, ETriggerEvent::Completed, this, &ABatarang::SpeedUp);

	_myInputCompo->BindAction(inputToRotateYaw, ETriggerEvent::Triggered, this, &ABatarang::RotateYaw);
	_myInputCompo->BindAction(inputToRotateYaw, ETriggerEvent::Completed, this, &ABatarang::RotateYaw);

	_myInputCompo->BindAction(inputToMove, ETriggerEvent::Completed, this, &ABatarang::ResetRotationAngle);

	_myInputCompo->BindAction(inputToRotatePitch, ETriggerEvent::Triggered, this, &ABatarang::RotatePitch);
	_myInputCompo->BindAction(inputToRotatePitch, ETriggerEvent::Completed, this, &ABatarang::RotatePitch);

	_myInputCompo->BindAction(inputToFly, ETriggerEvent::Triggered, this, &ABatarang::Fly);

	_myInputCompo->BindAction(inputToStopPossess, ETriggerEvent::Triggered, this, &ABatarang::StopPossess);
	_myInputCompo->BindAction(inputToStopPossess, ETriggerEvent::Completed, this, &ABatarang::StopPossess);


}

void ABatarang::InitInput()
{
	ULocalPlayer* _myPlayer = GetWorld()->GetFirstPlayerController()->GetLocalPlayer();
	if (!_myPlayer)return;
	UEnhancedInputLocalPlayerSubsystem* _inputSystem = _myPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!_inputSystem)return;
	_inputSystem->AddMappingContext(mappingContext, 0);
}

void ABatarang::Init()
{
	GetMoveSpeed();
	SetInitialSpeed(moveSpeed);

	GetRotateSpeed();
	SetInitialSelfRotateSpeed(selfRotateSpeed);
}

void ABatarang::SpeedUp(const FInputActionValue& _value)
{
	if (!inputToMove)return;
	float _delta = GetWorld()->DeltaTimeSeconds;
	const FVector _fwd = GetActorForwardVector();
	const FVector2D _movementValue = _value.Get<FVector2D>(); //* _delta * moveSpeed
	const float _forwardMovementValue = _movementValue.X * _delta * moveSpeed;
	forwardInputValue = _movementValue.X;
	//AddMovementInput(_fwd, _forwardMovementValue);

	const FVector _rght = GetActorRightVector();
	rightInputValue = _movementValue.Y;
	const float _rightMovementValue = _movementValue.Y * _delta * moveSpeed;
	AddMovementInput(_rght, _rightMovementValue);
	if (_movementValue.X > 0.1f)  // Check steps for auto decelerate and accelerate
	{
		if(moveSpeed <= maxSpeedAllowed)
			moveSpeed +=  accelerationRate;

		if (selfRotateSpeed < maxRotateSpeedAllowed)
			selfRotateSpeed += rotationIncreaseRate;

		UpdateLightColor(Green);
	}
	else
	{
		if (_movementValue.X < -0.1f)
		{

			if(moveSpeed > minSpeedAllowed)
			moveSpeed -=  decelerationRate;

			if (selfRotateSpeed > minRotateSpeedAllowed)
				selfRotateSpeed -= rotationIncreaseRate;

			UpdateLightColor(Red);
		
		}
		else 
		{
			UpdateLightColor(Blue);
			isReturningToInitialSpeed = true;
			isReturningToInitialRotationSpeed = true;
		}
	}
	if (FMath::Abs(rightInputValue) > 0.1f)
	{
		AddTilt(rightInputValue);
	}
	
}

void ABatarang::ReturnToInitialSpeed()
{
		float _toOriginalSpeed = FMath::FInterpTo((moveSpeed), initialMoveSpeed, GetWorld()->DeltaTimeSeconds , 1); //testing FInterpTo method
		moveSpeed = _toOriginalSpeed;

	if (moveSpeed <= initialMoveSpeed + 10 && moveSpeed >= initialMoveSpeed - 10) 
		isReturningToInitialSpeed = false; // not used
	// more some kind of a safety check because the first check will 
	// always prevent the tick from happening when it reaches the target speed
	// use later for blinking light when returning to initial speed
		
}

void ABatarang::ReturnToInitialSelfRotateSpeed()
{
		float _toOriginalRotation = FMath::FInterpTo((selfRotateSpeed), initialSelfRotateSpeed, GetWorld()->DeltaTimeSeconds, 1); //testing FInterpTo method
		selfRotateSpeed = _toOriginalRotation;

	if (selfRotateSpeed <= initialSelfRotateSpeed + 10 && selfRotateSpeed >= initialSelfRotateSpeed - 10)
		isReturningToInitialRotationSpeed = false; // not used
}

void ABatarang::RotateYaw(const FInputActionValue& _value)
{
	const float _delta = GetWorld()->DeltaTimeSeconds;
	const float _rotateValue = _value.Get<float>() * _delta * rotateSpeed;
	rotateInputValue = _rotateValue;
	AddControllerYawInput(_rotateValue);

	//AddCameraTilt();

	if (FMath::Abs(rotateInputValue) > 0.1f)
	{
		AddTilt(rotateInputValue);
	}
}

void ABatarang::AddCameraTilt()
{
	//FRotator _newRotation = camera->GetRelativeRotation();
	//_newRotation.Yaw += rotateInputValue * 1; //camera tilt speed
	//// clamp the pitch if needed
	//_newRotation.Yaw = FMath::Clamp(_newRotation.Yaw, minCameraTiltAngle, maxCameraTiltAngle);
	//camera->SetRelativeRotation(_newRotation);
}

void ABatarang::Fly()
{
		float _upwardMovement = 10.0f; // adjust fly up value
		FVector _currentLocation = GetActorLocation();
		FVector _newLocation = _currentLocation + FVector(0.0f, 0.0f, _upwardMovement);
		SetActorLocation(_newLocation);
}

void ABatarang::ResetRotationAngle()
{
	meshCompo->SetRelativeRotation(FRotator(0,0,0)); //used for the input rotation yaw
}


void ABatarang::RotatePitch(const FInputActionValue& _value)
{
	 float _delta = GetWorld()->DeltaTimeSeconds;
	 float _rotateValue = _value.Get<float>() * _delta * rotateSpeed;
	rotateInputValue = _rotateValue;
	AddControllerPitchInput(-_rotateValue);
}

void ABatarang::AutoMove()
{
	FVector _direction = GetActorLocation() + (GetActorForwardVector() * deltaSeconds * (moveSpeed));
	SetActorLocation( _direction);
}

void ABatarang::SelfRotate()
{
	FRotator _currentRotation = meshCompo->GetRelativeRotation();
	FRotator _newRotation = _currentRotation;
	_newRotation.Yaw += selfRotateSpeed * deltaSeconds;
	meshCompo->SetRelativeRotation(_newRotation);
}

void ABatarang::AddTilt(const float& _inputValue)
{
	FRotator _currentRotation = meshCompo->GetRelativeRotation();
	FRotator _newRotation = _currentRotation;
	float _newTilt = _newRotation.Pitch - maxTiltAngle * _inputValue; //rightInputValue;
	_newRotation.Pitch = FMath::Clamp(_newTilt, minTiltAngle, maxTiltAngle);
	meshCompo->SetRelativeRotation(_newRotation);
}


void ABatarang::UpdateLightColor(const FLinearColor& _color)
{
	for (int i = 0; i < allLights.Num(); i++)
	{
		UPointLightComponent* _light = allLights[i];
		_light->SetLightColor(_color);
	}
}

void ABatarang::InitBatarang()
{
	//{
	UWorld* _world = GetWorld();
	//if (!_batman)return;
	if (!_world) return;
	APlayerController* _batarangController = _world->GetFirstPlayerController();
	//_batman->GetInstigatorController()->UnPossess();
	//if (!_batarangController)return;
	//	_batarangController->Possess(this);
	UE_LOG(LogTemp, Warning, TEXT("possession done"));
	FTimerHandle _timerPossess;
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 1);
	GetWorldTimerManager().SetTimer(_timerPossess, this, &ABatarang::TakeControl, 2);
	//
	//
	//	}
}

void ABatarang::StopPossess()
{

	//if (!playerController)return;
	GetInstigatorController()->UnPossess();
	//Possess();

}

//void ABatarang::Possess()
//{
//	if (!playerController)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("issue"));
//	}
//	AActor* _batman = GetOwner();
//	ABatman* _batManOwner = Cast<ABatman>(_batman);
//	if (!_batManOwner)return;
//	_batManOwner->Possess();
//	UE_LOG(LogTemp, Warning, TEXT("possessing"));
//
//}

void ABatarang::TakeControl()
{
	GetWorld()->GetFirstPlayerController()->Possess(this);
}


