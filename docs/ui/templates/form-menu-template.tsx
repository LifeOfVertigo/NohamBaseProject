/**
 * Form Menu Template
 *
 * Template for menus with input fields (settings, character creation, etc.)
 * Includes form state management and validation patterns.
 *
 * Features:
 * - Text inputs
 * - Checkboxes
 * - Sliders (would need shadcn slider component)
 * - Form validation
 * - Save/Cancel buttons
 */

import React, { useState, useCallback } from 'react';
import { createRoot } from 'react-dom/client';
import { motion } from 'framer-motion';
import { Button } from '@/Components/shadcn/button';
import { Card, CardContent, CardHeader, CardTitle, CardFooter } from '@/Components/shadcn/card';
import { Input } from '@/Components/shadcn/input';
import { Checkbox } from '@/Components/shadcn/checkbox';
import { Separator } from '@/Components/shadcn/separator';

declare global {
  interface Window {
    ue?: {
      interface?: {
        save_settings?: (settings: any) => void;
        cancel_settings?: () => void;
      };
    };
  }
}

interface FormData {
  playerName: string;
  enableSounds: boolean;
  enableMusic: boolean;
  difficulty: string;
}

function [FormMenuName]() {
  const [formData, setFormData] = useState<FormData>({
    playerName: '',
    enableSounds: true,
    enableMusic: true,
    difficulty: 'normal',
  });

  const [errors, setErrors] = useState<Partial<Record<keyof FormData, string>>>({});

  const callUE5 = useCallback((functionName: string, ...args: any[]) => {
    if (window.ue?.interface?.[functionName]) {
      (window.ue.interface as any)[functionName](...args);
    }
  }, []);

  // Validate form
  const validate = (): boolean => {
    const newErrors: Partial<Record<keyof FormData, string>> = {};

    if (formData.playerName.length < 3) {
      newErrors.playerName = 'Name must be at least 3 characters';
    }
    if (formData.playerName.length > 20) {
      newErrors.playerName = 'Name must be less than 20 characters';
    }

    setErrors(newErrors);
    return Object.keys(newErrors).length === 0;
  };

  // Handle save
  const handleSave = () => {
    if (validate()) {
      callUE5('save_settings', formData);
    }
  };

  // Handle cancel
  const handleCancel = () => {
    callUE5('cancel_settings');
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-slate-950 via-purple-950 to-slate-950 flex items-center justify-center p-8">
      <motion.div
        initial={{ opacity: 0, scale: 0.9 }}
        animate={{ opacity: 1, scale: 1 }}
        transition={{ duration: 0.5 }}
      >
        <Card className="glass-dark border-purple-500/30 shadow-2xl max-w-lg w-full">
          <CardHeader className="text-center">
            <CardTitle className="text-3xl font-bold text-white text-glow">
              [Form Title]
            </CardTitle>
          </CardHeader>

          <CardContent className="space-y-6">
            {/* Text Input Example */}
            <div className="space-y-2">
              <label className="text-sm font-medium text-white">
                Player Name
              </label>
              <Input
                type="text"
                placeholder="Enter your name..."
                value={formData.playerName}
                onChange={(e) => setFormData({ ...formData, playerName: e.target.value })}
                className="bg-slate-900 border-slate-700 text-white"
              />
              {errors.playerName && (
                <p className="text-xs text-red-400">{errors.playerName}</p>
              )}
            </div>

            <Separator className="bg-purple-500/20" />

            {/* Checkbox Examples */}
            <div className="space-y-3">
              <div className="flex items-center space-x-2">
                <Checkbox
                  id="sounds"
                  checked={formData.enableSounds}
                  onCheckedChange={(checked) =>
                    setFormData({ ...formData, enableSounds: checked as boolean })
                  }
                />
                <label htmlFor="sounds" className="text-sm text-white">
                  Enable Sound Effects
                </label>
              </div>

              <div className="flex items-center space-x-2">
                <Checkbox
                  id="music"
                  checked={formData.enableMusic}
                  onCheckedChange={(checked) =>
                    setFormData({ ...formData, enableMusic: checked as boolean })
                  }
                />
                <label htmlFor="music" className="text-sm text-white">
                  Enable Music
                </label>
              </div>
            </div>

            <Separator className="bg-purple-500/20" />

            {/* Button Group Example */}
            <div className="space-y-2">
              <label className="text-sm font-medium text-white">
                Difficulty
              </label>
              <div className="grid grid-cols-3 gap-2">
                <Button
                  variant={formData.difficulty === 'easy' ? 'default' : 'outline'}
                  onClick={() => setFormData({ ...formData, difficulty: 'easy' })}
                  className="game-button"
                >
                  Easy
                </Button>
                <Button
                  variant={formData.difficulty === 'normal' ? 'default' : 'outline'}
                  onClick={() => setFormData({ ...formData, difficulty: 'normal' })}
                  className="game-button"
                >
                  Normal
                </Button>
                <Button
                  variant={formData.difficulty === 'hard' ? 'default' : 'outline'}
                  onClick={() => setFormData({ ...formData, difficulty: 'hard' })}
                  className="game-button"
                >
                  Hard
                </Button>
              </div>
            </div>
          </CardContent>

          <CardFooter className="flex gap-3">
            <Button
              variant="outline"
              onClick={handleCancel}
              className="flex-1 game-button"
            >
              Cancel
            </Button>
            <Button
              variant="default"
              onClick={handleSave}
              className="flex-1 game-button glow-hover"
            >
              Save
            </Button>
          </CardFooter>
        </Card>
      </motion.div>
    </div>
  );
}

const container = document.getElementById('root');
if (container) {
  const root = createRoot(container);
  root.render(<[FormMenuName] />);
}
