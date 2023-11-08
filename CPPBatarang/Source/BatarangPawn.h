// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BatarangPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CPPBATARANG_API ABatarangPawn : public APawn
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> cameraAttachPoint;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> springArm;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> camera;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> meshCompo;


	UPROPERTY(EditAnywhere)
	float deltaSeconds;
	UPROPERTY(EditAnywhere)
	float moveSpeed = 200.f;
	UPROPERTY(EditAnywhere)
	float rotateSpeed = 200.f;



public:
	// Sets default values for this pawn's properties
	ABatarangPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AutoMove();
	//void BatarangMove();
	void SelfRotate();
	void InverseRotationForCamera();
};
