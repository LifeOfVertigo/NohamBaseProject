import * as React from 'react';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '@/Components/shadcn/card';
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from '@/Components/shadcn/select';
import { Slider } from '@/Components/shadcn/slider';
import { Button } from '@/Components/shadcn/button';
import { Switch } from '@/Components/shadcn/switch';
import { Label } from '@/Components/shadcn/label';
import { Separator } from '@/Components/shadcn/separator';
import { useSettings } from './SettingsContext';

interface AudioDevice {
  deviceId: string;
  deviceName: string;
  isDefault: boolean;
}

const AudioSettings: React.FC = () => {
  const { audioSettings, setAudioSettings } = useSettings();
  const [audioDevices, setAudioDevices] = React.useState<AudioDevice[]>([]);

  const handleVolumeChange = (category: string, value: number[]) => {
    const volume = value[0];
    const newSettings = { ...audioSettings, [`${category}Volume`]: volume };
    setAudioSettings(newSettings);
    // Note: Does NOT send to UE5 immediately - only updates local state
    // Settings are sent when Apply button is clicked in Management tab
  };

  const handleMuteToggle = (category: string, checked: boolean) => {
    // When muted, set volume to 0; when unmuted, restore to default (1.0 for most, 0.8 for music)
    const defaultVolume = category === 'music' ? 0.8 : 1.0;
    const volume = checked ? 0.0 : defaultVolume;

    const newSettings = { ...audioSettings, [`${category}Volume`]: volume };
    setAudioSettings(newSettings);
    // Note: Does NOT send to UE5 immediately - only updates local state
    // Settings are sent when Apply button is clicked in Management tab
  };

  const handleDeviceChange = (deviceId: string) => {
    const newSettings = { ...audioSettings, selectedAudioDeviceId: deviceId };
    setAudioSettings(newSettings);
    // Note: Does NOT send to UE5 immediately - only updates local state
    // Settings are sent when Apply button is clicked in Management tab
  };

  const handleTestAudio = (category: string) => {
    // TODO: Implement audio test sample playback via NEON
    console.log('[Audio Settings] Test audio for category:', category);
  };

  const VolumeControl = ({
    label,
    category,
    volume
  }: {
    label: string;
    category: string;
    volume: number;
  }) => {
    const isMuted = volume === 0;

    return (
    <div className="space-y-3">
      <div className="flex items-center justify-between">
        <div className="flex items-center space-x-3">
          <Label htmlFor={`${category}-volume`} className="text-sm font-medium">
            {label}
          </Label>
          <div className="flex items-center space-x-2">
            <Switch
              id={`${category}-mute`}
              checked={isMuted}
              onCheckedChange={(checked) => handleMuteToggle(category, checked)}
            />
            <Label htmlFor={`${category}-mute`} className="text-xs text-muted-foreground">
              {isMuted ? 'Muted' : 'Unmuted'}
            </Label>
          </div>
        </div>
        <div className="flex items-center space-x-2">
          <span className="text-sm font-mono w-12 text-right">
            {Math.round(volume * 100)}%
          </span>
          <Button
            size="sm"
            variant="outline"
            onClick={() => handleTestAudio(category)}
            disabled={isMuted}
          >
            Test
          </Button>
        </div>
      </div>
      <Slider
        id={`${category}-volume`}
        value={[volume]}
        onValueChange={(value) => handleVolumeChange(category, value)}
        max={1}
        step={0.01}
        disabled={isMuted}
        className="w-full"
      />
    </div>
  );
  };

  return (
    <Card className="w-full">
      <CardHeader>
        <CardTitle>Audio Settings</CardTitle>
        <CardDescription>
          Customize volume levels and audio output device
        </CardDescription>
      </CardHeader>
      <CardContent className="space-y-6">
        {/* Audio Device Selection */}
        <div className="space-y-2">
          <Label htmlFor="audio-device">Audio Output Device</Label>
          <Select
            value={audioSettings.selectedAudioDeviceId}
            onValueChange={handleDeviceChange}
          >
            <SelectTrigger id="audio-device">
              <SelectValue placeholder="Select audio device" />
            </SelectTrigger>
            <SelectContent>
              {audioDevices.map((device) => (
                <SelectItem key={device.deviceId} value={device.deviceId}>
                  {device.deviceName} {device.isDefault && '(Default)'}
                </SelectItem>
              ))}
            </SelectContent>
          </Select>
        </div>

        <Separator />

        {/* Master Volume */}
        <VolumeControl
          label="Master Volume"
          category="master"
          volume={audioSettings.masterVolume}
        />

        <Separator />

        {/* Music Volume */}
        <VolumeControl
          label="Music Volume"
          category="music"
          volume={audioSettings.musicVolume}
        />

        {/* SFX Volume */}
        <VolumeControl
          label="SFX Volume"
          category="sfx"
          volume={audioSettings.sfxVolume}
        />

        {/* Voice Volume */}
        <VolumeControl
          label="Voice Volume"
          category="voice"
          volume={audioSettings.voiceVolume}
        />
      </CardContent>
    </Card>
  );
};

export default AudioSettings;
