# Project 5

Starting from the code provided, your team will add some features to a game.

As I give it to you, it has some interesting levels but there isn't much that works. Play it a little bit to see what it is like currently.

## Part 1: Simple change. (20 points)

The first part of the assignment is to make a simple change that only requires creating a new class and hooking it into the program. You will only need to change game.h and game.cpp. Using SolidGameElement as an example, implement one of the following features:

*    A ladder, which is symbol 'H' in the game, which overrides isClimbable() to return true, much like SolidGameElement overrides isSolid().
*    A spring, which is symbol 'Z' in the game, which overrides isSpringy() to return true.
*    A tack, which is symbol '^' in the game, which overrides touch() to call the die(game) function on whatever agent touched it.
*    You may pick any other character that can be implemented simply, such as a landmine that kills willy when he steps off it, a pile of gold that gives points every time it is touched, something that kills multiple lives at once, an invisible tack, etc.

## Part 2: Complex Change.

The first part was easy, because I had already implemented a pretty smart worm for you, and you just needed to make props. Other features will require you to know more about how the game works and make changes to existing classes to add the functionality.

Pick a feature from the list below and then work through the steps that follow. Make sure to get help if you get stuck.

*    A present should give willy 100 points when touched, but then the present should disappear. Getting that disappearing behavior to work is the hard part.
*    Thin floors, symbol '-', disappear when Willy steps off one.
*    A side spring should reverse Willy's direction if he runs into it. Bossing Willy around is not so hard, but there is not any current functionality to find out what direction Willy is headed.
*    A bell is tricky, because it has to give Willy however many points are on the bonus counter and switch to a new level. Switching to a new level is definitely the hard part here!
*    Balls are malevolent agents like Willy. You could probably make a ball by copying worm.h and worm.cpp and then changing the behavior. They move around the screen, reversing direction whenever they hit an obstacle and pick a random direction when they land after falling. When willy touches one, he dies. When he jumps over on, he gains 20 points.
*    Fix one of the bugs in the program: When willy jumps, he should stay at his highest point two turns instead of just one. When Willy runs into a wall, he should stop instead of continuing to try moving in that direction. Add error handling code everywhere it is missing in display.cpp and display.h. NCurses is written in C, not C++, so it doesn't have exceptions.
*    Replace the current user interface with a graphical user interface. In theory, this should just require you to replace display.h and display.cpp.
*    Make the game into a 2 player game.
*    Add any other special character to the game that requires substantial changes to existing code.

### Process:

1. Make sure your code is committed. You don't want to lose the work you did on part 1!
2. Create a diagram showing how Willy and your change will interact. Interaction, state and sequence diagrams are all reasonable choices. (5 points)
3. Explore the code to see what parts are related to your change. For example, if you choose to do the present, you know that Willy moves onto the square with the present, then he touches the present, then the present gives him 100 points, then the present tells the game to take it away. So, you are trying to figure out how touching things works and how game elements are added to the map and removed from the map. Good tools for this include:
    * Start with something you know, like you know that the touch() function will be called on the present. Search through the code to find all places that call a touch function. Make note of the ones that could be calling your function. Work backwards from those, figuring out the circumstance in which they call your touch function and what calls them.
    * Start with something you know, like the touch() function. Run the program in a debugger, with a breakpoint set on the touch() function. When it hits the breakpoint, examine the stack ("bt" in gdb) to find out everything about how the function was called, including the values of various variables.
    * You can try a general trace through the code, starting from the main() function and tracing how the program executes. Of course, once you know that a part of the code is probably not relevant you can skip that part.
4.   Create a chart showing the existing places in the code that are related to the interactions you need between Willy and your change. (5 points)
5.  Devise a strategy. What do you need to do to the code to add your feature? Pretend that you have one chance to get this right. (5 points)
6.  Divide the solution up into stages. What is the smallest change you can make that moves towards your goal while not changing very much? How can you test that change and make sure it works correctly? You should get a series of toddler steps that go from the current code to having your feature added. (5 points)
7.  Complete the steps, one at a time, compiling and testing after each one. Don't forget to make backups along the way. (20 points)
8.  Cheer with great gusto when it works ;-). (Should I give points for this?)
