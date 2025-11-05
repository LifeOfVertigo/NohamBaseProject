import type { ReactNode } from 'react';
import { Card, CardContent } from '@/components/ui/card';

interface MenuLayoutProps {
  title: string;
  children: ReactNode;
  maxWidth?: 'sm' | 'md' | 'lg' | 'xl' | '2xl';
}

export function MenuLayout({ title, children, maxWidth = 'md' }: MenuLayoutProps) {
  const widthClasses = {
    sm: 'max-w-sm',
    md: 'max-w-md',
    lg: 'max-w-lg',
    xl: 'max-w-xl',
    '2xl': 'max-w-2xl',
  };

  return (
    <div className="min-h-screen bg-background flex flex-col items-center justify-center p-8 gap-16">
      {/* Title */}
      <h1 className="text-8xl font-bold text-foreground">
        {title}
      </h1>

      {/* Content Card */}
      <Card className={`${widthClasses[maxWidth]} w-full`}>
        <CardContent className="pt-6 space-y-3">
          {children}
        </CardContent>
      </Card>
    </div>
  );
}
