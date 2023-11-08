// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Batarang.generated.h"

class ABatman;
class USpringArmComponent;
class UCameraComponent;
class UPointLightComponent;

UCLASS()
class CPPBATARANG_API ABatarang : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY()
		float deltaSeconds;

#pragma region Appearance
		UPROPERTY(EditAnywhere,Category = "Appearance")
		TObjectPtr<USpringArmComponent> springArm;
		UPROPERTY(EditAnywhere, Category = "Appearance")
		TObjectPtr<UStaticMeshComponent> meshCompo;
		UPROPERTY(EditAnywhere, Category = "Appearance")
		TObjectPtr<UCameraComponent> camera;
		UPROPERTY(EditAnywhere, Category = "Appearance")
		TObjectPtr<UPointLightComponent> pointLightFirst;
		UPROPERTY(EditAnywhere, Category = "Appearance")
		TObjectPtr<UPointLightComponent> pointLightSecond;
		UPROPERTY(VisibleAnywhere)
		TArray<UPointLightComponent*> allLights;
		UPROPERTY(VisibleAnywhere)
		TObjectPtr<UCharacterMovementComponent> characterMoveCompo;
		UPROPERTY(EditAnywhere)
		TObjectPtr<APlayerController> playerController;
public:		
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
		FLinearColor Blue = FLinearColor::Blue;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
		FLinearColor Green = FLinearColor::Green;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
		FLinearColor Red = FLinearColor::Red;
#pragma endregion Appearance

#pragma region Rotation

private:

		UPROPERTY(EditAnywhere, Category = "Rotation")
		float rotateSpeed = 100.f;
		UPROPERTY(EditAnywhere, Category = "Rotation")
		float selfRotateSpeed = 200.f;
		UPROPERTY(EditAnywhere, Category = "Rotation")
		float minTiltAngle = -15.f;
		UPROPERTY(EditAnywhere, Category = "Rotation")
		float maxTiltAngle = 15.f;
		UPROPERTY(EditAnywhere, Category = "Rotation")
		float minCameraTiltAngle = -20.f;
		UPROPERTY(EditAnywhere, Category = "Rotation")
		float maxCameraTiltAngle = 20.f;


	public:

		UPROPERTY(EditAnywhere, Category = "Rotation")
		float initialSelfRotateSpeed = 200;
		UPROPERTY(EditAnywhere, Category = "Rotation")
		float maxRotateSpeedAllowed = 2000;
		UPROPERTY(EditAnywhere, Category = "Rotation")
		float minRotateSpeedAllowed = 200;
		UPROPERTY(EditAnywhere, Category = "Rotation")
		float rotationIncreaseRate = 2;
#pragma endregion Rotation

#pragma region Movement

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float initialMoveSpeed = 200;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float maxSpeedAllowed = 5000;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float minSpeedAllowed = 100;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float accelerationRate = 20;
		UPROPERTY(EditAnywhere, Category = "Movement")
		float decelerationRate = 2;
		UPROPERTY(EditAnywhere, Category = "Movement")
		float autoDecelerationRate = 200;

		UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Movement")
		float moveSpeed = 2000.f;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool isReturningToInitialSpeed = false;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool isReturningToInitialRotationSpeed = false;

#pragma endregion Movement

	
#pragma region Input

		UPROPERTY(EditAnywhere)
		TObjectPtr<UInputAction> inputToMove;
		UPROPERTY(EditAnywhere)
		TObjectPtr<UInputAction> inputToFly;
		UPROPERTY(EditAnywhere)
		TObjectPtr<UInputAction> inputToRotateYaw;
		UPROPERTY(EditAnywhere)
		TObjectPtr<UInputAction> inputToRotatePitch;
		UPROPERTY(EditAnywhere)
		TObjectPtr<UInputAction> inputToAction;
		UPROPERTY(EditAnywhere)
		TObjectPtr<UInputMappingContext> mappingContext = nullptr;
		UPROPERTY(EditAnywhere)
		TObjectPtr<UInputAction> inputToStopPossess;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float forwardInputValue;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float rightInputValue;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float rotateInputValue;

#pragma endregion Input


public:
	ABatarang();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void InitInput();
	void Init();

	float GetMoveSpeed() { return moveSpeed; }
	float GetRotateSpeed() { return selfRotateSpeed; }
	void SetInitialSpeed(const float& _speed) { initialMoveSpeed = _speed; }
	void SetInitialSelfRotateSpeed(const float& _rotateSpeed) { initialSelfRotateSpeed = _rotateSpeed; }

	void SpeedUp(const FInputActionValue& _value);
	void ReturnToInitialSpeed();
	void ReturnToInitialSelfRotateSpeed();

	void Rotate(const FInputActionValue& _value);
	void RotateYaw(const FInputActionValue& _value);
	void RotatePitch(const FInputActionValue& _value);
	void ResetRotationAngle();

	void Fly();

	void AutoMove();
	void SelfRotate();
	void AddTilt(const float& _inputValue);
	void AddCameraTilt();

	void UpdateLightColor(const FLinearColor& _color);
	//void Possess(ABatman* _batman);
	UFUNCTION(BlueprintCallable)
	void StopPossess();
	UFUNCTION(BlueprintCallable)
	void Possess();
};
