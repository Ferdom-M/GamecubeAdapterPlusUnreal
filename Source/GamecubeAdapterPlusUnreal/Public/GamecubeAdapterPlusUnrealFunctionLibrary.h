// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "GamecubeAdapterPlusUnreal.h"
#include "GamecubeAdapterPlusUnrealFunctionLibrary.generated.h"


struct GAMECUBEADAPTERPLUSUNREAL_API FGamecubeKeyNames
{
	static const FGamepadKeyNames::Type Gamecube_LeftX;
	static const FGamepadKeyNames::Type Gamecube_LeftY;
	static const FGamepadKeyNames::Type Gamecube_Left2D;
	static const FGamepadKeyNames::Type Gamecube_RightX;
	static const FGamepadKeyNames::Type Gamecube_RightY;
	static const FGamepadKeyNames::Type Gamecube_Right2D;
	static const FGamepadKeyNames::Type Gamecube_LeftTriggerAxis;
	static const FGamepadKeyNames::Type Gamecube_RightTriggerAxis;

	static const FGamepadKeyNames::Type Gamecube_DPad_Up;
	static const FGamepadKeyNames::Type Gamecube_DPad_Down;
	static const FGamepadKeyNames::Type Gamecube_DPad_Right;
	static const FGamepadKeyNames::Type Gamecube_DPad_Left;
	static const FGamepadKeyNames::Type Gamecube_Start;
	static const FGamepadKeyNames::Type Gamecube_Shoulder_Z;
	static const FGamepadKeyNames::Type Gamecube_LeftTriggerPress;
	static const FGamepadKeyNames::Type Gamecube_RightTriggerPress;
	static const FGamepadKeyNames::Type Gamecube_FaceButton_A;
	static const FGamepadKeyNames::Type Gamecube_FaceButton_B;
	static const FGamepadKeyNames::Type Gamecube_FaceButton_X;
	static const FGamepadKeyNames::Type Gamecube_FaceButton_Y;

};

struct GAMECUBEADAPTERPLUSUNREAL_API FGamecubeKeys
{
	static const FKey Gamecube_LeftX;
	static const FKey Gamecube_LeftY;
	static const FKey Gamecube_Left2D;
	static const FKey Gamecube_RightX;
	static const FKey Gamecube_RightY;
	static const FKey Gamecube_Right2D;
	static const FKey Gamecube_LeftTriggerAxis;
	static const FKey Gamecube_RightTriggerAxis;

	static const FKey Gamecube_DPad_Up;
	static const FKey Gamecube_DPad_Down;
	static const FKey Gamecube_DPad_Right;
	static const FKey Gamecube_DPad_Left;
	static const FKey Gamecube_Start;
	static const FKey Gamecube_Shoulder_Z;
	static const FKey Gamecube_LeftTriggerPress;
	static const FKey Gamecube_RightTriggerPress;
	static const FKey Gamecube_FaceButton_A;
	static const FKey Gamecube_FaceButton_B;
	static const FKey Gamecube_FaceButton_X;
	static const FKey Gamecube_FaceButton_Y;
};

UCLASS()
class GAMECUBEADAPTERPLUSUNREAL_API UGamecubeAdapterPlusFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="GamecubeAdapterPlus")
	static int GetNumConnectedDevices();
	
	static FOnGamecubeControllerConnected* GetOnDeviceConnectedDelegate();
	static FOnGamecubeControllerConnected* GetOnDeviceDisconnectedDelegate();
};

