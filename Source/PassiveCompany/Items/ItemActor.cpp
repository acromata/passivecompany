#include "../Items/ItemActor.h"
#include "../Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AItemActor::AItemActor()
{

	ItemRange = CreateDefaultSubobject<USphereComponent>("ItemRange");
	ItemRange->SetupAttachment(RootComponent);
	ItemRange->SetCollisionProfileName("OverlapAllDynamic");
	ItemRange->SetSphereRadius(300.0f);

	ItemRange->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlapBegin);
	ItemRange->OnComponentEndOverlap.AddDynamic(this, &AItemActor::OnOverlapEnd);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	ItemMesh->SetupAttachment(ItemRange);
}

void AItemActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (IsValid(Player))
	{
		Player->ItemInRange = this;
	}
}

void AItemActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (IsValid(Player))
	{
		Player->ItemInRange = nullptr;
	}
}

