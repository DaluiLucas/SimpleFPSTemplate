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

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetupAttachment(MeshComp);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Block);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	OverlapComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.0f,200.0f,20),true);
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLunchPad::HadleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f, 200.0f, 200.0f);
	DecalComp->SetupAttachment(MeshComp);

}

void AFPSLunchPad::HadleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapwith Jump Pad. "));
	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn) {
		UE_LOG(LogTemp, Log, TEXT("Pawn "));
		const FVector AddForce = MyPawn->GetActorRotation().Vector() * 1000 + FVector(0, 0, 1) * 1000;
		MyPawn->LaunchCharacter(AddForce,false,true);
		PlayEffects();
	}

	else {
		if (OtherComp && OtherComp->IsSimulatingPhysics()) {
			const FVector AddForce =FVector(0, 0, 20000);
			OtherComp->AddRadialImpulse(GetActorLocation(),1000.0f,-20000.f,ERadialImpulseFalloff::RIF_MAX,true);
			PlayEffects();
			UE_LOG(LogTemp, Log, TEXT("UPrimitiveComponent "));
		}
	}



}

void AFPSLunchPad::PlayEffects() {
	UGameplayStatics::SpawnEmitterAtLocation(this, JumpVFX, GetActorLocation());
}
