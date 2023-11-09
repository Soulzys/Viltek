# Viltek

# Project description

This project is another procedural generation project and, as such, should be regarded as complementary to the ProArchPro project.  

***Why is that ?*** Because ProArchPro is about filling an already designed room space procedurally, whereas Viltek's project is about generating rooms in a procedural way. The ultimate goal of this procedural generation serie is therefore to be able to fully procedurally generate a whole level, most likely in the shape of a set of rooms linked with corridors.  
One of the heaviest source of inspiration for this project is the manga BLAME! 


# Project instructions

When you run the project, you will be faced with a input field to enter a text message. You need to input "play" (no case sensitive") and press the ENTER key to effectively start the game.  

This project is still a work in progress. Consequently, it is advised not to mess with it too much as it is likely to end up with crashes. However, you can safely go to the *SpectatorGameMode.cpp* and modify the *FStartingRoomBuildOptions* as you please. This would allow you to observe the various room shapes that can be generated with the algorithm. 

### Notes

As the rooms walls are made using static meshes and that collisions are enabled, moving in the level by controlling the original *ABaseSpectator* pawn can prove not to be the most efficient way. It is therefore advised to detach yourself from the player controller, which can be done by pressing F8 while in the game when playing in the editor. 
