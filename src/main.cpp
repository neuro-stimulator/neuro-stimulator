#ifdef USE_MBED
#include "mbed.h"
// #include "TextLCD.h"
#else
#include <iostream>
#include <list>
#include <stdint.h>
#include <time.h>
#endif // USE_MBED
#include "string.h"



#define TOTAL_OUTPUT_COUNT            8

#ifdef USE_MBED
#ifdef BOARD_NUCLEO
#include "mapping-nucleo.h"
#endif
#ifdef BOARD_DISCO
#include "mapping-disco.h"
#endif
#endif // USE_MBED
#include "mapping-server-command.h"
#include "command.h"

/*------------ Constant definitions --------------*/
#define BRATE           9600
#define BUFF_LENGTH     128

/*------------------ Variables -------------------*/
#ifdef USE_MBED
Serial                         pc(TX_PIN, RX_PIN);
Serial                         graphicsSerial(GRAPHICS_TX_PIN, GRAPHICS_RX_PIN);
I2C                            i2c_lcd(SDA_PIN, SCL_PIN);
// TextLCD_I2C                    lcd(&i2c_lcd, DISPLAY_ADDRESS, TextLCD::LCD16x2);
DigitalOut                     ledReady(LED_READY);
DigitalOut                     ledBussy(LED_BUSSY);
event_callback_t               serialEventCb;
uint8_t                        rx_buf[BUFF_LENGTH + 1];
CircularBuffer<Command, 16>    commands;
CircularBuffer<ServerCommandData, 16> serverCommands;
Timer                          globalTimer;
#endif // USE_MBED

/*------------------ Callbacks -------------------*/

/**
* Callback, který se zavolá, když příjdou data na seriovou linku
*  - 1. pokud přišel znak ukončující příkaz
*  - 2. pokud buffer přetekl
**/
#ifdef USE_MBED
void serialCb(int events) {
    uint8_t i;
    // Pokud přišel úplný příkaz ukončený ukončovacím znakem
    if(events & SERIAL_EVENT_RX_CHARACTER_MATCH) {
        // Vytvoříme novou instanci 'příkazu'
        Command command;
        // Vynulujeme přidělenou paměť
        memset(&command, 0, sizeof(command));
        // Projdeme buffer s daty
        for(i = 0; i < BUFF_LENGTH; i++) {
            // Pokud narazím na ukončovací znak příkazu
            if(rx_buf[i] == 'S') {
                // Ukončím cyklus
                break;
            }
            // byte je součástí příkazu, tak ho překopíruji
            command.commandData.rawData[i] = rx_buf[i];
        }
        // Uložím délku příkazu
        // Je o 1 menší, protože je zahrnut i ukončovací znak
        command.length = i - 1;

        // Nakonec vložím příkaz do fronty ke zpracování
        commands.push(command);
    } else
    // Pokud nám buffer přetekl
    if (events & SERIAL_EVENT_RX_COMPLETE) {
        // TODO ošetřit přetečení bufferu
        i = BUFF_LENGTH - 1;
    } else {
        rx_buf[0] = 'E';
        rx_buf[1] = 'R';
        rx_buf[2] = 'R';
        rx_buf[3] = '!';
        rx_buf[4] = 0;
        i = 3;
    }

    // Restart příjmu dat po seriové lince
    pc.read(rx_buf, BUFF_LENGTH, serialEventCb, SERIAL_EVENT_RX_ALL, 'S');
}
#endif // USE_MBED

/*--------------- Helper methods -----------------*/
#ifdef USE_MBED
void updateInfoLeds(bool ready, bool bussy) {
    ledReady = ready;
    ledBussy = bussy;
}
#endif // USE_MBED

/*-------------------- Main ----------------------*/
#ifdef USE_MBED
int main() {
    ExperimentProgram experimentProgram;
    used_peripherals_t usedPeripherals;


    globalTimer.start();
    updateInfoLeds(false, true);

    // Inicializace jednotlivých periferií
    PwmOut out1(OUTPUT_PIN_1);
    PwmOut out2(OUTPUT_PIN_2);
    PwmOut out3(OUTPUT_PIN_3);
    PwmOut out4(OUTPUT_PIN_4);
    PwmOut out5(OUTPUT_PIN_5);
    PwmOut out6(OUTPUT_PIN_6);
    PwmOut out7(OUTPUT_PIN_7);
    PwmOut out8(OUTPUT_PIN_8);

    Ticker ticker1;
    Ticker ticker2;
    Ticker ticker3;
    Ticker ticker4;
    Ticker ticker5;
    Ticker ticker6;
    Ticker ticker7;
    Ticker ticker8;

    Timeout timeout1;
    Timeout timeout2;
    Timeout timeout3;
    Timeout timeout4;
    Timeout timeout5;
    Timeout timeout6;
    Timeout timeout7;
    Timeout timeout8;

    InterruptIn interruptIn1(INPUT_PIN_1);
    interruptIn1.mode(PullUp);
    InterruptIn interruptIn2(INPUT_PIN_2);
    interruptIn2.mode(PullUp);
    InterruptIn interruptIn3(INPUT_PIN_3);
    interruptIn3.mode(PullUp);
    InterruptIn interruptIn4(INPUT_PIN_4);
    interruptIn4.mode(PullUp);
    InterruptIn interruptIn5(INPUT_PIN_5);
    interruptIn5.mode(PullUp);
    InterruptIn interruptIn6(INPUT_PIN_6);
    interruptIn6.mode(PullUp);
    InterruptIn interruptIn7(INPUT_PIN_7);
    interruptIn7.mode(PullUp);
    InterruptIn interruptIn8(INPUT_PIN_8);
    interruptIn8.mode(PullUp);


    // // Vložení proměnných do kontextu
    usedPeripherals.out1 = &out1;
    usedPeripherals.out2 = &out2;
    usedPeripherals.out3 = &out3;
    usedPeripherals.out4 = &out4;
    usedPeripherals.out5 = &out5;
    usedPeripherals.out6 = &out6;
    usedPeripherals.out7 = &out7;
    usedPeripherals.out8 = &out8;
    usedPeripherals.interruptIn1 = &interruptIn1;
    usedPeripherals.interruptIn2 = &interruptIn2;
    usedPeripherals.interruptIn3 = &interruptIn3;
    usedPeripherals.interruptIn4 = &interruptIn4;
    usedPeripherals.interruptIn5 = &interruptIn5;
    usedPeripherals.interruptIn6 = &interruptIn6;
    usedPeripherals.interruptIn7 = &interruptIn7;
    usedPeripherals.interruptIn8 = &interruptIn8;
    usedPeripherals.ticker1 = &ticker1;
    usedPeripherals.ticker2 = &ticker2;
    usedPeripherals.ticker3 = &ticker3;
    usedPeripherals.ticker4 = &ticker4;
    usedPeripherals.ticker5 = &ticker5;
    usedPeripherals.ticker6 = &ticker6;
    usedPeripherals.ticker7 = &ticker7;
    usedPeripherals.ticker8 = &ticker8;
    usedPeripherals.timeout1 = &timeout1;
    usedPeripherals.timeout2 = &timeout2;
    usedPeripherals.timeout3 = &timeout3;
    usedPeripherals.timeout4 = &timeout4;
    usedPeripherals.timeout5 = &timeout5;
    usedPeripherals.timeout6 = &timeout6;
    usedPeripherals.timeout7 = &timeout7;
    usedPeripherals.timeout8 = &timeout8;
    usedPeripherals.globalTimer = &globalTimer;
    usedPeripherals.graphicsSerial = &graphicsSerial;

    // Naplnění polí v contextu pro snadnější přístup k proměnným
    usedPeripherals.outputs[0] = &out1;
    usedPeripherals.outputs[1] = &out2;
    usedPeripherals.outputs[2] = &out3;
    usedPeripherals.outputs[3] = &out4;
    usedPeripherals.outputs[4] = &out5;
    usedPeripherals.outputs[5] = &out6;
    usedPeripherals.outputs[6] = &out7;
    usedPeripherals.outputs[7] = &out8;
    usedPeripherals.interuptsIn[0] = &interruptIn1;
    usedPeripherals.interuptsIn[1] = &interruptIn2;
    usedPeripherals.interuptsIn[2] = &interruptIn3;
    usedPeripherals.interuptsIn[3] = &interruptIn4;
    usedPeripherals.interuptsIn[4] = &interruptIn5;
    usedPeripherals.interuptsIn[5] = &interruptIn6;
    usedPeripherals.interuptsIn[6] = &interruptIn7;
    usedPeripherals.interuptsIn[7] = &interruptIn8;
    usedPeripherals.tickers[0] = &ticker1;
    usedPeripherals.tickers[1] = &ticker2;
    usedPeripherals.tickers[2] = &ticker3;
    usedPeripherals.tickers[3] = &ticker4;
    usedPeripherals.tickers[4] = &ticker5;
    usedPeripherals.tickers[5] = &ticker6;
    usedPeripherals.tickers[6] = &ticker7;
    usedPeripherals.tickers[7] = &ticker8;
    usedPeripherals.timeouts[0] = &timeout1;
    usedPeripherals.timeouts[1] = &timeout2;
    usedPeripherals.timeouts[2] = &timeout3;
    usedPeripherals.timeouts[3] = &timeout4;
    usedPeripherals.timeouts[4] = &timeout5;
    usedPeripherals.timeouts[5] = &timeout6;
    usedPeripherals.timeouts[6] = &timeout7;
    usedPeripherals.timeouts[7] = &timeout8;

    // Tímto dáme přístup experimentu k periferiím
    // Není to nejlepší řešení, ale funguje to
    experimentProgram.setPeripherals(&usedPeripherals);
    experimentProgram.setServerCommandQueue(&serverCommands);
    experimentProgram.initInterrupts();
    experimentProgram.clear();


    // Registrace callbacku na seriovou linku
    serialEventCb.attach(callback(&serialCb));

    // Nastavení seriove linky
    pc.baud(BRATE);
    pc.read(rx_buf, BUFF_LENGTH, serialEventCb, SERIAL_EVENT_RX_ALL, 'S');

    updateInfoLeds(true, false);

    {
        pc.putc('S');
        pc.putc(0xFF);

        ServerCommandData readyCommand;
        readyCommand.header.type = COMMAND_STIMULATOR_STATE;
        readyCommand.header.length = 7; //sizeof(server_command_stimulator_state_t);
        uint32_t timestamp = globalTimer.read_us();
        readyCommand.commandStimulatorState.state = experimentProgram.getState();
        readyCommand.commandStimulatorState.timestamp = timestamp & 0xFFFFFFFF;
        serverCommands.push(readyCommand);
    }


    // Hlavní nekonečna smyčka celého programu
    while(1) {
        // Pokud se ve frontě příkazů nějaký nachází
        if (!commands.empty()) {
            updateInfoLeds(true, true);
            // Založím novou instanci
            Command command;
            ServerCommandData resultData;
            resultData.header.type = 0x00;
            // Získám příkaz z fronty
            commands.pop(command);
            // Vykonám požadovaný příkaz
            command.execute(experimentProgram, resultData, globalTimer);
            if (resultData.header.type != 0x00) {
                serverCommands.push(resultData);
            }
        } else {
            updateInfoLeds(true, false);
        }

        // Pokud se ve frontě příkazů na server nějaký nachází
        if (!serverCommands.empty()) {
            updateInfoLeds(true, true);
            // Založím novou instanci
            ServerCommandData commandData;
            // Získám příkaz z fronty
            serverCommands.pop(commandData);
            // Odešle se 1 byte typu commandu
            //pc.putc(commandData.header.type);
            // Pokud command obsahuje dodatečná data, tak se odešlou
            // Začínám na indexu: 'i=2', protože
            //    1. byte = typ
            //    2. byte = délka dodatečných dat
            for (uint8_t i = 0; i < commandData.header.length; i++) {
                pc.putc(commandData.rawData[i]);
            }
            pc.putc('S');
            // Na konec odešlu ukončovací znak příkazu
            pc.putc(0xFF);

        } else {
            updateInfoLeds(true, false);
        }

        wait_ms(100);
    }
}
#else
int main() {
    // Inicializace proměnných
    std::list<Command> commands{};
    std::list<ServerCommandData> serverCommands;

    // Načtení všech příkazů do bufferu
    char commandBuffer[2048];
    // Vyčistím paměť
    memset(commandBuffer, 0, 2048 * sizeof(char));

    // Nahrnu veškeré příkazy do bufferu
    std::cin >> commandBuffer;

    Command command;
    size_t j = 0;
    for (size_t i = 0; i < 2048; i++) {
        char c = commandBuffer[i];
        if (c == 'S') {
            commands.push_back(command);
            memset(&command, 0, sizeof(command));
            j = 0;
        } else {
            command.commandData.rawData[j++] = c;
        }
    }

}
#endif // USE_MBED
