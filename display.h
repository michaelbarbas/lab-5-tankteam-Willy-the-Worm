#include "game.h"

#ifndef DISPLAY_H
#define DISPLAY_H

#include <curses.h>

#include <string>

/* The user interface input/output functionality.
 * Supports:
 *   Individual characters can be placed in a scrolling space that
 *   may be larger than the display.
 *   Text may be placed on the display, non-scrolling.
 *   The display can be scrolled with some flexibility.
 *   Any keyboard input is mapped to commands.
 */
class GameDisplay
{ private:
    unsigned width, height, // Size of the virtual map.
	     y0, x0; // Location of the upper left corner of the displayed
	             // part of the map.
    WINDOW *win; // Handle to the Curses display.
    chtype *buffer; // Contents of the full map.
    Command command[256], // The mapping from key pressed to Command.
            lastCommand, newCommand; // The previous and new commands.
    pthread_t inputThread; // Thread used to process key presses.
    pthread_mutex_t inputLock; // Lock to coordinate threads.
    bool done; // Flag to tell the input thread to exit.

  public:
    /* Constructor, for creating a new display.
     * Input: size of the virtual map.
     */
    GameDisplay(unsigned width, unsigned height);

    // Destructor, for cleaning up a display before releasing the memory.
    ~GameDisplay();

    /* Write the character c at the row and column of the virtual map.
     * The character is stored in a buffer in case the map is scrolled.
     */
    void writeAt(unsigned row, unsigned column, const chtype c);

    /* Write the string at the absolute location in the window.
     * The string is not scrolled or buffered.
     */
    void writeAt(unsigned row, unsigned column, const std::string s);

    /* If necessary, scroll so that the specified location is displayed.
     * Input:
     *   row, column: The location in the virtual map that must display.
     *   border: The size of the border on the screen. If the location
     *           is further from the edge than this border, we do not
     *           scroll.
     *   scroll: The unit of scrolling. 1 will give a smooth scroll,
     *           larger numbers will hold the map still longer and jump
     *           instead.
     */
    void center(unsigned row, unsigned column,
                unsigned border=10, unsigned scroll=1);

    /* Function called to tell the display to update all the recent
     * changes.
     */
    bool clock(Game *game);

    /* Get the command that was most recently issued by the user. */
    Command getCommand();

    /* Set the next command, as if the user had entered it. */
    void setCommand(Command command);

    /* For internal user only, called by the input thread to check
     * for any user input.
     */
    bool checkInput();
};

#endif
