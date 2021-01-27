// Fill out your copyright notice in the Description page of Project Settings.
#include "Muffin.h"
#include "VehiclePawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMuffin::AMuffin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MuffinMesh(TEXT("/Game/Vehicle/Sedan/SK_Muffin.SK_Muffin"));
	GetMesh()->SetSkeletalMesh(MuffinMesh.Object);
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->AddLocalOffset(FVector(0.f, 0.f, -80.f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(90.f);
	GetCapsuleComponent()->SetCapsuleRadius(90.f);
	GetCapsuleComponent()->AddLocalOffset(FVector(0.f, 0.f, -15.f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 150.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AMuffin::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMuffin::OnOverlap);
}

// Called every frame
void AMuffin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMuffin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMuffin::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMuffin::MoveRight);
	PlayerInputComponent->BindAction("EnterExitVehicle", IE_Pressed, this, &AMuffin::EnterVehicle);
}

void AMuffin::MoveForward(float fVal)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, fVal);
}

void AMuffin::MoveRight(float fVal)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, fVal);
}

void AMuffin::EnterVehicle()
{
	
}

void AMuffin::OnOverlap
(
	class UPrimitiveComponent* OverlappedCamp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	AVehiclePawn* Car = Cast<AVehiclePawn>(OtherActor);
	if (Car)
	{
		AddActorLocalOffset(FVector(230.f, 0.f, 100.f));
		AController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		FAttachmentTransformRules AttachmentRule(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
		AttachToActor(OtherActor, AttachmentRule);
		PlayerController->Possess(Car);
		FaceOtherActorDirection(Car);
		Car->SetPassenger(this);
	}
}

void AMuffin::FaceOtherActorDirection(AActor* OtherActor)
{
	float fDotProduct = FVector::DotProduct(GetActorRightVector(), OtherActor->GetActorForwardVector());
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += (fDotProduct > 0) ? 90.f : -90.f;
	SetActorRotation(FRotator(0, Rotation.Yaw, 0));
}