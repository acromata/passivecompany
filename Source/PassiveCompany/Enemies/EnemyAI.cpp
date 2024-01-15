#include "../Enemies/EnemyAI.h"
#include "../Player/PlayerCharacter.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Pawn sensing
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing");
	PawnSensing->SetPeripheralVisionAngle(70.f);

	// Attacking
	AttackingRange = 250.f;
	AttackRadius = 500.f;
	TimeBetweenAttacks = 30.f;

	StopAttackingRange = 1500.f;
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	
	// Pawn Sensing Bindings
	PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyAI::OnSeePawn);

	EnemyState = EEnemyState::EnemyIdle;

	// Chance of spawning
	if (FMath::RandRange(1, ChanceToSpawn) != 1)
	{
		Destroy();
	}
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// State machine
	switch (EnemyState)
	{
	case EEnemyState::EnemyIdle:
		Idle();
		break;
	case EEnemyState::EnemyChase:
		Chase();
		break;
	case EEnemyState::EnemyAttack:
		Attack();
		break;
	}

	// Attack delay
	TimeSinceLastAttack--;
}

void AEnemyAI::OnSeePawn(APawn* Pawn)
{
	Target = Cast<APlayerCharacter>(Pawn);
	if (IsValid(Target))
	{
		EnemyState = EEnemyState::EnemyChase;
	}
}

void AEnemyAI::Idle()
{
	// stay still lmao u r fuk level
}

void AEnemyAI::Chase()
{
	// Get AI controller
	AAIController* AIController = Cast<AAIController>(Controller);

	// Sound
	UAudioComponent* ChaseAudio = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ChaseSound, GetActorLocation(), GetActorRotation(), 0.f);
	FVector SoundLocation = GetActorLocation();
	bool bPlayedSound = false;
	if (!bPlayedSound)
	{
		ChaseAudio = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ChaseSound, SoundLocation,
			GetActorRotation(), 1.f, 1.f, 0.f, AttenuationSettings);
	}

	// Get the distance from the target
	float DistanceFromTarget = GetDistanceTo(Target);
	if (DistanceFromTarget <= AttackingRange && TimeSinceLastAttack <= 0)
	{
		// Attack
		EnemyState = EEnemyState::EnemyAttack;
	}
	else if (DistanceFromTarget >= StopAttackingRange)
	{
		// Idle
		EnemyState = EEnemyState::EnemyIdle;
		bPlayedSound = false;

		if (IsValid(ChaseAudio))
		{
			ChaseAudio->Stop();
		}
	}
	else if (IsValid(AIController) && !AIController->IsFollowingAPath())
	{
		AIController->MoveToActor(Target);
	}
}

void AEnemyAI::Attack()
{
	// Get the distance from the target
	float DistanceFromTarget = GetDistanceTo(Target);
	if (DistanceFromTarget <= AttackingRange && TimeSinceLastAttack <= 0)
	{
		// Attacking
		FHitResult HitResult;

		// Create a collision sphere
		FCollisionShape SphereShape = FCollisionShape::MakeSphere(AttackRadius);

		bool bHit = GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), GetActorLocation(),
			FQuat::Identity, ECC_Visibility, SphereShape);
		TimeSinceLastAttack = TimeBetweenAttacks;

		if (bHit)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(HitResult.GetActor());

			if (IsValid(Player))
			{
				Player->Damage(Damage);

				UGameplayStatics::PlaySound2D(GetWorld(), AttackSound);
			}
		}

		// Draw collision sphere
		//DrawDebugSphere(GetWorld(), GetActorLocation(), SphereShape.GetSphereRadius(), 50, FColor::Red, false, 5.f);
	}
	else
	{
		EnemyState = EEnemyState::EnemyChase;
	}
}

