# LegacyEngine

Legacy Engine is a 3D Graphics Engine created by Jordi Bach and Adrián Mirabel for Engines subject at [UPC CITM Tech Talent Center](https://www.citm.upc.edu/) in Barcelona.

## Team Members
- [Adrián Mirabel](https://github.com/M1R4B3L)
- [Jordi Bach](https://github.com/bottzo)

You can find our engine code in here: 
[Reporsitory](https://github.com/M1R4B3L/LegacyEngine).

This is our Engine
[Web Page](https://m1r4b3l.github.io/LegacyEngine/).

## Camera Controls
### Mouse
- MiddleButtonDrag: Move the camera in the plane of the camera frame.
- RightButtonDrag: Rotates the camera.
- WheelUP: Zoom camera in.
- WheelDown: Zoom camera out.

### Mouse & Keyboard
- RightButtonDrag + WASD: Move camera forward, backwards, sideways and vertically.

### Keyboard & Mouse
- LShift + Movement Action: Multiplies the speed. (No zoom or rotation)
- Left Alt + LeftDrag: Rotates around reference.

### Keyboard 
- F: Focus on the selected game object

### Engine Info/Controls
- The Main Menu bar at the top 
	- File 
	- Edit (change engine configurations)
	- Create (create objects, cameras...)
	- Window (manage engine windows)
	- Help (information about the engine)

- Hierarchy (Shows the list of objects in the current scene)
	- Left click (Select/Drag&Drop)
	- Right click (Delete/Create child)

- Middle bar (Plays and Pauses the scene)

- Inspector (Game object components, not all the objects have all the componets. You can remove/add them)
	- Transform (Position, Rotation & Scale)
	- Mesh
	- Material (texture)
	- Camera
	- Script 

- Assets (List with all the assests save in the assets folder)
	- Meshes
	- Models
	- Scenes
	- Scripts
	- Shaders
	- Textures

- Console (Shows debug information)

### Special information
- You can create objects from 0 if you start from an Empty object in the Main Menu
- We can only import meshes to the scene if you want to import scripts or textures you will need a mesh or a game object.
- To import meshes to the scene just press the buttons in the Assets Folder.
!!!If the root is selected the mesh will not be imported.
- City_building_10/City_building_16/City_building_17 we don't handle 2 meshes
- We don't handle materials  


