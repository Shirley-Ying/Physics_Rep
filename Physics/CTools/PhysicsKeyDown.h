// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsKeyDown.generated.h"

UCLASS()
class PHYSICS_API APhysicsKeyDown : public AActor
{
	GENERATED_BODY()
	
public:	
	APhysicsKeyDown();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void StartPush();
	void ResetTimer();

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	AActor* PushActor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	float PushDuration = 0.5f;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	float PushForceSize = 10000.f;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	FVector ForceDirection = FVector(1,0,0);

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* TransComponent = nullptr;

private:
	bool bPushed = false;
	
	FTimerHandle PushHandle;
	
};
