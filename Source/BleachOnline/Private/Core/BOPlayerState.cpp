// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerState.h"
#include "BOGameState.h"
#include "BOPlayerController.h"
#include "BOFunctionLibrary.h"

#include "UnrealNetwork.h"
#include "Engine\World.h"

#include "BOGameInstanceSubsystem.h"
#include "IDMComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerState, All, All);

ABOPlayerState::ABOPlayerState()
{
	SetReplicates(true);
	IDMComp = CreateDefaultSubobject<UIDMComponent>("IDM");
}

void ABOPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABOPlayerState, Profile, COND_None);
}

void ABOPlayerState::BeginPlay()
{
	Super::BeginPlay();

	auto PC = Cast<ABOPlayerController>(GetOwner());
	if (! PC) return;

	if (PC->IsLocalController())
	{
		auto GISubsystem = BOGetterLib::GetGameInstanceSubsystem(this);
		if (GISubsystem)
		{
			SendPlayerProfileToServer(GISubsystem->GetPlayerProfile());
		}

		// IDM
		IDMComp->UploadImage(static_cast<uint8>(EIDMImageType::Avatar));
	}
}

void ABOPlayerState::ChangePlayerState_Implementation(const FName& StateName)
{
	auto PC = Cast<ABOPlayerController>(GetOwner());
	if (PC)
	{
		PC->ChangeState(StateName);
	}
}

void ABOPlayerState::SendPlayerProfileToServer_Implementation(FPlayerProfile NewProfile)
{
	Profile = NewProfile;
	OnRep_PlayerProfile();
}

void ABOPlayerState::OnRep_PlayerProfile()
{
	if (! GetWorld()) return;

	auto GS = GetWorld()->GetGameState<ABOGameState>();
	if (! GS) return;

	GS->Join(this);
}

bool ABOPlayerState::IDM_GetImageAsByte(uint8 ImageId, TArray<uint8>* OutArray)
{
	EIDMImageType ImageType = static_cast<EIDMImageType>(ImageId);
	switch (ImageType)
	{
	case EIDMImageType::Avatar:
	{
		auto GISubsystem = BOGetterLib::GetGameInstanceSubsystem(this);
		if (GISubsystem)
		{
			return GISubsystem->GetAvatarRaw(*OutArray);
		}
		break;
	}
	} // switch end
	return false;
}

void ABOPlayerState::IDM_SetImage(uint8 ImageId, TArray<uint8>* ImageRaw)
{
	EIDMImageType ImageType = static_cast<EIDMImageType>(ImageId);
	switch (ImageType)
	{
	case EIDMImageType::Avatar:
	{
		auto GISubsystem = BOGetterLib::GetGameInstanceSubsystem(this);
		if (GISubsystem)
		{
			GISubsystem->RawToTexture2D(*ImageRaw, Profile.AvatarWidth, Profile.AvatarHeight, Profile.Avatar);
		}
		break;
	}
	} // switch end
}
