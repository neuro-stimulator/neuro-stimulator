
#include "LCDDisplay.h"


LCDDisplay::LCDDisplay(TextLCD_I2C *device) {
    this->lcd = device;
    this->columns = this->lcd->columns();
    this->rows = this->lcd->rows();

	device->setBacklight(TextLCD::LightOn);
	device->setCursor(TextLCD::CurOff_BlkOff);

	//nastaveni ukoncovaciho znaku
	lcdMemory[rows * columns + 1] = 0;
	memset(lcdMemory, FREE_SPACE, rows * columns);
}

LCDDisplay::~LCDDisplay() {

}

void LCDDisplay::print(char *new_lines) {
    int text_index = 0;
	int mem_index = 0;

    memset(lcdMemory, FREE_SPACE, rows * columns);
    
    while(mem_index < rows * columns && new_lines[text_index] != 0) {
    	if(new_lines[text_index] == '\n') {
    		//pokud narazim na odradkovani, zacnu vypisovat o radku dale
    		mem_index = mem_index / columns;
    		mem_index = (mem_index + 1) * columns;
    		
			++text_index;
    		continue;
		}
    	lcdMemory[mem_index] = new_lines[text_index];
    	
    	++mem_index;
    	++text_index;
	}

    lcd->locate(0, 0);
    lcd->printf(lcdMemory);
}

void LCDDisplay::shift_lines(char *lcdMemory) {
	//posunuti dat o jednu radku nahoru
	memcpy(lcdMemory, lcdMemory + columns, columns * (rows - 1));
	//zapsani mezer do uvolnene posledni radky
	memset(lcdMemory + (columns * (rows - 1)), FREE_SPACE, columns);
}

void LCDDisplay::clear() {
    memset(lcdMemory, FREE_SPACE, rows * columns);
    lcd->locate(0, 0);
    lcd->printf(lcdMemory);
}

void LCDDisplay::printLine(char *new_line) {
    int text_index = 0;
	int column_index = 0;
	int last_line = (rows - 1) * columns;
	
	shift_lines(lcdMemory);
	
	//smycka bezi dokud se nezapise cely displej nebo se nenarazi na ukoncovaci znak stringu
	while(text_index < rows * columns && new_line[text_index] != 0) {
		//pokud narazim na odradkovani v novych datech, opet posunu radky a zacnu psat od zacatku
		if(new_line[text_index] == '\n') {
			shift_lines(lcdMemory);
			column_index = 0;
			++text_index;
			continue;
		}
        //pokud je radka delsi nez pocet sloupcu displeje, musim radky opet posunout a odradkovat
		if(column_index == columns) {
			shift_lines(lcdMemory);
			column_index = 0;
			continue;
		}
		lcdMemory[last_line + column_index] = new_line[text_index];
		++column_index;
		++text_index;
	}

    lcd->locate(0, 0);
    lcd->printf(lcdMemory);
}
