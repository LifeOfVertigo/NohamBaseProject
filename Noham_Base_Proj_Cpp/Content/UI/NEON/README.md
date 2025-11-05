# NEON UI Setup Instructions

This directory contains the NEON web UI for the Noham Base Project, built with shadcn/ui + Tailwind CSS.

## ✅ Configuration Files Created

The following configuration files have been set up:

- `package.json` - Node.js dependencies
- `components.json` - shadcn/ui configuration
- `tailwind.config.js` - Tailwind CSS configuration
- `postcss.config.js` - PostCSS configuration
- `vite.config.js` - Vite bundler configuration
- `Styles/globals.css` - Tailwind base styles with theme variables

## 📦 Installation Steps

### 1. Install Node.js Dependencies

```bash
cd "Content/UI/NEON"
npm install
```

This will install:
- Vite (bundler)
- Tailwind CSS (styling)
- shadcn/ui dependencies (lucide-react, clsx, tailwind-merge, etc.)

### 2. Install shadcn Components

```bash
bunx shadcn@latest add @shadcn/button @shadcn/card @shadcn/checkbox @shadcn/input @shadcn/badge @shadcn/separator
```

This will create the component files in `Components/shadcn/`

## 🚀 Development Workflow

### Start Development Server
```bash
npm run dev
```

### Build for Production
```bash
npm run build
```

Output will be in `dist/` directory

### Preview Production Build
```bash
npm run preview
```

## 📁 Directory Structure

```
Content/UI/NEON/
├── Components/
│   ├── shadcn/          ← shadcn components (created by install command)
│   ├── Game/            ← Game-specific components
│   └── Utils/           ← Utility functions and NEON bridge
│       └── neon-bridge.js
├── Developer/           ← Debug tools UI
├── MainMenu/            ← Main menu UI (future)
├── Settings/            ← Settings UI (future)
├── HUD/                 ← In-game HUD (future)
├── Styles/
│   └── globals.css      ← Tailwind base styles
├── dist/                ← Build output (gitignored)
├── node_modules/        ← Dependencies (gitignored)
├── package.json
├── components.json
├── tailwind.config.js
├── postcss.config.js
└── vite.config.js
```

## 🎨 shadcn Components Available

After installation, you'll have:

- **Button** - For actions and controls
- **Card** - Container component for sections
- **Checkbox** - For toggles and options
- **Input** - Text input fields
- **Badge** - Status indicators and labels
- **Separator** - Visual dividers

## 🔗 Integration with UE5

The NEON bridge (`Components/Utils/neon-bridge.js`) handles communication between the web UI and UE5 C++ subsystems.

### Usage in Components
```javascript
import { neonBridge } from '@utils/neon-bridge';

// Send command to UE5
neonBridge.toggleCollisionVisualization(currentState, (response) => {
    if (response.success) {
        console.log('Visualization toggled');
    }
});
```

## 🎯 Next Steps

1. Run `npm install` to install dependencies
2. Run the shadcn add command to install components
3. Implement the debug overlay UI in `Developer/debug-overlay.html`
4. Connect to UE5 `UNohamDeveloperToolsSubsystem` backend

## 📚 Documentation

- [shadcn/ui Documentation](https://ui.shadcn.com)
- [Tailwind CSS Documentation](https://tailwindcss.com)
- [Vite Documentation](https://vitejs.dev)
- Project coding standards: `docs/architecture/coding-standards.md`
- Tech stack details: `docs/architecture/tech-stack.md`
