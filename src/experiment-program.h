#ifndef EXPERIMENT_PROGRAM_H
#define EXPERIMENT_PROGRAM_H

#include "string.h"
#include "mapping-experiment.h"
#include "mapping-server-command.h"
#include "used-peripherals.h"
#include "platform-types.h"


enum ExperimentState {
    READY,
    UPLOADED,
    INITIALIZED,
    RUNNING,
    PAUSED,
    FINISHED,
    CLEARED
};

class ExperimentProgram {

private:
/*--------------- Proměnné -----------------*/
    // Multifunkční pole counterů
    // Slouží k počítání výstupů
    uint16_t m_counters[TOTAL_OUTPUT_COUNT];
    // Multifunkční pole accumulatorů
    // Využito hlavně v ERP pro sekvence
    uint32_t m_accumulators[TOTAL_OUTPUT_COUNT];
    // Pole pro zabránění několikanásobnému stisku tlačítka
    // během jednoho cyklu
    uint8_t m_inputButtonDelays[TOTAL_OUTPUT_COUNT];
    // Počet výstupů, se kterým experiment pracuje
    volatile experiment_output_count_t m_outputCount;
    // Aktuální stav stimulátoru
    ExperimentState m_state;
    // Pointer na periferie
    used_peripherals_t *m_usedPeripherals;
    // konfigurace experimentu;
    ExperimentConfig m_experimentConfig;
#ifdef USE_MBED
    // Pointer na cyklický buffer s příkazy, které se budou posílat ven
    CircularBuffer<ServerCommandData, 16> *m_serverCommandQueue;
#endif // USE_MBED


/*------- Setup funkce experimentů ---------*/
    /**
     * Provede se inicializace ERP experimentu.
     * Uloží se počet výstupů do samostatné proměnné.
     * Dále inicializuje jeden časovač @link ticker1,
     *  který bude pohánět celý experiment.
     */
    void setupERP() {
        this->m_outputCount = this->m_experimentConfig.experimentERP.head.outputCount;
        const float period = this->m_experimentConfig.experimentERP.head.out
                           + this->m_experimentConfig.experimentERP.head.wait
                           + 0.0f;

#ifdef USE_MBED
        this->m_usedPeripherals->ticker1->attach(callback(this, &ExperimentProgram::tickerERP), period);
#endif // USE_MBED
        this->sendSequencePartRequest(0, 0);
        this->sendSequencePartRequest(1, 8);
    }


    /**
     * Provede se inicializace CVEP experimentu.
     * Uloží se počet výstupů do samostatné proměnné.
     * Dále inicializuje jeden časovač @link ticker1,
     *  který bude pohánět celý experiment.
     */
    void setupCVEP() {
        this->m_outputCount = this->m_experimentConfig.experimentCVEP.head.outputCount;
        const float period = this->m_experimentConfig.experimentCVEP.head.out
                           + this->m_experimentConfig.experimentCVEP.head.wait
                           + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->ticker1->attach(callback(this, &ExperimentProgram::tickerCVEP), period);
#endif // USE_MBED
    }

    /**
     * Provede se inicializace FVEP experimentu.
     * Uloží se počet výstupů do samostatné proměnné.
     * Dále se podle počtu použitých výstupů inicializuje
     *  odpovídající počet časovačů - každý časovač pro jeden výstup.
     *
     * Využívá se automatického propadání ve switch konstrukci - nikde tu nenaleznete break.
     */
    void setupFVEP() {
        this->m_outputCount = this->m_experimentConfig.experimentFVEP.head.outputCount;
        float period = 0.0f;

        switch (this->m_outputCount) {
            case 8: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[7].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[7].timeOff
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker8->attach(callback(this, &ExperimentProgram::tickerFVEP8), period);
#endif // USE_MBED
             }
            case 7: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[6].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[6].timeOff
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker7->attach(callback(this, &ExperimentProgram::tickerFVEP7), period);
#endif // USE_MBED
             }
            case 6: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[5].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[5].timeOff
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker6->attach(callback(this, &ExperimentProgram::tickerFVEP6), period);
#endif // USE_MBED
             }
            case 5: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[4].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[4].timeOff
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker5->attach(callback(this, &ExperimentProgram::tickerFVEP5), period);
#endif // USE_MBED
             }
             case 4: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[3].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[3].timeOff
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker4->attach(callback(this, &ExperimentProgram::tickerFVEP4), period);
#endif // USE_MBED
             }
             case 3: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[2].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[2].timeOff
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker3->attach(callback(this, &ExperimentProgram::tickerFVEP3), period);
#endif // USE_MBED
             }
             case 2: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[1].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[1].timeOff
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker2->attach(callback(this, &ExperimentProgram::tickerFVEP2), period);
#endif // USE_MBED
             }
             case 1: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[0].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[0].timeOff
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker1->attach(callback(this, &ExperimentProgram::tickerFVEP1), period);
#endif // USE_MBED
             }
        }
    }

    /**
     * Provede se inicializace TVEP experimentu.
     * Uloží se počet výstupů do samostatné proměnné.
     * Dále se podle počtu použitých výstupů inicializuje
     *  odpovídající počet časovačů - každý časovač pro jeden výstup.
     *
     * Využívá se automatického propadání ve switch konstrukci - nikde tu nenaleznete break.
     */
    void setupTVEP() {
        this->m_outputCount = this->m_experimentConfig.experimentTVEP.head.outputCount;
        float period = 0.0f;

        switch (this->m_outputCount) {
             case 8: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[7].out
                        + this->m_experimentConfig.experimentTVEP.outputs[7].wait
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker8->attach(callback(this, &ExperimentProgram::tickerTVEP8), period);
#endif // USE_MBED
             }
             case 7: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[6].out
                        + this->m_experimentConfig.experimentTVEP.outputs[6].wait
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker7->attach(callback(this, &ExperimentProgram::tickerTVEP7), period);
#endif // USE_MBED
             }
             case 6: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[5].out
                        + this->m_experimentConfig.experimentTVEP.outputs[5].wait
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker6->attach(callback(this, &ExperimentProgram::tickerTVEP6), period);
#endif // USE_MBED
             }
             case 5: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[4].out
                        + this->m_experimentConfig.experimentTVEP.outputs[4].wait
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker5->attach(callback(this, &ExperimentProgram::tickerTVEP5), period);
#endif // USE_MBED
             }

             case 4: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[3].out
                        + this->m_experimentConfig.experimentTVEP.outputs[3].wait
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker4->attach(callback(this, &ExperimentProgram::tickerTVEP4), period);
#endif // USE_MBED
             }
             case 3: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[2].out
                        + this->m_experimentConfig.experimentTVEP.outputs[2].wait
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker3->attach(callback(this, &ExperimentProgram::tickerTVEP3), period);
#endif // USE_MBED
             }
             case 2: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[1].out
                        + this->m_experimentConfig.experimentTVEP.outputs[1].wait
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker2->attach(callback(this, &ExperimentProgram::tickerTVEP2), period);
#endif // USE_MBED
             }
             case 1: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[0].out
                        + this->m_experimentConfig.experimentTVEP.outputs[0].wait
                        + 0.0f;
#ifdef USE_MBED
                 this->m_usedPeripherals->ticker1->attach(callback(this, &ExperimentProgram::tickerTVEP1), period);
#endif // USE_MBED
             }
        }
    }

    /**
     * Provede se inicializace REA experimentu.
     * Uloží se počet výstupů do samostatné proměnné.
     */
    void setupREA() {
        this->m_outputCount = this->m_experimentConfig.experimentTVEP.head.outputCount;
    }

/*---------- Setup funkce výstupů ------------*/


/*------------- Pomocné funkce ---------------*/

    bool isRunning() {
        return this->m_state == RUNNING;
    }

    /**
     * Pomocná funkce pro odeslání informace na řídící zařízení,
     * že se změnila hodnota na výstupu.
     *
     * @param ioType Hodnota výstupu.
     * @param index Index výstupu, u kterého se změnila hodnota.
     */
    void ioChange(uint8_t ioType, uint8_t index) {
        if (!this->isRunning()) {
            return;
        }

        ServerCommandData commandData;
        commandData.header.type = ioType;
        commandData.header.length = 7; //sizeof(server_command_io_change_t);
        commandData.commandIOChange.index = index;
#ifdef USE_MBED
        uint32_t timestamp = this->m_usedPeripherals->globalTimer->read_us();
        commandData.commandIOChange.timestamp = timestamp & 0xFFFFFFFF;
        this->m_serverCommandQueue->push(commandData);
#else
        commandData.commandIOChange.timestamp = 0xFFFFFFFF;
        // TODO odeslat IO změnu na výstup
#endif // USE_MBED

    }

    /**
     * Pomocná zjednodušená funkce pro vypnutí výstupu.
     *
     * @param index Index výstupu, který se má vypnout.
     */
    void turnOffOutput(uint8_t index) {
        this->setOutput(index, 0.0f, 7); // 7 = 0b0111
    }

    /**
     * Pomocná funkce pro nastavení výstupu
     *
     * @param index Index výstupu, kterému se má nastavit hodnota
     * @param brightness Svítivost výstupu (pouze v případě LED výstupu)
     * @param outputType Typ výstupu @link experiment_output_type_t
     */
    void setOutput(uint8_t index, experiment_output_brightness_t brightness, experiment_output_type_t outputType) {
#ifdef USE_MBED
        if ((outputType & 0x01) == 1) {
            // Jedná se o LED -> tak ji rozsvítím podle svítivosti
            *this->m_usedPeripherals->outputs[index] = brightness;
        } else {
            // Jedná se o grafický, nebo zvukový výstup, tak to signalizuji externímu zařízení
            this->m_usedPeripherals->graphicsSerial->putc(index);
            this->m_usedPeripherals->graphicsSerial->putc(outputType);
            this->m_usedPeripherals->graphicsSerial->putc(0xFF);
        }
#else
#endif // USE_MBED
    }

    /**
     * Pomocná funkce, která vytvoří příkaz s informací o dokončeném
     * experimentu.
     */
    void sendExperimentFinishedCommand() {
        this->m_state = FINISHED;
        ServerCommandData commandData;
        commandData.header.type = COMMAND_STIMULATOR_STATE;
        commandData.header.length = 8; //sizeof(server_command_io_change_t);
        commandData.commandStimulatorState.state = this->getState();
        commandData.commandStimulatorState.noUpdate = 0;
#ifdef USE_MBED
        uint32_t timestamp = this->m_usedPeripherals->globalTimer->read_us();
        commandData.commandStimulatorState.timestamp = timestamp & 0xFFFFFFFF;
        this->m_serverCommandQueue->push(commandData);
#else
        commandData.commandStimulatorState.timestamp = 0xFFFFFFFF;
#endif // USE_MBED
    }

/*------ Interrupty výstupů a tlačítek -------*/
#ifdef USE_MBED
    void input1Interrupt() {this->inputInterrupt(0);}
    void input2Interrupt() {this->inputInterrupt(1);}
    void input3Interrupt() {this->inputInterrupt(2);}
    void input4Interrupt() {this->inputInterrupt(3);}
    void input5Interrupt() {this->inputInterrupt(4);}
    void input6Interrupt() {this->inputInterrupt(5);}
    void input7Interrupt() {this->inputInterrupt(6);}
    void input8Interrupt() {this->inputInterrupt(7);}
    void inputInterrupt(uint8_t index) {
        if (this->m_inputButtonDelays[index] == 0) {
            this->ioChange(COMMAND_INPUT_ACTIVATED, index);
        }
        this->m_inputButtonDelays[index]++;
    }

    /**
     * Pomocná funkce pro inicializaci interruptů tlačítek.
     */
    void setupInputInterrupts() {
        this->m_usedPeripherals->interruptIn8->fall(callback(this, &ExperimentProgram::input8Interrupt));
        this->m_usedPeripherals->interruptIn7->fall(callback(this, &ExperimentProgram::input7Interrupt));
        this->m_usedPeripherals->interruptIn6->fall(callback(this, &ExperimentProgram::input6Interrupt));
        this->m_usedPeripherals->interruptIn5->fall(callback(this, &ExperimentProgram::input5Interrupt));
        this->m_usedPeripherals->interruptIn4->fall(callback(this, &ExperimentProgram::input4Interrupt));
        this->m_usedPeripherals->interruptIn3->fall(callback(this, &ExperimentProgram::input3Interrupt));
        this->m_usedPeripherals->interruptIn2->fall(callback(this, &ExperimentProgram::input2Interrupt));
        this->m_usedPeripherals->interruptIn1->fall(callback(this, &ExperimentProgram::input1Interrupt));
    }
#endif // USE_MBED

/*---------------- ERP funkce ----------------*/

    /**
     * Pomocná ERP funkce pro odeslání požadavku na získání
     * další části ERP sekvence.
     *
     * @param offset
     * @param index
     */
    void sendSequencePartRequest(uint16_t offset, uint8_t index) {
        ServerCommandData commandData;
        commandData.header.type = COMMAND_SEQUENCE_NEXT_PART_REQUEST;
        commandData.header.length = 9; //sizeof(server_command_io_change_t);
        commandData.commandSequencePartRequest.offset = offset;
        commandData.commandSequencePartRequest.index = index;
#ifdef USE_MBED
        uint32_t timestamp = this->m_usedPeripherals->globalTimer->read_us();
        commandData.commandSequencePartRequest.timestamp = timestamp & 0xFFFFFFFF;
        this->m_serverCommandQueue->push(commandData);
#else
        commandData.commandSequencePartRequest.timestamp = 0xFFFFFFFF;
#endif // USE_MBED
    }

/*---------------- Časovače ------------------*/
    void tickerERP() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        const uint16_t index = this->m_counters[0];
        const uint16_t offset = this->m_counters[1];
        uint8_t output = (this->m_accumulators[index] >> (offset * 4)) & 0xF;
        uint8_t period = 0;
        experiment_output_brightness_t brightness = 0;
        experiment_output_type_t outputType = 0;
        if (output == 0) {
            for (size_t i = 0; i < this->m_experimentConfig.experimentERP.head.outputCount; i++) {
                this->ioChange(COMMAND_OUTPUT_DEACTIVATED, i);
            }

            goto erp_acc_update;
        }
        output -= 1;
        period = this->m_experimentConfig.experimentERP.head.out;
        brightness = this->m_experimentConfig.experimentERP.outputs[output].brightness;
        outputType = this->m_experimentConfig.experimentERP.outputs[output].outputType;

        this->setOutput(output, brightness, outputType);
        this->ioChange(COMMAND_OUTPUT_ACTIVATED, output);

#ifdef USE_MBED
        switch (output) {
             case 7:
                 this->m_usedPeripherals->timeout8->attach(callback(this, &ExperimentProgram::timeoutERP8), period);
                 break;
             case 6:
                 this->m_usedPeripherals->timeout7->attach(callback(this, &ExperimentProgram::timeoutERP7), period);
                 break;
             case 5:
                 this->m_usedPeripherals->timeout6->attach(callback(this, &ExperimentProgram::timeoutERP6), period);
                 break;
             case 4:
                 this->m_usedPeripherals->timeout5->attach(callback(this, &ExperimentProgram::timeoutERP5), period);
                 break;
             case 3:
                 this->m_usedPeripherals->timeout4->attach(callback(this, &ExperimentProgram::timeoutERP4), period);
                 break;
             case 2:
                 this->m_usedPeripherals->timeout3->attach(callback(this, &ExperimentProgram::timeoutERP3), period);
                 break;
             case 1:
                 this->m_usedPeripherals->timeout2->attach(callback(this, &ExperimentProgram::timeoutERP2), period);
                 break;
             case 0:
                 this->m_usedPeripherals->timeout1->attach(callback(this, &ExperimentProgram::timeoutERP1), period);
                 break;
        }
#endif // USE_MBED
erp_acc_update:
        const experiment_erp_sequence_size_t sequenceSize = this->m_experimentConfig.experimentERP.head.sequenceSize;
        if (this->m_counters[4] < sequenceSize) {
            this->m_counters[4]++;
        } else {
            this->sendExperimentFinishedCommand();
            return;
        }
        // Čítač offsetu v aktuálním accumulatoru zvětším
        this->m_counters[1]++;
        // Pokud offset ukazuje "mimo pole"
        if (this->m_counters[1] == 8) {
            // Vynuluji offset
            this->m_counters[1] = 0;
            // Inkrementuji index na celý accumulator
            this->m_counters[0]++;
            // Pokud index na accumulator ukazuje "mimo pole"
            if (this->m_counters[0] == 8) {
                // Vynuluji index na accumulator
                this->m_counters[0] = 0;
            }
            this->sendSequencePartRequest(this->m_counters[2], this->m_counters[3]);
        }
    }
/*---------------- Timeouty ------------------*/

#ifdef USE_MBED
    void timeoutERP1() {this->timeoutERP(0);}
    void timeoutERP2() {this->timeoutERP(1);}
    void timeoutERP3() {this->timeoutERP(2);}
    void timeoutERP4() {this->timeoutERP(3);}
    void timeoutERP5() {this->timeoutERP(4);}
    void timeoutERP6() {this->timeoutERP(5);}
    void timeoutERP7() {this->timeoutERP(6);}
    void timeoutERP8() {this->timeoutERP(7);}
    void timeoutERP(uint8_t index) {
        this->turnOffOutput(index);
        this->ioChange(COMMAND_OUTPUT_DEACTIVATED, index);
    }
#endif // USE_MBED

/*--------------- CVEP funkce ----------------*/
/*---------------- Časovače ------------------*/
    void tickerCVEP() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        const experiment_output_type_t outputType = this->m_experimentConfig.experimentCVEP.head.outputType;
        const experiment_output_brightness_t brightness = this->m_experimentConfig.experimentCVEP.head.brightness / 100.0f;
        const experiment_cvep_bit_shift_t bitShift = this->m_experimentConfig.experimentCVEP.head.bitShift;
        const experiment_cvep_pattern_t pattern = this->m_experimentConfig.experimentCVEP.head.pattern;
        for (experiment_output_count_t i = 0; i < this->m_outputCount; i++) {
            if ((pattern >> ((i * bitShift) + this->m_counters[0]) % 32) & 1) {
                this->setOutput(i, brightness, outputType);
                this->ioChange(COMMAND_OUTPUT_ACTIVATED, i);
            }
        }
        this->m_counters[0] = (this->m_counters[0] + 1) % 32;

        const experiment_output_out_t period = this->m_experimentConfig.experimentCVEP.head.out;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout1->attach(callback(this, &ExperimentProgram::timeoutCVEP), period);
#endif // USE_MBED
    }
/*---------------- Timeouty ------------------*/

#ifdef USE_MBED
    void timeoutCVEP() {
        for (experiment_output_count_t i = 0; i < this->m_outputCount; i++) {
           this->turnOffOutput(i);
           this->ioChange(COMMAND_OUTPUT_DEACTIVATED, i);
           this->m_inputButtonDelays[i] = 0;
        }
    }
#endif // USE_MBED

/*--------------- FVEP funkce ----------------*/
/*---------------- Časovače ------------------*/

    void tickerFVEP1() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(0);
        const experiment_fvep_output_time_on_t period = this->m_experimentConfig.experimentFVEP.outputs[0].timeOn + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout1->attach(callback(this, &ExperimentProgram::timeoutFVEP1), period);
#endif // USE_MBED
    }
    void tickerFVEP2() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(1);
        const experiment_fvep_output_time_on_t period = this->m_experimentConfig.experimentFVEP.outputs[1].timeOn + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout2->attach(callback(this, &ExperimentProgram::timeoutFVEP2), period);
#endif // USE_MBED
    }
    void tickerFVEP3() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(2);
        const experiment_fvep_output_time_on_t period = this->m_experimentConfig.experimentFVEP.outputs[2].timeOn + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout3->attach(callback(this, &ExperimentProgram::timeoutFVEP3), period);
#endif // USE_MBED
    }
    void tickerFVEP4() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(3);
        const experiment_fvep_output_time_on_t period = this->m_experimentConfig.experimentFVEP.outputs[3].timeOn + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout4->attach(callback(this, &ExperimentProgram::timeoutFVEP4), period);
#endif // USE_MBED
    }
void tickerFVEP5() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(4);
        const experiment_fvep_output_time_on_t period = this->m_experimentConfig.experimentFVEP.outputs[4].timeOn + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout5->attach(callback(this, &ExperimentProgram::timeoutFVEP5), period);
#endif // USE_MBED
    }
void tickerFVEP6() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(5);
        const experiment_fvep_output_time_on_t period = this->m_experimentConfig.experimentFVEP.outputs[5].timeOn + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout6->attach(callback(this, &ExperimentProgram::timeoutFVEP6), period);
#endif // USE_MBED
    }
void tickerFVEP7() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(6);
        const experiment_fvep_output_time_on_t period = this->m_experimentConfig.experimentFVEP.outputs[6].timeOn + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout7->attach(callback(this, &ExperimentProgram::timeoutFVEP7), period);
#endif // USE_MBED
    }
void tickerFVEP8() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(7);
        const experiment_fvep_output_time_on_t period = this->m_experimentConfig.experimentFVEP.outputs[7].timeOn + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout8->attach(callback(this, &ExperimentProgram::timeoutFVEP8), period);
#endif // USE_MBED
    }
    void tickerFVEP(uint8_t index) {
        const experiment_output_brightness_t brightness = this->m_experimentConfig.experimentFVEP.outputs[index].brightness / 100.0f;
        const experiment_output_type_t outputType = this->m_experimentConfig.experimentFVEP.outputs[index].outputType;
        this->setOutput(index, brightness, outputType);
        this->ioChange(COMMAND_OUTPUT_ACTIVATED, index);
        this->m_inputButtonDelays[index] = 0;
    }
/*---------------- Timeouty ------------------*/

#ifdef USE_MBED
    void timeoutFVEP1() {this->timeoutFVEP(0);}
    void timeoutFVEP2() {this->timeoutFVEP(1);}
    void timeoutFVEP3() {this->timeoutFVEP(2);}
    void timeoutFVEP4() {this->timeoutFVEP(3);}
    void timeoutFVEP5() {this->timeoutFVEP(4);}
    void timeoutFVEP6() {this->timeoutFVEP(5);}
    void timeoutFVEP7() {this->timeoutFVEP(6);}
    void timeoutFVEP8() {this->timeoutFVEP(7);}
    void timeoutFVEP(uint8_t index) {
        this->turnOffOutput(index);
        this->ioChange(COMMAND_OUTPUT_DEACTIVATED, index);
    }
#endif // USE_MBED


/*--------------- TVEP funkce ----------------*/
/*---------------- Časovače ------------------*/

    void tickerTVEP1() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerTVEP(0);
        const float period = this->m_experimentConfig.experimentTVEP.outputs[0].out + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout1->attach(callback(this, &ExperimentProgram::timeoutTVEP1), period);
#endif // USE_MBED
    }
    void tickerTVEP2() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerTVEP(1);
        const float period = this->m_experimentConfig.experimentTVEP.outputs[1].out + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout2->attach(callback(this, &ExperimentProgram::timeoutTVEP2), period);
#endif // USE_MBED
    }
    void tickerTVEP3() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerTVEP(2);
        const float period = this->m_experimentConfig.experimentTVEP.outputs[2].out + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout3->attach(callback(this, &ExperimentProgram::timeoutTVEP3), period);
#endif // USE_MBED
    }
    void tickerTVEP4() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerTVEP(3);
        const float period = this->m_experimentConfig.experimentTVEP.outputs[3].out + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout4->attach(callback(this, &ExperimentProgram::timeoutTVEP4), period);
#endif // USE_MBED
    }
    void tickerTVEP5() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerTVEP(4);
        const float period = this->m_experimentConfig.experimentTVEP.outputs[4].out + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout5->attach(callback(this, &ExperimentProgram::timeoutTVEP5), period);
#endif // USE_MBED
    }
    void tickerTVEP6() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerTVEP(5);
        const float period = this->m_experimentConfig.experimentTVEP.outputs[5].out + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout6->attach(callback(this, &ExperimentProgram::timeoutTVEP6), period);
#endif // USE_MBED
    }
    void tickerTVEP7() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerTVEP(6);
        const float period = this->m_experimentConfig.experimentTVEP.outputs[6].out + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout7->attach(callback(this, &ExperimentProgram::timeoutTVEP7), period);
#endif // USE_MBED
    }
    void tickerTVEP8() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerTVEP(7);
        const float period = this->m_experimentConfig.experimentTVEP.outputs[7].out + 0.0f;
#ifdef USE_MBED
        this->m_usedPeripherals->timeout8->attach(callback(this, &ExperimentProgram::timeoutTVEP8), period);
#endif // USE_MBED
    }
    void tickerTVEP(uint8_t index) {
        const experiment_output_brightness_t brightness = this->m_experimentConfig.experimentTVEP.outputs[index].brightness / 100.0f;
        const experiment_tvep_output_pattern_length_t patternLength = this->m_experimentConfig.experimentTVEP.outputs[index].patternLength;
        const experiment_tvep_output_pattern_t pattern = this->m_experimentConfig.experimentTVEP.outputs[index].pattern;
        const experiment_output_type_t outputType = this->m_experimentConfig.experimentTVEP.outputs[index].outputType;
        if ((pattern >> (this->m_counters[index] % patternLength)) & 1) {
            this->setOutput(index, brightness, outputType);
            this->ioChange(COMMAND_OUTPUT_ACTIVATED, index);
        }
        this->m_counters[index] = (this->m_counters[index] + 1) % patternLength;
        this->m_inputButtonDelays[index] = 0;
    }
/*---------------- Timeouty ------------------*/

    void timeoutTVEP1() {this->timeoutTVEP(0);}
    void timeoutTVEP2() {this->timeoutTVEP(1);}
    void timeoutTVEP3() {this->timeoutTVEP(2);}
    void timeoutTVEP4() {this->timeoutTVEP(3);}
    void timeoutTVEP5() {this->timeoutTVEP(4);}
    void timeoutTVEP6() {this->timeoutTVEP(5);}
    void timeoutTVEP7() {this->timeoutTVEP(6);}
    void timeoutTVEP8() {this->timeoutTVEP(7);}
    void timeoutTVEP(uint8_t index) {
        this->turnOffOutput(index);
        this->ioChange(COMMAND_OUTPUT_DEACTIVATED, index);
    }

/*---------------- REA funkce ----------------*/
/*---------------- Časovače ------------------*/
/*---------------- Timeouty ------------------*/





public:

    ExperimentProgram(): m_outputCount(0), m_state(READY),
                         m_usedPeripherals(NULL) {
        memset(&this->m_experimentConfig, 0, sizeof(ExperimentConfig));
    }

    /**
     * Uloží konfiguraci experimentu do paměti
     */
    void init(ExperimentConfig &experimentConfig) {
        this->clear();
        m_experimentConfig = experimentConfig;
        this->m_state = UPLOADED;
    }

    /**
     * Inicializuje uloženou konfiguraci v paměti
     */
    void setup() {
        switch(this->m_experimentConfig.type) {
            // Experiment ERP
            case 0x01:
                this->setupERP();
                break;
            // Experiment CVEP
            case 0x02:
                this->setupCVEP();
                break;
            // Experiment FVEP
            case 0x03:
                this->setupFVEP();
                break;
            // Experiment TVEP
            case 0x04:
                this->setupTVEP();
                break;
            // Experiment REA
            case 0x05:
                this->setupREA();
                break;
        }
        this->m_state = INITIALIZED;
    }

    /**
     * Uloží konfiguraci příslušného výstupu.
     * Musí být zavoláno až poté, co se nahraje experiment!
     *
     * @param index Index výstupu, pro který se má uložit konfigurace.
     * @param outputConfig Konfigurace výstupu.
     */
    void setupOutput(uint8_t index, OutputConfig &outputConfig) {
        switch(m_experimentConfig.type) {
            // Experiment ERP
            case 0x01:
                m_experimentConfig.experimentERP.outputs[index] = outputConfig.outputERP;
                break;
            // Experiment CVEP
            case 0x02:
                // m_experimentConfig.experimentCVEP.outputs[index] = outputConfig;
                break;
            // Experiment FVEP
            case 0x03:
                m_experimentConfig.experimentFVEP.outputs[index] = outputConfig.outputFVEP;
                break;
            // Experiment TVEP
            case 0x04:
                m_experimentConfig.experimentTVEP.outputs[index] = outputConfig.outputTVEP;
                break;
            // Experiment REA
            case 0x05:
                // m_experimentConfig.experimentREA.outputs[indes] = outputConfig;
                break;
        }
    }

#ifdef USE_MBED
    void initInterrupts() {
        this->setupInputInterrupts();
    }
#endif // USE_MBED

    /**
     * Spustí experiment
     */
    void start() {
        this->m_state = RUNNING;
    }

    /**
     * Pozastaví experiment
     */
    void pause() {
        this->m_state = PAUSED;
    }

    /**
     * Zastaví experiment
     */
    void stop() {
        this->m_state = FINISHED;
    }

    /**
     * Vymaže data experimentu.
     * Dále vymaže veškeré pomocné buffery.
     */
    void clear() {
#ifdef USE_MBED
        for (uint8_t i = 0; i < TOTAL_OUTPUT_COUNT; i++) {
            // Temito dvema radky se zastavi blikani LEDek
            *this->m_usedPeripherals->outputs[i] = 0;
            // Timto se detachnou veškeré timery, takže přestanou
            // generovat přerušení
#ifdef USE_MBED
            this->m_usedPeripherals->tickers[i]->detach();
#endif // USE_MBED
        }
#endif // USE_MBED
        memset(&this->m_experimentConfig, 0, sizeof(ExperimentConfig));
        memset(this->m_inputButtonDelays, 0, sizeof(uint8_t) * TOTAL_OUTPUT_COUNT);
        memset(this->m_counters, 0, sizeof(uint16_t) * TOTAL_OUTPUT_COUNT);
        memset(this->m_accumulators, 0, sizeof(uint32_t) * TOTAL_OUTPUT_COUNT);
        this->m_state = CLEARED;
    }

#ifdef USE_MBED
    void setPeripherals(used_peripherals_t *peripherals) {
        this->m_usedPeripherals = peripherals;
    }

    void setServerCommandQueue(CircularBuffer<ServerCommandData, 16> *serverCommandQueue) {
        this->m_serverCommandQueue = serverCommandQueue;
    }
#endif // USE_MBED

    void copyExperimentConfig(void *config) {
        memcpy(config, &this->m_experimentConfig, sizeof(ExperimentConfig));
    }

    void copyAccumulators(void *accumulators) {
        memcpy(accumulators, this->m_accumulators, sizeof(uint32_t) * TOTAL_OUTPUT_COUNT);
    }

    void copyCounters(void *counters) {
        memcpy(counters, this->m_counters, sizeof(uint16_t) * TOTAL_OUTPUT_COUNT);
    }

    uint8_t getState() {
        return this->m_state;
    }

    void updateAccumulator(uint8_t index, uint32_t data) {
        this->m_accumulators[index] = data;
        this->m_counters[2]++;
        if (this->m_counters[2] == 8) {
            this->m_counters[2] = 0;
        }
        this->m_counters[3] += 8;
    }
};



#endif