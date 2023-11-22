// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Batman.generated.h"

class ABatarang;
UCLASS()
class CPPBATARANG_API ABatman : public ACharacter
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> springArm;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> camera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ABatarang> batarangToPossess;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABatarang> spawnedBatarang;

	UPROPERTY(EditAnywhere)
	TObjectPtr<APlayerController> playerController;

	UPROPERTY(EditAnywhere)
	FTimerHandle cameraTransitionTimer;
	UPROPERTY(EditAnywhere)
	FTimerHandle possessionTimer;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> inputToMove;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> inputToRotateYaw;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> inputToRotatePitch;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> mappingContext = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> inputToLaunch;
	
	
	
	UPROPERTY(EditAnywhere)
	FVector spawnLocation;

	UPROPERTY(EditAnywhere)
	float forwardDistance = 0;
	UPROPERTY(EditAnywhere)
	float upDistance = 0;
	UPROPERTY(EditAnywhere)
	float rotateSpeed = 20;
	UPROPERTY(EditAnywhere)
	float moveSpeed = 500;

	UPROPERTY(EditAnywhere)
	float minPitchRotation = -65;
	UPROPERTY(EditAnywhere)
	float maxPitchRotation = 40;


	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool canLaunch = false;


public:
	// Sets default values for this character's properties
	ABatman();

	void Move(const FInputActionValue& _value);
	void RotateYaw(const FInputActionValue& _value);
	void RotatePitch(const FInputActionValue& _value);
	UFUNCTION(BlueprintCallable)
	void Launch(const FInputActionValue& _value);
	UFUNCTION(BlueprintCallable)
	void SpawnBatarang();
	

	void Possess();
	void SetViewTarget();
	void InitInput();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
