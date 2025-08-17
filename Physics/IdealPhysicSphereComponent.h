
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IdealPhysicSphereComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PHYSICS_API UIdealPhysicSphereComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UIdealPhysicSphereComponent();

protected:

	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// 起步速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartVelocity = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRandomStart = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector Velocity = FVector::ZeroVector;


	bool TryMoveOneFrame(float DeltaTime, AActor* Owner, bool bSweep = true);
	
	void OnCollision();

	FHitResult HitResult;




	// bool IsCollisionDone = false;
};
