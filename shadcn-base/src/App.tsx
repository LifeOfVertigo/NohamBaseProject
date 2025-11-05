import { useState, useEffect } from 'react';
import { NavigationProvider } from './contexts/NavigationContext';
import MainMenu from './MainMenu';
import NewGame from './menus/NewGame';
import LoadGame from './menus/LoadGame';
import PauseMenu from './menus/PauseMenu';
import Settings from './menus/Settings';

// NEON Bridge TypeScript definitions
declare global {
  interface Window {
    NEON?: {
      invokeUnrealEvent: (eventName: string, params: any) => void;
      invokeUnrealFunction: (functionName: string, params: any[]) => Promise<any>;
    };
    // Navigation function exposed to UE5
    navigateToMenu?: (menuName: string) => void;
  }
}

type MenuScreen = 'main' | 'new-game' | 'load-game' | 'pause' | 'settings';

function App() {
  const [currentMenu, setCurrentMenu] = useState<MenuScreen>('main');

  // Set up navigation functions for NEON bridge
  useEffect(() => {
    // Helper to navigate between menus
    const navigate = (menu: MenuScreen) => {
      setCurrentMenu(menu);
    };

    // Expose navigation to UE5 (for potential UE5 → Web communication)
    window.navigateToMenu = (menuName: string) => {
      navigate(menuName as MenuScreen);
    };

    // Check NEON availability
    const checkNEON = () => {
      const neonAvailable = typeof (window as any).NEON !== 'undefined';
      console.log('[NEON] Menu routing initialized. Web app handles internal navigation.');
      console.log('[NEON] NEON API available:', neonAvailable);

      if (neonAvailable) {
        console.log('[NEON] ✓ Ready for UE5 communication (quit, start game, apply settings)');
      } else {
        console.warn('[NEON] ✗ NEON API not found - ensure widget parent is UNEONWidget');
        console.warn('[NEON] Available globals:', Object.keys(window).filter(k => k.toLowerCase().includes('ue') || k.toLowerCase().includes('neon')));
      }
    };

    // Check immediately
    checkNEON();

    // Also check after a delay in case NEON loads asynchronously
    setTimeout(checkNEON, 500);

    return () => {
      delete window.navigateToMenu;
    };
  }, []);

  // Render current menu
  const renderMenu = () => {
    switch (currentMenu) {
      case 'main':
        return <MainMenu />;
      case 'new-game':
        return <NewGame />;
      case 'load-game':
        return <LoadGame />;
      case 'pause':
        return <PauseMenu />;
      case 'settings':
        return <Settings />;
      default:
        return <MainMenu />;
    }
  };

  return (
    <NavigationProvider navigate={(screen) => setCurrentMenu(screen)}>
      {renderMenu()}
    </NavigationProvider>
  );
}

export default App;
