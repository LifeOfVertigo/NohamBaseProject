# Quick Build Commands for NEON

## When Working on UI (Fast Iteration)
```bash
npm run dev
```
- Opens dev server at `http://localhost:5173`
- Live reload when you save files
- Use your browser to see changes instantly
- **NOT for Unreal Engine testing**

## When Testing in Unreal Engine
```bash
npm run build:ue
```
- Builds production bundle
- Automatically copies to NEON folder
- **Use this before testing in UE**
- No dev server needed

## Manual Build (if needed)
```bash
npm run build                    # Build to dist/
npm run copy:ue                  # Copy to NEON folder
```

## For Players/Distribution
**Nothing!** The bundled files in `dist/` folder are automatically included when you package your game.

---

## What Each Command Does

| Command | Use Case | Server Needed? |
|---------|----------|----------------|
| `npm run dev` | UI development in browser | Yes (auto-starts) |
| `npm run build:ue` | Testing in Unreal Engine | No |
| Packaged game | Player experience | No |

## Your Typical Workflow

1. **Start UI work:**
   ```bash
   npm run dev
   ```
   Edit React files, see changes in browser instantly

2. **Ready to test in UE:**
   ```bash
   npm run build:ue
   ```
   Open Unreal Engine, test your UI

3. **Repeat:**
   - Make changes → `npm run build:ue` → test in UE
   - For quick UI tweaks, use `npm run dev` in browser first
   - Then `npm run build:ue` when ready to test in Unreal

4. **Ship it:**
   - Package your game
   - Players get the bundled files
   - No web server required!
