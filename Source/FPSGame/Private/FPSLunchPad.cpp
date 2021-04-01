// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSLunchPad::AFPSLunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Block);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	OverlapComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(75.0f,75.0f,50));
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLunchPad::HadleOverlap);

	RootComponent = OverlapComp;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(OverlapComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(75.0f, 75.0f, 200.0f);
	DecalComp->SetupAttachment(MeshComp);

	LaunchStrength = 1500.0f;
	LaunchPitchAngle = 35.0f;

}

void AFPSLunchPad::HadleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);

	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;


	if (MyPawn) {

		const FVector AddForce = MyPawn->GetActorRotation().Vector() * 1000 + FVector(0, 0, 1) * 1000;
		MyPawn->LaunchCharacter(LaunchVelocity,true,true);
		PlayEffects();
	}

	else {
		if (OtherComp && OtherComp->IsSimulatingPhysics()) {
			OtherComp->AddImpulse(LaunchVelocity,NAME_None,true);
			PlayEffects();

		}
	}

}

void AFPSLunchPad::PlayEffects() {
	UGameplayStatics::SpawnEmitterAtLocation(this, JumpVFX, GetActorLocation());
}
