// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GamecubeAdapterPlusUnreal.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "IInputDeviceModule.h"
#include "IInputDevice.h"
#include "Interfaces/IPluginManager.h"
#include "../Public/GamecubeAdapterPlusUnrealFunctionLibrary.h"

#include <GCAdapter.h>


#if PLATFORM_WINDOWS
#include "Windows/GamecubeAdapterPlusUnrealWindows.h"
#endif

#define LOCTEXT_NAMESPACE "GamecubeAdapterPlusUnrealPlugin"

const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_LeftX("Gamecube_LeftX");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_LeftY("Gamecube_LeftY");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_Left2D("Gamecube_Left2D");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_RightX("Gamecube_RightX");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_RightY("Gamecube_RightY");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_Right2D("Gamecube_Right2D");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_LeftTriggerAxis("Gamecube_LeftTriggerAxis");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_RightTriggerAxis("Gamecube_RightTriggerAxis");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_DPad_Up("Gamecube_DPad_Up");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_DPad_Down("Gamecube_DPad_Down");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_DPad_Right("Gamecube_DPad_Right");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_DPad_Left("Gamecube_DPad_Left");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_Start("Gamecube_Start");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_Shoulder_Z("Gamecube_Shoulder_Z");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_LeftTriggerPress("Gamecube_LeftTriggerPress");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_RightTriggerPress("Gamecube_RightTriggerPress");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_FaceButton_A("Gamecube_FaceButton_A");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_FaceButton_B("Gamecube_FaceButton_B");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_FaceButton_X("Gamecube_FaceButton_X");
const FGamepadKeyNames::Type FGamecubeKeyNames::Gamecube_FaceButton_Y("Gamecube_FaceButton_Y");

const FKey FGamecubeKeys::Gamecube_LeftX(FGamecubeKeyNames::Gamecube_LeftX);
const FKey FGamecubeKeys::Gamecube_LeftY(FGamecubeKeyNames::Gamecube_LeftY);
const FKey FGamecubeKeys::Gamecube_Left2D(FGamecubeKeyNames::Gamecube_Left2D);
const FKey FGamecubeKeys::Gamecube_RightX(FGamecubeKeyNames::Gamecube_RightX);
const FKey FGamecubeKeys::Gamecube_RightY(FGamecubeKeyNames::Gamecube_RightY);
const FKey FGamecubeKeys::Gamecube_Right2D(FGamecubeKeyNames::Gamecube_Right2D);
const FKey FGamecubeKeys::Gamecube_LeftTriggerAxis(FGamecubeKeyNames::Gamecube_LeftTriggerAxis);
const FKey FGamecubeKeys::Gamecube_RightTriggerAxis(FGamecubeKeyNames::Gamecube_RightTriggerAxis);
const FKey FGamecubeKeys::Gamecube_DPad_Up(FGamecubeKeyNames::Gamecube_DPad_Up);
const FKey FGamecubeKeys::Gamecube_DPad_Down(FGamecubeKeyNames::Gamecube_DPad_Down);
const FKey FGamecubeKeys::Gamecube_DPad_Right(FGamecubeKeyNames::Gamecube_DPad_Right);
const FKey FGamecubeKeys::Gamecube_DPad_Left(FGamecubeKeyNames::Gamecube_DPad_Left);
const FKey FGamecubeKeys::Gamecube_Start(FGamecubeKeyNames::Gamecube_Start);
const FKey FGamecubeKeys::Gamecube_Shoulder_Z(FGamecubeKeyNames::Gamecube_Shoulder_Z);
const FKey FGamecubeKeys::Gamecube_LeftTriggerPress(FGamecubeKeyNames::Gamecube_LeftTriggerPress);
const FKey FGamecubeKeys::Gamecube_RightTriggerPress(FGamecubeKeyNames::Gamecube_RightTriggerPress);
const FKey FGamecubeKeys::Gamecube_FaceButton_A(FGamecubeKeyNames::Gamecube_FaceButton_A);
const FKey FGamecubeKeys::Gamecube_FaceButton_B(FGamecubeKeyNames::Gamecube_FaceButton_B);
const FKey FGamecubeKeys::Gamecube_FaceButton_X(FGamecubeKeyNames::Gamecube_FaceButton_X);
const FKey FGamecubeKeys::Gamecube_FaceButton_Y(FGamecubeKeyNames::Gamecube_FaceButton_Y);

FGamecubeAdapterPlusUnreal::FGamecubeAdapterPlusUnreal(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
	: MessageHandler(InMessageHandler)
{
	SetupAdapter();

	ControllerStatusArray.AddUninitialized(NUM_CONTROLLERS);
	LastButtonStatus.AddUninitialized(NUM_CONTROLLERS); 
	LastForceFeedbackValues.AddUninitialized(NUM_CONTROLLERS);
};


FGamecubeAdapterPlusUnreal::~FGamecubeAdapterPlusUnreal()
{
	StopAdapter();
}


void FGamecubeAdapterPlusUnreal::DeviceChanged()
{
	if (bHasAdapterConnected)
	{
		StopAdapter();
	}
	else
	{
		SetupAdapter();
	}
}

void FGamecubeAdapterPlusUnreal::SetupAdapter()
{
	bHasAdapterConnected = gca::Setup() != 0;
	if (!bHasAdapterConnected) return;
	gca::Load();

}


void FGamecubeAdapterPlusUnreal::StopAdapter()
{
	if (!bHasAdapterConnected) return;
	gca::Stop();
	bHasAdapterConnected = false;
}

void FGamecubeAdapterPlusUnreal::SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

void FGamecubeAdapterPlusUnreal::Tick(float DeltaTime)
{

}

void FGamecubeAdapterPlusUnreal::HandleButtonPress(bool _bButtonStatus, bool _bLastButtonStatus, FName _KeyName, int _PlayerId)
{
	if (_bLastButtonStatus != _bButtonStatus)
	{
		if (_bButtonStatus)
		{
			MessageHandler->OnControllerButtonPressed(_KeyName, _PlayerId, false);
		}
		else
		{
			MessageHandler->OnControllerButtonReleased(_KeyName, _PlayerId, false);
		}
	}
	else if (_bButtonStatus)
	{
		MessageHandler->OnControllerButtonPressed(_KeyName, _PlayerId, true);
	}
}

void FGamecubeAdapterPlusUnreal::SendControllerEvents()
{
	if (!bHasAdapterConnected) return;

    PollTimer += FApp::GetDeltaTime();

	if (PollTimer < TimeBetweenPolls) return;
	PollTimer = 0.f;
    gca::Process(ControllerStatusArray.GetData(), ControllerStatusArray.Num());

	for (int i = 0; i < ControllerStatusArray.Num(); ++i)
	{
		ControllerStatus& Status = ControllerStatusArray[i];
		if (!Status.connected) continue;

		HandleButtonPress(Status.button_a, LastButtonStatus[i].button_a, FGamecubeKeyNames::Gamecube_FaceButton_A, i);
		HandleButtonPress(Status.button_b, LastButtonStatus[i].button_b, FGamecubeKeyNames::Gamecube_FaceButton_B, i);
		HandleButtonPress(Status.button_x, LastButtonStatus[i].button_x, FGamecubeKeyNames::Gamecube_FaceButton_X, i);
		HandleButtonPress(Status.button_y, LastButtonStatus[i].button_y, FGamecubeKeyNames::Gamecube_FaceButton_Y, i);

		HandleButtonPress(Status.pad_left, LastButtonStatus[i].pad_left, FGamecubeKeyNames::Gamecube_DPad_Left, i);
		HandleButtonPress(Status.pad_right, LastButtonStatus[i].pad_right, FGamecubeKeyNames::Gamecube_DPad_Right, i);
		HandleButtonPress(Status.padUp, LastButtonStatus[i].padUp, FGamecubeKeyNames::Gamecube_DPad_Up, i);
		HandleButtonPress(Status.padDown, LastButtonStatus[i].padDown, FGamecubeKeyNames::Gamecube_DPad_Down, i);

		HandleButtonPress(Status.button_l, LastButtonStatus[i].button_l, FGamecubeKeyNames::Gamecube_LeftTriggerPress, i);
		HandleButtonPress(Status.button_r, LastButtonStatus[i].button_r, FGamecubeKeyNames::Gamecube_RightTriggerPress, i);
		HandleButtonPress(Status.button_z, LastButtonStatus[i].button_z, FGamecubeKeyNames::Gamecube_Shoulder_Z, i);
		HandleButtonPress(Status.button_start, LastButtonStatus[i].button_start, FGamecubeKeyNames::Gamecube_Start, i);

		MessageHandler->OnControllerAnalog(FGamecubeKeyNames::Gamecube_LeftX, i, Status.primary_x / 80.f);
		MessageHandler->OnControllerAnalog(FGamecubeKeyNames::Gamecube_LeftY, i, Status.primary_y / 85.f);
		MessageHandler->OnControllerAnalog(FGamecubeKeyNames::Gamecube_RightX, i, Status.secondary_x / 80.f);
		MessageHandler->OnControllerAnalog(FGamecubeKeyNames::Gamecube_RightY, i, Status.secondary_y / 85.f);
		MessageHandler->OnControllerAnalog(FGamecubeKeyNames::Gamecube_LeftTriggerAxis, i, Status.trigger_l / 170.f);
		MessageHandler->OnControllerAnalog(FGamecubeKeyNames::Gamecube_RightTriggerAxis, i, Status.trigger_r / 170.f);

		LastButtonStatus[i] = Status;
	}
}


void FGamecubeAdapterPlusUnreal::SetChannelValue(int32 PlayerId, FForceFeedbackChannelType ChannelType, float Value)
{
    if (!bHasAdapterConnected) return;

    bool bShouldRumble = Value > ForceFeedbackThreshold;
    if (bShouldRumble == LastForceFeedbackValues[PlayerId])
    {
        RepeatForceFeedbackTimer += FApp::GetDeltaTime();
        if (RepeatForceFeedbackTimer < RepeatForceFeedbackTime) return;
    }

    RepeatForceFeedbackTimer = 0.f;
    LastForceFeedbackValues[PlayerId] = bShouldRumble;

    gca::CheckRumble(LastForceFeedbackValues.GetData(), LastForceFeedbackValues.Num());
}


void FGamecubeAdapterPlusUnreal::SetChannelValues(int32 PlayerId, const FForceFeedbackValues& Values)
{
    if (!bHasAdapterConnected) return;

	bool bShouldRumble = FMath::Max3(FMath::Max(Values.LeftLarge, Values.LeftSmall), Values.RightLarge, Values.RightSmall) > ForceFeedbackThreshold;
	if (bShouldRumble == LastForceFeedbackValues[PlayerId])
    {
        RepeatForceFeedbackTimer += FApp::GetDeltaTime();
        if (RepeatForceFeedbackTimer < RepeatForceFeedbackTime) return;
    }

    RepeatForceFeedbackTimer = 0.f;
	LastForceFeedbackValues[PlayerId] = bShouldRumble;

	gca::CheckRumble(LastForceFeedbackValues.GetData(), LastForceFeedbackValues.Num());
}

bool FGamecubeAdapterPlusUnreal::IsGamepadAttached() const
{
	for (int i = 0; i < LastButtonStatus.Num(); ++i)
	{
		return LastButtonStatus[i].connected;
	}
	return false;
}


TSharedPtr<class IInputDevice> FGamecubeAdapterPlusUnrealPlugin::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	GamecubeAdapterPlusDevice = MakeShareable(new FGamecubeAdapterPlusUnreal(InMessageHandler));
	return GamecubeAdapterPlusDevice;
}


void FGamecubeAdapterPlusUnrealPlugin::StartupModule()
{
	FString BaseDir = IPluginManager::Get().FindPlugin("GamecubeAdapterPlusUnreal")->GetBaseDir();


	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
	LibraryPath = FPaths::Combine(
		*BaseDir,
		TEXT("Source/GamecubeAdapterPlusUnreal/ThirdParty/libusb-cygwin-x64/bin/libusb-1.0.dll"));

	// Getting the handle to the DLL methods
	GamecubeAdapterPlusHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
	if (GamecubeAdapterPlusHandle)
	{
		// Do something when the DLL is first loaded
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load Gamecube third party library"));
	}

	IInputDeviceModule::StartupModule();

	const FName NAME_Gamecube(TEXT("Gamecube"));

	EKeys::AddMenuCategoryDisplayInfo(NAME_Gamecube, LOCTEXT("GamecubeSubCategory", "Gamecube Controller"), TEXT("GraphEditor.PadEvent_16x"));

	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_LeftX, LOCTEXT("Gamecube_LeftX", "Gamecube Control Stick X-Axis"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_LeftY, LOCTEXT("Gamecube_LeftY", "Gamecube Control Stick Y-Axis"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, NAME_Gamecube));
	EKeys::AddPairedKey(FKeyDetails(FGamecubeKeyNames::Gamecube_Left2D, LOCTEXT("Gamecube_Left2D", "Gamecube Control Stick 2D-Axis"), FKeyDetails::GamepadKey | FKeyDetails::Axis2D, NAME_Gamecube),
		FGamecubeKeyNames::Gamecube_LeftX, FGamecubeKeyNames::Gamecube_LeftY);
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_RightX, LOCTEXT("Gamecube_RightX", "Gamecube C Stick X-Axis"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_RightY, LOCTEXT("Gamecube_RightY", "Gamecube C Stick Y-Axis"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, NAME_Gamecube));
	EKeys::AddPairedKey(FKeyDetails(FGamecubeKeyNames::Gamecube_Right2D, LOCTEXT("Gamecube_Right2D", "Gamecube C Stick 2D-Axis"), FKeyDetails::GamepadKey | FKeyDetails::Axis2D, NAME_Gamecube),
		FGamecubeKeyNames::Gamecube_RightX, FGamecubeKeyNames::Gamecube_RightY);
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_LeftTriggerAxis, LOCTEXT("Gamecube_LeftTriggerAxis", "Gamecube Left Trigger Axis"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_RightTriggerAxis, LOCTEXT("Gamecube_RightTriggerAxis", "Gamecube Right Trigger Axis"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_DPad_Up, LOCTEXT("Gamecube_DPad_Up", "Gamecube DPad Up"), FKeyDetails::GamepadKey, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_DPad_Down, LOCTEXT("Gamecube_DPad_Down", "Gamecube DPad Down"), FKeyDetails::GamepadKey, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_DPad_Right, LOCTEXT("Gamecube_DPad_Right", "Gamecube DPad Right"), FKeyDetails::GamepadKey, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_DPad_Left, LOCTEXT("Gamecube_DPad_Left", "Gamecube DPad Left"), FKeyDetails::GamepadKey, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_Start, LOCTEXT("Gamecube_Start", "Gamecube Start"), FKeyDetails::GamepadKey, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_Shoulder_Z, LOCTEXT("Gamecube_Shoulder_Z", "Gamecube Shoulder Z"), FKeyDetails::GamepadKey, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_LeftTriggerPress, LOCTEXT("Gamecube_LeftTriggerPress", "Gamecube Left Trigger Press"), FKeyDetails::GamepadKey, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_RightTriggerPress, LOCTEXT("Gamecube_RightTriggerPress", "Gamecube Right Trigger Press"), FKeyDetails::GamepadKey, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_FaceButton_A, LOCTEXT("Gamecube_FaceButton_A", "Gamecube FaceButton A"), FKeyDetails::GamepadKey, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_FaceButton_B, LOCTEXT("Gamecube_FaceButton_B", "Gamecube FaceButton B"), FKeyDetails::GamepadKey, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_FaceButton_X, LOCTEXT("Gamecube_FaceButton_X", "Gamecube FaceButton X"), FKeyDetails::GamepadKey, NAME_Gamecube));
	EKeys::AddKey(FKeyDetails(FGamecubeKeyNames::Gamecube_FaceButton_Y, LOCTEXT("Gamecube_FaceButton_Y", "Gamecube FaceButton Y"), FKeyDetails::GamepadKey, NAME_Gamecube));

}

IMPLEMENT_MODULE(FGamecubeAdapterPlusUnrealPlugin, GamecubeAdapterPlusUnreal)

//#undef LOCTEXT_NAMESPACE

void FGamecubeAdapterPlusUnrealPlugin::ShutdownModule()
{
	FPlatformProcess::FreeDllHandle(GamecubeAdapterPlusHandle);
	GamecubeAdapterPlusHandle = nullptr;
}
