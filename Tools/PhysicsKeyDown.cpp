

#include "PhysicsKeyDown.h"

#include "Kismet/GameplayStatics.h"


APhysicsKeyDown::APhysicsKeyDown()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	TransComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(TransComponent);
}


void APhysicsKeyDown::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if(PC && PC->InputComponent) {
		auto InputCmp = PC->InputComponent;
		InputCmp->BindKey(EKeys::One, IE_Pressed, this, &APhysicsKeyDown::StartPush);
	}
}

// Called every frame
void APhysicsKeyDown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsValid(PushActor)){
		if(bPushed) {
			auto MeshCmp  = Cast<UStaticMeshComponent>(PushActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			if(MeshCmp) {
				// 设置直接控制加速度
				// 转为世界方向
				FTransform TargetTransform = PushActor->GetTransform();
				FRotator TargetRot = TargetTransform.TransformRotation(ForceDirection.Rotation().Quaternion()).Rotator();
				FVector ForceDirFix = TargetRot.Vector();
				MeshCmp->AddForce(PushForceSize * ForceDirFix, NAME_None, true);
			}
		}
	}
}

void APhysicsKeyDown::StartPush() {
	if(bPushed == false) {
		bPushed = true;
		GetWorldTimerManager().SetTimer(PushHandle,this,&APhysicsKeyDown::ResetTimer,PushDuration, false);
	}
}

void APhysicsKeyDown::ResetTimer() {
	bPushed = false;
	GetWorldTimerManager().ClearTimer(PushHandle);
}

