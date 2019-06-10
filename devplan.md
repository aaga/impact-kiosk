# Development Plan

## Overview

### UI sketch
![ui sketch](https://i.ibb.co/342HCYM/ac-I6-J6p-LRBur-LWxm0-Lr-R7g-thumb-1533.jpg)

### Key features
- 1280x720 UI
- OpenFrameworks application
  - ofxGUI
  - ofVideoGrabber (webcam)
  - ofxCV (image processing)
- Optimized for and tested on Mac (but should compile on any OpenFrameworks platform)
- Radar chart for data visualization
  - use [Chart Director](https://www.advsofteng.com/doc/cdcpp.htm) to generate png (on separate thread)
- JSON for loading static info (questions)

## Development timeline
### In order of priority:
1. [x] Load JSON into standard C++ data structure, display question on screen
2. [x] Add YES/NO buttons, cycle through questions
3. [x] Generate basic Radar Chart
4. [ ] Capture image from webcam, apply basic filters
5. [ ] Tie pieces together (answer question, update graph, update image, display new question)
6. [ ] Add Welcome screen ("Type in your name..." etc.)
7. [ ] Add Summary screen ("Thank you for participating. Here are your results...")

### Stretch goals
- Timeout and reset for new users
- Fancy image shaders, live video feed
- Animations and transition (make it less jumpy)
