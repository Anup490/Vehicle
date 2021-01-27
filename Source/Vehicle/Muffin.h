// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Muffin.generated.h"

UCLASS(config = Game)
class AMuffin : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	class AVehiclePawn* Vehicle;
public:
	// Sets default values for this character's properties
	AMuffin();

	void ForgetVehicle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlapBegin
	(
		class UPrimitiveComponent* OverlappedCamp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	void FaceOtherActorDirection(AActor* OtherActor);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float fVal);

	void MoveRight(float fVal);

	void EnterVehicle();

	void LookRight(float fVal);
};
