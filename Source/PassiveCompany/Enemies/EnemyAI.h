#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "EnemyAI.generated.h"

UENUM(BlueprintType)
enum class EEnemyState
{
	EnemyIdle,
	EnemyChase,
	EnemyAttack
};

UCLASS()
class PASSIVECOMPANY_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPawnSensingComponent* PawnSensing;

public:
	// Sets default values for this character's properties
	AEnemyAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// Pawn Sensing
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);
	bool bCanSeePlayer;

	APawn* Target;

	// States
	EEnemyState EnemyState;

	void Idle();
	void Chase();
	void Attack();

	// Attacking
	UPROPERTY(EditAnywhere, Category = "Attacking")
	float AttackingRange;
	UPROPERTY(EditAnywhere, Category = "Attacking")
	float StopAttackingRange;
	UPROPERTY(EditAnywhere, Category = "Attacking")
	float AttackRadius;
	UPROPERTY(EditAnywhere, Category = "Attacking")
	int32 Damage;
	UPROPERTY(EditAnywhere, Category = "Attacking")
	float TimeBetweenAttacks;
	UPROPERTY(EditAnywhere, Category = "Attacking")
	USoundBase* ChaseSound;
	UPROPERTY(EditAnywhere, Category = "Attacking")
	USoundBase* AttackSound;
	UPROPERTY(EditAnywhere, Category = "Attacking")
	USoundAttenuation* AttenuationSettings;

	float TimeSinceLastAttack;

	// Spawning
	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 ChanceToSpawn;


};
