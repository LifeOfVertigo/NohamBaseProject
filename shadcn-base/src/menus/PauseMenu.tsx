import { useCallback } from 'react';
import { Button } from '@/components/ui/button';
import { MenuLayout } from '@/components/shared/MenuLayout';
import { useNavigation } from '../contexts/NavigationContext';

export default function PauseMenu() {
  const { navigate } = useNavigation();

  const handleResume = useCallback(() => {
    // ACTUAL UE5 action - resume game
    if (typeof (window as any).NEON !== 'undefined') {
      (window as any).NEON.invokeUnrealEvent('ResumeGame', {});
      console.log('[NEON] Resuming game');
    } else {
      console.warn('[NEON] NEON not available');
    }
  }, []);

  const handleSettings = useCallback(() => {
    navigate('settings'); // Internal navigation
  }, [navigate]);

  const handleMainMenu = useCallback(() => {
    // ACTUAL UE5 action - return to main menu (closes game, goes to main menu)
    if (typeof (window as any).NEON !== 'undefined') {
      (window as any).NEON.invokeUnrealEvent('ReturnToMainMenu', {});
      console.log('[NEON] Returning to main menu');
    } else {
      console.warn('[NEON] NEON not available');
    }
  }, []);

  const handleQuit = useCallback(() => {
    // ACTUAL UE5 action - quit game
    if (typeof (window as any).NEON !== 'undefined') {
      (window as any).NEON.invokeUnrealEvent('QuitGame', {});
      console.log('[NEON] Quitting game');
    } else {
      console.warn('[NEON] NEON not available');
    }
  }, []);

  return (
    <MenuLayout title="Paused">
      <Button size="lg" className="w-full cursor-pointer" onClick={handleResume}>
        Resume
      </Button>
      <Button size="lg" className="w-full cursor-pointer" onClick={handleSettings}>
        Settings
      </Button>
      <Button size="lg" className="w-full cursor-pointer" onClick={handleMainMenu}>
        Main Menu
      </Button>
      <Button size="lg" className="w-full cursor-pointer" onClick={handleQuit}>
        Quit Game
      </Button>
    </MenuLayout>
  );
}
