# CS-EE-120B-Custom-Project
LCD Racer
Descriptions:

In my custom lab project I have created a racing game on the LCD screen. Player 1 will navigate through the randomly generated obstacles and will have to avoid being hit by walls, broken walls, and the second player’s bullet. Player 1 will have to move up and down on four separate lanes. As time progresses the game speeds up as well and when player one loses, a death animation is played and their score is displayed. Player one is equipped with a gun that can destroy broken walls and player’s 2 bullet as well. Player 2 has the freedom to move up and down as well and shoot in whichever lane they choose. There will also be custom characters to represent the two players, the bullets, and the randomly generated terrain. 3 State machines were used in to making this game. One for button presses, one for the real game state, and another one the handle the visuals. I used custom characters to represent the two players, the walls, the broken walls, and both bullets. The terrain is randomly generated with a seed that is calculated when the player starts the game. From start up the game will keep count until the game starts and that is used as the seed.

User Guide:

Rules: Player 1 objective is to survive as long as possible while avoiding and shooting obstacles in their way. Player 2 is to try to shoot player 1 down.

Controls:  Player 1 has up, down, shoot, and pause(PINB 1 - 4). Player 2 has to control the keypad and their buttons are to move up, down, shoot, and a soft reset(4,7,8, and D).

Some Considerations: Player 2 and player 1 will have to wait for their bullets to disappear before shooting again or else their bullet will disappear mid flight.

Technologies and Components Used: 
AVR Studio(Atmel Studio 6.2)
ATmega 1284
Keypad
LCD Screen
Buttons
Wires
Potentiometer
AVR ISP mkII

Link to Video Demonstration
https://www.youtube.com/watch?v=696as7_V3Gs&feature=youtu.be

Link Containing Source Code
https://github.com/along002/CS-EE-120B-Custom-Project/tree/master

