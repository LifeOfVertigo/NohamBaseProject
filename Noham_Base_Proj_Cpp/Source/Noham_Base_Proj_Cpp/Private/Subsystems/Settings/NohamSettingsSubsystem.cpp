// Copyright Noham Studios. All Rights Reserved.

#include "Subsystems/Settings/NohamSettingsSubsystem.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/Engine.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "JsonObjectConverter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

DEFINE_LOG_CATEGORY_STATIC(LogNohamSettings, Log, All);

void UNohamSettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	bIsInitialized = true;

	// Refresh audio devices on startup
	RefreshAudioDevices();

	// Load saved settings or use defaults
	if (!LoadSettings())
	{
		LoadDefaultSettings();
	}

	UE_LOG(LogNohamSettings, Log, TEXT("NohamSettingsSubsystem initialized successfully"));
}

void UNohamSettingsSubsystem::Deinitialize()
{
	// Save settings before shutdown
	SaveSettings();

	bIsInitialized = false;

	UE_LOG(LogNohamSettings, Log, TEXT("NohamSettingsSubsystem deinitialized"));

	Super::Deinitialize();
}

// ======== Graphics Settings ========

TArray<FIntPoint> UNohamSettingsSubsystem::GetSupportedResolutions()
{
	TArray<FIntPoint> Resolutions;

	// Get supported fullscreen resolutions from engine
	FScreenResolutionArray ResolutionArray;
	if (RHIGetAvailableResolutions(ResolutionArray, true))
	{
		for (const FScreenResolutionRHI& Resolution : ResolutionArray)
		{
			Resolutions.Add(FIntPoint(Resolution.Width, Resolution.Height));
		}
	}

	// If we couldn't get resolutions, provide common defaults
	if (Resolutions.Num() == 0)
	{
		Resolutions.Add(FIntPoint(1920, 1080));
		Resolutions.Add(FIntPoint(2560, 1440));
		Resolutions.Add(FIntPoint(3840, 2160));
		Resolutions.Add(FIntPoint(1280, 720));
	}

	return Resolutions;
}

bool UNohamSettingsSubsystem::UpdateGraphicsSettings(const FNohamGraphicsSettings& NewSettings)
{
	if (!ValidateGraphicsSettings(NewSettings))
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Graphics settings validation failed"));
		return false;
	}

	GraphicsSettings = NewSettings;
	ApplyGraphicsSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Graphics settings updated: %dx%d, WindowMode=%d, VSync=%d"),
		GraphicsSettings.Resolution.X, GraphicsSettings.Resolution.Y,
		GraphicsSettings.WindowMode, GraphicsSettings.bVSyncEnabled);

	return true;
}

bool UNohamSettingsSubsystem::SetResolution(FIntPoint NewResolution)
{
	if (NewResolution.X < 640 || NewResolution.Y < 480)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Resolution too small: %dx%d"), NewResolution.X, NewResolution.Y);
		return false;
	}

	GraphicsSettings.Resolution = NewResolution;

	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		UserSettings->SetScreenResolution(NewResolution);
		UserSettings->ApplyResolutionSettings(false);
		UserSettings->SaveSettings();
	}

	UE_LOG(LogNohamSettings, Log, TEXT("Resolution changed to: %dx%d"), NewResolution.X, NewResolution.Y);
	return true;
}

bool UNohamSettingsSubsystem::SetWindowMode(int32 WindowMode)
{
	if (WindowMode < 0 || WindowMode > 2)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid window mode: %d"), WindowMode);
		return false;
	}

	GraphicsSettings.WindowMode = WindowMode;

	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		EWindowMode::Type Mode = EWindowMode::Fullscreen;
		switch (WindowMode)
		{
		case 0: Mode = EWindowMode::Fullscreen; break;
		case 1: Mode = EWindowMode::Windowed; break;
		case 2: Mode = EWindowMode::WindowedFullscreen; break;
		}

		UserSettings->SetFullscreenMode(Mode);
		UserSettings->ApplyResolutionSettings(false);
		UserSettings->SaveSettings();
	}

	UE_LOG(LogNohamSettings, Log, TEXT("Window mode changed to: %d"), WindowMode);
	return true;
}

bool UNohamSettingsSubsystem::SetVSync(bool bEnabled)
{
	GraphicsSettings.bVSyncEnabled = bEnabled;

	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		UserSettings->SetVSyncEnabled(bEnabled);
		UserSettings->ApplySettings(false);
		UserSettings->SaveSettings();
	}

	UE_LOG(LogNohamSettings, Log, TEXT("VSync %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
	return true;
}

bool UNohamSettingsSubsystem::SetFrameRateLimit(int32 Limit)
{
	if (Limit < 0 || (Limit > 0 && Limit < 30))
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid frame rate limit: %d"), Limit);
		return false;
	}

	GraphicsSettings.FrameRateLimit = Limit;

	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		UserSettings->SetFrameRateLimit(Limit > 0 ? Limit : 0);
		UserSettings->ApplySettings(false);
		UserSettings->SaveSettings();
	}

	UE_LOG(LogNohamSettings, Log, TEXT("Frame rate limit set to: %d"), Limit);
	return true;
}

bool UNohamSettingsSubsystem::SetQualityPreset(int32 Preset)
{
	if (Preset < 0 || Preset > 3)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid quality preset: %d"), Preset);
		return false;
	}

	GraphicsSettings.QualityPreset = Preset;

	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		UserSettings->SetOverallScalabilityLevel(Preset);
		UserSettings->ApplySettings(false);
		UserSettings->SaveSettings();
	}

	UE_LOG(LogNohamSettings, Log, TEXT("Quality preset set to: %d"), Preset);
	return true;
}

// ======== Audio Settings ========

TArray<FNohamAudioDevice> UNohamSettingsSubsystem::GetAudioDevices()
{
	RefreshAudioDevices();
	return CachedAudioDevices;
}

bool UNohamSettingsSubsystem::UpdateAudioSettings(const FNohamAudioSettings& NewSettings)
{
	if (!ValidateAudioSettings(NewSettings))
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Audio settings validation failed"));
		return false;
	}

	AudioSettings = NewSettings;
	ApplyAudioSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Audio settings updated: Master=%.2f, Music=%.2f, SFX=%.2f, Voice=%.2f"),
		AudioSettings.MasterVolume, AudioSettings.MusicVolume,
		AudioSettings.SFXVolume, AudioSettings.VoiceVolume);

	return true;
}

bool UNohamSettingsSubsystem::SetMasterVolume(float Volume)
{
	if (Volume < 0.0f || Volume > 1.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid master volume: %.2f"), Volume);
		return false;
	}

	AudioSettings.MasterVolume = Volume;
	ApplyAudioSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Master volume set to: %.2f"), Volume);
	return true;
}

bool UNohamSettingsSubsystem::SetMusicVolume(float Volume)
{
	if (Volume < 0.0f || Volume > 1.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid music volume: %.2f"), Volume);
		return false;
	}

	AudioSettings.MusicVolume = Volume;
	ApplyAudioSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Music volume set to: %.2f"), Volume);
	return true;
}

bool UNohamSettingsSubsystem::SetSFXVolume(float Volume)
{
	if (Volume < 0.0f || Volume > 1.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid SFX volume: %.2f"), Volume);
		return false;
	}

	AudioSettings.SFXVolume = Volume;
	ApplyAudioSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("SFX volume set to: %.2f"), Volume);
	return true;
}

bool UNohamSettingsSubsystem::SetVoiceVolume(float Volume)
{
	if (Volume < 0.0f || Volume > 1.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid voice volume: %.2f"), Volume);
		return false;
	}

	AudioSettings.VoiceVolume = Volume;
	ApplyAudioSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Voice volume set to: %.2f"), Volume);
	return true;
}


bool UNohamSettingsSubsystem::SetAudioDevice(const FString& DeviceId)
{
	// Validate device ID exists
	bool bDeviceExists = false;
	for (const FNohamAudioDevice& Device : CachedAudioDevices)
	{
		if (Device.DeviceId == DeviceId)
		{
			bDeviceExists = true;
			break;
		}
	}

	if (!bDeviceExists && !DeviceId.IsEmpty())
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Audio device not found: %s"), *DeviceId);
		return false;
	}

	AudioSettings.SelectedAudioDeviceId = DeviceId;

	// TODO: Apply audio device change through UE5 audio system when NEON is fully integrated
	// For now, just store the preference

	UE_LOG(LogNohamSettings, Log, TEXT("Audio device set to: %s"), *DeviceId);
	return true;
}

void UNohamSettingsSubsystem::PlayAudioTestSample(const FString& Category)
{
	// TODO: Implement audio test sample playback
	// This will play a short audio clip in the specified category to test volume levels
	UE_LOG(LogNohamSettings, Log, TEXT("Playing audio test sample for: %s"), *Category);
}

bool UNohamSettingsSubsystem::ValidateAudioSettings(const FNohamAudioSettings& NewSettings)
{
	// Validate volume ranges
	if (NewSettings.MasterVolume < 0.0f || NewSettings.MasterVolume > 1.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid master volume"));
		return false;
	}

	if (NewSettings.MusicVolume < 0.0f || NewSettings.MusicVolume > 1.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid music volume"));
		return false;
	}

	if (NewSettings.SFXVolume < 0.0f || NewSettings.SFXVolume > 1.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid SFX volume"));
		return false;
	}

	if (NewSettings.VoiceVolume < 0.0f || NewSettings.VoiceVolume > 1.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid voice volume"));
		return false;
	}

	return true;
}

// ======== Input Settings ========

bool UNohamSettingsSubsystem::UpdateInputSettings(const FNohamInputSettings& NewSettings)
{
	if (!ValidateInputSettings(NewSettings))
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Input settings validation failed"));
		return false;
	}

	InputSettings = NewSettings;
	ApplyInputSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Input settings updated: MouseSens=%.2f/%.2f, GamepadSens=%.2f/%.2f"),
		InputSettings.MouseSensitivityX, InputSettings.MouseSensitivityY,
		InputSettings.GamepadSensitivityX, InputSettings.GamepadSensitivityY);

	return true;
}

bool UNohamSettingsSubsystem::SetMouseSensitivityX(float Sensitivity)
{
	if (Sensitivity < 0.1f || Sensitivity > 5.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid mouse sensitivity X: %.2f"), Sensitivity);
		return false;
	}

	InputSettings.MouseSensitivityX = Sensitivity;
	ApplyInputSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Mouse sensitivity X set to: %.2f"), Sensitivity);
	return true;
}

bool UNohamSettingsSubsystem::SetMouseSensitivityY(float Sensitivity)
{
	if (Sensitivity < 0.1f || Sensitivity > 5.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid mouse sensitivity Y: %.2f"), Sensitivity);
		return false;
	}

	InputSettings.MouseSensitivityY = Sensitivity;
	ApplyInputSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Mouse sensitivity Y set to: %.2f"), Sensitivity);
	return true;
}

bool UNohamSettingsSubsystem::SetADSSensitivityMultiplier(float Multiplier)
{
	if (Multiplier < 0.1f || Multiplier > 1.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid ADS sensitivity multiplier: %.2f"), Multiplier);
		return false;
	}

	InputSettings.ADSSensitivityMultiplier = Multiplier;
	ApplyInputSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("ADS sensitivity multiplier set to: %.2f"), Multiplier);
	return true;
}

bool UNohamSettingsSubsystem::SetInvertMouseY(bool bInvert)
{
	InputSettings.bInvertMouseY = bInvert;
	ApplyInputSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Mouse Y inversion %s"), bInvert ? TEXT("enabled") : TEXT("disabled"));
	return true;
}

bool UNohamSettingsSubsystem::SetGamepadSensitivityX(float Sensitivity)
{
	if (Sensitivity < 0.1f || Sensitivity > 5.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid gamepad sensitivity X: %.2f"), Sensitivity);
		return false;
	}

	InputSettings.GamepadSensitivityX = Sensitivity;
	ApplyInputSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Gamepad sensitivity X set to: %.2f"), Sensitivity);
	return true;
}

bool UNohamSettingsSubsystem::SetGamepadSensitivityY(float Sensitivity)
{
	if (Sensitivity < 0.1f || Sensitivity > 5.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid gamepad sensitivity Y: %.2f"), Sensitivity);
		return false;
	}

	InputSettings.GamepadSensitivityY = Sensitivity;
	ApplyInputSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Gamepad sensitivity Y set to: %.2f"), Sensitivity);
	return true;
}

bool UNohamSettingsSubsystem::SetInvertGamepadY(bool bInvert)
{
	InputSettings.bInvertGamepadY = bInvert;
	ApplyInputSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Gamepad Y inversion %s"), bInvert ? TEXT("enabled") : TEXT("disabled"));
	return true;
}

bool UNohamSettingsSubsystem::SetLeftStickDeadZone(float DeadZone)
{
	if (DeadZone < 0.0f || DeadZone > 0.9f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid left stick dead zone: %.2f"), DeadZone);
		return false;
	}

	InputSettings.LeftStickDeadZone = DeadZone;
	ApplyInputSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Left stick dead zone set to: %.2f"), DeadZone);
	return true;
}

bool UNohamSettingsSubsystem::SetRightStickDeadZone(float DeadZone)
{
	if (DeadZone < 0.0f || DeadZone > 0.9f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid right stick dead zone: %.2f"), DeadZone);
		return false;
	}

	InputSettings.RightStickDeadZone = DeadZone;
	ApplyInputSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Right stick dead zone set to: %.2f"), DeadZone);
	return true;
}

bool UNohamSettingsSubsystem::SetEnableVibration(bool bEnable)
{
	InputSettings.bEnableVibration = bEnable;
	ApplyInputSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Vibration %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
	return true;
}

bool UNohamSettingsSubsystem::SetVibrationIntensity(float Intensity)
{
	if (Intensity < 0.0f || Intensity > 1.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid vibration intensity: %.2f"), Intensity);
		return false;
	}

	InputSettings.VibrationIntensity = Intensity;
	ApplyInputSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Vibration intensity set to: %.2f"), Intensity);
	return true;
}

bool UNohamSettingsSubsystem::ValidateInputSettings(const FNohamInputSettings& NewSettings)
{
	// Validate mouse sensitivity ranges
	if (NewSettings.MouseSensitivityX < 0.1f || NewSettings.MouseSensitivityX > 5.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid mouse sensitivity X"));
		return false;
	}

	if (NewSettings.MouseSensitivityY < 0.1f || NewSettings.MouseSensitivityY > 5.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid mouse sensitivity Y"));
		return false;
	}

	if (NewSettings.ADSSensitivityMultiplier < 0.1f || NewSettings.ADSSensitivityMultiplier > 1.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid ADS sensitivity multiplier"));
		return false;
	}

	// Validate gamepad sensitivity ranges
	if (NewSettings.GamepadSensitivityX < 0.1f || NewSettings.GamepadSensitivityX > 5.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid gamepad sensitivity X"));
		return false;
	}

	if (NewSettings.GamepadSensitivityY < 0.1f || NewSettings.GamepadSensitivityY > 5.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid gamepad sensitivity Y"));
		return false;
	}

	// Validate dead zone ranges
	if (NewSettings.LeftStickDeadZone < 0.0f || NewSettings.LeftStickDeadZone > 0.9f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid left stick dead zone"));
		return false;
	}

	if (NewSettings.RightStickDeadZone < 0.0f || NewSettings.RightStickDeadZone > 0.9f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid right stick dead zone"));
		return false;
	}

	// Validate vibration intensity
	if (NewSettings.VibrationIntensity < 0.0f || NewSettings.VibrationIntensity > 1.0f)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid vibration intensity"));
		return false;
	}

	return true;
}

// ======== Settings Persistence ========

bool UNohamSettingsSubsystem::SaveSettings()
{
	// Create JSON object for all settings
	TSharedPtr<FJsonObject> SettingsObject = MakeShareable(new FJsonObject());

	// Serialize graphics settings
	FString GraphicsJson;
	if (FJsonObjectConverter::UStructToJsonObjectString(GraphicsSettings, GraphicsJson))
	{
		TSharedPtr<FJsonObject> GraphicsObj;
		TSharedRef<TJsonReader<>> GraphicsReader = TJsonReaderFactory<>::Create(GraphicsJson);
		if (FJsonSerializer::Deserialize(GraphicsReader, GraphicsObj) && GraphicsObj.IsValid())
		{
			SettingsObject->SetObjectField(TEXT("Graphics"), GraphicsObj);
		}
	}

	// Serialize audio settings
	FString AudioJson;
	if (FJsonObjectConverter::UStructToJsonObjectString(AudioSettings, AudioJson))
	{
		TSharedPtr<FJsonObject> AudioObj;
		TSharedRef<TJsonReader<>> AudioReader = TJsonReaderFactory<>::Create(AudioJson);
		if (FJsonSerializer::Deserialize(AudioReader, AudioObj) && AudioObj.IsValid())
		{
			SettingsObject->SetObjectField(TEXT("Audio"), AudioObj);
		}
	}

	// Serialize input settings
	FString InputJson;
	if (FJsonObjectConverter::UStructToJsonObjectString(InputSettings, InputJson))
	{
		TSharedPtr<FJsonObject> InputObj;
		TSharedRef<TJsonReader<>> InputReader = TJsonReaderFactory<>::Create(InputJson);
		if (FJsonSerializer::Deserialize(InputReader, InputObj) && InputObj.IsValid())
		{
			SettingsObject->SetObjectField(TEXT("Input"), InputObj);
		}
	}

	// Write combined settings to file
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (FJsonSerializer::Serialize(SettingsObject.ToSharedRef(), Writer))
	{
		FString FilePath = GetSettingsFilePath();
		if (FFileHelper::SaveStringToFile(OutputString, *FilePath))
		{
			UE_LOG(LogNohamSettings, Log, TEXT("Settings saved to: %s"), *FilePath);
			return true;
		}
	}

	UE_LOG(LogNohamSettings, Error, TEXT("Failed to save settings"));
	return false;
}

bool UNohamSettingsSubsystem::LoadSettings()
{
	FString FilePath = GetSettingsFilePath();
	FString JsonString;

	if (FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		TSharedPtr<FJsonObject> SettingsObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

		if (FJsonSerializer::Deserialize(Reader, SettingsObject) && SettingsObject.IsValid())
		{
			bool bSuccess = true;

			// Load graphics settings
			if (SettingsObject->HasField(TEXT("Graphics")))
			{
				TSharedPtr<FJsonObject> GraphicsObj = SettingsObject->GetObjectField(TEXT("Graphics"));
				FString GraphicsJson;
				TSharedRef<TJsonWriter<>> GraphicsWriter = TJsonWriterFactory<>::Create(&GraphicsJson);
				if (FJsonSerializer::Serialize(GraphicsObj.ToSharedRef(), GraphicsWriter))
				{
					if (FJsonObjectConverter::JsonObjectStringToUStruct(GraphicsJson, &GraphicsSettings, 0, 0))
					{
						ApplyGraphicsSettings();
					}
					else
					{
						bSuccess = false;
					}
				}
			}

			// Load audio settings
			if (SettingsObject->HasField(TEXT("Audio")))
			{
				TSharedPtr<FJsonObject> AudioObj = SettingsObject->GetObjectField(TEXT("Audio"));
				FString AudioJson;
				TSharedRef<TJsonWriter<>> AudioWriter = TJsonWriterFactory<>::Create(&AudioJson);
				if (FJsonSerializer::Serialize(AudioObj.ToSharedRef(), AudioWriter))
				{
					if (FJsonObjectConverter::JsonObjectStringToUStruct(AudioJson, &AudioSettings, 0, 0))
					{
						ApplyAudioSettings();
					}
					else
					{
						bSuccess = false;
					}
				}
			}

			// Load input settings
			if (SettingsObject->HasField(TEXT("Input")))
			{
				TSharedPtr<FJsonObject> InputObj = SettingsObject->GetObjectField(TEXT("Input"));
				FString InputJson;
				TSharedRef<TJsonWriter<>> InputWriter = TJsonWriterFactory<>::Create(&InputJson);
				if (FJsonSerializer::Serialize(InputObj.ToSharedRef(), InputWriter))
				{
					if (FJsonObjectConverter::JsonObjectStringToUStruct(InputJson, &InputSettings, 0, 0))
					{
						ApplyInputSettings();
					}
					else
					{
						bSuccess = false;
					}
				}
			}

			if (bSuccess)
			{
				UE_LOG(LogNohamSettings, Log, TEXT("Settings loaded from: %s"), *FilePath);
				return true;
			}
		}
	}

	UE_LOG(LogNohamSettings, Warning, TEXT("Could not load settings, using defaults"));
	return false;
}

void UNohamSettingsSubsystem::ResetToDefaults()
{
	LoadDefaultSettings();
	ApplyGraphicsSettings();
	ApplyAudioSettings();
	ApplyInputSettings();
	SaveSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Settings reset to defaults"));
}

bool UNohamSettingsSubsystem::BackupSettings()
{
	BackupGraphicsSettings = GraphicsSettings;
	BackupAudioSettings = AudioSettings;
	BackupInputSettings = InputSettings;
	bHasBackup = true;

	// Optionally save backup to file
	FString BackupPath = GetBackupFilePath();

	TSharedPtr<FJsonObject> BackupObject = MakeShareable(new FJsonObject());

	// Serialize all settings to backup
	FString GraphicsJson, AudioJson, InputJson;
	if (FJsonObjectConverter::UStructToJsonObjectString(GraphicsSettings, GraphicsJson))
	{
		TSharedPtr<FJsonObject> GraphicsObj;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(GraphicsJson);
		if (FJsonSerializer::Deserialize(Reader, GraphicsObj))
		{
			BackupObject->SetObjectField(TEXT("Graphics"), GraphicsObj);
		}
	}

	if (FJsonObjectConverter::UStructToJsonObjectString(AudioSettings, AudioJson))
	{
		TSharedPtr<FJsonObject> AudioObj;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(AudioJson);
		if (FJsonSerializer::Deserialize(Reader, AudioObj))
		{
			BackupObject->SetObjectField(TEXT("Audio"), AudioObj);
		}
	}

	if (FJsonObjectConverter::UStructToJsonObjectString(InputSettings, InputJson))
	{
		TSharedPtr<FJsonObject> InputObj;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InputJson);
		if (FJsonSerializer::Deserialize(Reader, InputObj))
		{
			BackupObject->SetObjectField(TEXT("Input"), InputObj);
		}
	}

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (FJsonSerializer::Serialize(BackupObject.ToSharedRef(), Writer))
	{
		if (FFileHelper::SaveStringToFile(OutputString, *BackupPath))
		{
			UE_LOG(LogNohamSettings, Log, TEXT("Settings backup created: %s"), *BackupPath);
			return true;
		}
	}

	UE_LOG(LogNohamSettings, Warning, TEXT("Settings backup to file failed, using memory backup only"));
	return true; // Memory backup succeeded even if file backup failed
}

bool UNohamSettingsSubsystem::RestoreFromBackup()
{
	if (!bHasBackup)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("No backup available to restore"));
		return false;
	}

	GraphicsSettings = BackupGraphicsSettings;
	AudioSettings = BackupAudioSettings;
	InputSettings = BackupInputSettings;

	ApplyGraphicsSettings();
	ApplyAudioSettings();
	ApplyInputSettings();
	SaveSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Settings restored from backup"));
	return true;
}

bool UNohamSettingsSubsystem::HasBackup() const
{
	return bHasBackup || FPaths::FileExists(GetBackupFilePath());
}

bool UNohamSettingsSubsystem::ExportSettings(const FString& FilePath)
{
	if (FilePath.IsEmpty())
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Export file path is empty"));
		return false;
	}

	TSharedPtr<FJsonObject> ExportObject = MakeShareable(new FJsonObject());

	// Add version info
	ExportObject->SetStringField(TEXT("Version"), TEXT("1.0"));
	ExportObject->SetStringField(TEXT("ExportDate"), FDateTime::Now().ToString());

	// Serialize all settings
	FString GraphicsJson, AudioJson, InputJson;
	if (FJsonObjectConverter::UStructToJsonObjectString(GraphicsSettings, GraphicsJson))
	{
		TSharedPtr<FJsonObject> GraphicsObj;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(GraphicsJson);
		if (FJsonSerializer::Deserialize(Reader, GraphicsObj))
		{
			ExportObject->SetObjectField(TEXT("Graphics"), GraphicsObj);
		}
	}

	if (FJsonObjectConverter::UStructToJsonObjectString(AudioSettings, AudioJson))
	{
		TSharedPtr<FJsonObject> AudioObj;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(AudioJson);
		if (FJsonSerializer::Deserialize(Reader, AudioObj))
		{
			ExportObject->SetObjectField(TEXT("Audio"), AudioObj);
		}
	}

	if (FJsonObjectConverter::UStructToJsonObjectString(InputSettings, InputJson))
	{
		TSharedPtr<FJsonObject> InputObj;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InputJson);
		if (FJsonSerializer::Deserialize(Reader, InputObj))
		{
			ExportObject->SetObjectField(TEXT("Input"), InputObj);
		}
	}

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (FJsonSerializer::Serialize(ExportObject.ToSharedRef(), Writer))
	{
		if (FFileHelper::SaveStringToFile(OutputString, *FilePath))
		{
			UE_LOG(LogNohamSettings, Log, TEXT("Settings exported to: %s"), *FilePath);
			return true;
		}
	}

	UE_LOG(LogNohamSettings, Error, TEXT("Failed to export settings"));
	return false;
}

bool UNohamSettingsSubsystem::ImportSettings(const FString& FilePath)
{
	if (!FPaths::FileExists(FilePath))
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Import file does not exist: %s"), *FilePath);
		return false;
	}

	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Failed to read import file"));
		return false;
	}

	TSharedPtr<FJsonObject> ImportObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(Reader, ImportObject) || !ImportObject.IsValid())
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Failed to parse import file"));
		return false;
	}

	// Create backup before importing
	BackupSettings();

	bool bSuccess = true;

	// Import graphics settings
	if (ImportObject->HasField(TEXT("Graphics")))
	{
		TSharedPtr<FJsonObject> GraphicsObj = ImportObject->GetObjectField(TEXT("Graphics"));
		FString GraphicsJson;
		TSharedRef<TJsonWriter<>> GraphicsWriter = TJsonWriterFactory<>::Create(&GraphicsJson);
		if (FJsonSerializer::Serialize(GraphicsObj.ToSharedRef(), GraphicsWriter))
		{
			FNohamGraphicsSettings NewGraphics;
			if (FJsonObjectConverter::JsonObjectStringToUStruct(GraphicsJson, &NewGraphics, 0, 0))
			{
				if (ValidateGraphicsSettings(NewGraphics))
				{
					GraphicsSettings = NewGraphics;
				}
				else
				{
					bSuccess = false;
				}
			}
		}
	}

	// Import audio settings
	if (ImportObject->HasField(TEXT("Audio")))
	{
		TSharedPtr<FJsonObject> AudioObj = ImportObject->GetObjectField(TEXT("Audio"));
		FString AudioJson;
		TSharedRef<TJsonWriter<>> AudioWriter = TJsonWriterFactory<>::Create(&AudioJson);
		if (FJsonSerializer::Serialize(AudioObj.ToSharedRef(), AudioWriter))
		{
			FNohamAudioSettings NewAudio;
			if (FJsonObjectConverter::JsonObjectStringToUStruct(AudioJson, &NewAudio, 0, 0))
			{
				if (ValidateAudioSettings(NewAudio))
				{
					AudioSettings = NewAudio;
				}
				else
				{
					bSuccess = false;
				}
			}
		}
	}

	// Import input settings
	if (ImportObject->HasField(TEXT("Input")))
	{
		TSharedPtr<FJsonObject> InputObj = ImportObject->GetObjectField(TEXT("Input"));
		FString InputJson;
		TSharedRef<TJsonWriter<>> InputWriter = TJsonWriterFactory<>::Create(&InputJson);
		if (FJsonSerializer::Serialize(InputObj.ToSharedRef(), InputWriter))
		{
			FNohamInputSettings NewInput;
			if (FJsonObjectConverter::JsonObjectStringToUStruct(InputJson, &NewInput, 0, 0))
			{
				if (ValidateInputSettings(NewInput))
				{
					InputSettings = NewInput;
				}
				else
				{
					bSuccess = false;
				}
			}
		}
	}

	if (bSuccess)
	{
		ApplyGraphicsSettings();
		ApplyAudioSettings();
		ApplyInputSettings();
		SaveSettings();
		UE_LOG(LogNohamSettings, Log, TEXT("Settings imported successfully from: %s"), *FilePath);
	}
	else
	{
		RestoreFromBackup();
		UE_LOG(LogNohamSettings, Error, TEXT("Settings import failed, restored from backup"));
	}

	return bSuccess;
}

bool UNohamSettingsSubsystem::ApplyPendingSettings()
{
	// Create backup before applying
	if (!BackupSettings())
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Failed to create backup before applying settings"));
		return false;
	}

	// Settings are already applied in real-time through individual setters
	// This function primarily ensures backup is created and settings are saved
	SaveSettings();

	UE_LOG(LogNohamSettings, Log, TEXT("Pending settings applied (backup created)"));
	return true;
}

bool UNohamSettingsSubsystem::RevertSettings()
{
	return RestoreFromBackup();
}

bool UNohamSettingsSubsystem::ValidateGraphicsSettings(const FNohamGraphicsSettings& NewSettings)
{
	// Validate resolution
	if (NewSettings.Resolution.X < 640 || NewSettings.Resolution.Y < 480)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid resolution"));
		return false;
	}

	// Validate window mode
	if (NewSettings.WindowMode < 0 || NewSettings.WindowMode > 2)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid window mode"));
		return false;
	}

	// Validate frame rate limit
	if (NewSettings.FrameRateLimit < 0 || (NewSettings.FrameRateLimit > 0 && NewSettings.FrameRateLimit < 30))
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid frame rate limit"));
		return false;
	}

	// Validate quality preset
	if (NewSettings.QualityPreset < 0 || NewSettings.QualityPreset > 3)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Invalid quality preset"));
		return false;
	}

	return true;
}

// ======== Private Helper Functions ========

void UNohamSettingsSubsystem::ApplyGraphicsSettings()
{
	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	if (!UserSettings)
	{
		UE_LOG(LogNohamSettings, Error, TEXT("Could not get GameUserSettings"));
		return;
	}

	// Apply resolution
	UserSettings->SetScreenResolution(GraphicsSettings.Resolution);

	// Apply window mode
	EWindowMode::Type Mode = EWindowMode::Fullscreen;
	switch (GraphicsSettings.WindowMode)
	{
	case 0: Mode = EWindowMode::Fullscreen; break;
	case 1: Mode = EWindowMode::Windowed; break;
	case 2: Mode = EWindowMode::WindowedFullscreen; break;
	}
	UserSettings->SetFullscreenMode(Mode);

	// Apply VSync
	UserSettings->SetVSyncEnabled(GraphicsSettings.bVSyncEnabled);

	// Apply frame rate limit
	UserSettings->SetFrameRateLimit(GraphicsSettings.FrameRateLimit);

	// Apply quality preset
	UserSettings->SetOverallScalabilityLevel(GraphicsSettings.QualityPreset);

	// Apply and save
	UserSettings->ApplySettings(false);
	UserSettings->SaveSettings();
}

void UNohamSettingsSubsystem::ApplyAudioSettings()
{
	// Apply audio settings through UE5 audio system
	// Note: Volume of 0.0 = muted, no separate mute state needed

	// TODO: Apply to UE5 Sound Classes when fully integrated
	// For now, these settings are stored and logged

	UE_LOG(LogNohamSettings, Log, TEXT("Audio settings applied - Master: %.2f, Music: %.2f, SFX: %.2f, Voice: %.2f"),
		AudioSettings.MasterVolume, AudioSettings.MusicVolume, AudioSettings.SFXVolume, AudioSettings.VoiceVolume);
}

void UNohamSettingsSubsystem::ApplyInputSettings()
{
	// Apply input settings through Enhanced Input System
	// Note: This logs settings. Actual application requires InputManagerSubsystem integration

	UE_LOG(LogNohamSettings, Log, TEXT("Input settings applied - MouseSens: %.2f/%.2f, GamepadSens: %.2f/%.2f, Vibration: %s"),
		InputSettings.MouseSensitivityX, InputSettings.MouseSensitivityY,
		InputSettings.GamepadSensitivityX, InputSettings.GamepadSensitivityY,
		InputSettings.bEnableVibration ? TEXT("enabled") : TEXT("disabled"));

	// TODO: Apply to Enhanced Input System when InputManagerSubsystem is integrated
	// - Mouse/gamepad sensitivity multipliers
	// - Dead zone values
	// - Axis inversion
	// - Vibration settings
}

void UNohamSettingsSubsystem::RefreshAudioDevices()
{
	CachedAudioDevices.Empty();

	// TODO: Implement actual audio device enumeration through UE5 audio system
	// For now, provide a default device

	FNohamAudioDevice DefaultDevice;
	DefaultDevice.DeviceId = TEXT("default");
	DefaultDevice.DeviceName = TEXT("Default Audio Device");
	DefaultDevice.bIsDefault = true;

	CachedAudioDevices.Add(DefaultDevice);

	UE_LOG(LogNohamSettings, Log, TEXT("Refreshed audio devices: %d found"), CachedAudioDevices.Num());
}

void UNohamSettingsSubsystem::LoadDefaultSettings()
{
	// Set sensible defaults - Graphics
	GraphicsSettings.Resolution = FIntPoint(1920, 1080);
	GraphicsSettings.WindowMode = 0; // Fullscreen
	GraphicsSettings.bVSyncEnabled = true;
	GraphicsSettings.FrameRateLimit = 0; // Unlimited
	GraphicsSettings.QualityPreset = 3; // Epic

	// Set sensible defaults - Audio (0.0 = muted)
	AudioSettings.MasterVolume = 1.0f;
	AudioSettings.MusicVolume = 0.8f;
	AudioSettings.SFXVolume = 1.0f;
	AudioSettings.VoiceVolume = 1.0f;
	AudioSettings.SelectedAudioDeviceId = TEXT("default");

	// Set sensible defaults - Input
	InputSettings.MouseSensitivityX = 1.0f;
	InputSettings.MouseSensitivityY = 1.0f;
	InputSettings.ADSSensitivityMultiplier = 0.5f;
	InputSettings.bInvertMouseY = false;
	InputSettings.GamepadSensitivityX = 1.0f;
	InputSettings.GamepadSensitivityY = 1.0f;
	InputSettings.bInvertGamepadY = false;
	InputSettings.LeftStickDeadZone = 0.25f;
	InputSettings.RightStickDeadZone = 0.25f;
	InputSettings.bEnableVibration = true;
	InputSettings.VibrationIntensity = 1.0f;

	UE_LOG(LogNohamSettings, Log, TEXT("Loaded default settings"));
}

FString UNohamSettingsSubsystem::GetSettingsFilePath() const
{
	return FPaths::ProjectSavedDir() / TEXT("Config") / TEXT("NohamSettings.json");
}

FString UNohamSettingsSubsystem::GetBackupFilePath() const
{
	return FPaths::ProjectSavedDir() / TEXT("Config") / TEXT("NohamSettings.backup.json");
}
