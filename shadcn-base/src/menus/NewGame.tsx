import { useState, useCallback } from 'react';
import { Button } from '@/components/ui/button';
import { MenuLayout } from '@/components/shared/MenuLayout';
import { useNavigation } from '../contexts/NavigationContext';

export default function NewGame() {
  const [selectedDifficulty, setSelectedDifficulty] = useState<string>('normal');
  const { navigate } = useNavigation();

  const difficulties = [
    { id: 'easy', label: 'Easy', description: 'Relaxed experience' },
    { id: 'normal', label: 'Normal', description: 'Balanced gameplay' },
    { id: 'hard', label: 'Hard', description: 'Challenging experience' },
  ];

  const handleStartGame = useCallback(() => {
    // ACTUAL UE5 action - start the game with selected difficulty
    if (typeof (window as any).NEON !== 'undefined') {
      (window as any).NEON.invokeUnrealEvent('StartNewGame', { difficulty: selectedDifficulty });
      console.log('[NEON] Starting new game with difficulty:', selectedDifficulty);
    } else {
      console.warn('[NEON] NEON not available - ensure UNEONWidget is parent class');
    }
  }, [selectedDifficulty]);

  const handleBack = useCallback(() => {
    navigate('main'); // Internal navigation - back to main menu
  }, [navigate]);

  return (
    <MenuLayout title="New Game">
      <div className="space-y-6">
        {/* Difficulty Selection */}
        <div className="space-y-3">
          <h2 className="text-xl font-semibold text-foreground">Select Difficulty</h2>
          {difficulties.map((difficulty) => (
            <Button
              key={difficulty.id}
              variant={selectedDifficulty === difficulty.id ? 'default' : 'outline'}
              size="lg"
              className="w-full cursor-pointer flex flex-col items-start h-auto py-4"
              onClick={() => setSelectedDifficulty(difficulty.id)}
            >
              <span className="text-lg font-semibold">{difficulty.label}</span>
              <span className="text-sm text-muted-foreground">{difficulty.description}</span>
            </Button>
          ))}
        </div>

        {/* Action Buttons */}
        <div className="space-y-3 pt-4">
          <Button
            size="lg"
            className="w-full cursor-pointer"
            onClick={handleStartGame}
          >
            Start Game
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
