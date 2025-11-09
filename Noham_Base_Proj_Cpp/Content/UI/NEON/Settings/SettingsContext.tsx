import React, { createContext, useContext, useState, useEffect } from 'react';

// NEON TypeScript declarations
declare const NEON: {
  invokeUnrealEvent(eventName: string, params: object): void;
};

// Extend Window interface for UE5 event callbacks
declare global {
  interface Window {
    InitializeSettings?: (settingsJSON: any) => void;
    SettingsReset?: (settingsJSON: any) => void;
  }
}

interface AudioSettings {
  masterVolume: number;
  musicVolume: number;
  sfxVolume: number;
  voiceVolume: number;
  selectedAudioDeviceId: string;
}

interface GraphicsSettings {
  resolution?: { x: number; y: number };
  windowMode?: number;
  vSyncEnabled?: boolean;
  frameRateLimit?: number;
  qualityPreset?: number;
}

interface InputSettings {
  mouseSensitivityX?: number;
  mouseSensitivityY?: number;
  adsSensitivityMultiplier?: number;
  invertMouseY?: boolean;
  gamepadSensitivityX?: number;
  gamepadSensitivityY?: number;
  invertGamepadY?: boolean;
  leftStickDeadZone?: number;
  rightStickDeadZone?: number;
  enableVibration?: boolean;
  vibrationIntensity?: number;
}

interface SettingsContextType {
  audioSettings: AudioSettings;
  graphicsSettings: GraphicsSettings;
  inputSettings: InputSettings;
  setAudioSettings: (settings: AudioSettings) => void;
  setGraphicsSettings: (settings: GraphicsSettings) => void;
  setInputSettings: (settings: InputSettings) => void;
  applyAllSettings: () => void;
  resetToDefaults: () => void;
}

const SettingsContext = createContext<SettingsContextType | undefined>(undefined);

export const SettingsProvider: React.FC<{ children: React.ReactNode }> = ({ children }) => {
  const [audioSettings, setAudioSettings] = useState<AudioSettings>({
    masterVolume: 1.0,
    musicVolume: 0.8,
    sfxVolume: 1.0,
    voiceVolume: 1.0,
    selectedAudioDeviceId: 'default',
  });

  const [graphicsSettings, setGraphicsSettings] = useState<GraphicsSettings>({});
  const [inputSettings, setInputSettings] = useState<InputSettings>({});

  // Notify UE5 when Settings page opens and listen for responses
  useEffect(() => {
    // STEP 1: Tell UE5 that Settings page has been opened
    if (typeof NEON !== 'undefined' && NEON.invokeUnrealEvent) {
      NEON.invokeUnrealEvent('SettingsPageOpened', {});
      console.log('[Settings Context] Notified UE5 that Settings page opened');
    }

    // STEP 2: Set up event listener - UE5 will call this with settings data
    window.InitializeSettings = (settingsJSON: any) => {
      console.log('[Settings Context] Received InitializeSettings from UE5:', settingsJSON);

      // Convert UE5 PascalCase to Web camelCase and update state
      if (settingsJSON.audio) {
        setAudioSettings({
          masterVolume: settingsJSON.audio.MasterVolume ?? 1.0,
          musicVolume: settingsJSON.audio.MusicVolume ?? 0.8,
          sfxVolume: settingsJSON.audio.SFXVolume ?? 1.0,
          voiceVolume: settingsJSON.audio.VoiceVolume ?? 1.0,
          selectedAudioDeviceId: settingsJSON.audio.SelectedAudioDeviceId || 'default',
        });
      }

      if (settingsJSON.graphics) {
        const g = settingsJSON.graphics;
        setGraphicsSettings({
          resolution: g.Resolution ? { x: g.Resolution.X, y: g.Resolution.Y } : undefined,
          windowMode: g.WindowMode,
          vSyncEnabled: g.bVSyncEnabled,
          frameRateLimit: g.FrameRateLimit,
          qualityPreset: g.QualityPreset,
        });
      }

      if (settingsJSON.input) {
        const i = settingsJSON.input;
        setInputSettings({
          mouseSensitivityX: i.MouseSensitivityX,
          mouseSensitivityY: i.MouseSensitivityY,
          adsSensitivityMultiplier: i.ADSSensitivityMultiplier,
          invertMouseY: i.bInvertMouseY,
          gamepadSensitivityX: i.GamepadSensitivityX,
          gamepadSensitivityY: i.GamepadSensitivityY,
          invertGamepadY: i.bInvertGamepadY,
          leftStickDeadZone: i.LeftStickDeadZone,
          rightStickDeadZone: i.RightStickDeadZone,
          enableVibration: i.bEnableVibration,
          vibrationIntensity: i.VibrationIntensity,
        });
      }
    };

    // Reset to defaults event listener - UE5 will call this after resetting
    window.SettingsReset = (settingsJSON: any) => {
      console.log('[Settings Context] Received SettingsReset from UE5:', settingsJSON);

      // Reuse the same initialization logic
      if (window.InitializeSettings) {
        window.InitializeSettings(settingsJSON);
      }
    };

    // Cleanup event listeners on unmount
    return () => {
      delete window.InitializeSettings;
      delete window.SettingsReset;
    };
  }, []);

  const applyAllSettings = () => {
    if (typeof NEON !== 'undefined' && NEON.invokeUnrealEvent) {
      // Send all settings to UE5 for saving
      NEON.invokeUnrealEvent('ApplySettings', {
        audio: audioSettings,
        graphics: graphicsSettings,
        input: inputSettings,
      });
      console.log('[Settings Context] Applied settings to UE5:', {
        audio: audioSettings,
        graphics: graphicsSettings,
        input: inputSettings,
      });
    } else {
      console.warn('[Settings Context] NEON API not available - settings not applied');
    }
  };

  const resetToDefaults = () => {
    if (typeof NEON !== 'undefined' && NEON.invokeUnrealEvent) {
      // Request UE5 to reset and send back default values
      NEON.invokeUnrealEvent('ResetToDefaults', {});
      console.log('[Settings Context] Requested reset to defaults from UE5');
      // UE5 will call window.SettingsReset with default values
    } else {
      console.warn('[Settings Context] NEON API not available - cannot reset');
    }
  };

  return (
    <SettingsContext.Provider
      value={{
        audioSettings,
        graphicsSettings,
        inputSettings,
        setAudioSettings,
        setGraphicsSettings,
        setInputSettings,
        applyAllSettings,
        resetToDefaults,
      }}
    >
      {children}
    </SettingsContext.Provider>
  );
};

export const useSettings = () => {
  const context = useContext(SettingsContext);
  if (!context) {
    throw new Error('useSettings must be used within a SettingsProvider');
  }
  return context;
};
