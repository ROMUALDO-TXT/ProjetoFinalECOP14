#include "config.h"
#include "ssd.h"
#include "timer.h"
#include "keypad.h"
#include "config.h"
#include "lcd.h"
#include "adc.h"
#include "pwm.h"
#include <pic18f4520.h>

#define L_ON  0x0F
#define L_OFF 0x08
#define L_CLR 0x01
#define L_L1  0x80
#define L_L2  0xC0


double fm, am, fmStored;
char disp = 1, soneca = '0';
char alarmeAtivo = '0', radioAtivo = '1', freq = 'f';
unsigned long hora = 320000, horaAlarme;
int AD = 0;

//FM = 88 - 108MHZ
//AM = 560 - 1630KHz
char alarmeTocando = '0';
unsigned int tecla = 0;

void radioConfig() {
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("Radio");
    if (radioAtivo == '0') {
        lcdCommand(L_L2);
        lcdString("Radio: desligado");
    } else {
        lcdCommand(L_L2);

        lcdString("Radio: ligado");
    }
    for (int i = 0; i < 100; i++) {
        timerReset(5000);

        ssdUpdate();

        if (kpRead() != tecla) {
            tecla = kpRead();
            switch (kpReadKey()) {
                case 'S':
                    if (radioAtivo == '0') {
                        radioAtivo = '1';
                    } else {
                        radioAtivo = '0';
                    }
                    i = 1000;
                    break;
                case 's':

                    break;
            }
        }
        timerWait();

    }
}

void radioFreq() {
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("Radio");
    if (freq == 'f') {
        lcdCommand(L_L2);
        lcdString("Frequencia: FM");
    } else {
        lcdCommand(L_L2);

        lcdString("Frequencia: AM");
    }
    for (int i = 0; i < 100; i++) {
        timerReset(5000);

        ssdUpdate();

        if (kpRead() != tecla) {
            tecla = kpRead();
            switch (kpReadKey()) {
                case 'S':
                    if (freq == 'f') {
                        freq = 'a';
                    } else {
                        freq = 'f';
                    }
                    i = 1000;
                    break;
                case 's':
                    break;
            }
        }
        timerWait();

    }
}

void alarmeHora(int pos) {
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("hora do alarme");

    for (int i = 0; i < 100; i++) {
        timerReset(5000);

        ssdUpdate();

        if (kpRead() != tecla) {
            tecla = kpRead();
            switch (kpReadKey()) {
                case 'X':
                    if (pos == 0) {
                        horaAlarme = horaAlarme + 360000;
                    } else {
                        horaAlarme = horaAlarme + 6000;
                    }
                    i = 1000;
                    break;
                case 'B':
                    if (pos == 0) {
                        horaAlarme = horaAlarme - 360000;
                    } else {
                        horaAlarme = horaAlarme - 6000;
                    }
                    i = 1000;
                    break;
                case 'Y':
                    if (pos == 1) {
                        pos = 0;
                    } else {
                        pos = 1;
                    }
                    break;
                case 'A':
                    if (pos == 1) {
                        pos = 0;
                    } else {
                        pos = 1;
                    }
                    break;
                case 's':

                    break;
            }
        }
        timerWait();
    }
}

void horaConfig(int pos) {
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("hora do alarme");

    for (int i = 0; i < 100; i++) {
        timerReset(5000);

        ssdUpdate();

        if (kpRead() != tecla) {
            tecla = kpRead();
            switch (kpReadKey()) {
                case 'X':
                    if (pos == 0) {
                        hora = hora + 360000;
                    } else {
                        hora = hora + 6000;
                    }
                    i = 1000;
                    break;
                case 'B':
                    if (pos == 0) {
                        hora = hora - 360000;
                    } else {
                        hora = hora - 6000;
                    }
                    i = 1000;
                    break;
                case 'Y':
                    if (pos == 1) {
                        pos = 0;
                    } else {
                        pos = 1;
                    }
                    break;
                case 'A':
                    if (pos == 1) {
                        pos = 0;
                    } else {
                        pos = 1;
                    }
                    break;
                case 's':
                    break;
            }
        }
        timerWait();
    }
}

void alarmeConfig() {
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("Alarme");
    if (alarmeAtivo == '0') {
        lcdCommand(L_L2);
        lcdString("Alarme: desligado");
    } else {
        lcdCommand(L_L2);
        lcdString("Alarme: ligado");
    }
    for (int i = 0; i < 5000; i++) {
        ssdUpdate();

        if (kpRead() != tecla) {
            tecla = kpRead();
            switch (kpReadKey()) {
                case 'S':
                    if (alarmeAtivo == '0') {
                        alarmeAtivo = '1';
                    } else {
                        alarmeAtivo = '0';
                    }
                    i = 1000;
                    break;
                case 's':
                    break;
            }
        }
    }
}

void selectFunction(int op) {
    switch (op) {
        case 0:
            alarmeConfig();
            break;
        case 1:
            alarmeHora(0);
            break;
        case 2:
            radioConfig();
            break;
        case 3:
            radioFreq();
            break;
        case 4:
            horaConfig(0);
            break;
    }
}

void menuText(int op) {
    switch (op) {
        case 0:
            lcdString("Ativar alarme  ");
            break;
        case 1:
            lcdString("Horario alarme ");
            break;
        case 2:
            lcdString("Ativar radio   ");
            break;
        case 3:
            lcdString("Freq. radio    ");
            break;
        case 4:
            lcdString("Mudar hora     ");
            break;
    }
}

void menu(void) {
    int op = 0;
    lcdCommand(L_L1);
    lcdString("Menu            ");
    for (int i = 0; i < 1000; i++) {
        timerReset(50000);

        ssdUpdate();

        if (kpRead() != tecla) {
            switch (kpReadKey()) {
                case 'X':
                    if (op == 0) {
                        op = 4;
                    } else {
                        op--;
                    }
                    lcdCommand(L_L2);
                    i = 0;
                    switch (op) {
                        case 0:
                            lcdString("Ativar alarme  ");
                            break;
                        case 1:
                            lcdString("Horario alarme ");
                            break;
                        case 2:
                            lcdString("Ativar radio   ");
                            break;
                        case 3:
                            lcdString("Freq. radio    ");
                            break;
                        case 4:
                            lcdString("Mudar hora     ");
                            break;
                    }

                case 'B':
                    if (op == 4) {
                        op = 0;
                    } else {
                        op++;
                    }
                    lcdCommand(L_L2);
                    i = 0;
                    switch (op) {
                        case 0:
                            lcdString("Ativar alarme  ");
                            break;
                        case 1:
                            lcdString("Horario alarme ");
                            break;
                        case 2:
                            lcdString("Ativar radio   ");
                            break;
                        case 3:
                            lcdString("Freq. radio    ");
                            break;
                        case 4:
                            lcdString("Mudar hora     ");
                            break;
                    }
                    break;
                case 'S':
                    selectFunction(op);
                    i = 0;
                    break;
                case 's':

                    break;

            }
        }
        timerWait();
    }
}

void menuAccess(void) {
    if (kpRead() != tecla) {
        tecla = kpRead();

        switch (kpReadKey()) {
            case 'S':
                menu();
                break;
            case 's':
                if (alarmeTocando == '1') {
                    alarmeTocando = '0';
                }
                break;
        }
    }
}

void compararAlarme() {
    if ((hora / 6000) == (horaAlarme / 6000)) {
        if (soneca == '0') {
            lcdCommand(L_CLR);
            pwmFrequency(600);
            for (int i = 0; i > 5000; i++) {
                timerReset(5000);
                ssdUpdate();
                lcdCommand(L_L1);
                lcdString("Alarme tocando");
                lcdCommand(L_L2);
                lcdString("pressione s ");
                if (kpRead() != tecla) {
                    tecla = kpRead();

                    switch (kpReadKey()) {
                        case 's':
                            pwmFrequency(0);
                            soneca = '1';
                            break;
                    }
                }
                timerWait();
            }
        }
    } else {
        soneca = '0';
    }
}

void radioADC() {
    AD = adcRead(0);
    fm = (AD * 0.01953125) + 88;
    am = (AD * 0.044921875) + 530;
}

void MostraDisplay(void) {
    lcdCommand(L_CLR);
    if (fmStored == fm) {
    } else {
        if (freq == 'f') {
            lcdCommand(L_L1);
            lcdString("Radio FM");
            lcdCommand(L_L2);
            lcdNumber(fm);
        } else {
            lcdCommand(L_L1);
            lcdString("Radio AM");
            lcdCommand(L_L2);
            lcdNumber(am);
        }
        fmStored = fm;
    }

    ssdDigit(((((hora / 360000) % 24) / 10) + 48), 1);
    ssdDigit(((((hora / 360000) % 24) % 10) + 48), 2);
    ssdDigit(((hora / 60000) % 6 + 48), 3);
    ssdDigit(((hora / 6000) % 10 + 48), 4);
}

void main(void) {
    char slot;

    pwmInit();
    ssdInit();
    adcInit();
    timerInit();
    kpInit();
    lcdInit();

    ssdDigit(0x00, 0);
    ssdDigit(0x00, 1);
    ssdDigit(0x00, 2);
    ssdDigit(0x00, 3);

    lcdCommand(L_CLR);

    for (;;) {
        timerReset(5000);
        ssdUpdate();

        switch (slot) {
            case 0:
                menuAccess();
                slot = 1;
                break;
            case 1:
                compararAlarme();
                slot = 2;
                break;
            case 2:
                kpDebounce();
                slot = 3;
                break;
            case 3:

                MostraDisplay();

                slot = 0;
                break;
            default:
                slot = 0;
                break;
        }
        hora++;
        timerWait();
    }
}

