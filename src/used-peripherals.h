#ifndef USED_PERIPHERALS_H
#define USED_PERIPHERALS_H

typedef struct used_peripherals_s {
    // Pointer pro jednotlivé PWM výstupy
    PwmOut *out1;
    PwmOut *out2;
    PwmOut *out3;
    PwmOut *out4;
    PwmOut *out5;
    PwmOut *out6;
    PwmOut *out7;
    PwmOut *out8;

    // Pole všech výstupů pro snadnější přístup
    PwmOut* outputs[TOTAL_OUTPUT_COUNT];

    // Pointer pro jednotlivé interrupty pro tlačítka
    InterruptIn *interruptIn1;
    InterruptIn *interruptIn2;
    InterruptIn *interruptIn3;
    InterruptIn *interruptIn4;
    InterruptIn *interruptIn5;
    InterruptIn *interruptIn6;
    InterruptIn *interruptIn7;
    InterruptIn *interruptIn8;

    InterruptIn* interuptsIn[TOTAL_OUTPUT_COUNT];

    // Pointer pro jednotlivé tickery
    Ticker *ticker1;
    Ticker *ticker2;
    Ticker *ticker3;
    Ticker *ticker4;
    Ticker *ticker5;
    Ticker *ticker6;
    Ticker *ticker7;
    Ticker *ticker8;

    // Pole všech tickerů pro snadnější přístup
    Ticker* tickers[TOTAL_OUTPUT_COUNT];

    // Timeouty pro každý výstup zvlášť
    Timeout *timeout1;
    Timeout *timeout2;
    Timeout *timeout3;
    Timeout *timeout4;
    Timeout *timeout5;
    Timeout *timeout6;
    Timeout *timeout7;
    Timeout *timeout8;

    // Pole timeoutů pro snadnější přístup
    Timeout* timeouts[TOTAL_OUTPUT_COUNT];

    Timer *globalTimer;

    Serial *graphicsSerial;

    // Reference na LCD displej
    LCDDisplay *display;
} used_peripherals_t;

#endif