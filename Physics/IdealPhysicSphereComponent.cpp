// Fill out your copyright notice in the Description page of Project Settings.


#include "IdealPhysicSphereComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Particles/Velocity/ParticleModuleVelocityOverLifetime.h"


UIdealPhysicSphereComponent::UIdealPhysicSphereComponent() {
	PrimaryComponentTick.bCanEverTick = true;

}


void UIdealPhysicSphereComponent::BeginPlay() {
	Super::BeginPlay();

	if(bRandomStart) {
		StartVelocity = FVector(FMath::FRandRange(-1000.f,1000.f), FMath::FRandRange(-1000.f, 1000.f), 0.0f);
	}
	Velocity = StartVelocity;
}


void UIdealPhysicSphereComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto Owner = GetOwner();
	if(!Owner) {
		return;
	}
	
	if(Velocity.Size() > 1e-2) {
		bool bHit = TryMoveOneFrame(DeltaTime, Owner);
		if(bHit) {
			// 更新一下速度
			OnCollision();
		}
		FVector StartLocation = Owner->GetActorLocation();
		FVector DesireLocation = StartLocation + Velocity * DeltaTime;
		Owner->SetActorLocation(DesireLocation,true);
	}
	
}


bool UIdealPhysicSphereComponent::TryMoveOneFrame(float DeltaTime,AActor* Owner, bool bSweep) {
	if(Velocity.Size() > 1e-2) {
		// 约束不会往上飞
		Velocity.Z = 0;
		FVector StartLocation = Owner->GetActorLocation();
		FVector DesireLocation = StartLocation + Velocity * DeltaTime;
		// 碰撞检查
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(Owner);
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(Owner);
		// UKismetSystemLibrary::LineTraceSingle(Owner,StartLocation,DesireLocation,UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
		// 	IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true);
		
		float SphereRadius = 50.f;
		FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SphereRadius);

		bool bHit = GetWorld()->SweepSingleByChannel(
			HitResult,                // 输出碰撞结果
			StartLocation,              // 起点
			DesireLocation,                // 终点
			FQuat::Identity,    // 球体旋转，Sphere 可以用 Identity
			ECC_WorldDynamic,   // 碰撞通道
			CollisionShape, // 碰撞体积
			CollisionParams
		);
		return bHit;
	}
	return false;
}

void UIdealPhysicSphereComponent::OnCollision() {
	bool CollisionSphere = false;
	auto OtherActor = HitResult.GetActor();
	if(!OtherActor) {
		return;
	}
	
	if(OtherActor->FindComponentByClass(StaticClass())) {
		CollisionSphere = true;
	}

	// 碰撞法线
	FVector ImpactNormal = HitResult.ImpactNormal;
	ImpactNormal.Z = 0;
	ImpactNormal.Normalize();
	if(!CollisionSphere){
		// 这里不考虑 Z

		// 此处简化处理，只要计算 和 Normal 对称且反向的输入向量
		FVector VelocityRN = Velocity * -1;
		// 计算对称的向量
		FVector RVelocity = (2 * FVector::DotProduct(ImpactNormal ,VelocityRN)) * ImpactNormal - VelocityRN;
		Velocity = RVelocity;
	}
	else{
		// 如果撞上了球体
		// 这里会稍稍复杂
		auto OtherIdealSphereComp = Cast<UIdealPhysicSphereComponent>(OtherActor->GetComponentByClass(StaticClass()));
		auto& V2 = OtherIdealSphereComp->Velocity;
		auto& V1 = Velocity;
	
		// 速度取两个球球心的连线
		FVector Location1 = GetOwner()->GetActorLocation();
		FVector Location2 = OtherActor->GetActorLocation();
		FVector RelativeLoc = Location2 - Location1;
		RelativeLoc.Z = 0;
		RelativeLoc.Normalize();
		
		FVector VDiff = V1 - V2;
		float Amplitude = FVector::DotProduct(VDiff, RelativeLoc);
		// 计算出 撞击造成的速度变化值
		FVector PushVelocity = Amplitude * RelativeLoc;
	
		V1 = V1 - PushVelocity;
		V2 = V2 + PushVelocity;
	}
}

