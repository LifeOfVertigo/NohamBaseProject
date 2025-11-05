import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import './index.css'
import App from './App.tsx'
// @ts-ignore - NEON library from Unreal
import NEON from './neon.js'

// Make NEON globally available for UE5 integration
(window as any).NEON = NEON;
console.log('[NEON] Library loaded and attached to window');

createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <App />
  </StrictMode>,
)
