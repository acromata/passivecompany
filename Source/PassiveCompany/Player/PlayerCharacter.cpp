#include "../Player/PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Camera
    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(RootComponent);
    Camera->bUsePawnControlRotation = true;

    // Item
    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
    ItemMesh->SetupAttachment(Camera);

    // Stanima
    bHasStanima = true;
    CurrentStanima = 100.f;
    MaxStanima = CurrentStanima;
    IncreaseStanimaRate = 1.f;
    StanimaToSubtractWhileWalking = 1.f;
    StanimaToSubtractWhileJumping = 5.f;
    DelayToIncreaseStanima = 20.f;

    // Sprint
    SprintSpeed = 1000.f;
    WalkSpeed = 800.f;
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsSprinting)
    {
        SubtractStanima(StanimaToSubtractWhileWalking);
    }
    else
    {
        IncreaseStanima();
    }
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Add input mapping context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMapping, 0);
        }
    }

    // Input actions
    if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
        Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);

        Input->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartSprint);
        Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndSprint);
        
        Input->BindAction(PickupAction, ETriggerEvent::Completed, this, &APlayerCharacter::PickupItem);
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (IsValid(Controller))
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // get right vector
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }

}

void APlayerCharacter::Jump()
{
    if (CurrentStanima >= 0)
    {
        ACharacter::Jump();
        SubtractStanima(StanimaToSubtractWhileJumping);
        CurrentStanimaDelay = DelayToIncreaseStanima;
    }
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (IsValid(Controller))
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void APlayerCharacter::StartSprint()
{
    if (CurrentStanima >= 0)
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
    else
    {
        EndSprint();
    }
}

void APlayerCharacter::EndSprint()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::SubtractStanima(float StanimaToSubtract)
{
    CurrentStanima -= StanimaToSubtract;
    CurrentStanimaDelay = DelayToIncreaseStanima;

    if (CurrentStanima <= 0)
    {
        CurrentStanima = 0;
        bHasStanima = false;
        EndSprint();
    }
}

void APlayerCharacter::IncreaseStanima()
{
    if (CurrentStanima <= MaxStanima && CurrentStanimaDelay <= 0)
    {
        CurrentStanima += IncreaseStanimaRate;
    }
    else
    {
        CurrentStanimaDelay--;
    }
}

void APlayerCharacter::PickupItem()
{
    if (IsValid(ItemInRange) && !bIsCarryingTwoHandedItem)
    {
        ItemsInHotbar.Add(ItemInRange->ItemData);

        if (ItemInRange->ItemData->bTwoHanded == true)
        {
            bIsCarryingTwoHandedItem = true;
        }

        ItemInRange->Destroy();
        if (IsValid(PickupSound))
        {
            UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
        }
    }
}

void APlayerCharacter::Damage(int32 Health)
{
    CurrentHealth -= Health;

    if (CurrentHealth <= 0)
    {
        Die();
    }
}