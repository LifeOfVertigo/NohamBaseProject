import React, { useState, useEffect, useCallback } from 'react';
import { createRoot } from 'react-dom/client';
import { motion, AnimatePresence } from 'framer-motion';
import { Button } from '@/Components/shadcn/button';
import { Card, CardContent, CardHeader, CardTitle } from '@/Components/shadcn/card';
import { Separator } from '@/Components/shadcn/separator';

// Import Tailwind CSS - Vite will process it
import '../Styles/globals.css';

// NEON Bridge TypeScript definitions
declare global {
  interface Window {
    ue?: {
      interface?: {
        new_game?: () => void;
        continue_game?: () => void;
        open_settings?: () => void;
        quit_game?: () => void;
        log_message?: (message: string) => void;
      };
    };
  }
}

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
  const [isVisible, setIsVisible] = useState(false);
  const [inputMode, setInputMode] = useState<'keyboard' | 'gamepad'>('keyboard');
  const [hasSaveData, setHasSaveData] = useState(false);

  // Check for save data (mock implementation - replace with actual UE5 check)
  useEffect(() => {
    // In real implementation, query UE5 for save data
    // For now, assume no save data
    setHasSaveData(false);
  }, []);

  // Fade in animation on mount
  useEffect(() => {
    setIsVisible(true);
  }, []);

  // NEON Bridge helper function
  const callUE5Function = useCallback((functionName: string, ...args: any[]) => {
    if (window.ue?.interface) {
      const func = (window.ue.interface as any)[functionName];
      if (typeof func === 'function') {
        func(...args);
        console.log(`[NEON] Called UE5 function: ${functionName}`, args);
      } else {
        console.warn(`[NEON] Function not bound: ${functionName}`);
      }
    } else {
      console.warn('[NEON] UE5 interface not available');
    }
  }, []);

  // Menu items configuration
  const menuItems: MenuItem[] = [
    {
      id: 'new_game',
      label: 'New Game',
      icon: '🎮',
      action: () => {
        callUE5Function('new_game');
      },
      variant: 'default',
    },
    {
      id: 'continue',
      label: 'Continue',
      icon: '▶️',
      action: () => {
        callUE5Function('continue_game');
      },
      variant: 'secondary',
      disabled: !hasSaveData,
    },
    {
      id: 'settings',
      label: 'Settings',
      icon: '⚙️',
      action: () => {
        callUE5Function('open_settings');
      },
      variant: 'outline',
    },
    {
      id: 'quit',
      label: 'Quit Game',
      icon: '🚪',
      action: () => {
        callUE5Function('quit_game');
      },
      variant: 'destructive',
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
      setInputMode('keyboard');

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
          callUE5Function('log_message', 'Escape pressed on main menu');
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
  }, [selectedIndex, menuItems, activateMenuItem, callUE5Function]);

  // Gamepad navigation
  useEffect(() => {
    let gamepadCheckInterval: NodeJS.Timeout;
    let lastButtonStates: { [key: number]: boolean } = {};

    const checkGamepad = () => {
      const gamepads = navigator.getGamepads();
      const gamepad = gamepads[0]; // Use first connected gamepad

      if (gamepad) {
        setInputMode('gamepad');

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
          callUE5Function('log_message', 'B button pressed on main menu');
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
  }, [selectedIndex, menuItems, activateMenuItem, callUE5Function]);

  // Mouse hover updates selection
  const handleMouseEnter = (index: number) => {
    setInputMode('keyboard'); // Mouse = keyboard mode
    setSelectedIndex(index);
  };

  // Direct button click handler (bypasses animation delays)
  const handleButtonClick = useCallback((event: React.MouseEvent, item: MenuItem) => {
    event.preventDefault();
    event.stopPropagation();

    if (!item.disabled) {
      console.log(`[NEON] Button clicked: ${item.id}`);
      item.action();
    }
  }, []);

  // Animation variants
  const containerVariants = {
    hidden: { opacity: 0, scale: 0.9 },
    visible: {
      opacity: 1,
      scale: 1,
      transition: {
        duration: 0.5,
        ease: 'easeOut',
        staggerChildren: 0.1,
      },
    },
    exit: {
      opacity: 0,
      scale: 0.9,
      transition: { duration: 0.3 },
    },
  };

  const itemVariants = {
    hidden: { opacity: 0, x: -20 },
    visible: {
      opacity: 1,
      x: 0,
      transition: { duration: 0.3, ease: 'easeOut' },
    },
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-slate-950 via-purple-950 to-slate-950 flex items-center justify-center p-8 overflow-hidden">
      {/* Animated background particles */}
      <div className="absolute inset-0 overflow-hidden pointer-events-none">
        {[...Array(20)].map((_, i) => (
          <motion.div
            key={i}
            className="absolute w-1 h-1 bg-purple-500 rounded-full opacity-30"
            initial={{
              x: Math.random() * window.innerWidth,
              y: Math.random() * window.innerHeight,
            }}
            animate={{
              y: [null, Math.random() * window.innerHeight],
              opacity: [0.3, 0.1, 0.3],
            }}
            transition={{
              duration: Math.random() * 10 + 10,
              repeat: Infinity,
              ease: 'linear',
            }}
          />
        ))}
      </div>

      <AnimatePresence>
        {isVisible && (
          <motion.div
            className="relative z-10"
            variants={containerVariants}
            initial="hidden"
            animate="visible"
            exit="exit"
          >
            <Card className="glass-dark border-purple-500/30 shadow-2xl max-w-md w-full">
              <CardHeader className="text-center pb-4">
                <motion.div
                  initial={{ y: -20, opacity: 0 }}
                  animate={{ y: 0, opacity: 1 }}
                  transition={{ duration: 0.6, delay: 0.2 }}
                >
                  <CardTitle className="text-4xl font-bold text-white text-glow mb-2">
                    Noham Base Project
                  </CardTitle>
                  <p className="text-purple-300 text-sm">
                    {inputMode === 'gamepad' ? '🎮 Gamepad Mode' : '⌨️ Keyboard/Mouse Mode'}
                  </p>
                </motion.div>
              </CardHeader>

              <Separator className="bg-purple-500/20" />

              <CardContent className="pt-6 space-y-3">
                {menuItems.map((item, index) => (
                  <motion.div
                    key={item.id}
                    variants={itemVariants}
                    onMouseEnter={() => !item.disabled && handleMouseEnter(index)}
                  >
                    <Button
                      variant={item.variant}
                      className={`
                        w-full h-14 text-lg font-semibold
                        transition-all duration-200
                        ${selectedIndex === index && !item.disabled
                          ? 'ring-2 ring-purple-400 ring-offset-2 ring-offset-slate-900 glow'
                          : ''
                        }
                        ${item.disabled ? 'opacity-40 cursor-not-allowed' : 'game-button'}
                        ${!item.disabled ? 'hover:scale-105 active:scale-95' : ''}
                      `}
                      onClick={(e) => handleButtonClick(e, item)}
                      onMouseDown={(e) => e.stopPropagation()}
                      onMouseUp={(e) => e.stopPropagation()}
                      disabled={item.disabled}
                      style={{ pointerEvents: item.disabled ? 'none' : 'auto' }}
                    >
                      <span className="flex items-center justify-center gap-3">
                        <span className="text-2xl">{item.icon}</span>
                        <span>{item.label}</span>
                      </span>
                    </Button>

                    {item.disabled && (
                      <motion.p
                        initial={{ opacity: 0 }}
                        animate={{ opacity: 1 }}
                        className="text-xs text-slate-500 text-center mt-1"
                      >
                        No save data available
                      </motion.p>
                    )}
                  </motion.div>
                ))}
              </CardContent>

              {/* Input hints */}
              <div className="px-6 pb-4 pt-2">
                <Separator className="bg-purple-500/20 mb-3" />
                <div className="text-xs text-slate-400 text-center space-y-1">
                  {inputMode === 'keyboard' ? (
                    <>
                      <p>↑/↓ or W/S to navigate • Enter/Space to select • Tab to cycle</p>
                      <p>ESC to go back</p>
                    </>
                  ) : (
                    <>
                      <p>D-Pad or Left Stick to navigate • A button to select</p>
                      <p>B button to go back</p>
                    </>
                  )}
                </div>
              </div>
            </Card>

            {/* Version info */}
            <motion.div
              initial={{ opacity: 0 }}
              animate={{ opacity: 1 }}
              transition={{ delay: 1 }}
              className="text-center mt-4 text-slate-500 text-xs"
            >
              v1.0.0 Alpha • Built with NEON + shadcn/ui
            </motion.div>
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  );
}

// Mount React app
const container = document.getElementById('root');
if (container) {
  const root = createRoot(container);
  root.render(<MainMenu />);
} else {
  console.error('[MainMenu] Root container not found!');
}
