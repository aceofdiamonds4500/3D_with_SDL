# 3D With SDL
Experimenting with different ideas, all of which are 3D related

## What I have so far
What I'm experimenting with right now is how DOOM renders its graphics, and so far
I have two view modes (top down perspective with player around world or world around
player movement)

## Controls
- Classic DOOM style controls
  - `W` - Forward
  - `S` - Backward
  - `A` - Strafe left
  - `D` - Strafe right
  - `Left and right` - Turn left and right
- View controls
  - `M` - change perspective

## TO-DO
**5-13-25**: The generation of the map is done in all three modes, but it's important that I tweak some field-of-view
things and work on making the line drawing method more efficient.

## Building

- navigate to `/build`
- type `make` in the console
- run `./GameEngine`

lol idk how you're gonna use this if you're on windows, good luck (jk i'll figure it out)