#include "Items/ItemBase.h"

#include "Components/InventoryComponent.h"

UItemBase::UItemBase() : IsCopy(false) , IsPickUp(false)
{
}

void UItemBase::ResetItemFlags()
{
	IsCopy = false;
	IsPickUp = false;
}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase*  ItemCopy = NewObject<UItemBase>(StaticClass());
	
	ItemCopy -> ID = this ->ID;
	ItemCopy -> Quantity = this ->Quantity;
	ItemCopy -> ItemQuality= this ->ItemQuality;
	ItemCopy -> ItemType = this ->ItemType;
	ItemCopy -> ItemTextData = this ->ItemTextData;
	ItemCopy -> ItemNumericData= this ->ItemNumericData;
	ItemCopy -> ItemStatistics = this ->ItemStatistics;
	ItemCopy -> ItemAssetData = this ->ItemAssetData;
	ItemCopy -> IsCopy = true;
	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if(NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity,0,ItemNumericData.IsStackable ? ItemNumericData.MaximumStackSize: 1);

		 if(OwningInventory)
		{
				if(Quantity <= 0)
				{
					OwningInventory ->RemoveSingleInstanceOfItem(this);
				}
		}
	}
}

void UItemBase::Use(AProject_XCharacter* Character)
{
}
