/* 
 * File:   types.h
 * Author: alex
 *
 * Created on February 12, 2019, 4:01 PM
 */

#ifndef TYPES_H
#define	TYPES_H


struct outputs_vals {
    uint8_t val_cs;
    uint8_t val_sck;
    uint8_t val_mosi;
    uint8_t val_nres;

};

struct inputs_vals {
    uint8_t val_nirq;
    uint8_t val_miso;

};

typedef union {
    uint16_t reg;

    struct {
        uint8_t d;
        uint8_t g;

    } octets;
} registre;

typedef union {
    uint16_t sr;

    struct {
        uint8_t RGIT_FFIT : 1;
        uint8_t POR : 1;
        uint8_t RGUR_FFOV : 1;
        uint8_t WKUP : 1;
        uint8_t EXT : 1;
        uint8_t LBD : 1;
        uint8_t FFEM : 1;
        uint8_t ATS_RSSI : 1;
        uint8_t DQD : 1;
        uint8_t CRL : 1;
        uint8_t ATGL : 1;
        uint8_t OFFS_6 : 1;
        uint8_t OFFS : 4;
    } bits;

} registre_etat;

struct heure_format {

        uint8_t j;
        uint8_t ms;
        uint8_t a;
        uint8_t h;
        uint8_t m;
        uint8_t s;
        uint8_t wd;
};


#endif	/* TYPES_H */

