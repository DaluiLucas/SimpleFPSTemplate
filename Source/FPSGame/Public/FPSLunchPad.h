// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLunchPad.generated.h"

class UBoxComponent;
class UParticleSystem;
UCLASS()
class FPSGAME_API AFPSLunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLunchPad();

protected:

	void PlayEffects();

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComp; 

	UPROPERTY(VisibleAnywhere,Category="Component")
	UBoxComponent* OverlapComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UDecalComponent* DecalComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* JumpVFX;

	UFUNCTION()
	void HadleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
