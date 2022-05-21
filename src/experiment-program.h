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
/*---------------- Proměnné ------------------*/
    // Pole pro zabránění několikanásobnému stisku tlačítka
    // během jednoho cyklu
    uint8_t m_inputButtonDelays[TOTAL_OUTPUT_COUNT];
    // Poslední známé stisknuté tlačítko
    int8_t m_lastPressedButton = -1;
    // Aktuální stav stimulátoru
    ExperimentState m_state;
    // Pointer na periferie
    used_peripherals_t *m_usedPeripherals;
    // konfigurace experimentu;
    ExperimentConfig m_experimentConfig;
    // Pointer na cyklický buffer s příkazy, které se budou posílat ven
    CircularBuffer<ServerCommandData, 16> *m_serverCommandQueue;


/*-------- Setup funkce experimentů ----------*/
    /**
     * Provede se inicializace ERP experimentu.
     * Uloží se počet výstupů do samostatné proměnné.
     * Dále inicializuje jeden časovač @link ticker1,
     *  který bude pohánět celý experiment.
     */
    void setupERP() {
        this->m_experimentConfig.experimentERP.data.randomBase = 10;
        this->m_experimentConfig.experimentERP.data.currentOutput = 0;
        memset(&this->m_experimentConfig.experimentERP.data.sequence_data, 0, sizeof(experiment_erp_sequence_data_t));
        memset(this->m_experimentConfig.experimentERP.data.accumulators, 0, sizeof(uint32_t) * TOTAL_OUTPUT_COUNT);
        // us_timestamp_t period = this->m_experimentConfig.experimentERP.head.out
        //                    + this->m_experimentConfig.experimentERP.head.wait
        //                    + 0.0f;
        // const experiment_erp_random_t randomType = this->m_experimentConfig.experimentERP.head.random;

        // us_timestamp_t randomWait = 0;

        // switch (randomType) {
        //     case SHORT:
        //         randomWait = this->m_experimentConfig.experimentERP.data.randomBase * ((20 + 80) * 1000);
        //         break;
        //     case LONG:
        //         randomWait = this->m_experimentConfig.experimentERP.data.randomBase * ((20 + 100) * 1000);
        //         break;
        //     case SHORT_LONG:
        //         randomWait = this->m_experimentConfig.experimentERP.data.randomBase * ((40 + 80) * 1000);
        //         break;
        //     case OFF:
        //         break;
        // }

        // this->m_usedPeripherals->ticker1->attach_us(callback(this, &ExperimentProgram::tickerERP), period + randomWait);
        this->m_usedPeripherals->timeout1->attach(callback(this, &ExperimentProgram::tickerERP), 3.0);
        this->sendSequencePartRequest(0, 0);
        this->sendSequencePartRequest(8, 1);
    }

    /**
     * Provede se inicializace CVEP experimentu.
     * Uloží se počet výstupů do samostatné proměnné.
     * Dále inicializuje jeden časovač @link ticker1,
     *  který bude pohánět celý experiment.
     */
    void setupCVEP() {
        const us_timestamp_t period = this->m_experimentConfig.experimentCVEP.head.out
                           + this->m_experimentConfig.experimentCVEP.head.wait
                           + 0.0f;
        this->m_experimentConfig.experimentCVEP.data.counter = 0;
        this->m_usedPeripherals->ticker1->attach_us(callback(this, &ExperimentProgram::tickerCVEP), period);
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
        const uint8_t outputCount = this->m_experimentConfig.experimentFVEP.head.outputCount;
        us_timestamp_t period = 0.0f;

        switch (outputCount) {
            case 8: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[7].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[7].timeOff
                        + 0.0f;
                 this->m_usedPeripherals->ticker8->attach_us(callback(this, &ExperimentProgram::tickerFVEP8), period);
                 __attribute__ ((fallthrough));
             }
            case 7: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[6].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[6].timeOff
                        + 0.0f;
                 this->m_usedPeripherals->ticker7->attach_us(callback(this, &ExperimentProgram::tickerFVEP7), period);
                 __attribute__ ((fallthrough));
             }
            case 6: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[5].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[5].timeOff
                        + 0.0f;
                 this->m_usedPeripherals->ticker6->attach_us(callback(this, &ExperimentProgram::tickerFVEP6), period);
                 __attribute__ ((fallthrough));
             }
            case 5: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[4].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[4].timeOff
                        + 0.0f;
                 this->m_usedPeripherals->ticker5->attach_us(callback(this, &ExperimentProgram::tickerFVEP5), period);
                 __attribute__ ((fallthrough));
             }
             case 4: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[3].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[3].timeOff
                        + 0.0f;
                 this->m_usedPeripherals->ticker4->attach_us(callback(this, &ExperimentProgram::tickerFVEP4), period);
                 __attribute__ ((fallthrough));
             }
             case 3: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[2].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[2].timeOff
                        + 0.0f;
                 this->m_usedPeripherals->ticker3->attach_us(callback(this, &ExperimentProgram::tickerFVEP3), period);
                 __attribute__ ((fallthrough));
             }
             case 2: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[1].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[1].timeOff
                        + 0.0f;
                 this->m_usedPeripherals->ticker2->attach_us(callback(this, &ExperimentProgram::tickerFVEP2), period);
                 __attribute__ ((fallthrough));
             }
             case 1: {
                 period = this->m_experimentConfig.experimentFVEP.outputs[0].timeOn
                        + this->m_experimentConfig.experimentFVEP.outputs[0].timeOff
                        + 0.0f;
                 this->m_usedPeripherals->ticker1->attach_us(callback(this, &ExperimentProgram::tickerFVEP1), period);
                 __attribute__ ((fallthrough));
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
        const uint8_t outputCount = this->m_experimentConfig.experimentTVEP.head.outputCount;
        memset(this->m_experimentConfig.experimentTVEP.data.counters, 0, sizeof(uint16_t) * TOTAL_OUTPUT_COUNT);
        us_timestamp_t period = 0.0f;

        switch (outputCount) {
             case 8: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[7].out
                        + this->m_experimentConfig.experimentTVEP.outputs[7].wait
                        + 0.0f;
                 this->m_usedPeripherals->ticker8->attach_us(callback(this, &ExperimentProgram::tickerTVEP8), period);
                 __attribute__ ((fallthrough));
             }
             case 7: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[6].out
                        + this->m_experimentConfig.experimentTVEP.outputs[6].wait
                        + 0.0f;
                 this->m_usedPeripherals->ticker7->attach_us(callback(this, &ExperimentProgram::tickerTVEP7), period);
                 __attribute__ ((fallthrough));
             }
             case 6: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[5].out
                        + this->m_experimentConfig.experimentTVEP.outputs[5].wait
                        + 0.0f;
                 this->m_usedPeripherals->ticker6->attach_us(callback(this, &ExperimentProgram::tickerTVEP6), period);
                 __attribute__ ((fallthrough));
             }
             case 5: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[4].out
                        + this->m_experimentConfig.experimentTVEP.outputs[4].wait
                        + 0.0f;
                 this->m_usedPeripherals->ticker5->attach_us(callback(this, &ExperimentProgram::tickerTVEP5), period);
                 __attribute__ ((fallthrough));
             }

             case 4: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[3].out
                        + this->m_experimentConfig.experimentTVEP.outputs[3].wait
                        + 0.0f;
                 this->m_usedPeripherals->ticker4->attach_us(callback(this, &ExperimentProgram::tickerTVEP4), period);
                 __attribute__ ((fallthrough));
             }
             case 3: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[2].out
                        + this->m_experimentConfig.experimentTVEP.outputs[2].wait
                        + 0.0f;
                 this->m_usedPeripherals->ticker3->attach_us(callback(this, &ExperimentProgram::tickerTVEP3), period);
                 __attribute__ ((fallthrough));
             }
             case 2: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[1].out
                        + this->m_experimentConfig.experimentTVEP.outputs[1].wait
                        + 0.0f;
                 this->m_usedPeripherals->ticker2->attach_us(callback(this, &ExperimentProgram::tickerTVEP2), period);
                 __attribute__ ((fallthrough));
             }
             case 1: {
                 period = this->m_experimentConfig.experimentTVEP.outputs[0].out
                        + this->m_experimentConfig.experimentTVEP.outputs[0].wait
                        + 0.0f;
                 this->m_usedPeripherals->ticker1->attach_us(callback(this, &ExperimentProgram::tickerTVEP1), period);
                 __attribute__ ((fallthrough));
             }
        }
    }

    /**
     * Provede se inicializace REA experimentu.
     * Uloží se počet výstupů do samostatné proměnné.
     */
    void setupREA() {
        this->m_experimentConfig.experimentREA.data.counter = 0;
        this->m_experimentConfig.experimentREA.data.usedOutput = 0;
        const us_timestamp_t period = this->m_experimentConfig.experimentREA.head.waitTimeMax;
        this->m_usedPeripherals->ticker1->attach_us(callback(this, &ExperimentProgram::tickerREA), period);
    }

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
        commandData.header.commandId = HEADER_NO_ID;
        commandData.header.type = ioType;
        commandData.header.length = 8; //sizeof(server_command_io_change_t);
        commandData.commandIOChange.index = index;
        uint32_t timestamp = this->m_usedPeripherals->globalTimer->read_us();
        commandData.commandIOChange.timestamp = timestamp & 0xFFFFFFFF;
        this->m_serverCommandQueue->push(commandData);
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
        if ((outputType & 0x01) == 1) {
            // Jedná se o LED -> tak ji rozsvítím podle svítivosti
            *this->m_usedPeripherals->outputs[index] = brightness / 100.0;
        } else {
            // Jedná se o grafický, nebo zvukový výstup, tak to signalizuji externímu zařízení
            this->m_usedPeripherals->graphicsSerial->putc(index);
            this->m_usedPeripherals->graphicsSerial->putc(outputType);
            this->m_usedPeripherals->graphicsSerial->putc(0xFF);
        }
    }

    /**
     * Pomocná funkce, která vytvoří příkaz s informací o dokončeném
     * experimentu.
     */
    void sendExperimentFinishedCommand() {
        this->m_state = FINISHED;
        ServerCommandData commandData;
        commandData.header.commandId = HEADER_NO_ID;
        commandData.header.type = COMMAND_STIMULATOR_REQUEST_FINISH;
        commandData.header.length = 8; //sizeof(server_command_io_change_t);
        uint32_t timestamp = this->m_usedPeripherals->globalTimer->read_us();
        commandData.commandStimulatorRequestFinish.timestamp = timestamp & 0xFFFFFFFF;
        this->m_serverCommandQueue->push(commandData);
    }

/*------ Interrupty výstupů a tlačítek -------*/

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
        if (this->m_lastPressedButton == -1) {
            this->m_lastPressedButton = index;
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
        commandData.header.commandId = HEADER_NO_ID;
        commandData.header.type = COMMAND_SEQUENCE_NEXT_PART_REQUEST;
        commandData.header.length = 10; //sizeof(server_command_io_change_t);
        commandData.commandSequencePartRequest.offset = offset;
        commandData.commandSequencePartRequest.index = index;
        uint32_t timestamp = this->m_usedPeripherals->globalTimer->read_us();
        commandData.commandSequencePartRequest.timestamp = timestamp & 0xFFFFFFFF;
        this->m_serverCommandQueue->push(commandData);
    }

/*---------------- Časovače ------------------*/
    void tickerERP() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        // Získám index ukazující do pole akumulátorů
        const uint16_t index = this->m_experimentConfig.experimentERP.data.sequence_data.accIndex;
        // Získám offset ukazující v poli akumulátorů na jeden konkrétní prvek
        const uint16_t offset = this->m_experimentConfig.experimentERP.data.sequence_data.accOffset;
        // Číslo výstupu je vypočítáno na zákaldě indexu a offsetu; Pro jistotu se moduluje
        uint8_t output = ((this->m_experimentConfig.experimentERP.data.accumulators[index] >> (offset * 4)) & 0xF) % TOTAL_OUTPUT_COUNT;
        // Uložím aktuální číslo výstupu pro pozdější využití
        this->m_experimentConfig.experimentERP.data.currentOutput = output;
        us_timestamp_t period = 0;
        experiment_output_brightness_t brightness = 0;
        experiment_output_type_t outputType = 0;
        experiment_erp_edge_t edge = RISING;
        experiment_erp_random_t randomType = OFF;


        period = this->m_experimentConfig.experimentERP.outputs[output].pulseUp;
        brightness = this->m_experimentConfig.experimentERP.outputs[output].brightness;
        outputType = this->m_experimentConfig.experimentERP.outputs[output].outputType;
        edge = this->m_experimentConfig.experimentERP.head.edge;
        randomType = this->m_experimentConfig.experimentERP.head.random;

        // // Pokud je zapnutá náhodnost na náběžné hraně
        // if (randomType != OFF && edge == RISING) {
        //     us_timestamp_t randomWait = 0;
        //     switch (randomType) {
        //         case SHORT:
        //             randomWait = this->m_experimentConfig.experimentERP.data.randomBase * (((rand() % 21) + 80) * 1000);
        //             break;
        //         case LONG:
        //             randomWait = this->m_experimentConfig.experimentERP.data.randomBase * (((rand() % 21) + 100) * 1000);
        //             break;
        //         case SHORT_LONG:
        //             randomWait = this->m_experimentConfig.experimentERP.data.randomBase * (((rand() % 41) + 80) * 1000);
        //             break;
        //         case OFF:
        //             break;
        //     }
        //     wait_us(randomWait);
        // }

        // Nastavím příslušný výstup a jeho svítivost
        this->setOutput(output, brightness, outputType);
        // Odešlu informaci ven, že se aktivoval výstup
        this->ioChange(COMMAND_OUTPUT_ACTIVATED, output);

        switch (output) {
             case 7:
                 this->m_usedPeripherals->timeout1->attach_us(callback(this, &ExperimentProgram::timeoutERP8), period);
                 break;
             case 6:
                 this->m_usedPeripherals->timeout1->attach_us(callback(this, &ExperimentProgram::timeoutERP7), period);
                 break;
             case 5:
                 this->m_usedPeripherals->timeout1->attach_us(callback(this, &ExperimentProgram::timeoutERP6), period);
                 break;
             case 4:
                 this->m_usedPeripherals->timeout1->attach_us(callback(this, &ExperimentProgram::timeoutERP5), period);
                 break;
             case 3:
                 this->m_usedPeripherals->timeout1->attach_us(callback(this, &ExperimentProgram::timeoutERP4), period);
                 break;
             case 2:
                 this->m_usedPeripherals->timeout1->attach_us(callback(this, &ExperimentProgram::timeoutERP3), period);
                 break;
             case 1:
                 this->m_usedPeripherals->timeout1->attach_us(callback(this, &ExperimentProgram::timeoutERP2), period);
                 break;
             case 0:
                 this->m_usedPeripherals->timeout1->attach_us(callback(this, &ExperimentProgram::timeoutERP1), period);
                 break;
        }

        // Čítač offsetu v aktuálním accumulatoru zvětším
        this->m_experimentConfig.experimentERP.data.sequence_data.accOffset++;
        // Pokud offset ukazuje "mimo pole"
        if (this->m_experimentConfig.experimentERP.data.sequence_data.accOffset == 8) {
            // Vynuluji offset
            this->m_experimentConfig.experimentERP.data.sequence_data.accOffset = 0;
            // Inkrementuji index na celý accumulator
            this->m_experimentConfig.experimentERP.data.sequence_data.accIndex++;
            // Pokud index na accumulator ukazuje "mimo pole"
            if (this->m_experimentConfig.experimentERP.data.sequence_data.accIndex == 8) {
                // Vynuluji index na accumulator
                this->m_experimentConfig.experimentERP.data.sequence_data.accIndex = 0;
            }
            // Odešlu požadavek na získání další části ERP sekvence
            this->sendSequencePartRequest(this->m_experimentConfig.experimentERP.data.sequence_data.requestOffset, this->m_experimentConfig.experimentERP.data.sequence_data.requestIndex);
        }
    }
/*---------------- Timeouty ------------------*/

    void timeoutERP1() {this->timeoutERP(0);}
    void timeoutERP2() {this->timeoutERP(1);}
    void timeoutERP3() {this->timeoutERP(2);}
    void timeoutERP4() {this->timeoutERP(3);}
    void timeoutERP5() {this->timeoutERP(4);}
    void timeoutERP6() {this->timeoutERP(5);}
    void timeoutERP7() {this->timeoutERP(6);}
    void timeoutERP8() {this->timeoutERP(7);}
    void timeoutERP(uint8_t index) {
        // const experiment_erp_edge_t edge = this->m_experimentConfig.experimentERP.head.edge;
        // const experiment_erp_random_t randomType = this->m_experimentConfig.experimentERP.head.random;
        //
        // // Pokud je zapnutá náhodnost na náběžné hraně
        // if (randomType != OFF && edge == FALLING) {
        //     us_timestamp_t randomWait = 0;
        //     switch (randomType) {
        //         case SHORT:
        //             randomWait = this->m_experimentConfig.experimentERP.data.randomBase * (((rand() % 21) + 80) * 1000);
        //             break;
        //         case LONG:
        //             randomWait = this->m_experimentConfig.experimentERP.data.randomBase * (((rand() % 21) + 100) * 1000);
        //             break;
        //         case SHORT_LONG:
        //             randomWait = this->m_experimentConfig.experimentERP.data.randomBase * (((rand() % 41) + 80) * 1000);
        //             break;
        //         case OFF:
        //             break;
        //     }
        //     wait_us(randomWait);
        // }
        uint8_t output = this->m_experimentConfig.experimentERP.data.currentOutput;
        const us_timestamp_t period = this->m_experimentConfig.experimentERP.outputs[output].pulseDown;

        this->turnOffOutput(index);
        this->ioChange(COMMAND_OUTPUT_DEACTIVATED, index);
        memset(this->m_inputButtonDelays, 0, sizeof(uint8_t) * TOTAL_OUTPUT_COUNT);
        this->m_lastPressedButton = -1;

        // Získám celkovou velikost sekvence
        const experiment_erp_sequence_size_t sequenceSize = this->m_experimentConfig.experimentERP.head.sequenceSize;
        // Pokud jsem tuto délku nepřekoval
        if (this->m_experimentConfig.experimentERP.data.sequence_data.pointer < sequenceSize) {
            // Inkrementuji interní counter
            this->m_experimentConfig.experimentERP.data.sequence_data.pointer++;
        } else { // Dosáhl jsem délky sekvence
            // Ukončím experiment
            this->sendExperimentFinishedCommand();
            // Víc už dělat nebudu
            return;
        }

        this->m_usedPeripherals->timeout1->attach_us(callback(this, &ExperimentProgram::tickerERP), period);
    }

/*--------------- CVEP funkce ----------------*/
/*---------------- Časovače ------------------*/
    void tickerCVEP() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        const experiment_output_type_t outputType = this->m_experimentConfig.experimentCVEP.head.outputType;
        const experiment_output_brightness_t brightness = this->m_experimentConfig.experimentCVEP.head.brightness;
        const experiment_cvep_bit_shift_t bitShift = this->m_experimentConfig.experimentCVEP.head.bitShift;
        const experiment_cvep_pattern_t pattern = this->m_experimentConfig.experimentCVEP.head.pattern;
        const experiment_output_count_t outputCount = this->m_experimentConfig.experimentCVEP.head.outputCount;
        for (experiment_output_count_t i = 0; i < outputCount; i++) {
            if ((pattern >> ((i * bitShift) + this->m_experimentConfig.experimentCVEP.data.counter) % 32) & 1) {
                this->setOutput(i, brightness, outputType);
                this->ioChange(COMMAND_OUTPUT_ACTIVATED, i);
            }
        }
        this->m_experimentConfig.experimentCVEP.data.counter = (this->m_experimentConfig.experimentCVEP.data.counter + 1) % 32;

        const us_timestamp_t period = this->m_experimentConfig.experimentCVEP.head.out;
        this->m_usedPeripherals->timeout1->attach_us(callback(this, &ExperimentProgram::timeoutCVEP), period);
    }
/*---------------- Timeouty ------------------*/

    void timeoutCVEP() {
        const experiment_output_count_t outputCount = this->m_experimentConfig.experimentCVEP.head.outputCount;
        for (experiment_output_count_t i = 0; i < outputCount; i++) {
           this->turnOffOutput(i);
           this->ioChange(COMMAND_OUTPUT_DEACTIVATED, i);
        }
        memset(this->m_inputButtonDelays, 0, sizeof(uint8_t) * TOTAL_OUTPUT_COUNT);
        this->m_lastPressedButton = -1;
    }
/*--------------- FVEP funkce ----------------*/
/*---------------- Časovače ------------------*/

    void tickerFVEP1() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(0);
        const us_timestamp_t period = this->m_experimentConfig.experimentFVEP.outputs[0].timeOn;
        this->m_usedPeripherals->timeout1->attach_us(callback(this, &ExperimentProgram::timeoutFVEP1), period);
    }
    void tickerFVEP2() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(1);
        const us_timestamp_t period = this->m_experimentConfig.experimentFVEP.outputs[1].timeOn;
        this->m_usedPeripherals->timeout2->attach_us(callback(this, &ExperimentProgram::timeoutFVEP2), period);
    }
    void tickerFVEP3() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(2);
        const us_timestamp_t period = this->m_experimentConfig.experimentFVEP.outputs[2].timeOn;
        this->m_usedPeripherals->timeout3->attach_us(callback(this, &ExperimentProgram::timeoutFVEP3), period);
    }
    void tickerFVEP4() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(3);
        const us_timestamp_t period = this->m_experimentConfig.experimentFVEP.outputs[3].timeOn;
        this->m_usedPeripherals->timeout4->attach_us(callback(this, &ExperimentProgram::timeoutFVEP4), period);
    }
    void tickerFVEP5() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(4);
        const us_timestamp_t period = this->m_experimentConfig.experimentFVEP.outputs[4].timeOn;
        this->m_usedPeripherals->timeout5->attach_us(callback(this, &ExperimentProgram::timeoutFVEP5), period);
    }
    void tickerFVEP6() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(5);
        const us_timestamp_t period = this->m_experimentConfig.experimentFVEP.outputs[5].timeOn;
        this->m_usedPeripherals->timeout6->attach_us(callback(this, &ExperimentProgram::timeoutFVEP6), period);
    }
    void tickerFVEP7() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(6);
        const us_timestamp_t period = this->m_experimentConfig.experimentFVEP.outputs[6].timeOn;
        this->m_usedPeripherals->timeout7->attach_us(callback(this, &ExperimentProgram::timeoutFVEP7), period);
    }
    void tickerFVEP8() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        this->tickerFVEP(7);
        const us_timestamp_t period = this->m_experimentConfig.experimentFVEP.outputs[7].timeOn;
        this->m_usedPeripherals->timeout8->attach_us(callback(this, &ExperimentProgram::timeoutFVEP8), period);
    }
    void tickerFVEP(uint8_t index) {
        const experiment_output_brightness_t brightness = this->m_experimentConfig.experimentFVEP.outputs[index].brightness;
        const experiment_output_type_t outputType = this->m_experimentConfig.experimentFVEP.outputs[index].outputType;
        this->setOutput(index, brightness, outputType);
        this->ioChange(COMMAND_OUTPUT_ACTIVATED, index);
        memset(this->m_inputButtonDelays, 0, sizeof(uint8_t) * TOTAL_OUTPUT_COUNT);
        this->m_lastPressedButton = -1;
    }
/*---------------- Timeouty ------------------*/

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

/*--------------- TVEP funkce ----------------*/
/*---------------- Časovače ------------------*/

    void tickerTVEP1() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        bool outputActivated = this->tickerTVEP(0);
        if (outputActivated) {
            const us_timestamp_t period = this->m_experimentConfig.experimentTVEP.outputs[0].out;
            this->m_usedPeripherals->timeout1->attach_us(callback(this, &ExperimentProgram::timeoutTVEP1), period);
        }
    }
    void tickerTVEP2() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        bool outputActivated = this->tickerTVEP(1);
        if (outputActivated) {
            const us_timestamp_t period = this->m_experimentConfig.experimentTVEP.outputs[1].out;
            this->m_usedPeripherals->timeout2->attach_us(callback(this, &ExperimentProgram::timeoutTVEP2), period);
        }
    }
    void tickerTVEP3() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        bool outputActivated = this->tickerTVEP(2);
        if (outputActivated) {
            const us_timestamp_t period = this->m_experimentConfig.experimentTVEP.outputs[2].out;
            this->m_usedPeripherals->timeout3->attach_us(callback(this, &ExperimentProgram::timeoutTVEP3), period);
        }
    }
    void tickerTVEP4() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        bool outputActivated = this->tickerTVEP(3);
        if (outputActivated) {
            const us_timestamp_t period = this->m_experimentConfig.experimentTVEP.outputs[3].out;
            this->m_usedPeripherals->timeout4->attach_us(callback(this, &ExperimentProgram::timeoutTVEP4), period);
        }
    }
    void tickerTVEP5() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        bool outputActivated = this->tickerTVEP(4);
        if (outputActivated) {
            const us_timestamp_t period = this->m_experimentConfig.experimentTVEP.outputs[4].out;
            this->m_usedPeripherals->timeout5->attach_us(callback(this, &ExperimentProgram::timeoutTVEP5), period);
        }
    }
    void tickerTVEP6() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        bool outputActivated = this->tickerTVEP(5);
        if (outputActivated) {
            const us_timestamp_t period = this->m_experimentConfig.experimentTVEP.outputs[5].out;
            this->m_usedPeripherals->timeout6->attach_us(callback(this, &ExperimentProgram::timeoutTVEP6), period);
        }
    }
    void tickerTVEP7() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        bool outputActivated = this->tickerTVEP(6);
        if (outputActivated) {
            const us_timestamp_t period = this->m_experimentConfig.experimentTVEP.outputs[6].out;
            this->m_usedPeripherals->timeout7->attach_us(callback(this, &ExperimentProgram::timeoutTVEP7), period);
        }
    }
    void tickerTVEP8() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        bool outputActivated = this->tickerTVEP(7);
        if (outputActivated) {
            const us_timestamp_t period = this->m_experimentConfig.experimentTVEP.outputs[7].out;
            this->m_usedPeripherals->timeout8->attach_us(callback(this, &ExperimentProgram::timeoutTVEP8), period);
        }
    }
    bool tickerTVEP(uint8_t index) {
        const experiment_output_brightness_t brightness = this->m_experimentConfig.experimentTVEP.outputs[index].brightness;
        const experiment_tvep_output_pattern_length_t patternLength = this->m_experimentConfig.experimentTVEP.outputs[index].patternLength;
        const experiment_tvep_output_pattern_t pattern = this->m_experimentConfig.experimentTVEP.outputs[index].pattern;
        const experiment_output_type_t outputType = this->m_experimentConfig.experimentTVEP.outputs[index].outputType;
        bool outputActivated = false;
        if ((pattern >> (this->m_experimentConfig.experimentTVEP.data.counters[index] % patternLength)) & 1) {
            this->setOutput(index, brightness, outputType);
            this->ioChange(COMMAND_OUTPUT_ACTIVATED, index);
            outputActivated = true;
        }
        this->m_experimentConfig.experimentTVEP.data.counters[index] = (this->m_experimentConfig.experimentTVEP.data.counters[index] + 1) % patternLength;
        memset(this->m_inputButtonDelays, 0, sizeof(uint8_t) * TOTAL_OUTPUT_COUNT);
        this->m_lastPressedButton = -1;
        return outputActivated;
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

    void tickerREA() {
        // Pokud experiment není spuštěný, nebudu nic dělat
        if (!this->isRunning()) {
            return;
        }

        // Ověřím, že jsem ještě nepřekročil počet cyklů
        if (this->m_experimentConfig.experimentREA.data.counter < this->m_experimentConfig.experimentREA.head.cycleCount) {
            this->m_experimentConfig.experimentREA.data.counter++;
        } else {
            // Ukončím experiment
            this->sendExperimentFinishedCommand();
            return;
        }

        const uint8_t output = (rand() % this->m_experimentConfig.experimentREA.head.outputCount) % TOTAL_OUTPUT_COUNT;
        const experiment_output_brightness_t brightness = this->m_experimentConfig.experimentREA.head.brightness;
        const experiment_output_type_t outputType = this->m_experimentConfig.experimentREA.head.outputType;
        const us_timestamp_t delta = this->m_experimentConfig.experimentREA.head.waitTimeMax - this->m_experimentConfig.experimentREA.head.waitTimeMin;
        const us_timestamp_t period = this->m_experimentConfig.experimentREA.head.waitTimeMin + (rand() % delta);
        this->m_experimentConfig.experimentREA.data.usedOutput = output;

        this->setOutput(output, brightness, outputType);
        this->ioChange(COMMAND_OUTPUT_ACTIVATED, output);

        this->m_usedPeripherals->timeout1->attach_us(callback(this, &ExperimentProgram::timeoutREA), period);
    }

/*---------------- Timeouty ------------------*/

    void timeoutREA() {
        const uint8_t output = this->m_experimentConfig.experimentREA.data.usedOutput;
        // TODO zjistit, co je myšleno počkat na timeout při chybné reakci
        // const experiment_rea_on_fail_t onFail = this->m_experimentConfig.experimentREA.head.onFail;
        // // Pokud jsem stiskl tlačítko, které ale neodpovídá výstupu
        // if (this->m_lastPressedButton != output) {
        //     // Pokud má při chybné reakci počkat na timeout
        //     if (onFail != 0) {
        //         this->sendExperimentFinishedCommand();
        //         return;
        //     }
        // }

        this->turnOffOutput(output);
        this->ioChange(COMMAND_OUTPUT_DEACTIVATED, output);

        memset(this->m_inputButtonDelays, 0, sizeof(uint8_t) * TOTAL_OUTPUT_COUNT);
        this->m_lastPressedButton = -1;
    }




public:

    ExperimentProgram(): m_state(READY), m_usedPeripherals(NULL) {
        memset(&this->m_experimentConfig, 0, sizeof(ExperimentConfig));
    }

    /**
     * Uloží konfiguraci experimentu do paměti
     */
    void save(ExperimentConfig &experimentConfig) {
        this->clear();
        this->m_experimentConfig = experimentConfig;
        this->m_state = UPLOADED;
    }

    /**
     * Inicializuje uloženou konfiguraci v paměti
     */
    void init() {
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

    void initInterrupts() {
        this->setupInputInterrupts();
    }

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
        for (uint8_t i = 0; i < TOTAL_OUTPUT_COUNT; i++) {
            // Timto se detachnou veškeré timery, takže přestanou
            // generovat přerušení
            this->m_usedPeripherals->tickers[i]->detach();
            // Tímto nastavím veškeré výstupy na nulu
            *this->m_usedPeripherals->outputs[i] = 0;
        }
        memset(&this->m_experimentConfig, 0, sizeof(ExperimentConfig));
        memset(this->m_inputButtonDelays, 0, sizeof(uint8_t) * TOTAL_OUTPUT_COUNT);
        this->m_state = CLEARED;
    }

    void setOutput(uint8_t index, experiment_output_brightness_t brightness) {
        this->setOutput(index, brightness, 1);
    }

    void setPeripherals(used_peripherals_t *peripherals) {
        this->m_usedPeripherals = peripherals;
    }

    void setServerCommandQueue(CircularBuffer<ServerCommandData, 16> *serverCommandQueue) {
        this->m_serverCommandQueue = serverCommandQueue;
    }

    void copyExperimentConfig(void *config) {
        memcpy(config, &this->m_experimentConfig, sizeof(ExperimentConfig));
    }

    void copyAccumulators(void *accumulators) {
        memcpy(accumulators, this->m_experimentConfig.experimentERP.data.accumulators, sizeof(uint32_t) * TOTAL_OUTPUT_COUNT);
    }

    void copyCounters(void *counters) {
        // memcpy(counters, this->m_experimentConfig.experimentERP.data.counters, sizeof(uint16_t) * TOTAL_OUTPUT_COUNT);
    }

    uint8_t getState() {
        return this->m_state;
    }

    LCDDisplay *getLCD() {
        return this->m_usedPeripherals->display;
    }

    void updateAccumulator(uint8_t index, uint32_t data) {
        this->m_experimentConfig.experimentERP.data.accumulators[index] = data;
        this->m_experimentConfig.experimentERP.data.sequence_data.requestIndex++;
        if (this->m_experimentConfig.experimentERP.data.sequence_data.requestIndex == 8) {
            this->m_experimentConfig.experimentERP.data.sequence_data.requestIndex = 0;
        }
        this->m_experimentConfig.experimentERP.data.sequence_data.requestOffset += 8;
    }
};

#endif