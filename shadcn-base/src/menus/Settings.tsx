import { useState, useCallback } from 'react';
import { Button } from '@/components/ui/button';
import { Card, CardContent } from '@/components/ui/card';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { Label } from '@/components/ui/label';
import { Slider } from '@/components/ui/slider';
import { Switch } from '@/components/ui/switch';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from '@/components/ui/select';
import { useNavigation } from '../contexts/NavigationContext';

export default function Settings() {
  const { navigate } = useNavigation();

  // Graphics Settings
  const [resolution, setResolution] = useState('1920x1080');
  const [quality, setQuality] = useState('high');
  const [vsync, setVsync] = useState(true);
  const [brightness, setBrightness] = useState([50]);

  // Audio Settings
  const [masterVolume, setMasterVolume] = useState([75]);
  const [musicVolume, setMusicVolume] = useState([60]);
  const [sfxVolume, setSfxVolume] = useState([80]);

  const handleApply = useCallback(() => {
    // ACTUAL UE5 action - apply settings to game engine
    if (typeof (window as any).NEON !== 'undefined') {
      (window as any).NEON.invokeUnrealEvent('ApplySettings', {
        graphics: { resolution, quality, vsync, brightness: brightness[0] },
        audio: { master: masterVolume[0], music: musicVolume[0], sfx: sfxVolume[0] },
      });
      console.log('[NEON] Applying settings to UE5');
    } else {
      console.warn('[NEON] NEON not available - ensure UNEONWidget is parent class');
    }
  }, [resolution, quality, vsync, brightness, masterVolume, musicVolume, sfxVolume]);

  const handleBack = useCallback(() => {
    navigate('main'); // Internal navigation - back to main menu
  }, [navigate]);

  return (
    <div className="min-h-screen bg-background flex flex-col items-center pt-32 p-8 gap-16">
      {/* Title */}
      <h1 className="text-8xl font-bold text-foreground">Settings</h1>

      {/* Settings Card - Wider for tabs */}
      <Card className="max-w-2xl w-full">
        <CardContent className="pt-6">
          <Tabs defaultValue="graphics" className="w-full">
            <TabsList className="grid w-full grid-cols-2">
              <TabsTrigger value="graphics">Graphics</TabsTrigger>
              <TabsTrigger value="audio">Audio</TabsTrigger>
            </TabsList>

            {/* Graphics Tab */}
            <TabsContent value="graphics" className="space-y-6 mt-6">
              <div className="space-y-4">
                <div className="space-y-2">
                  <Label htmlFor="resolution">Resolution</Label>
                  <Select value={resolution} onValueChange={setResolution}>
                    <SelectTrigger id="resolution">
                      <SelectValue placeholder="Select resolution" />
                    </SelectTrigger>
                    <SelectContent>
                      <SelectItem value="1280x720">1280x720</SelectItem>
                      <SelectItem value="1920x1080">1920x1080</SelectItem>
                      <SelectItem value="2560x1440">2560x1440</SelectItem>
                      <SelectItem value="3840x2160">3840x2160</SelectItem>
                    </SelectContent>
                  </Select>
                </div>

                <div className="space-y-2">
                  <Label htmlFor="quality">Graphics Quality</Label>
                  <Select value={quality} onValueChange={setQuality}>
                    <SelectTrigger id="quality">
                      <SelectValue placeholder="Select quality" />
                    </SelectTrigger>
                    <SelectContent>
                      <SelectItem value="low">Low</SelectItem>
                      <SelectItem value="medium">Medium</SelectItem>
                      <SelectItem value="high">High</SelectItem>
                      <SelectItem value="ultra">Ultra</SelectItem>
                    </SelectContent>
                  </Select>
                </div>

                <div className="flex items-center justify-between">
                  <Label htmlFor="vsync">V-Sync</Label>
                  <Switch id="vsync" checked={vsync} onCheckedChange={setVsync} />
                </div>

                <div className="space-y-2">
                  <Label htmlFor="brightness">Brightness: {brightness[0]}%</Label>
                  <Slider
                    id="brightness"
                    value={brightness}
                    onValueChange={setBrightness}
                    max={100}
                    step={1}
                  />
                </div>
              </div>
            </TabsContent>

            {/* Audio Tab */}
            <TabsContent value="audio" className="space-y-6 mt-6">
              <div className="space-y-4">
                <div className="space-y-2">
                  <Label htmlFor="master-volume">Master Volume: {masterVolume[0]}%</Label>
                  <Slider
                    id="master-volume"
                    value={masterVolume}
                    onValueChange={setMasterVolume}
                    max={100}
                    step={1}
                  />
                </div>

                <div className="space-y-2">
                  <Label htmlFor="music-volume">Music Volume: {musicVolume[0]}%</Label>
                  <Slider
                    id="music-volume"
                    value={musicVolume}
                    onValueChange={setMusicVolume}
                    max={100}
                    step={1}
                  />
                </div>

                <div className="space-y-2">
                  <Label htmlFor="sfx-volume">SFX Volume: {sfxVolume[0]}%</Label>
                  <Slider
                    id="sfx-volume"
                    value={sfxVolume}
                    onValueChange={setSfxVolume}
                    max={100}
                    step={1}
                  />
                </div>
              </div>
            </TabsContent>
          </Tabs>

          {/* Action Buttons */}
          <div className="space-y-3 pt-6">
            <Button size="lg" className="w-full cursor-pointer" onClick={handleApply}>
              Apply Settings
            </Button>
            <Button
              variant="outline"
              size="lg"
              className="w-full cursor-pointer"
              onClick={handleBack}
            >
              Back
            </Button>
          </div>
        </CardContent>
      </Card>
    </div>
  );
}
