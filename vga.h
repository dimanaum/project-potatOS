/**
* CPE/CSC 159 - Operating System Pragmatics
* California State University, Sacramento
* Fall 2021
*
* VGA Functions
*/

#ifndef _VGA_H_
#define _VGA_H_

/* General definitions */
#define VGA_BASE 0xB8000
#define VGA_ROW_MAX 25
#define VGA_COL_MAX 80
#define VGA_TAB_SIZE 8

/* Colors available for use as text foregrounds or backgrounds */
enum vga_colors {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE,
    VGA_COLOR_GREEN,
    VGA_COLOR_CYAN,
    VGA_COLOR_RED,
    VGA_COLOR_PURPLE,
    VGA_COLOR_BROWN,
    VGA_COLOR_GREY,
    VGA_COLOR_DARK_GREY,
    VGA_COLOR_LIGHT_BLUE,
    VGA_COLOR_LIGHT_GREEN,
    VGA_COLOR_LIGHT_CYAN,
    VGA_COLOR_LIGHT_RED,
    VGA_COLOR_LIGHT_PURPLE,
    VGA_COLOR_YELLOW,
    VGA_COLOR_WHITE
};


/*
 * Prints a single character on the screen at the specified row/column
 * and applying the specified VGA attributes
 *
 * @param row - row number (0 to VGA_ROW_MAX)
 * @param col - column number (0 to VGA_COL_MAX);
 * @param attr - VGA attributes
 * @param ch - character to print
 */
void vga_print_char(int row, int col, int attr, char ch);

/*
 * Prints a string on the screen starting at the specified row/column
 * and applying the specified VGA attributes.
 *
 * In the event that the string reaches the last column, it will wrap
 * to the next row starting at the first column.
 *
 * In the event that the string reaches the last coulmn of the last
 * row, it will wrap to the first row starting at the first column.
 *
 * @param row - row number (0 to VGA_ROW_MAX)
 * @param col - column number (0 to VGA_COL_MAX);
 * @param attr - VGA attributes
 * @param str - string to print
 */
void vga_print_str(int row, int col, int attr, char *str);

/*
 * Constructs the byte representation of the VGA attributes
 *
 * @param bg_color - background color code
 * @param fg_color - foreground color code
 * @param blink - 0 if not blinking, other value if blinking
 * @notes {
 * VGA output consists of two bytes:
 *   High byte: attributes
 *   Low byte: ASCII character
 *
 * VGA attributes define:
 *   Background color
 *   Foreground color
 *   Blinking characters
 *
 * The attribute byte is defined as:
 *
 *   +---------------------------------------------------------------|
 *   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
 *   |---------------------------------------------------------------|
 *   | Blink |    Background Color   |        Foreground Color       |
 *   +---------------------------------------------------------------+
 *
 * Colors are one of 15 colors (0 to 15, 0x0 to 0xF)
 *   Black
 *   Blue
 *   Green
 *   Cyan
 *   Red
 *   Purple
 *   Brown
 *   Grey
 *   Dark Grey
 *   Light Blue
 *   Light Green
 *   Light Cyan
 *   Light Red
 *   Light Purple
 *   Yellow
 *   White
 * }
 */
int vga_attr(int bg_color, int fg_color, int blink);

#endif // _VGA_H_
