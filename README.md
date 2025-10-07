# HOI-Oversimplified

## Overview

HOI-Oversimplified is a C++ strategy game inspired by Hearts of Iron (HOI), developed as a university project for the Object-Oriented Programming (POO) course at the Faculty of Mathematics and Computer Science, University of Bucharest.

The project demonstrates advanced OOP principles through a modular simulation of nations managing resources, territories, and combat. It features a real-time graphical interface using SFML, including hex-based maps, UI panels and battle system.

## Core Features

- **Nation & Region System**: Nations control regions that produce resources and can build factories. Expand via combat and manage global influence.
- **Combat Mechanics**: Attack/defend regions. Battles are visualized in a dedicated interface.
- **Construction System**: Uses Factory Method and Singleton patterns for resource production via abstract `Construction` classes.
- **OOP Architecture**:
  - Encapsulation, inheritance, and polymorphism for units and constructions.
  - Templates for generic `Army<T>` handling.
  - Custom exceptions and dynamic casting.
- **Graphical Interface**: Interactive hex map with real-time updates for resources, popularity, and status using SFML.

## Concepts & Skills

- **Object-Oriented Programming**: Abstraction, encapsulation, inheritance, polymorphism.
- **Design Patterns**: Singleton, Factory Method, Observer-like logic.
- **C++ Advanced Features**: Templates, exception handling, copy/move semantics.
- **Game Development**: State management, rendering, input handling with SFML.

## Installation & Build

### Prerequisites

- CMake 3.10+
- Make (or equivalent build tool)
- C++17 compiler (e.g., g++, clang++)
- SFML library: Install via `sudo apt install libsfml-dev` (on Ubuntu/Debian) or equivalent for your OS.

### Build Steps

1. Clone the repository:
   ```
   git clone https://github.com/andrei65t/HOI-oversimplified.git
   cd HOI-oversimplified
   ```

2. Prepare the build directory:
   ```
   rm -rf build
   mkdir build && cd build
   ```

3. Configure and build:
   ```
   cmake ..
   make
   ```

## Running the Game

From the root folder:
```
./hoi
```
