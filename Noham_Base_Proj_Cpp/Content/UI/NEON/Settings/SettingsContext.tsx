import React, { createContext, useContext, useState, useEffect } from 'react';

// NEON TypeScript declarations
declare const NEON: {
  invokeUnrealEvent(eventName: string, params: object): void;
  invokeUnrealFunction(functionName: string, params: any[]): Promise<any>;
};

interface AudioSettings {
  masterVolume: number;
  musicVolume: number;
  sfxVolume: number;
  voiceVolume: number;
  selectedAudioDeviceId: string;
}

interface GraphicsSettings {
  // Add graphics settings when implemented
  [key: string]: any;
}

interface InputSettings {
  // Add input settings when implemented
  [key: string]: any;
}

interface SettingsContextType {
  audioSettings: AudioSettings;
  graphicsSettings: GraphicsSettings;
  inputSettings: InputSettings;
  setAudioSettings: (settings: AudioSettings) => void;
  setGraphicsSettings: (settings: GraphicsSettings) => void;
  setInputSettings: (settings: InputSettings) => void;
  applyAllSettings: () => Promise<void>;
  revertSettings: () => Promise<void>;
  resetToDefaults: () => Promise<void>;
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

  // Load initial settings from UE5
  useEffect(() => {
    const loadSettings = async () => {
      if (typeof NEON !== 'undefined' && NEON.invokeUnrealFunction) {
        try {
          // Load audio settings
          const audioResult = await NEON.invokeUnrealFunction('GetAudioSettings', []);
          console.log('[Settings Context] Loaded audio settings from UE5:', audioResult);

          if (audioResult) {
            setAudioSettings({
              masterVolume: audioResult.masterVolume ?? 1.0,
              musicVolume: audioResult.musicVolume ?? 0.8,
              sfxVolume: audioResult.sfxVolume ?? 1.0,
              voiceVolume: audioResult.voiceVolume ?? 1.0,
              selectedAudioDeviceId: audioResult.selectedAudioDeviceId || 'default',
            });
          }

          // TODO: Load graphics and input settings when implemented
        } catch (error) {
          console.error('[Settings Context] Failed to load settings from UE5:', error);
        }
      } else {
        console.warn('[Settings Context] NEON API not available');
      }
    };

    loadSettings();
  }, []);

  const applyAllSettings = async () => {
    if (typeof NEON !== 'undefined' && NEON.invokeUnrealEvent) {
      try {
        // Send all settings to UE5
        NEON.invokeUnrealEvent('ApplySettings', {
          audio: audioSettings,
          graphics: graphicsSettings,
          input: inputSettings,
        });
        console.log('[Settings Context] Applied settings:', {
          audio: audioSettings,
          graphics: graphicsSettings,
          input: inputSettings,
        });
      } catch (error) {
        console.error('[Settings Context] Failed to apply settings:', error);
        throw error;
      }
    } else {
      console.warn('[Settings Context] NEON API not available');
      throw new Error('NEON API not available');
    }
  };

  const revertSettings = async () => {
    if (typeof NEON !== 'undefined' && NEON.invokeUnrealEvent) {
      try {
        NEON.invokeUnrealEvent('RevertSettings', {});
        console.log('[Settings Context] Reverted settings');
        // Reload settings from UE5 after revert
        window.location.reload();
      } catch (error) {
        console.error('[Settings Context] Failed to revert settings:', error);
        throw error;
      }
    } else {
      throw new Error('NEON API not available');
    }
  };

  const resetToDefaults = async () => {
    if (typeof NEON !== 'undefined' && NEON.invokeUnrealEvent) {
      try {
        NEON.invokeUnrealEvent('ResetToDefaults', {});
        console.log('[Settings Context] Reset to defaults');
        // Reload settings from UE5 after reset
        window.location.reload();
      } catch (error) {
        console.error('[Settings Context] Failed to reset settings:', error);
        throw error;
      }
    } else {
      throw new Error('NEON API not available');
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
        revertSettings,
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
