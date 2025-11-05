import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import path from 'path';

export default defineConfig({
  plugins: [react()],
  root: '.',
  server: {
    // Ensure CORS is properly configured for NEON
    cors: true,
    headers: {
      'Access-Control-Allow-Origin': '*',
    },
  },
  build: {
    outDir: 'dist',
    // Inline all assets for CEF compatibility
    assetsInlineLimit: 100000000, // 100MB - forces inline
    cssCodeSplit: false,
    rollupOptions: {
      input: {
        testShadcn: path.resolve(__dirname, 'test-shadcn.html'),
        mainMenu: path.resolve(__dirname, 'MainMenu/index.html'),
        settings: path.resolve(__dirname, 'Settings/index.html'),
      },
    },
  },
  resolve: {
    alias: {
      '@': path.resolve(__dirname, './'),
      '@/Components': path.resolve(__dirname, './Components'),
      '@/Components/Utils': path.resolve(__dirname, './Components/Utils'),
      '@/Components/shadcn': path.resolve(__dirname, './Components/shadcn'),
    },
  },
});
