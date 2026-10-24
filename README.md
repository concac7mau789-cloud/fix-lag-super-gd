# Fix Lag Super (Geode mod for Geometry Dash)

Reduces lag by disabling particles and shaders, forcing Low Detail Mode in levels, cleaning memory, and optionally removing camera shake.

## Tutorial: how to download and install

### Option A: Build with GitHub (easiest, no setup, Windows only)
1. Create a free account at github.com and make a new repository.
2. Upload every file from this folder (keep the `.github/workflows/build.yml` path).
3. Open the **Actions** tab and wait for "Build Binaries" to finish (a few minutes).
4. Open the finished run and download **Build Output** under Artifacts.
5. Unzip it and find `yourname.fix-lag-super.geode`.
6. Put that file in your Geometry Dash `geode/mods` folder:
   - Windows: `Geometry Dash/geode/mods`
   - Android: `Android/media/com.geode.launcher/game/geode/mods`
   - macOS: `Geometry Dash/Contents/geode/mods` (or via the Geode mods menu)
7. Launch Geometry Dash. Open **Geode > Mods > Fix Lag Super > Settings** to tweak options.

### Option B: Build on your PC
1. Install Geode CLI and the Geode SDK: https://docs.geode-sdk.org/getting-started/
2. Run `geode sdk install`, then `geode sdk install-binaries`.
3. In this folder:
   ```
   geode build
   ```
4. The `.geode` file appears in `build/`. Copy it to `geode/mods` as above.

## Before building
- Change `"id"` and `"developer"` in `mod.json` (for example `yourname.fix-lag-super`).
- Make sure the `"gd"` version matches your game version and `"geode"` is not newer than your installed Geode.

## Troubleshooting
- Compile error in the `CCParticleSystemQuad` block: the `draw` hook may not match your bindings version. Delete that block and the `disable-particles` setting in `mod.json`.
- A shader still appears in some level: add that trigger's object ID to `isShaderTrigger` in `src/main.cpp`. Shader changes only apply when a level loads.
- A level misbehaves with shaders disabled: turn off **Disable Shaders** in the mod settings.
- Compile error on `shakeCamera`: the signature can change between GD versions. Delete the `FixLagBaseGameLayer` block in `src/main.cpp` and the `disable-shake` setting in `mod.json`.
- Compile error on `m_performanceMode`: check the member name in your bindings version, or remove the Low Detail block.
- No effect on lag: the mod cannot fix lag caused by very heavy levels, a weak GPU, or an unstable internet connection.
