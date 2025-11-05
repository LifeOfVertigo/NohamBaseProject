import React from 'react';
import { createRoot } from 'react-dom/client';
import { Button } from './Components/shadcn/button';
import { Card, CardHeader, CardTitle, CardDescription, CardContent, CardFooter } from './Components/shadcn/card';
import { Input } from './Components/shadcn/input';
import { Badge } from './Components/shadcn/badge';
import { Checkbox } from './Components/shadcn/checkbox';
import { Separator } from './Components/shadcn/separator';

function ShadcnTest() {
  const [checked, setChecked] = React.useState(false);
  const [inputValue, setInputValue] = React.useState('');

  const handleTestClick = () => {
    console.log('shadcn Button clicked!');
    // Test NEON bridge communication
    if (typeof (window as any).ue !== 'undefined' && (window as any).ue.interface) {
      (window as any).ue.interface.test_message('shadcn component test from React!');
    }
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-slate-900 via-purple-900 to-slate-900 p-8">
      <div className="max-w-4xl mx-auto space-y-6">
        {/* Header */}
        <div className="text-center mb-8">
          <h1 className="text-4xl font-bold text-white mb-2">
            🎨 shadcn/ui Component Test
          </h1>
          <p className="text-slate-300">
            Testing shadcn components in NEON + UE5 environment
          </p>
          <div className="flex justify-center gap-2 mt-4">
            <Badge variant="default">React</Badge>
            <Badge variant="secondary">shadcn/ui</Badge>
            <Badge variant="outline" className="text-white">NEON</Badge>
          </div>
        </div>

        {/* Button Variants */}
        <Card className="bg-slate-800/50 border-slate-700 backdrop-blur">
          <CardHeader>
            <CardTitle className="text-white">Button Components</CardTitle>
            <CardDescription className="text-slate-400">
              Different button variants and sizes from shadcn/ui
            </CardDescription>
          </CardHeader>
          <CardContent className="space-y-4">
            <div className="flex flex-wrap gap-3">
              <Button variant="default" onClick={handleTestClick}>
                Default Button
              </Button>
              <Button variant="secondary">Secondary</Button>
              <Button variant="destructive">Destructive</Button>
              <Button variant="outline">Outline</Button>
              <Button variant="ghost">Ghost</Button>
              <Button variant="link">Link</Button>
            </div>
            <Separator className="bg-slate-700" />
            <div className="flex flex-wrap gap-3">
              <Button size="sm">Small</Button>
              <Button size="default">Default</Button>
              <Button size="lg">Large</Button>
              <Button size="icon">📱</Button>
            </div>
          </CardContent>
        </Card>

        {/* Input & Form Components */}
        <Card className="bg-slate-800/50 border-slate-700 backdrop-blur">
          <CardHeader>
            <CardTitle className="text-white">Input Components</CardTitle>
            <CardDescription className="text-slate-400">
              Form inputs and interactive elements
            </CardDescription>
          </CardHeader>
          <CardContent className="space-y-4">
            <div className="space-y-2">
              <label className="text-sm font-medium text-white">Text Input</label>
              <Input
                type="text"
                placeholder="Enter your username..."
                value={inputValue}
                onChange={(e) => setInputValue(e.target.value)}
                className="bg-slate-900 border-slate-700 text-white placeholder:text-slate-500"
              />
              {inputValue && (
                <p className="text-sm text-slate-400">
                  Current value: <span className="text-white font-mono">{inputValue}</span>
                </p>
              )}
            </div>

            <Separator className="bg-slate-700" />

            <div className="flex items-center space-x-2">
              <Checkbox
                id="terms"
                checked={checked}
                onCheckedChange={(value) => setChecked(value as boolean)}
              />
              <label
                htmlFor="terms"
                className="text-sm font-medium text-white leading-none peer-disabled:cursor-not-allowed peer-disabled:opacity-70"
              >
                Accept terms and conditions
              </label>
            </div>
            {checked && (
              <Badge className="bg-green-600">✓ Terms accepted</Badge>
            )}
          </CardContent>
        </Card>

        {/* Card Layout Example */}
        <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
          <Card className="bg-slate-800/50 border-slate-700 backdrop-blur">
            <CardHeader>
              <CardTitle className="text-white text-lg">Feature 1</CardTitle>
              <CardDescription className="text-slate-400">
                Fast performance
              </CardDescription>
            </CardHeader>
            <CardContent className="text-slate-300">
              Optimized for real-time game UI with minimal overhead.
            </CardContent>
            <CardFooter>
              <Button variant="secondary" size="sm" className="w-full">
                Learn More
              </Button>
            </CardFooter>
          </Card>

          <Card className="bg-slate-800/50 border-slate-700 backdrop-blur">
            <CardHeader>
              <CardTitle className="text-white text-lg">Feature 2</CardTitle>
              <CardDescription className="text-slate-400">
                Modern design
              </CardDescription>
            </CardHeader>
            <CardContent className="text-slate-300">
              Beautiful components with Tailwind CSS styling.
            </CardContent>
            <CardFooter>
              <Button variant="secondary" size="sm" className="w-full">
                Learn More
              </Button>
            </CardFooter>
          </Card>

          <Card className="bg-slate-800/50 border-slate-700 backdrop-blur">
            <CardHeader>
              <CardTitle className="text-white text-lg">Feature 3</CardTitle>
              <CardDescription className="text-slate-400">
                UE5 Integration
              </CardDescription>
            </CardHeader>
            <CardContent className="text-slate-300">
              Seamless communication between UI and game logic.
            </CardContent>
            <CardFooter>
              <Button variant="secondary" size="sm" className="w-full">
                Learn More
              </Button>
            </CardFooter>
          </Card>
        </div>

        {/* NEON Bridge Test */}
        <Card className="bg-gradient-to-r from-purple-800/50 to-pink-800/50 border-purple-700 backdrop-blur">
          <CardHeader>
            <CardTitle className="text-white">NEON Bridge Test</CardTitle>
            <CardDescription className="text-purple-200">
              Test communication with UE5 C++ backend
            </CardDescription>
          </CardHeader>
          <CardContent>
            <Button
              variant="default"
              size="lg"
              onClick={handleTestClick}
              className="w-full bg-gradient-to-r from-purple-600 to-pink-600 hover:from-purple-700 hover:to-pink-700"
            >
              🚀 Send Test Message to UE5
            </Button>
          </CardContent>
          <CardFooter className="text-sm text-purple-200">
            Check UE5 Output Log for test message confirmation
          </CardFooter>
        </Card>
      </div>
    </div>
  );
}

// Mount React app
const container = document.getElementById('root');
if (container) {
  const root = createRoot(container);
  root.render(<ShadcnTest />);
} else {
  console.error('Root container not found!');
}
