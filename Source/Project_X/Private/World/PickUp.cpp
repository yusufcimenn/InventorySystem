#include "World/PickUp.h"

#include "IDetailTreeNode.h"
#include "Components/InventoryComponent.h"
#include "Items/ItemBase.h"

// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickUpMesh");
	PickUpMesh-> SetSimulatePhysics(true); // oyunun boyutu büyüdükçe performans problemi yaratabilir?
	SetRootComponent(PickUpMesh);
	
}

void APickUp::BeginPlay()
{
	Super::BeginPlay();

	InitializePickUp(UItemBase::StaticClass(),ItemQuantity);
	
}

void APickUp::InitializePickUp(const TSubclassOf<UItemBase> BaseClass, const int InQuantity)
{
	if(ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID,DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this,BaseClass);
		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->ItemNumericData = ItemData->ItemNumericData;
		ItemReference->ItemTextData = ItemData->ItemTextData;
		ItemReference->ItemAssetData = ItemData->ItemAssetData;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
		
		PickUpMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);

		UpdateInteractableData();
	}
}

void APickUp::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->ItemNumericData.Weight = ItemToDrop->GetItemSingleWeight();
	PickUpMesh->SetStaticMesh(ItemToDrop->ItemAssetData.Mesh);
	
	UpdateInteractableData();

}

void APickUp::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->ItemTextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->ItemTextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
}

void APickUp::BeginFocus()
{
	if(PickUpMesh)
	{
		PickUpMesh->SetRenderCustomDepth(true);
	}
}

void APickUp::EndFocus()
{
	if(PickUpMesh)
	{
		PickUpMesh->SetRenderCustomDepth(false);
	}
}

void APickUp::Interact(AProject_XCharacter* PlayerCharacter)
{
	if(PlayerCharacter)
	{
		TakePickUp(PlayerCharacter);
	}
}

void APickUp::TakePickUp(const AProject_XCharacter* Taker)
{
	if(!IsPendingKillPending()) 
	{

		if(ItemReference)
		{

			 if(UInventoryComponent* PlayerInventory = Taker->GetInventory())
			 {
				 const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				 switch (AddResult.OperationResult)
				 {
				 case EItemAddResult::IAR_NoItemAdded:
				 	break;
				 case EItemAddResult::IAR_PartialAmountItemAdded:
				 	UpdateInteractableData();
				 	Taker->UpdateInteractionWidget();
				 	break;
				 case EItemAddResult::IAR_AllItemAdded:
				 	Destroy();
				 	break;
				 }
			 	UE_LOG(LogTemp, Warning,TEXT("%s"), *AddResult.ResultMessage.ToString());
			 }
			 else
			 {
			 	UE_LOG(LogTemp, Warning,TEXT("Player inventory component is null!"));
			 }
		}
		else
		{
			UE_LOG(LogTemp, Warning,TEXT("Pickup internal item reference was somehow?? null!"));

		}
	}
}


void APickUp::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangePropertName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if(ChangePropertName == GET_MEMBER_NAME_CHECKED(APickUp,DesiredItemID))
	{
		if(ItemDataTable)
		{
			if(	const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID,DesiredItemID.ToString()))
			{
				PickUpMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);
			}

		}
	}
}
