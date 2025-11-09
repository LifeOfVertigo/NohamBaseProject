import * as React from 'react';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '@/Components/shadcn/card';
import { Button } from '@/Components/shadcn/button';
import { Separator } from '@/Components/shadcn/separator';
import { useSettings } from './SettingsContext';

const SettingsManagement: React.FC = () => {
  const { applyAllSettings, resetToDefaults } = useSettings();

  const handleApplySettings = () => {
    applyAllSettings();
    // Note: Settings are sent to UE5 immediately, no async needed
    console.log('[Settings Management] Settings applied');
  };

  const handleResetDefaults = () => {
    if (confirm('Reset all settings to defaults? You will still need to click Apply to save them.')) {
      resetToDefaults();
      // UE5 will call window.SettingsReset with default values
      console.log('[Settings Management] Reset to defaults requested');
    }
  };

  return (
    <div className="space-y-6">
      {/* Apply Settings */}
      <Card>
        <CardHeader>
          <CardTitle>Apply Settings</CardTitle>
          <CardDescription>
            Save your settings changes to disk
          </CardDescription>
        </CardHeader>
        <CardContent className="space-y-4">
          <Button
            onClick={handleApplySettings}
            className="w-full"
            size="lg"
          >
            Apply All Settings
          </Button>
          <p className="text-sm text-muted-foreground">
            Click Apply to save all audio, graphics, and input settings to Unreal Engine.
            Changes are not saved until you click this button. Close the menu without applying to discard changes.
          </p>
        </CardContent>
      </Card>

      <Separator />

      {/* Reset to Defaults */}
      <Card>
        <CardHeader>
          <CardTitle>Reset to Defaults</CardTitle>
          <CardDescription>
            Restore all settings to their default values
          </CardDescription>
        </CardHeader>
        <CardContent>
          <Button onClick={handleResetDefaults} variant="destructive" className="w-full">
            Reset All Settings to Defaults
          </Button>
          <p className="text-sm text-muted-foreground mt-4">
            This will load default values for all graphics, audio, and input settings.
            You must still click "Apply" to save the defaults.
          </p>
        </CardContent>
      </Card>
    </div>
  );
};

export default SettingsManagement;
