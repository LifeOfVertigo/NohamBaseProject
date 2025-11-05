/**
 * Basic Menu Template
 *
 * A reusable template for creating simple menus in NEON with shadcn/ui.
 * Copy this file and customize for your specific menu needs.
 *
 * Usage:
 * 1. Copy to Content/UI/NEON/[YourMenu]/index.tsx
 * 2. Replace [MenuName] with your menu name
 * 3. Customize menuItems array
 * 4. Update styling/animations as needed
 * 5. Create corresponding index.html
 * 6. Add to vite.config.js build entries
 */

import React, { useState, useEffect, useCallback } from 'react';
import { createRoot } from 'react-dom/client';
import { motion, AnimatePresence } from 'framer-motion';
import { Button } from '@/Components/shadcn/button';
import { Card, CardContent, CardHeader, CardTitle } from '@/Components/shadcn/card';

// NEON Bridge TypeScript definitions
declare global {
  interface Window {
    ue?: {
      interface?: {
        [key: string]: (...args: any[]) => void;
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

function [MenuName]() {
  const [selectedIndex, setSelectedIndex] = useState(0);
  const [isVisible, setIsVisible] = useState(false);

  // Fade in on mount
  useEffect(() => {
    setIsVisible(true);
  }, []);

  // NEON Bridge helper
  const callUE5 = useCallback((functionName: string, ...args: any[]) => {
    if (window.ue?.interface?.[functionName]) {
      window.ue.interface[functionName](...args);
      console.log(`[NEON] Called: ${functionName}`, args);
    } else {
      console.warn(`[NEON] Function not available: ${functionName}`);
    }
  }, []);

  // Define your menu items here
  const menuItems: MenuItem[] = [
    {
      id: 'option1',
      label: 'Option 1',
      icon: '🎮',
      action: () => callUE5('your_function_name'),
      variant: 'default',
    },
    {
      id: 'option2',
      label: 'Option 2',
      icon: '⚙️',
      action: () => callUE5('another_function'),
      variant: 'secondary',
    },
    {
      id: 'back',
      label: 'Back',
      icon: '◀️',
      action: () => callUE5('go_back'),
      variant: 'outline',
    },
  ];

  // Keyboard navigation
  useEffect(() => {
    const handleKeyDown = (e: KeyboardEvent) => {
      switch (e.key) {
        case 'ArrowUp':
        case 'w':
          e.preventDefault();
          setSelectedIndex((prev) => (prev - 1 + menuItems.length) % menuItems.length);
          break;
        case 'ArrowDown':
        case 's':
          e.preventDefault();
          setSelectedIndex((prev) => (prev + 1) % menuItems.length);
          break;
        case 'Enter':
        case ' ':
          e.preventDefault();
          if (!menuItems[selectedIndex]?.disabled) {
            menuItems[selectedIndex]?.action();
          }
          break;
      }
    };

    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, [selectedIndex, menuItems]);

  // Animation variants
  const containerVariants = {
    hidden: { opacity: 0, scale: 0.9 },
    visible: {
      opacity: 1,
      scale: 1,
      transition: { duration: 0.5, staggerChildren: 0.1 },
    },
  };

  const itemVariants = {
    hidden: { opacity: 0, x: -20 },
    visible: { opacity: 1, x: 0, transition: { duration: 0.3 } },
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-slate-950 via-purple-950 to-slate-950 flex items-center justify-center p-8">
      <AnimatePresence>
        {isVisible && (
          <motion.div
            variants={containerVariants}
            initial="hidden"
            animate="visible"
            exit="hidden"
          >
            <Card className="glass-dark border-purple-500/30 shadow-2xl max-w-md w-full">
              <CardHeader className="text-center">
                <CardTitle className="text-3xl font-bold text-white text-glow">
                  [Menu Title]
                </CardTitle>
              </CardHeader>

              <CardContent className="space-y-3">
                {menuItems.map((item, index) => (
                  <motion.div
                    key={item.id}
                    variants={itemVariants}
                    onMouseEnter={() => !item.disabled && setSelectedIndex(index)}
                  >
                    <Button
                      variant={item.variant}
                      className={`
                        w-full h-12 text-lg font-semibold game-button
                        ${selectedIndex === index && !item.disabled ? 'ring-2 ring-purple-400 glow' : ''}
                        ${item.disabled ? 'opacity-40 cursor-not-allowed' : ''}
                      `}
                      onClick={() => !item.disabled && item.action()}
                      disabled={item.disabled}
                    >
                      <span className="flex items-center justify-center gap-2">
                        {item.icon && <span className="text-xl">{item.icon}</span>}
                        <span>{item.label}</span>
                      </span>
                    </Button>
                  </motion.div>
                ))}
              </CardContent>
            </Card>
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
  root.render(<[MenuName] />);
}
