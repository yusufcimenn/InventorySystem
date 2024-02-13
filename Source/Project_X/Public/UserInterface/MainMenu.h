// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class AProject_XCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_X_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY()
	AProject_XCharacter* PlayerCharacter;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

};
