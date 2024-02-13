// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemVisual.generated.h"
class UBorder;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_X_API UDragItemVisual : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Drag Item Visual" , meta=(BindWidget))
	UBorder* ItemBorder;
	
	UPROPERTY(EditDefaultsOnly, Category="Drag Item Visual" , meta=(BindWidget))
	UImage* ItemIcon;
	
	UPROPERTY(EditDefaultsOnly, Category="Drag Item Visual" , meta=(BindWidget))
	UTextBlock* ItemQuantity;

};
