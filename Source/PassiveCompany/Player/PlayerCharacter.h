#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "../Misc/ItemsDataAsset.h"
#include "../Items/ItemActor.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PASSIVECOMPANY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent* ItemMesh;

protected:

    // Input context
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    class UInputMappingContext* InputMapping;

    // Input actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    class UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    class UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    class UInputAction* SprintAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    class UInputAction* PickupAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    class UInputAction* ScrollUp;


public:
    // Sets default values for this character's properties
    APlayerCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

    void Move(const FInputActionValue& Value);
    void Jump();
    void Look(const FInputActionValue& Value);

    // Sprint System
    void StartSprint();
    void EndSprint();

    UPROPERTY(EditAnywhere, Category = "Sprint")
    float WalkSpeed;
    UPROPERTY(EditAnywhere, Category = "Sprint")
    float SprintSpeed;
    UPROPERTY(EditAnywhere, Category = "Sprint")
    bool bIsSprinting;

    // Stanima System
    void SubtractStanima(float StanimaToSubtract);
    void IncreaseStanima();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stanima")
    float MaxStanima;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stanima")
    float CurrentStanima;
    UPROPERTY(EditAnywhere, Category = "Stanima")
    float StanimaToSubtractWhileWalking;
    UPROPERTY(EditAnywhere, Category = "Stanima")
    float StanimaToSubtractWhileJumping;
    UPROPERTY(EditAnywhere, Category = "Stanima")
    float IncreaseStanimaRate;
    UPROPERTY(EditAnywhere, Category = "Stanima")
    float DelayToIncreaseStanima;

    float CurrentStanimaDelay;

    bool bHasStanima;

    // Inventory
    void PickupItem();

    UPROPERTY(BlueprintReadWrite)
    TArray<UItemsDataAsset*> ItemsInHotbar;
    UPROPERTY(BlueprintReadWrite)
    bool bIsCarryingTwoHandedItem;

    UPROPERTY(BlueprintReadWrite)
    int32 ItemNumEquipped;

    // Health
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentHealth;

    UFUNCTION(BlueprintImplementableEvent)
    void Die();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
    USoundBase* PickupSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
    USoundBase* DropSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
    USoundBase* FootstepSound;

public:

    AItemActor* ItemInRange;

    UFUNCTION(BlueprintCallable)
    void Damage(int32 Health);
};
