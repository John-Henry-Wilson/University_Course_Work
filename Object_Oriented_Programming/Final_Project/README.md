TheFinalsWeek

Executable File Download AND video presentation(Was too big for Github): https://drive.google.com/drive/folders/1RZnNCrwFrAD4Qz7Wp49Dn8UNa0pq4SlI?usp=sharing
Team:

Isabelle Brandes

Trevor Jedziniak

John Wilson

built on unity 2020.3.22f1


instructions to run: 

If unity editor is installed, then open the project in unity editor and click the play button on the top of the editor
Otherwise, a windows executable will be included to start the program

Patterns used:
Singleton - PlayerManager.cs  
Facade - Our ScoreUpdate.cs file manages the incrementing of the score on the surface, but it actually controls the display of the win screen and win condition class. 
Decorator - We have a base key type, and then a normal and jumpscare key. We also have a base jumpscare type, and then a crawling jumpscare and a hall jumpscare.

(We are aware that these implementations arnt textbook implementations of the patterns, but it is to be noted that we believe they still maintain the original purposes of the patterns themselves)

Known bugs:
If you run while attempting to pick up a key, the pick up will not register. 

WASD:movement  
F:flashlight  
ESC:pause  
Shift:Run  
C:crouch  


External Sources:
https://assetstore.unity.com/packages/3d/props/barprops-137130
https://assetstore.unity.com/packages/3d/props/furniture/folding-table-and-chair-pbr-111726
https://assetstore.unity.com/packages/3d/props/interior/free-pbr-lamps-70181
https://assetstore.unity.com/packages/3d/handpainted-keys-42044
https://assetstore.unity.com/packages/audio/sound-fx/minimal-ui-sounds-78266
https://assetstore.unity.com/packages/3d/characters/modular-first-person-controller-189884
https://assetstore.unity.com/publishers/4986
https://assetstore.unity.com/packages/2d/textures-materials/wood/15-original-wood-texture-71286
https://assetstore.unity.com/packages/3d/true-horror-crawler-70609
https://assetstore.unity.com/packages/3d/characters/humanoids/zombie-30232




https://www.youtube.com/watch?v=D0lx90n0s-4
https://www.youtube.com/watch?v=BTxqErZ1JqE used for toggle key
https://www.youtube.com/watch?v=j1-OyLo77ss referenced for enemy field of view calculation


