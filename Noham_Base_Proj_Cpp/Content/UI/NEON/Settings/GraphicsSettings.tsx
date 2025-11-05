import * as React from 'react';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '@/Components/shadcn/card';
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from '@/Components/shadcn/select';
import { Slider } from '@/Components/shadcn/slider';
import { Button } from '@/Components/shadcn/button';
import { Checkbox } from '@/Components/shadcn/checkbox';

interface GraphicsSettings {
  resolution: { x: number; y: number };
  windowMode: number;
  vSyncEnabled: boolean;
  frameRateLimit: number;
  qualityPreset: number;
}

const GraphicsSettings: React.FC = () => {
  const [settings, setSettings] = React.useState<GraphicsSettings>({
    resolution: { x: 1920, y: 1080 },
    windowMode: 0,
    vSyncEnabled: true,
    frameRateLimit: 0,
    qualityPreset: 3,
  });

  const [availableResolutions, setAvailableResolutions] = React.useState<string[]>([]);

  React.useEffect(() => {
    // Load current settings from UE5
    if (window.ue && window.ue.noham) {
      window.ue.noham.getGraphicsSettings().then((data: GraphicsSettings) => {
        setSettings(data);
      });

      window.ue.noham.getSupportedResolutions().then((resolutions: { x: number; y: number }[]) => {
        const formattedResolutions = resolutions.map(r => `${r.x}x${r.y}`);
        setAvailableResolutions(formattedResolutions);
      });
    }
  }, []);

  const handleResolutionChange = (value: string) => {
    const [x, y] = value.split('x').map(Number);
    const newSettings = { ...settings, resolution: { x, y } };
    setSettings(newSettings);

    if (window.ue && window.ue.noham) {
      window.ue.noham.setResolution(x, y);
    }
  };

  const handleWindowModeChange = (value: string) => {
    const windowMode = parseInt(value);
    const newSettings = { ...settings, windowMode };
    setSettings(newSettings);

    if (window.ue && window.ue.noham) {
      window.ue.noham.setWindowMode(windowMode);
    }
  };

  const handleVSyncChange = (checked: boolean) => {
    const newSettings = { ...settings, vSyncEnabled: checked };
    setSettings(newSettings);

    if (window.ue && window.ue.noham) {
      window.ue.noham.setVSync(checked);
    }
  };

  const handleFrameRateLimitChange = (values: number[]) => {
    const frameRateLimit = values[0];
    const newSettings = { ...settings, frameRateLimit };
    setSettings(newSettings);
  };

  const applyFrameRateLimit = () => {
    if (window.ue && window.ue.noham) {
      window.ue.noham.setFrameRateLimit(settings.frameRateLimit);
    }
  };

  const handleQualityPresetChange = (value: string) => {
    const qualityPreset = parseInt(value);
    const newSettings = { ...settings, qualityPreset };
    setSettings(newSettings);

    if (window.ue && window.ue.noham) {
      window.ue.noham.setQualityPreset(qualityPreset);
    }
  };

  const resetToDefaults = () => {
    if (window.ue && window.ue.noham) {
      window.ue.noham.resetGraphicsSettings().then(() => {
        // Reload settings after reset
        window.ue.noham.getGraphicsSettings().then((data: GraphicsSettings) => {
          setSettings(data);
        });
      });
    }
  };

  const qualityPresets = [
    { value: '0', label: 'Low' },
    { value: '1', label: 'Medium' },
    { value: '2', label: 'High' },
    { value: '3', label: 'Epic' },
  ];

  const windowModes = [
    { value: '0', label: 'Fullscreen' },
    { value: '1', label: 'Windowed' },
    { value: '2', label: 'Borderless' },
  ];

  return (
    <div className="space-y-6">
      <Card>
        <CardHeader>
          <CardTitle>Graphics Settings</CardTitle>
          <CardDescription>
            Configure your graphics preferences. Changes apply immediately.
          </CardDescription>
        </CardHeader>
        <CardContent className="space-y-6">
          {/* Quality Preset */}
          <div className="space-y-2">
            <label className="text-sm font-medium">Quality Preset</label>
            <Select
              value={settings.qualityPreset.toString()}
              onValueChange={handleQualityPresetChange}
            >
              <SelectTrigger>
                <SelectValue placeholder="Select quality" />
              </SelectTrigger>
              <SelectContent>
                {qualityPresets.map(preset => (
                  <SelectItem key={preset.value} value={preset.value}>
                    {preset.label}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
          </div>

          {/* Resolution */}
          <div className="space-y-2">
            <label className="text-sm font-medium">Resolution</label>
            <Select
              value={`${settings.resolution.x}x${settings.resolution.y}`}
              onValueChange={handleResolutionChange}
            >
              <SelectTrigger>
                <SelectValue placeholder="Select resolution" />
              </SelectTrigger>
              <SelectContent>
                {availableResolutions.map(res => (
                  <SelectItem key={res} value={res}>
                    {res}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
          </div>

          {/* Window Mode */}
          <div className="space-y-2">
            <label className="text-sm font-medium">Window Mode</label>
            <Select
              value={settings.windowMode.toString()}
              onValueChange={handleWindowModeChange}
            >
              <SelectTrigger>
                <SelectValue placeholder="Select window mode" />
              </SelectTrigger>
              <SelectContent>
                {windowModes.map(mode => (
                  <SelectItem key={mode.value} value={mode.value}>
                    {mode.label}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
          </div>

          {/* VSync */}
          <div className="flex items-center space-x-2">
            <Checkbox
              id="vsync"
              checked={settings.vSyncEnabled}
              onCheckedChange={handleVSyncChange}
            />
            <label
              htmlFor="vsync"
              className="text-sm font-medium leading-none peer-disabled:cursor-not-allowed peer-disabled:opacity-70"
            >
              Enable VSync
            </label>
          </div>

          {/* Frame Rate Limit */}
          <div className="space-y-2">
            <div className="flex justify-between">
              <label className="text-sm font-medium">Frame Rate Limit</label>
              <span className="text-sm text-muted-foreground">
                {settings.frameRateLimit === 0 ? 'Unlimited' : `${settings.frameRateLimit} FPS`}
              </span>
            </div>
            <div className="flex gap-4 items-center">
              <Slider
                value={[settings.frameRateLimit]}
                onValueChange={handleFrameRateLimitChange}
                onValueCommit={applyFrameRateLimit}
                max={240}
                step={30}
                className="flex-1"
              />
            </div>
          </div>

          {/* Reset Button */}
          <div className="pt-4">
            <Button variant="outline" onClick={resetToDefaults}>
              Reset to Defaults
            </Button>
          </div>
        </CardContent>
      </Card>
    </div>
  );
};

export default GraphicsSettings;
