import * as React from 'react';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '@/Components/shadcn/card';
import { Button } from '@/Components/shadcn/button';
import { Separator } from '@/Components/shadcn/separator';
import { useSettings } from './SettingsContext';

const SettingsManagement: React.FC = () => {
  const { applyAllSettings, revertSettings, resetToDefaults } = useSettings();
  const [isApplying, setIsApplying] = React.useState(false);

  const handleApplySettings = async () => {
    setIsApplying(true);
    try {
      await applyAllSettings();
      alert('Settings applied successfully!');
    } catch (error) {
      console.error('[Settings Management] Apply failed:', error);
      alert('Failed to apply settings. Check console for details.');
    } finally {
      setIsApplying(false);
    }
  };

  const handleRevertSettings = async () => {
    if (confirm('Revert to previous settings? This will undo recent changes.')) {
      try {
        await revertSettings();
        alert('Settings reverted successfully.');
      } catch (error) {
        console.error('[Settings Management] Revert failed:', error);
        alert('Failed to revert settings.');
      }
    }
  };

  const handleResetDefaults = async () => {
    if (confirm('Reset all settings to defaults? This cannot be undone.')) {
      try {
        await resetToDefaults();
        alert('Settings reset to defaults.');
      } catch (error) {
        console.error('[Settings Management] Reset failed:', error);
        alert('Failed to reset settings.');
      }
    }
  };

  return (
    <div className="space-y-6">
      {/* Apply/Revert Controls */}
      <Card>
        <CardHeader>
          <CardTitle>Apply & Revert</CardTitle>
          <CardDescription>
            Apply or revert your settings changes
          </CardDescription>
        </CardHeader>
        <CardContent className="space-y-4">
          <div className="flex gap-4">
            <Button
              onClick={handleApplySettings}
              className="flex-1"
              disabled={isApplying}
            >
              {isApplying ? 'Applying...' : 'Apply All Settings'}
            </Button>
            <Button
              onClick={handleRevertSettings}
              variant="outline"
              className="flex-1"
              disabled={isApplying}
            >
              Revert to Previous
            </Button>
          </div>
          <p className="text-sm text-muted-foreground">
            Apply button saves all audio, graphics, and input settings to Unreal Engine.
            Revert restores the last applied settings.
          </p>
        </CardContent>
      </Card>

      <Separator />

      {/* Reset to Defaults */}
      <Card>
        <CardHeader>
          <CardTitle>Reset Settings</CardTitle>
          <CardDescription>
            Restore all settings to their default values
          </CardDescription>
        </CardHeader>
        <CardContent>
          <Button onClick={handleResetDefaults} variant="destructive" className="w-full">
            Reset All Settings to Defaults
          </Button>
          <p className="text-sm text-muted-foreground mt-4">
            This will reset all graphics, audio, and input settings to their default values. This action cannot be undone.
          </p>
        </CardContent>
      </Card>
    </div>
  );
};

export default SettingsManagement;
