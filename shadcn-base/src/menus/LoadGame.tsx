import { useState, useCallback } from 'react';
import { Button } from '@/components/ui/button';
import { MenuLayout } from '@/components/shared/MenuLayout';
import { Card } from '@/components/ui/card';
import { useNavigation } from '../contexts/NavigationContext';

interface SaveSlot {
  id: number;
  name: string;
  level: string;
  playtime: string;
  date: string;
  isEmpty: boolean;
}

export default function LoadGame() {
  const { navigate } = useNavigation();

  // Mock save slots - in real implementation, load from UE5
  const [saveSlots] = useState<SaveSlot[]>([
    {
      id: 1,
      name: 'Save Slot 1',
      level: 'N/A',
      playtime: 'N/A',
      date: 'Empty',
      isEmpty: true,
    },
    {
      id: 2,
      name: 'Save Slot 2',
      level: 'N/A',
      playtime: 'N/A',
      date: 'Empty',
      isEmpty: true,
    },
    {
      id: 3,
      name: 'Save Slot 3',
      level: 'N/A',
      playtime: 'N/A',
      date: 'Empty',
      isEmpty: true,
    },
  ]);

  const [selectedSlot, setSelectedSlot] = useState<number | null>(null);

  const handleLoadGame = useCallback(() => {
    if (selectedSlot !== null) {
      // ACTUAL UE5 action - load save slot
      if (typeof (window as any).NEON !== 'undefined') {
        (window as any).NEON.invokeUnrealEvent('LoadSaveSlot', { slotId: selectedSlot });
        console.log('[NEON] Loading save slot:', selectedSlot);
      } else {
        console.warn('[NEON] NEON not available');
      }
    }
  }, [selectedSlot]);

  const handleBack = useCallback(() => {
    navigate('main'); // Internal navigation
  }, [navigate]);

  return (
    <MenuLayout title="Load Game">
      <div className="space-y-6">
        {/* Save Slots */}
        <div className="space-y-3">
          {saveSlots.map((slot) => (
            <Card
              key={slot.id}
              className={`p-4 cursor-pointer transition-colors ${
                selectedSlot === slot.id
                  ? 'border-primary bg-accent'
                  : 'hover:bg-accent/50'
              }`}
              onClick={() => setSelectedSlot(slot.id)}
            >
              <div className="flex justify-between items-start">
                <div>
                  <h3 className="font-semibold text-lg">{slot.name}</h3>
                  <div className="text-sm text-muted-foreground space-y-1 mt-2">
                    <p>Level: {slot.level}</p>
                    <p>Playtime: {slot.playtime}</p>
                    <p>Last Saved: {slot.date}</p>
                  </div>
                </div>
                {slot.isEmpty && (
                  <span className="text-xs text-muted-foreground bg-muted px-2 py-1 rounded">
                    Empty
                  </span>
                )}
              </div>
            </Card>
          ))}
        </div>

        {/* Action Buttons */}
        <div className="space-y-3 pt-4">
          <Button
            size="lg"
            className="w-full cursor-pointer"
            onClick={handleLoadGame}
            disabled={selectedSlot === null}
          >
            Load Selected Save
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
      </div>
    </MenuLayout>
  );
}
