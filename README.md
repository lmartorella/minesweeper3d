# Minesweeper 3D

A 3D clone of the Microsoft Windows Minesweeper game, built back in 2001/2002 just for fun.

Notable features for that juvenile time:

- OpenGL interop with MFC
- Dynamic library loading of maps with MSVC C++ API
- Null-soft installer (NSIS)
- Developed in Visual Studio 6

A later (not concluded) attempt to port the game to the novel C# language was made in 2002, and the effort ended with the collaboration of the Gtk# team to develop the GtkGlExt C# wrapper to run on Linux with the new-born Mono project.

## Code structure

The game engine was written to run with a generic rendering engine, and on generic tasseleted maps in order to increase the playability.

The game target was to use the same fast mouse gestures (move on a 2D surface and click left/right buttons to flag mines and to discover free areas). In order to support free-form 3D maps, each map was containing the logic to pan the camera over the 3D map only using the 2D mouse movement.

Basically, the mouse pointer would then hover/pan over the 3D map without requiring additional keyboard or mouse gestures. This allow the fastest response from a "classic" Minesweeper pro player.

## Maps

Each map was described by the algorithm to tessellate a sphere (for the N-th grade icosahedron iterations) or a toroid (for the toroid maps).

In addition, a classic 2D map was included to benchmark the response times of the program.

## Textures

The (ugly) textures was actually an exact pixel-by-pixel rip-off of the standard Microsoft game, waiting for an artist to redraw them in high-resolution.
