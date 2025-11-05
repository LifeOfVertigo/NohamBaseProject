import React from 'react';
import { createRoot } from 'react-dom/client';
import GraphicsSettings from './GraphicsSettings';
import AudioSettings from './AudioSettings';
import InputSettings from './InputSettings';
import SettingsManagement from './SettingsManagement';
import { SettingsProvider } from './SettingsContext';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/Components/shadcn/tabs';
import '../Styles/globals.css';

const container = document.getElementById('root');
if (container) {
  const root = createRoot(container);
  root.render(
    <SettingsProvider>
      <div className="min-h-screen bg-gradient-to-br from-slate-900 to-purple-900 p-8">
        <div className="max-w-4xl mx-auto">
          <h1 className="text-3xl font-bold text-white mb-8">Settings</h1>
          <Tabs defaultValue="graphics" className="w-full">
            <TabsList className="grid w-full grid-cols-4">
              <TabsTrigger value="graphics">Graphics</TabsTrigger>
              <TabsTrigger value="audio">Audio</TabsTrigger>
              <TabsTrigger value="input">Input</TabsTrigger>
              <TabsTrigger value="manage">Management</TabsTrigger>
            </TabsList>
            <TabsContent value="graphics">
              <GraphicsSettings />
            </TabsContent>
            <TabsContent value="audio">
              <AudioSettings />
            </TabsContent>
            <TabsContent value="input">
              <InputSettings />
            </TabsContent>
            <TabsContent value="manage">
              <SettingsManagement />
            </TabsContent>
          </Tabs>
        </div>
      </div>
    </SettingsProvider>
  );
}
