#ifndef COMMAND_H
#define COMMAND_H


#include "mapping-commands.h"
#include "mapping-server-command.h"
#include "experiment-program.h"

class Command {

private:

#ifdef USE_MBED
void resultStimulatorState(ServerCommandData &resultCommand, Timer &globalTimer, uint8_t state) {
    resultCommand.header.type = COMMAND_STIMULATOR_STATE;
    resultCommand.header.length = 7; //sizeof(server_command_io_change_t);
    uint32_t timestamp = globalTimer.read_us();
    resultCommand.commandStimulatorState.state = state;
    resultCommand.commandStimulatorState.timestamp = timestamp & 0xFFFFFFFF;
}
#else
void resultStimulatorState(ServerCommandData &resultCommand, uint8_t state) {
    resultCommand.header.type = COMMAND_STIMULATOR_STATE;
    resultCommand.header.length = 7; //sizeof(server_command_io_change_t);
    resultCommand.commandStimulatorState.state = state;
    resultCommand.commandStimulatorState.timestamp = 0xFFFFFFFF;
}
#endif // USE_MBED

public:
    union CommandData commandData;
    uint8_t length;

    Command() {};
    ~Command() {}
#ifdef USE_MBED
    void execute(ExperimentProgram &experimentProgram, ServerCommandData &resultCommand, Timer &globalTimer) {
#else
    void execute(ExperimentProgram &experimentProgram, ServerCommandData &resultCommand) {
#endif // USE_MBED
        const char cmd = commandData.type;
        switch(cmd) {
            // Reset HW
            case COMMAND_REBOOT: { // 0x00
                //context.serial->printf("HW resetS");
                break;
            }
            // Nastavení času stimulátoru
            case COMMAND_TIME_SET: { // 0x01
                //time_t time = data[1] | data[2] << 8 | data[3] << 16 | data[4] << 24;
                // set_time(commandData.commandTimeSet.time);
                break;
            }
            // Práce s obrazovkou
            case COMMAND_DISPLAY: {// 0x02
                // Vymazání obsahu obrazovky
                //if (data[1] == 0x00) {
                if (commandData.commandLCD.action == 0x00) {
                    // context.lcd->cls();
                } else {
                // Zápis na obrazovku
                // if (data[1] == 0x01) {
                if (commandData.commandLCD.action == 0x01) {
                    // Získám X-ovou souřadnici začátku textu
                    // Hodnotu moduluji, abych se nedostal mimo offset
                    //const char x = data[2] % context.lcd->columns();
                    // const char x = commandData.commandLCD.column % context.lcd->columns();
                    // Záskám Y-ovou souřadnici začátku textu
                    // Hodnotu moduluji, abych se nedostal mimo offset
                    //const char y = data[3] % context.lcd->rows();
                    // const char y = commandData.commandLCD.row % context.lcd->rows();
                    // Nastavím požadovanou souřadnici začátku textu
                    // context.lcd->locate(x,y);
                    // Založím pomocnou proměnnou, která bude obsahovat samotný
                    // text
                    //char text[length];
                    // Znaky pěkně jeden po druhém nakopíruji do této pomocné
                    // proměnné
                    //for (int i = 0; i < length; i++) {
                        //text[i] = data[i+4];
                    //    commandData.commandLCD.text[i];
                    //}
                    // Nakonec text vypíšu na display
                    //context.lcd->printf(text);
                    // context.lcd->printf(commandData.commandLCD.text);
                } }
                break;
            }
            // Správa experimentu
            case COMMAND_MANAGE_EXPERIMENT: { // 0x03
                const char action = commandData.commandManageExperiment.action;
                switch (action) {
                    // Inicializuje experiment
                    case 0x01:
                        experimentProgram.init(commandData.commandSetupExperiment.experimentConfig);
                        break;
                    // Nastavi experiment
                    case 0x02:
                        experimentProgram.setup();
                        break;
                    // Spustí experiment
                    case 0x03:
                        experimentProgram.start();
                        break;
                    // Zastaví experiment
                    case 0x04:
                        experimentProgram.pause();
                        break;
                    case 0x05:
                        experimentProgram.stop();
                        break;
                    // Vymaže nastavení experimentu
                    case 0x06:
                        experimentProgram.clear();
                        break;
                }
#ifdef USE_MBED
                this->resultStimulatorState(resultCommand, globalTimer, experimentProgram.getState());
#else
                this->resultStimulatorState(resultCommand, experimentProgram.getState());
#endif // USE_MBED
                break;
            }
            // Nastavení jednotlivých výstupů experimentu
            case COMMAND_SETUP_OUTPUT: { // 0x11
                uint8_t outputIndex = commandData.commandSetupOutput.index;
                experimentProgram.setupOutput(outputIndex, commandData.commandSetupOutput.outputConfig);
#ifdef USE_MBED
                this->resultStimulatorState(resultCommand, globalTimer, 0x11);
#else
                this->resultStimulatorState(resultCommand, 0x11);
#endif // USE_MBED
                break;
            }

            // Zpracování části sekvence
            case COMMAND_NEXT_SEQUENCE_PART: { // 0x20
                uint8_t index = commandData.commandSequencePart.index;
                uint32_t data = commandData.commandSequencePart.data;
                experimentProgram.updateAccumulator(index, data);
#ifdef USE_MBED
                this->resultStimulatorState(resultCommand, globalTimer, 0x20);
#else
                this->resultStimulatorState(resultCommand, 0x20);
#endif // USE_MBED
                break;
            }

            // Backdory do systemu

            // Přímé nastavení jednotlivých výstupů
            case COMMAND_BACKDOR_1: { // 0xF0
                uint8_t index = commandData.commandBackdor1.index % TOTAL_OUTPUT_COUNT;
                const float brightness = commandData.commandBackdor1.brightness / 100.0;
                // *context.usedPeripherals.outputs[index] = brightness;
#ifdef USE_MBED
                this->resultStimulatorState(resultCommand, globalTimer, 0xF2);
#else
                this->resultStimulatorState(resultCommand, 0xF2);
#endif // USE_MBED

                break;
            }
            case COMMAND_DEBUG: { // 0xF1
                resultCommand.header.type = COMMAND_DEBUG;
                uint8_t memoryType = commandData.commandDebug.memoryType;
                switch (memoryType) {
                    // Experiment config
                    case 0x00: {
                        experimentProgram.copyExperimentConfig(&resultCommand.rawData[2]);
                        resultCommand.header.length = sizeof(ExperimentConfig) + sizeof(server_command_data_header_t);
                        //resultCommand.commandDebug.experimentConfig = experimentProgram.getExperimentConfig();
                        break;
                    }
                    // Expeiment counters
                    case 0x01: {
                        experimentProgram.copyCounters(&resultCommand.rawData[2]);
                        resultCommand.header.length = sizeof(uint16_t) * TOTAL_OUTPUT_COUNT + sizeof(server_command_data_header_t);
                        break;
                    }
                    // Experiment accumulators
                    case 0x02: {
                        experimentProgram.copyAccumulators(&resultCommand.rawData[2]);
                        resultCommand.header.length = sizeof(uint32_t) * TOTAL_OUTPUT_COUNT + sizeof(server_command_data_header_t);
                        break;
                    }
                }
                break;
            }
            default: {
                resultCommand.header.type = COMMAND_STIMULATOR_STATE;
                resultCommand.header.length = 8; //sizeof(server_command_io_change_t);
                resultCommand.commandStimulatorState.state = 0xF9;
#ifdef USE_MBED
                uint32_t timestamp = globalTimer.read_us();
                resultCommand.commandStimulatorState.timestamp = timestamp & 0xFFFFFFFF;
#else
                resultCommand.commandStimulatorState.timestamp = 0xFFFFFFFF;
#endif // USE_MBED
                resultCommand.rawData[7] = cmd;
                break;
                // context.serial->printf("Neznamy prikaz!S");
                // context.serial->putc(0xFF);
            }
        }
    }

};

#endif