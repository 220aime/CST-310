# Unity Hello World Shader Project

A beginner-friendly animated rainbow gradient shader built with Unity's Shader Graph and interactive UI controls.

## Overview

This project demonstrates fundamental shader programming concepts by creating an animated color gradient that moves across a 3D sphere. Perfect for developers transitioning from WebGL to Unity or learning shader development basics.

**Features:**
- Animated rainbow gradient using UV coordinates
- Real-time speed control via UI elements
- Visual Shader Graph implementation
- Interactive controls (sliders, toggles, buttons)
- Optimized rendering with MaterialPropertyBlock

## Quick Start

### Prerequisites
- Unity 2022.3 LTS or newer
- Universal Render Pipeline (URP)

### Setup
1. **Create New Project**
   ```
   Unity Hub → New Project → 3D (URP) → Name: "ShaderHelloWorld"
   ```

2. **Scene Setup**
   - Add 3D Sphere to scene
   - Position camera at (0, 0, -3)

3. **Create Shader**
   - Assets → Create → Shader → Universal Render Pipeline → Unlit Shader Graph
   - Name: "HelloWorldShader"

4. **Apply SelectedHelper Script**
   - Add SelectedHelper.cs to a UI element
   - Assign sphere's renderer to TargetRenderers list

## Project Structure

```
Assets/
├── Shaders/
│   ├── HelloWorldShader.shadergraph
│   └── HelloWorldMaterial.mat
├── Scripts/
│   └── SelectedHelper.cs
├── UI/
│   └── Control elements
└── Scenes/
    └── Main.unity
```

## Controls

| Control Type | Method | Description |
|-------------|--------|-------------|
| **Slider** | `SetSpeedFromSlider()` | Continuous speed control |
| **Toggle** | `ApplyToggleSpeed()` | On/off animation toggle |
| **Button** | `SetSpeedTo(float)` | Direct speed setting |
| **Keyboard** | `NudgeSpeed(float)` | Incremental adjustments |

## Usage Example

```csharp
// Get SelectedHelper component
var helper = GetComponent<SelectedHelper>();

// Set animation speed
helper.SetSpeed(2.0f);

// Connect to UI slider
slider.onValueChanged.AddListener(helper.SetSpeedFromSlider);

// Connect to toggle
toggle.onValueChanged.AddListener(helper.ApplyToggleSpeed);
```

## Customization

### Shader Parameters
- **Speed**: Animation velocity (0-5 recommended)
- **Gradient**: Color stops and transitions
- **UV Channel**: Horizontal (U) or Vertical (V) animation

### Gradient Colors
Default: Red → Yellow → Blue → Red
```
Position 0.0:  Red (1,0,0)
Position 0.33: Yellow (1,1,0) 
Position 0.66: Blue (0,0,1)
Position 1.0:  Red (1,0,0)
```

## Learning Resources

- **[Technical Documentation](DOCUMENTATION.md)** - Detailed mathematical concepts and algorithms
- **Unity Shader Graph Manual** - Official Unity documentation
- **Graphics Programming Fundamentals** - UV mapping, color theory, animation

## Troubleshooting

| Issue | Solution |
|-------|----------|
| Shader appears black | Check Shader Graph connections and save |
| No animation | Verify Time node connection and Speed > 0 |
| Material not found | Ensure shader is saved before creating material |
| UI controls not working | Check TargetRenderers list assignment |

## Contributing

1. Fork the repository
2. Create feature branch (`git checkout -b feature/new-effect`)
3. Commit changes (`git commit -am 'Add new effect'`)
4. Push to branch (`git push origin feature/new-effect`)
5. Create Pull Request

## Acknowledgments
- WebGL/OpenGL shader tutorials and resources


