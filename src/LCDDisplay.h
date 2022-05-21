#pragma once

#include "TextLCD.h"
#include "string.h"

/* Pocet znaku v jedne radce. */
#define LINE_SIZE 20

/* Pocet radek, omezujici vektor. */
#define NUMBER_OF_LINES 4

/* ASCII hodnota mezery. */
#define FREE_SPACE 32

/* Buffer pro znaky */
#define MEMORY_SIZE 100

/* Trida pro praci LCD displeje s vice radky. */
class LCDDisplay
{
public:

    LCDDisplay(TextLCD_I2C *device);

    virtual ~LCDDisplay();

    /* Nastavi radky displeje. */
    void print(char *new_lines);

    /* Posun radek pro scroll vypis */
    void shift_lines(char *lcdMemory);

    /* Vycisti pamet a vsechny radky displeje. */
    void clear();

    /* Prida radku a vsechny ostatni posune o jednu pozici. */
    void printLine(char *line);

public:

    /* Objekt tridy TextLCD_I2C. */
    TextLCD_I2C *lcd;

    /* Pocet sloupcu. */
    int columns;

    /* Pocet radku. */
    int rows;

    /* Pole obsahujici radky LCD displeje. */
    char lcdMemory[MEMORY_SIZE] = { FREE_SPACE };
};
