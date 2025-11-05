import * as React from 'react';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '@/Components/shadcn/card';
import { Slider } from '@/Components/shadcn/slider';
import { Button } from '@/Components/shadcn/button';
import { Switch } from '@/Components/shadcn/switch';
import { Label } from '@/Components/shadcn/label';
import { Separator } from '@/Components/shadcn/separator';

interface InputSettings {
  mouseSensitivityX: number;
  mouseSensitivityY: number;
  adsSensitivityMultiplier: number;
  invertMouseY: boolean;
  gamepadSensitivityX: number;
  gamepadSensitivityY: number;
  invertGamepadY: boolean;
  leftStickDeadZone: number;
  rightStickDeadZone: number;
  enableVibration: boolean;
  vibrationIntensity: number;
}

const InputSettings: React.FC = () => {
  const [settings, setSettings] = React.useState<InputSettings>({
    mouseSensitivityX: 1.0,
    mouseSensitivityY: 1.0,
    adsSensitivityMultiplier: 0.5,
    invertMouseY: false,
    gamepadSensitivityX: 1.0,
    gamepadSensitivityY: 1.0,
    invertGamepadY: false,
    leftStickDeadZone: 0.25,
    rightStickDeadZone: 0.25,
    enableVibration: true,
    vibrationIntensity: 1.0,
  });

  React.useEffect(() => {
    // Load current settings from UE5
    if (window.ue && window.ue.noham) {
      window.ue.noham.getInputSettings().then((data: InputSettings) => {
        setSettings(data);
      });
    }
  }, []);

  const handleSliderChange = (field: keyof InputSettings, value: number[]) => {
    const newValue = value[0];
    const newSettings = { ...settings, [field]: newValue };
    setSettings(newSettings);

    if (window.ue && window.ue.noham) {
      // Convert camelCase to PascalCase and call the appropriate setter
      const setterName = `set${field.charAt(0).toUpperCase() + field.slice(1)}`;
      if (window.ue.noham[setterName]) {
        window.ue.noham[setterName](newValue);
      }
    }
  };

  const handleSwitchChange = (field: keyof InputSettings, checked: boolean) => {
    const newSettings = { ...settings, [field]: checked };
    setSettings(newSettings);

    if (window.ue && window.ue.noham) {
      const setterName = `set${field.charAt(0).toUpperCase() + field.slice(1)}`;
      if (window.ue.noham[setterName]) {
        window.ue.noham[setterName](checked);
      }
    }
  };

  const handleResetDefaults = () => {
    if (window.ue && window.ue.noham) {
      window.ue.noham.resetToDefaults().then(() => {
        window.ue.noham.getInputSettings().then((data: InputSettings) => {
          setSettings(data);
        });
      });
    }
  };

  const SensitivitySlider = ({
    label,
    field,
    value,
    min = 0.1,
    max = 5.0,
    step = 0.1
  }: {
    label: string;
    field: keyof InputSettings;
    value: number;
    min?: number;
    max?: number;
    step?: number;
  }) => (
    <div className="space-y-2">
      <div className="flex items-center justify-between">
        <Label htmlFor={field} className="text-sm font-medium">
          {label}
        </Label>
        <span className="text-sm font-mono w-12 text-right">
          {value.toFixed(2)}
        </span>
      </div>
      <Slider
        id={field}
        value={[value]}
        onValueChange={(val) => handleSliderChange(field, val)}
        min={min}
        max={max}
        step={step}
        className="w-full"
      />
    </div>
  );

  const InversionToggle = ({
    label,
    field,
    checked
  }: {
    label: string;
    field: keyof InputSettings;
    checked: boolean;
  }) => (
    <div className="flex items-center justify-between">
      <Label htmlFor={field} className="text-sm font-medium">
        {label}
      </Label>
      <Switch
        id={field}
        checked={checked}
        onCheckedChange={(val) => handleSwitchChange(field, val)}
      />
    </div>
  );

  return (
    <div className="space-y-6">
      {/* Mouse Settings */}
      <Card>
        <CardHeader>
          <CardTitle>Mouse Settings</CardTitle>
          <CardDescription>
            Customize mouse sensitivity and inversion
          </CardDescription>
        </CardHeader>
        <CardContent className="space-y-4">
          <SensitivitySlider
            label="Horizontal Sensitivity"
            field="mouseSensitivityX"
            value={settings.mouseSensitivityX}
          />
          <SensitivitySlider
            label="Vertical Sensitivity"
            field="mouseSensitivityY"
            value={settings.mouseSensitivityY}
          />
          <SensitivitySlider
            label="ADS Sensitivity Multiplier"
            field="adsSensitivityMultiplier"
            value={settings.adsSensitivityMultiplier}
            min={0.1}
            max={1.0}
          />
          <InversionToggle
            label="Invert Y-Axis"
            field="invertMouseY"
            checked={settings.invertMouseY}
          />
        </CardContent>
      </Card>

      {/* Gamepad Settings */}
      <Card>
        <CardHeader>
          <CardTitle>Gamepad Settings</CardTitle>
          <CardDescription>
            Configure gamepad sensitivity, dead zones, and vibration
          </CardDescription>
        </CardHeader>
        <CardContent className="space-y-4">
          <SensitivitySlider
            label="Horizontal Sensitivity"
            field="gamepadSensitivityX"
            value={settings.gamepadSensitivityX}
          />
          <SensitivitySlider
            label="Vertical Sensitivity"
            field="gamepadSensitivityY"
            value={settings.gamepadSensitivityY}
          />
          <InversionToggle
            label="Invert Y-Axis"
            field="invertGamepadY"
            checked={settings.invertGamepadY}
          />

          <Separator />

          <SensitivitySlider
            label="Left Stick Dead Zone"
            field="leftStickDeadZone"
            value={settings.leftStickDeadZone}
            min={0.0}
            max={0.9}
            step={0.05}
          />
          <SensitivitySlider
            label="Right Stick Dead Zone"
            field="rightStickDeadZone"
            value={settings.rightStickDeadZone}
            min={0.0}
            max={0.9}
            step={0.05}
          />

          <Separator />

          <InversionToggle
            label="Enable Vibration"
            field="enableVibration"
            checked={settings.enableVibration}
          />
          <SensitivitySlider
            label="Vibration Intensity"
            field="vibrationIntensity"
            value={settings.vibrationIntensity}
            min={0.0}
            max={1.0}
            step={0.1}
            disabled={!settings.enableVibration}
          />
        </CardContent>
      </Card>

      {/* Reset Button */}
      <div className="flex justify-end">
        <Button onClick={handleResetDefaults} variant="outline">
          Reset to Defaults
        </Button>
      </div>
    </div>
  );
};

export default InputSettings;
