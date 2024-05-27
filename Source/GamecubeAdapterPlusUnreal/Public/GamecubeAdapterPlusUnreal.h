// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IInputDeviceModule.h"
#include "IInputDevice.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include <ControllerStatus.h>

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGamecubeControllerConnected, int);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGamecubeControllerDisconnected, int);

class FGamecubeAdapterPlusUnreal : public IInputDevice
{
public:
	FGamecubeAdapterPlusUnreal(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);

	virtual ~FGamecubeAdapterPlusUnreal();

	void DeviceChanged();
	void SetupAdapter();
	void StopAdapter();

	void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler);
	void HandleButtonPress(bool _bButtonStatus, bool _bLastButtonStatus, FName _KeyName, int _PlayerId);

	virtual void Tick(float DeltaTime) override;
	virtual void SendControllerEvents() override;
	virtual void SetChannelValue(int32 PlayerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues(int32 PlayerId, const FForceFeedbackValues& Values) override;
	virtual bool SupportsForceFeedback(int32 ControllerId) override { return true; }
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override { return false; }
	virtual bool IsGamepadAttached() const override;


	FOnGamecubeControllerConnected OnGamecubeControllerConnected;
	FOnGamecubeControllerDisconnected OnGamecubeControllerDisconnected;
protected:

	/** Handler to send all messages to. */
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;

private:

	const float TimeBetweenPolls = 0.008f;
	float PollTimer = 0.f;
	TArray<ControllerStatus> ControllerStatusArray;
	TArray<ControllerStatus> LastButtonStatus;
	const int NUM_BUTTONS = 17;
	const int NUM_CONTROLLERS = 4;
	const float ForceFeedbackThreshold = 0.3f;
	
	TArray<bool> LastForceFeedbackValues;

	float RepeatForceFeedbackTimer = 0.f;
	const float RepeatForceFeedbackTime = 4.f;

	bool bHasAdapterConnected = false;
};

class FGamecubeAdapterPlusUnrealPlugin : public IInputDeviceModule
{
	virtual TSharedPtr<class IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;

	TSharedPtr<class FGamecubeAdapterPlusUnreal> GamecubeAdapterPlusDevice;

public:
	TSharedPtr<class FGamecubeAdapterPlusUnreal>& GetGamecubeAdapterPlusDevice() { return GamecubeAdapterPlusDevice; }

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	static inline FGamecubeAdapterPlusUnrealPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked<FGamecubeAdapterPlusUnrealPlugin>("GamecubeAdapterPlusUnreal");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("GamecubeAdapterPlusUnreal");
	}
};


void* GamecubeAdapterPlusHandle = nullptr;