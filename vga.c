/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * VGA Functions
 */

// for definition of NULL
#include <spede/stdio.h>

// VGA Function prototypes, enumerations, and defintions
#include "vga.h"

void vga_print_char(int row, int col, int attr, char ch) {
    unsigned short *vga_ptr = (unsigned short*)VGA_BASE + row * VGA_COL_MAX + col;

    // Ensure rows/columns are valid
    if (row < 0 || row >= VGA_ROW_MAX || col < 0 || col >= VGA_COL_MAX) {
        return;
    }

    // Ensure that only the first byte of the  attribute is considered
    attr &= 0xff;

    // Set the two bytes (attribute + character) in the video memory
    *vga_ptr = (attr << 8) + ch;
}

void vga_print_str(int row, int col, int attr, char *str) {
    /* Handle a null pointer */
    if (str == NULL) {
        return;
    }

    while (*str != '\0') {
        if (*str == '\n') {
            col = 0;
            row++;
        } else if (*str == '\t') {
            col = ((col / VGA_TAB_SIZE) + 1) * VGA_TAB_SIZE;
        } else if (*str >= 0x20 && *str != 0x7f) {
            vga_print_char(row, col, attr, *str);
            col++;
        }

        if (col >= VGA_COL_MAX) {
            row++;
            col = 0;
        }

        if (row >= VGA_ROW_MAX) {
            row = 0;
        }

        str++;
    }
}

int vga_attr(int bg_color, int fg_color, int blink) {
    /* Initialize attribute to default: black on back, no bold, no blinking */
    int attr = 0x0;

    /* Apply the background color */
    attr |= ((bg_color & 0x7) << 4);

    /* Apply the foreground color */
    attr |= (fg_color & 0xf);

    /* Apply the blinking attribute */
    if (blink != 0) {
        attr |= 0x80;
    }

    /* Return the constructed attribute */
    return attr;
}

