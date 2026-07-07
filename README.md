# GAS_Dura

GAS_Dura is an Unreal Engine RPG practice project built around Unreal's Gameplay Ability System. The project is used to study RPG combat architecture, ability activation, attribute handling, UI binding, input, AI, motion warping, and gameplay feedback in a real UE project.

## Project Status

This repository is a learning and practice project. The current Unreal project file targets Unreal Engine 5.3 and enables the core plugins/modules needed for a GAS-based RPG prototype.

## Engine Version

- Unreal Engine 5.3
- Visual Studio toolchain configured through `.vsconfig`

## Main Unreal Modules and Plugins

From `Dura.uproject`, the project depends on:

- GameplayAbilities
- EnhancedInput
- UMG
- AIModule
- Niagara
- MotionWarping
- ModelViewViewModel

## Setup

1. Install Unreal Engine 5.3.
2. Install Visual Studio with the components listed in `.vsconfig`.
3. Clone this repository.
4. Right-click `Dura.uproject` and generate Visual Studio project files.
5. Open the generated solution or open `Dura.uproject` directly from Unreal Editor.
6. Build the `Dura` module from Visual Studio or Unreal Editor.

## Repository Notes

Generated Unreal directories are ignored by `.gitignore`, including:

- `Binaries`
- `DerivedDataCache`
- `Intermediate`
- `Saved`
- `Build`
- `.vs`
- generated solution/project files

This keeps the repository focused on source assets and project configuration rather than local build artifacts.

## Learning Focus

- Gameplay Ability System architecture
- Attribute and effect design
- RPG character combat flow
- Enhanced Input integration
- Gameplay UI with UMG and MVVM
- AI and combat behavior experiments
- Motion Warping and gameplay animation support
## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

Unless otherwise noted, third-party dependencies, assets, course/tutorial materials, and externally sourced resources remain under their original licenses.
