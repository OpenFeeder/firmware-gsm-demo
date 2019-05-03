#include "./debug.h"

void getPinsOutput(struct outputs_vals* o_v) {
    o_v->val_cs = CS_GetValue();
    o_v->val_sck = SCK_GetValue();
    o_v->val_mosi = MOSI_GetValue();
    o_v->val_nres = nRES_GetValue();
}

void getPinsInput(struct inputs_vals* o_v) {
    o_v->val_nirq = nIRQ_GetValue();
    o_v->val_miso = MISO_GetValue();
}

void debugOutput(struct outputs_vals* o_v) {
    printf("Valeur de cs : %u\n", o_v->val_cs);
    printf("Valeur de sck : %u\n", o_v->val_sck);
    printf("Valeur de mosi : %u\n", o_v->val_mosi);
    printf("Valeur de nres : %u\n", o_v->val_nres);
}

void debugInput(struct inputs_vals* o_v) {
    printf("Valeur de nriq : %u\n", o_v->val_nirq);
    printf("Valeur de miso : %u\n", o_v->val_miso);
}

void debug_buffer(uint8_t* buffer, int size) {
    int i;
    printf("Buffer : ");

    for (i = 0; i < size; i++) {
        printf("%u ", buffer[i]);
    }
    printf("\n");
}

void debug_datetime(int* compteur) {
    int period = 20000;
    int c_temp = *compteur;
    struct heure_format hf;
    c_temp = (c_temp + 1) % period;
    if (c_temp == 0) {
        get_time(&hf);
        printf("%u/%u/%u %u:%u:%u \n", hf.j, hf.ms, hf.a, hf.h, hf.m, hf.s);
    }
    *compteur = c_temp;
}

