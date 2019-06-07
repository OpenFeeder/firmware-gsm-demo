#include <stdio.h>
#include "./temps.h"

inline uint8_t conv_dec(uint8_t g, uint8_t d) {
    return (uint8_t) ((g - 48) * 10 + (d - 48));
}


void conv_date_readed(uint8_t* buff, struct heure_format* res) {
    res->j = conv_dec(buff[0], buff[1]);
    res->ms = conv_dec(buff[2], buff[3]);
    res->a = conv_dec(buff[4], buff[5]);
    res->h = conv_dec(buff[6], buff[7]);
    res->m = conv_dec(buff[8], buff[9]);
    res->s = conv_dec(buff[10], buff[11]);
    res->wd = buff[12];
}


//7 bits pour l'horloge
void serial_buffer(uint8_t* buff, struct heure_format hf) {
    buff[0] = hf.j;
    buff[1] = hf.ms;
    buff[2] = hf.a;
    buff[3] = hf.h;
    buff[4] = hf.m;
    buff[5] = hf.s;
    buff[6] = hf.wd;
}

//7 bits pour l'horloge
void deserial_buffer(uint8_t* buff, struct heure_format* hf) {
    hf->j = buff[0];
    hf->ms = buff[1];
    hf->a = buff[2];
    hf->h = buff[3];
    hf->m = buff[4];
    hf->s = buff[5];
    hf->wd = buff[6];
}

void read_clock_uart(struct heure_format* hf) {
    int i = 0;
    uint8_t data_uart[13];
    printf("Lecture de la date... \n\r");
    while (i < 13) {
        data_uart[i] = UART1_Read();
        i++;
    }
    printf("%s\n", data_uart);
    conv_date_readed(data_uart,hf);
}

void set_time(struct heure_format hf) {
    struct tm time_set;
    time_set.tm_sec = (uint8_t) hf.s;
    time_set.tm_min = (uint8_t) hf.m;
    time_set.tm_hour = (uint8_t) hf.h;
    time_set.tm_mday = (uint8_t) hf.j;
    time_set.tm_mon = (uint8_t) hf.ms;
    time_set.tm_year = (uint16_t) hf.a;
    time_set.tm_wday = (uint8_t) hf.wd;
    RTCC_TimeSet(&time_set);
}

void get_time(struct heure_format* hf) {
    struct tm time_pic;
    RTCC_TimeGet(&time_pic);
    hf->s = time_pic.tm_sec;
    hf->m = time_pic.tm_min;
    hf->h = time_pic.tm_hour;
    hf->j = time_pic.tm_mday;
    hf->ms = time_pic.tm_mon;
    hf->a = time_pic.tm_year;
    hf->wd = time_pic.tm_wday;
}
/*
void datetime_buf(uint8_t* buff) {

    uint8_t s, m, h, j, ms, wd;
    uint16_t a;
    registre ann;
    ann.reg = a;
    get_time(&s, &m, &h, &j, &ms, &a, &wd);
    buff[0] = j;
    buff[1] = ms;
    buff[2] = ann.octets.g;
    buff[3] = ann.octets.d;
    buff[4] = h;
    buff[5] = m;
    buff[6] = s;

}
 */