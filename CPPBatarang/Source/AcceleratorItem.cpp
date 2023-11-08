// Fill out your copyright notice in the Description page of Project Settings.


#include "AcceleratorItem.h"

// Sets default values
AAcceleratorItem::AAcceleratorItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAcceleratorItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAcceleratorItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

