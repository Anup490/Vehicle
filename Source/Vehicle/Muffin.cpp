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

	Vehicle = 0;
}

void AMuffin::ForgetVehicle()
{
	Vehicle = 0;
}

// Called when the game starts or when spawned
void AMuffin::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMuffin::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMuffin::OnOverlapEnd);
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
	PlayerInputComponent->BindAxis("LookRight", this, &AMuffin::LookRight);
}

void AMuffin::MoveForward(float fVal)
{
	AController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		const FRotator Rotation = PlayerController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, fVal);
	}
}

void AMuffin::MoveRight(float fVal)
{
	AController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		const FRotator Rotation = PlayerController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, fVal);
	}
}

void AMuffin::EnterVehicle()
{
	if (Vehicle)
	{
		FaceTowardsVehicle();
		AttachToVehicle();
		Controller->Possess(Vehicle);
		FaceForward();
		Vehicle->SetPassenger(this);
	}
}

void AMuffin::LookRight(float fVal)
{
	AddControllerYawInput(fVal * 45.f * GetWorld()->GetDeltaSeconds());
}

void AMuffin::OnOverlapBegin
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
		Vehicle = Car;
	}
}

void AMuffin::OnOverlapEnd
(
	class UPrimitiveComponent* OverlappedCamp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	if (Cast<AVehiclePawn>(OtherActor))
	{
		Vehicle = 0;
	}
}

void AMuffin::FaceTowardsVehicle()
{
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Vehicle->GetActorLocation());
	Controller->SetControlRotation(Rotator);
	SetActorRotation(FRotator(0.f, Rotator.Yaw, 0.f));
}

void AMuffin::AttachToVehicle()
{
	AddActorLocalOffset(FVector(200.f, 0.f, 100.f));
	FAttachmentTransformRules AttachmentRule(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
	AttachToActor(Vehicle, AttachmentRule);
}

void AMuffin::FaceForward()
{
	float fDotProduct = FVector::DotProduct(GetActorRightVector(), Vehicle->GetActorForwardVector());
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += (fDotProduct >= 0.9) ? 90.f : -90.f;
	SetActorRotation(FRotator(0, Rotation.Yaw, 0));
}