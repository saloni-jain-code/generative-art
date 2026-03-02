# Generative Art Battery Installation
## Overview

This project is a real-time generative lyric visualizer for the song *“Yellow”* by Coldplay, built on an ESP32 with a display. The system synchronizes timed lyrics with animated background visuals to create an emotionally aligned viewing experience while listening to the song.

The visual design includes:

- A warm, dynamically shifting color palette (yellows, oranges, soft greens)
- Drifting animated stars to reflect the lyric “look at the stars”
- Highlighted instances of the word **“you”** using a complementary color
- A flickering animation effect for the word **“shine”**
- Carefully tuned typography and layout for readability on the target screen

The lyrics are manually synchronized to the song’s timestamps to ensure precise visual alignment with the music.
![look_at_stars](https://github.com/user-attachments/assets/5c7c093a-b14b-47b9-9607-a1e2c61f5e64)
![drew_line](https://github.com/user-attachments/assets/84d7fb55-a027-4ee5-859c-b5eb23d5312e)

## Hardware Requirements

- ESP32 TTGO T-display
- USB-C cable

## Software Requirements

- VS Code  
- PlatformIO extension for VS Code  
- ESP32 board support (installed via PlatformIO)  

## Project Files
- `platformio.ini` – PlatformIO configuration file (board, framework, libraries)
- `src/main.cpp` – Main application code containing:
  - Display initialization
  - Color generation logic
  - Star animation system
  - Lyric timing and rendering logic
  - Word-specific visual effects

## Reproducibility

1. Install PlatformIO
2. Open VS Code
3. Install the PlatformIO extension
4. Open a new PlatformIO project
5. Copy and paste the contents of the main.cpp code from this repo into the barebones main.cpp file under the src folder of the new project
6. Copy and paste the platformio.ini into the new project's platformio.ini file
7. Plug USB-C cable into ESP32 and upload the code onto the ESP32
8. Watch a generative interpretation of the lyrics of *Yellow*! 
