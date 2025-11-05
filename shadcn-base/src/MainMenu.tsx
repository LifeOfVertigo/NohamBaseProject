import { useState, useEffect, useCallback } from 'react';
import { Button } from '@/components/ui/button';
import { Card, CardContent } from '@/components/ui/card';
import { useNavigation } from './contexts/NavigationContext';

interface MenuItem {
  id: string;
  label: string;
  action: () => void;
  variant?: 'default' | 'secondary' | 'outline' | 'destructive';
  icon?: string;
  disabled?: boolean;
}

function MainMenu() {
  const [selectedIndex, setSelectedIndex] = useState(0);
  const { navigate } = useNavigation();

  // NEON Bridge helper function - ONLY for actual UE5 communication
  const callUE5Event = useCallback((eventName: string, params: any = {}) => {
    if (typeof (window as any).NEON !== 'undefined') {
      (window as any).NEON.invokeUnrealEvent(eventName, params);
      console.log(`[NEON] Called UE5 event: ${eventName}`, params);
    } else {
      console.warn('[NEON] NEON not available - ensure UNEONWidget is parent class');
    }
  }, []);

  // Menu items configuration
  const menuItems: MenuItem[] = [
    {
      id: 'new_game',
      label: 'New Game',
      action: () => {
        navigate('new-game'); // Internal navigation - handled by web app
      },
      variant: 'default',
    },
    {
      id: 'load_game',
      label: 'Load Game',
      action: () => {
        navigate('load-game'); // Internal navigation - handled by web app
      },
      variant: 'default',
    },
    {
      id: 'settings',
      label: 'Settings',
      action: () => {
        navigate('settings'); // Internal navigation - handled by web app
      },
      variant: 'default',
    },
    {
      id: 'quit',
      label: 'Quit Game',
      action: () => {
        callUE5Event('QuitGame', {}); // ACTUAL UE5 action - quit the game
      },
      variant: 'default',
    },
  ];

  // Handle menu item activation
  const activateMenuItem = useCallback((index: number) => {
    const item = menuItems[index];
    if (item && !item.disabled) {
      item.action();
    }
  }, [menuItems]);

  // Keyboard navigation
  useEffect(() => {
    const handleKeyDown = (event: KeyboardEvent) => {
      switch (event.key) {
        case 'ArrowUp':
        case 'w':
        case 'W':
          event.preventDefault();
          setSelectedIndex((prev) => {
            let newIndex = prev - 1;
            if (newIndex < 0) newIndex = menuItems.length - 1;
            // Skip disabled items
            while (menuItems[newIndex]?.disabled && newIndex !== prev) {
              newIndex--;
              if (newIndex < 0) newIndex = menuItems.length - 1;
            }
            return newIndex;
          });
          break;

        case 'ArrowDown':
        case 's':
        case 'S':
          event.preventDefault();
          setSelectedIndex((prev) => {
            let newIndex = (prev + 1) % menuItems.length;
            // Skip disabled items
            while (menuItems[newIndex]?.disabled && newIndex !== prev) {
              newIndex = (newIndex + 1) % menuItems.length;
            }
            return newIndex;
          });
          break;

        case 'Enter':
        case ' ':
          event.preventDefault();
          activateMenuItem(selectedIndex);
          break;

        case 'Escape':
          event.preventDefault();
          // ESC could close menu or go back - for main menu, might do nothing
          callUE5Event('LogMessage', { message: 'Escape pressed on main menu' });
          break;

        case 'Tab':
          event.preventDefault();
          setSelectedIndex((prev) => {
            let newIndex = event.shiftKey ? prev - 1 : prev + 1;
            if (newIndex < 0) newIndex = menuItems.length - 1;
            if (newIndex >= menuItems.length) newIndex = 0;
            // Skip disabled items
            while (menuItems[newIndex]?.disabled && newIndex !== prev) {
              newIndex = event.shiftKey ? newIndex - 1 : newIndex + 1;
              if (newIndex < 0) newIndex = menuItems.length - 1;
              if (newIndex >= menuItems.length) newIndex = 0;
            }
            return newIndex;
          });
          break;
      }
    };

    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, [selectedIndex, menuItems, activateMenuItem, callUE5Event]);

  // Gamepad navigation
  useEffect(() => {
    let gamepadCheckInterval: ReturnType<typeof setInterval>;
    let lastButtonStates: { [key: number]: boolean } = {};

    const checkGamepad = () => {
      const gamepads = navigator.getGamepads();
      const gamepad = gamepads[0]; // Use first connected gamepad

      if (gamepad) {
        // D-pad up (button 12) or Left stick up (axis 1 < -0.5)
        const upPressed = gamepad.buttons[12]?.pressed || gamepad.axes[1] < -0.5;
        if (upPressed && !lastButtonStates[12]) {
          setSelectedIndex((prev) => {
            let newIndex = prev - 1;
            if (newIndex < 0) newIndex = menuItems.length - 1;
            while (menuItems[newIndex]?.disabled && newIndex !== prev) {
              newIndex--;
              if (newIndex < 0) newIndex = menuItems.length - 1;
            }
            return newIndex;
          });
        }

        // D-pad down (button 13) or Left stick down (axis 1 > 0.5)
        const downPressed = gamepad.buttons[13]?.pressed || gamepad.axes[1] > 0.5;
        if (downPressed && !lastButtonStates[13]) {
          setSelectedIndex((prev) => {
            let newIndex = (prev + 1) % menuItems.length;
            while (menuItems[newIndex]?.disabled && newIndex !== prev) {
              newIndex = (newIndex + 1) % menuItems.length;
            }
            return newIndex;
          });
        }

        // A button (button 0) to activate
        const aPressed = gamepad.buttons[0]?.pressed;
        if (aPressed && !lastButtonStates[0]) {
          activateMenuItem(selectedIndex);
        }

        // B button (button 1) to go back/escape
        const bPressed = gamepad.buttons[1]?.pressed;
        if (bPressed && !lastButtonStates[1]) {
          callUE5Event('LogMessage', { message: 'B button pressed on main menu' });
        }

        // Update button states
        lastButtonStates = {
          12: upPressed,
          13: downPressed,
          0: aPressed,
          1: bPressed,
        };
      }
    };

    gamepadCheckInterval = setInterval(checkGamepad, 100); // Check every 100ms

    return () => clearInterval(gamepadCheckInterval);
  }, [selectedIndex, menuItems, activateMenuItem, callUE5Event]);

  // Mouse hover updates selection
  const handleMouseEnter = (index: number) => {
    setSelectedIndex(index);
  };

  return (
    <div className="min-h-screen bg-background flex flex-col items-center pt-32 p-8 gap-16">
      {/* Title - separated from card */}
      <h1 className="text-8xl font-bold text-foreground">
        Noham Base Project
      </h1>

      {/* Menu Card */}
      <Card className="max-w-md w-full">
        <CardContent className="pt-6 space-y-3">
          {menuItems.map((item, index) => (
            <div
              key={item.id}
              onMouseEnter={() => handleMouseEnter(index)}
            >
              <Button
                variant={item.variant}
                size="lg"
                className="w-full cursor-pointer"
                onClick={() => item.action()}
              >
                {item.label}
              </Button>
            </div>
          ))}
        </CardContent>
      </Card>
    </div>
  );
}

export default MainMenu;
