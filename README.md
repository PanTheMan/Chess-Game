# Chess-Game
# PawnPusher

To just see pictures of the game go to the Wiki section!

## Setting up
###X11
You will need XQuartz if on a mac. http://www.xquartz.org/

You may need to log out and log in after installation.
When XQuartz is installed, you will get a non empty output with
>$echo DISPLAY

If you're on Windows, you'll need XMing instead. https://sourceforge.net/projects/xming/

To start a game in linux environment, first run the comnand "make" to create an executable pp9k file.
Now run ./pp9k with the sequence of commands to start a game as quick as possible:

>game human computer5

>auto

>move e6 e4

>...

##Commands
###Set up commands
>game (white) (black)

Starts a new game with the given white and black players. <white> and <black> can be "human", "computer1",...,"computer5".

>setup

Starts setup mode

###In-game commands

>move [ (starting square) (ending square) [(promotion piece)] ]

Makes the next player move. If human, you need to provide the pair of starting and ending squares i.e. "move g1 f3". 
You will also need to provide a promotion piece when necessary, i.e. "move g7 g8 Q".

>auto

Automates computer moves, so you don't have to type "move" for the computer.

>undo

Undos the last move. These are unlimited.

>resign

Resigns the game

###Setup mode

>+ (piece) (square)

Adds the piece to the given square. e.x. "+ r g8" adds a black rook at g8.

>- (square)

Removes the piece at the given square.

>FEN (FEN)

Sets up the board according to a given FEN string.
i.e. "FEN rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"

>done

Exits setup mode. Will not exit if certain conditions make the position obviously illegal.

###File mode
You can also set up a position from a file:

>./pp9k (file)

For example, "./pp9k breakthrough.txt"

