// Copyright Noham Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NohamSettingsSubsystem.generated.h"

/**
 * FNohamGraphicsSettings
 * Data structure for graphics settings
 */
USTRUCT(BlueprintType)
struct FNohamGraphicsSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Graphics")
	FIntPoint Resolution = FIntPoint(1920, 1080);

	UPROPERTY(BlueprintReadWrite, Category = "Graphics")
	int32 WindowMode = 0; // 0=Fullscreen, 1=Windowed, 2=Borderless

	UPROPERTY(BlueprintReadWrite, Category = "Graphics")
	bool bVSyncEnabled = true;

	UPROPERTY(BlueprintReadWrite, Category = "Graphics")
	int32 FrameRateLimit = 0; // 0=Unlimited

	UPROPERTY(BlueprintReadWrite, Category = "Graphics")
	int32 QualityPreset = 3; // 0=Low, 1=Medium, 2=High, 3=Epic
};

/**
 * FNohamAudioDevice
 * Data structure representing an audio output device
 */
USTRUCT(BlueprintType)
struct FNohamAudioDevice
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	FString DeviceId;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	FString DeviceName;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	bool bIsDefault = false;
};

/**
 * FNohamAudioSettings
 * Data structure for audio settings
 * Note: Volume of 0.0 = muted, no separate mute booleans needed
 */
USTRUCT(BlueprintType)
struct FNohamAudioSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	float MasterVolume = 1.0f; // 0.0 to 1.0 (0.0 = muted)

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	float MusicVolume = 0.8f;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	float SFXVolume = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	float VoiceVolume = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	FString SelectedAudioDeviceId;
};

/**
 * FNohamInputSettings
 * Data structure for input and control settings
 */
USTRUCT(BlueprintType)
struct FNohamInputSettings
{
	GENERATED_BODY()

	// Mouse sensitivity
	UPROPERTY(BlueprintReadWrite, Category = "Input|Mouse")
	float MouseSensitivityX = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Input|Mouse")
	float MouseSensitivityY = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Input|Mouse")
	float ADSSensitivityMultiplier = 0.5f; // Aim-down-sights sensitivity modifier

	UPROPERTY(BlueprintReadWrite, Category = "Input|Mouse")
	bool bInvertMouseY = false;

	// Gamepad sensitivity
	UPROPERTY(BlueprintReadWrite, Category = "Input|Gamepad")
	float GamepadSensitivityX = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Input|Gamepad")
	float GamepadSensitivityY = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Input|Gamepad")
	bool bInvertGamepadY = false;

	// Gamepad dead zones
	UPROPERTY(BlueprintReadWrite, Category = "Input|Gamepad")
	float LeftStickDeadZone = 0.25f;

	UPROPERTY(BlueprintReadWrite, Category = "Input|Gamepad")
	float RightStickDeadZone = 0.25f;

	// Gamepad vibration
	UPROPERTY(BlueprintReadWrite, Category = "Input|Gamepad")
	bool bEnableVibration = true;

	UPROPERTY(BlueprintReadWrite, Category = "Input|Gamepad")
	float VibrationIntensity = 1.0f;
};

/**
 * UNohamSettingsSubsystem
 *
 * Centralized settings management subsystem
 * Handles graphics, audio, input settings with persistence and validation
 *
 * References:
 * - docs/architecture/components.md - Settings subsystem architecture
 * - docs/architecture/coding-standards.md - Settings validation standards
 */
UCLASS()
class NOHAM_BASE_PROJ_CPP_API UNohamSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Subsystem lifecycle
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// ======== Graphics Settings ========

	/**
	 * Get current graphics settings
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Graphics")
	FNohamGraphicsSettings GetGraphicsSettings() const { return GraphicsSettings; }

	/**
	 * Get supported screen resolutions for current display
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Graphics")
	TArray<FIntPoint> GetSupportedResolutions();

	/**
	 * Update graphics settings and apply immediately
	 * @param NewSettings - New graphics settings to apply
	 * @return true if settings were valid and applied successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Graphics")
	bool UpdateGraphicsSettings(const FNohamGraphicsSettings& NewSettings);

	/**
	 * Apply resolution change
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Graphics")
	bool SetResolution(FIntPoint NewResolution);

	/**
	 * Apply window mode change
	 * @param WindowMode - 0=Fullscreen, 1=Windowed, 2=Borderless
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Graphics")
	bool SetWindowMode(int32 WindowMode);

	/**
	 * Toggle VSync
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Graphics")
	bool SetVSync(bool bEnabled);

	/**
	 * Set frame rate limit (0 = unlimited)
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Graphics")
	bool SetFrameRateLimit(int32 Limit);

	/**
	 * Set overall quality preset
	 * @param Preset - 0=Low, 1=Medium, 2=High, 3=Epic
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Graphics")
	bool SetQualityPreset(int32 Preset);

	// ======== Audio Settings ========

	/**
	 * Get current audio settings
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Audio")
	FNohamAudioSettings GetAudioSettings() const { return AudioSettings; }

	/**
	 * Get available audio output devices
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Audio")
	TArray<FNohamAudioDevice> GetAudioDevices();

	/**
	 * Update audio settings and apply immediately
	 * @param NewSettings - New audio settings to apply
	 * @return true if settings were valid and applied successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Audio")
	bool UpdateAudioSettings(const FNohamAudioSettings& NewSettings);

	/**
	 * Set master volume (0.0 to 1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Audio")
	bool SetMasterVolume(float Volume);

	/**
	 * Set music volume (0.0 to 1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Audio")
	bool SetMusicVolume(float Volume);

	/**
	 * Set SFX volume (0.0 to 1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Audio")
	bool SetSFXVolume(float Volume);

	/**
	 * Set voice volume (0.0 to 1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Audio")
	bool SetVoiceVolume(float Volume);

	/**
	 * Set audio output device
	 * @param DeviceId - ID of the audio device to use
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Audio")
	bool SetAudioDevice(const FString& DeviceId);

	/**
	 * Play audio test sample for specified category
	 * @param Category - "Master", "Music", "SFX", or "Voice"
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Audio")
	void PlayAudioTestSample(const FString& Category);

	/**
	 * Validate audio settings before applying
	 * @param NewSettings - Settings to validate
	 * @return true if settings are valid
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Audio")
	bool ValidateAudioSettings(const FNohamAudioSettings& NewSettings);

	// ======== Input Settings ========

	/**
	 * Get current input settings
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	FNohamInputSettings GetInputSettings() const { return InputSettings; }

	/**
	 * Update input settings and apply immediately
	 * @param NewSettings - New input settings to apply
	 * @return true if settings were valid and applied successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	bool UpdateInputSettings(const FNohamInputSettings& NewSettings);

	/**
	 * Set mouse sensitivity X axis (0.1 to 5.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	bool SetMouseSensitivityX(float Sensitivity);

	/**
	 * Set mouse sensitivity Y axis (0.1 to 5.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	bool SetMouseSensitivityY(float Sensitivity);

	/**
	 * Set ADS sensitivity multiplier (0.1 to 1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	bool SetADSSensitivityMultiplier(float Multiplier);

	/**
	 * Set mouse Y-axis inversion
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	bool SetInvertMouseY(bool bInvert);

	/**
	 * Set gamepad sensitivity X axis (0.1 to 5.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	bool SetGamepadSensitivityX(float Sensitivity);

	/**
	 * Set gamepad sensitivity Y axis (0.1 to 5.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	bool SetGamepadSensitivityY(float Sensitivity);

	/**
	 * Set gamepad Y-axis inversion
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	bool SetInvertGamepadY(bool bInvert);

	/**
	 * Set left stick dead zone (0.0 to 0.9)
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	bool SetLeftStickDeadZone(float DeadZone);

	/**
	 * Set right stick dead zone (0.0 to 0.9)
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	bool SetRightStickDeadZone(float DeadZone);

	/**
	 * Enable/disable gamepad vibration
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	bool SetEnableVibration(bool bEnable);

	/**
	 * Set vibration intensity (0.0 to 1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	bool SetVibrationIntensity(float Intensity);

	/**
	 * Validate input settings before applying
	 * @param NewSettings - Settings to validate
	 * @return true if settings are valid
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings|Input")
	bool ValidateInputSettings(const FNohamInputSettings& NewSettings);

	// ======== Settings Persistence ========

	/**
	 * Save all settings to persistent storage
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings")
	bool SaveSettings();

	/**
	 * Load settings from persistent storage
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings")
	bool LoadSettings();

	/**
	 * Reset all settings to defaults
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings")
	void ResetToDefaults();

	/**
	 * Create backup of current settings
	 * @return true if backup was created successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings")
	bool BackupSettings();

	/**
	 * Restore settings from backup
	 * @return true if restore was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings")
	bool RestoreFromBackup();

	/**
	 * Check if a settings backup exists
	 * @return true if backup file exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings")
	bool HasBackup() const;

	/**
	 * Export settings to a file
	 * @param FilePath - Path to export file
	 * @return true if export was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings")
	bool ExportSettings(const FString& FilePath);

	/**
	 * Import settings from a file
	 * @param FilePath - Path to import file
	 * @return true if import was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings")
	bool ImportSettings(const FString& FilePath);

	/**
	 * Apply pending settings changes (with backup)
	 * @return true if settings were applied successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings")
	bool ApplyPendingSettings();

	/**
	 * Revert to previous settings (from backup)
	 * @return true if revert was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings")
	bool RevertSettings();

	/**
	 * Validate settings change before applying
	 * @param NewSettings - Settings to validate
	 * @return true if settings are valid
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Settings")
	bool ValidateGraphicsSettings(const FNohamGraphicsSettings& NewSettings);

private:
	// Current settings state
	FNohamGraphicsSettings GraphicsSettings;
	FNohamAudioSettings AudioSettings;
	FNohamInputSettings InputSettings;

	// Internal helper functions
	void ApplyGraphicsSettings();
	void ApplyAudioSettings();
	void ApplyInputSettings();
	void LoadDefaultSettings();
	FString GetSettingsFilePath() const;
	FString GetBackupFilePath() const;

	// Settings backup state
	FNohamGraphicsSettings BackupGraphicsSettings;
	FNohamAudioSettings BackupAudioSettings;
	FNohamInputSettings BackupInputSettings;
	bool bHasBackup = false;

	// Audio device cache
	TArray<FNohamAudioDevice> CachedAudioDevices;
	void RefreshAudioDevices();

	// Initialization flag
	bool bIsInitialized = false;
};
