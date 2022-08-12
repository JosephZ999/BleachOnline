// Fill out your copyright notice in the Description page of Project Settings.

#include "BOSpriteComponent.h"
#include "PaperFlipbook.h"
#include "ConstructorHelpers.h"
#include "EngineUtils.h"

UBOSpriteComponent::UBOSpriteComponent() {}

void UBOSpriteComponent::InitBaseAnimations(TMap<FName, UPaperFlipbook*>& OutAnimations, FString AnimsFolder)
{
	OutAnimations.Empty();

	TArray<UObject*> AnimAsset;
	EngineUtils::FindOrLoadAssetsByPath(AnimsFolder, AnimAsset, EngineUtils::ATL_Regular);
	for (auto Asset : AnimAsset)
	{
		auto Flipbook = Cast<UPaperFlipbook>(Asset);
		if (Flipbook != nullptr) { OutAnimations.Add(Flipbook->GetFName(), Flipbook); }
	}
}

void UBOSpriteComponent::SetBaseAnimations(TMap<FName, UPaperFlipbook*>& NewAnimations)
{
	BaseAnimations = NewAnimations;
}
