# Real‑Time Fur Rendering – README

## Software Requirements
- **Unity**: 2021.3 LTS or newer (URP recommended)
- **Render Pipeline**: Universal Render Pipeline (URP)
- **Platforms**: Windows/macOS; optional Android (mobile) and Quest 2 (VR)
- **GPU**: Desktop class for 32–48 shells; mobile class for 8–16 shells
- **Packages/Settings**
  - Enable **SRP Batcher** in the URP Asset
  - Project Color Space: **Linear** (recommended)

## Project Structure
```
Assets/Fur/
  ├─ Shader/
  │   └─ FurShaderVariants.shader
  ├─ Materials/
  │   └─ Fur_Mat.mat
  ├─ FurCoreMain.cs
  ├─ FurLOD.cs
  ├─ FurBatcher.cs
  ├─ FurDebugger.cs
  ├─ FurProfiler.cs
  └─ (optional) FurPhysics.compute
```

## Setup & Running
1. Create a new **URP** project in Unity.
2. **Add a Sphere** to the scene.
3. Create a **Material** `Fur_Mat` and assign shader: `AdvancedFur/FurShaderVariants`.
4. Import textures:
   - **Fur Pattern**: grayscale noise (sRGB Off, Wrap Repeat, Mipmaps On).
   - **Tiger Color Map** (optional): stripes (sRGB On, Wrap Repeat).
5. Select the **Sphere**:
   - Add **FurCoreMain**. Drag `Fur_Mat` into **Fur Material**.
   - Click **Rebuild Shells** (context menu) or press **Play**.
6. Tuning for fat fur:
   - `Fur_Mat`: `_FurLength 0.35–0.45`, `_FurDensity 80–96`, `_Cutoff 0.45`, `_CutoffEnd 0.85`.
   - `FurCoreMain`: `shellCount 40–48` (desktop), `gravity 0.15–0.25`, `enableWind` on.
   - For tiger stripes: assign **_ColorMap** and adjust `_StripeToLength`, `_StripeToDensity`, `_StripeContrast`.
7. **Add LOD**:
   - Add **FurLOD** to the sphere. Distances: near 8, mid 16, far 30.
   - Shell tiers: near 40, mid 16, far 8, very far 4.
   - Optionally disable physics beyond mid.
8. **Batching**:
   - Add **FurBatcher** and enable **GPU Instancing**.
   - For maximum batching, refactor to shared material + MaterialPropertyBlock (advanced).
9. **Debug & Profile**:
   - Add **FurDebugger** for runtime sliders.
   - Add **FurProfiler** to see CPU/GPU timing.
10. **Build**: File → Build Settings → select platform → Build.

## Controls & Tips
- If fur looks dark, ensure there is a **Directional Light** and the shader uses `GetMainLight()` properly.
- If shells fight each other, verify per‑shell renderQueue = `3000 + shellIndex` and the shader’s z‑bias line `positionHCS.z += _ShellIndex * 1e-4`.
- Mobile tips: reduce shells (8–12), reuse texture samples, keep density lower, consider disabling wind/physics.

## License / Attribution
School project. feel free to use it for educational purpose :)

