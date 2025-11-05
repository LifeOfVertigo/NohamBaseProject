import { createContext, useContext } from 'react';
import type { ReactNode } from 'react';

type MenuScreen = 'main' | 'new-game' | 'load-game' | 'pause' | 'settings';

interface NavigationContextType {
  navigate: (screen: MenuScreen) => void;
}

const NavigationContext = createContext<NavigationContextType | undefined>(undefined);

export function useNavigation() {
  const context = useContext(NavigationContext);
  if (!context) {
    throw new Error('useNavigation must be used within NavigationProvider');
  }
  return context;
}

interface NavigationProviderProps {
  children: ReactNode;
  navigate: (screen: MenuScreen) => void;
}

export function NavigationProvider({ children, navigate }: NavigationProviderProps) {
  return (
    <NavigationContext.Provider value={{ navigate }}>
      {children}
    </NavigationContext.Provider>
  );
}
