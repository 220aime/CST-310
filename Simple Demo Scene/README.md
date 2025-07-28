# Project 8: Simple Demo Scene - Exploring the Universe

"Exploring the Universe" is a real-time 3D space scene featuring a complete solar system, stellar evolution, space exploration, and dynamic cosmic phenomena. Built with Three.js.

## Features

### Solar System
- Sun with animated solar flares and corona
- 8 planets with realistic oblique orbits at different speeds
- Earth with orbiting Moon
- Planet rings for Saturn, Jupiter, and Uranus
- Some planets with dynamic size changes

### Stellar Evolution
- Continuous star birth from nebulae
- Star aging with color and size changes
- Supernova explosions
- Massive white star with cascading particle destruction
- 3-generation chain reaction system
- Automatic particle cleanup for performance

### Space Objects
- 6 robotic astronauts in two groups of 3 holding hands
- 10 advanced exploration satellites with solar panels
- Traditional space stations
- 100+ asteroids in asteroid belt
- Comets with streaming tails
- Background spiral galaxy
- 10,000 procedural stars

### Visual Effects
- 6 types of space flashes (gamma rays, solar flares, lightning)
- Particle systems for explosions and energy effects
- Dynamic lighting and shadows
- Realistic physics for object movement

### Camera System
- 4 automatic camera modes: Exploration, Galaxy Focus, Sun Focus, Planet Chase
- 15-second cycle between modes
- Dynamic zooming and transitions
- No manual controls required

## Installation

### Requirements
- Modern web browser with WebGL support
- Internet connection for Three.js CDN

### Quick Start
1. Download or clone the project files
2. Open `index.html` in a web browser

### Local Server (Recommended)
```bash
# Python 3
python -m http.server 8000

# Node.js
npx serve .

# PHP
php -S localhost:8000
```

Then navigate to `http://localhost:8000`

## Controls

The scene runs automatically with no user input required. Optional controls:
- Mouse wheel: Adjust camera movement speed

## Technical Details

### Dependencies
- Three.js r128 (loaded via CDN)
- No additional libraries required

### Browser Support
- Chrome 60+
- Firefox 55+
- Safari 12+
- Edge 79+

### Performance
- Optimized particle systems
- Automatic cleanup of old objects
- Maximum limits on simultaneous effects
- Efficient memory management

## Customization

### Adding New Objects
Edit the relevant creation functions in the JavaScript:
- `createSolarSystemPlanets()` - Add more planets
- `createRoboticHumans()` - Modify astronaut groups
- `createSpaceFlashes()` - Add new flash types

### Camera Settings
Modify camera variables:
- `cameraSpeed` - Overall movement speed
- Camera mode durations in the animation loop

### Visual Effects
Adjust particle counts and effect parameters in:
- Star creation functions
- Explosion systems
- Flash generation

## License

This project is open source. Feel free to use and modify.

## Credits

Built with Three.js library for 3D graphics rendering.
