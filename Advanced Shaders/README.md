# Shader Hello World (URP) – README

## Target
- **Unity**: 2022.3.40f1 (LTS)
- **Render Pipeline**: Universal Render Pipeline (URP)

## Goal
A sphere with a scrolling rainbow gradient authored in **Shader Graph (Unlit)**.

## Files You May Add
- `Assets/Scripts/HelloWorldSceneBootstrap.cs`
- `Assets/Scripts/HelloWorldSpeedController.cs`
- *(Optional)* `Assets/Scripts/SimpleCapture.cs`
- *(Optional)* `Assets/Shaders/HelloWorldCode.shader`

## Steps
1) Create a **URP** project in **Unity 2022.3.40f1**.  
2) In **Project**: **Create → Shader → Universal Render Pipeline → Unlit Shader Graph**.  
   - Name it: **HelloWorldShader**.  
3) Open the graph and add nodes as follows:
   - `UV → Split → R → Add.A`
   - `Time (Time pin) → Multiply.A`
   - Float property **Speed** (default **1.0**) → `Multiply.B`
   - `Multiply → Add.B`
   - *(Optional)* `Add → Fraction → Sample Gradient.Time`
   - `Gradient → Sample Gradient.Gradient`
   - `Sample Gradient.Color → Fragment.Base Color`
4) **Save the graph** (top‑left **Save Asset**).  
5) Create an empty **GameObject** and add **HelloWorldSceneBootstrap**.
   - Press **Play** → Sphere is created, material set to Shader Graph, camera positioned.
   - Adjust **Speed** at runtime via **HelloWorldSpeedController** on the sphere.
6) **Screenshots/Frames (optional)**: add **SimpleCapture** to any object.
   - **P** = Screenshot to `Project/Screenshots/`
   - **F10** = Toggle frame dump to `Project/Frames/`

## Recording a 30–60 second MP4 (Unity Recorder)
1) **Window → General → Recorder → Recorder Window**  
2) **Add Recorder → Movie (MP4/H.264)**, **Source: Game View**, **30 FPS**  
3) **Record Mode**: **Time Interval (30–60 s)**  
4) **Start Recording**, navigate with the camera

## Troubleshooting
- **Speed not changing**: confirm the Shader Graph property Reference is `_Speed` and the material uses your graph.  
- **Wires won't connect to `Sample Gradient.Time`**: ensure your Add/Fraction output is **Vector1** (use `Split.R`).  
- **Colors are dull**: ensure the material is **Unlit** and gradient stops are at full intensity **(1.0)**, not low HDR brightness.  
- **Time is slow**: increase **Speed** (the graph uses Unity’s `TimeParameters.x`).

— End —
