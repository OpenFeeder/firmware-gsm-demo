/* _____________________________________________________________________________
 *
 *                        S T R U C T U R E    T R A M E
 * _____________________________________________________________________________
 *
 * Titre            : Structuration d'une trame de donnée
 * Version          : v01r00
 * Date de création : 28/09/2013
 * Auteur           : Arnauld BIGANZOLI (AB)
 * Contact          : arnauld.biganzoli@gmail.com
 * Web page         : http://www.u825.inserm.fr/25949988/0/fiche___pagelibre
 * Collaborateur    : ...
 * Tools used       : MPLAB® X IDE v1.90
 * Compiler         : Microchip XC8, X16, XC32
 *
 *
 * Structure d'une trame d'un message RF:
 *      +-----+--------+----------+------------+--------+---+---+
 *      |Start|ID_Carte|ID_Message|Data_Message|Checksum|Ack|End|
 *      +-----+--------+----------+------------+--------+---+---+
 * BYTE =  1  +    2   +     2    +      6     +    2   + 1 + 1
 * Nombre d'octet total pour une trame = 1+2+2+6+2+1+1= 15
 * Ex Frame_To_String: ">0x0001:0x0001:SYNCH:0x0187:0<"
 *
 * Marqueurs ou identifieurs:
 *  . Start: '>' (0x3E), symbole indiquant le début d'une trame
 *  . End  : '<' (0x3C), symbole indiquant la fin de la trame
 * ID_Carte / ID_Message:
 *  . valeur min: 0x0001 (Hexa)
 *  . valeur max: 0xFFFF (Hexa)
 * Data_Message:
 *  . champ contenant les données à transmettre
 *    ex: "ALARM", "SYNCH"
 * Checksum:
 *  . champ de contrôle de la cohérence de ID_Carte+ID_Message+Data_Message
 *    --> détection d'erreur par checksum
 * Ack :
 *  . champ accusé de réception
 *
 */

#ifndef FRAME_H
#define	FRAME_H

#include <stdint.h>

#define FRAME_STRING_LENGHT 31
#define FRAME_LENGTH        15    // Longueur total d'une trame en octet
#define START_CHAR          '>'   // 0x3E: symbole indiquant le début d'une trame, Start Of Frame (SOF)
#define END_CHAR            '<'   // 0x3C: symbole indiquant la fin de la trame, End Of Frame (EOF)
#define String_ALARM        "ALARM"
#define String_SYNCH        "SYNCH"
#define String_STOP         "STOP"

/* PUBLIC DATA TYPES */
// union/structure for read/write of frame data
typedef union _FRAME_VAL_T
{
    char v[FRAME_LENGTH];          // BYTE access
    struct
    {
        uint8_t Start;                // '>' (0x3E), symbole indiquant le début d'une trame
        uint8_t ID_Carte_LB;
        uint8_t ID_Carte_HB;
        uint8_t ID_Message_LB;
        uint8_t ID_Message_HB;
        uint8_t Data_Message_Char1;
        uint8_t Data_Message_Char2;
        uint8_t Data_Message_Char3;
        uint8_t Data_Message_Char4;
        uint8_t Data_Message_Char5;
        uint8_t Data_Message_CharNUL;
        uint8_t Checksum_LB;          // champ de contrôle de la cohérence de Data_Message
        uint8_t Checksum_HB;
        uint8_t Ack;                  // champ accusé de réception
        uint8_t End;                  // '<' (0x3C), symbole indiquant la fin de la trame
    } byte;                         // field access
    struct
    {
        uint8_t  Start;               // '>' (0x3E), symbole indiquant le début d'une trame
        uint16_t ID_Carte;            // identifiant de la carte
        uint16_t ID_Message;          // identifiant du message
        uint8_t  Data_Message[6];     // contenu du message (au format ASCII)
        uint16_t Checksum;            // champ de contrôle de la cohérence de Data_Message
        uint8_t  Ack;                 // champ accusé de réception
        uint8_t  End;                 // '<' (0x3C), symbole indiquant la fin de la trame
        uint8_t  f1;                 // '<' (0x3C), symbole indiquant la fin de la trame
        uint8_t  f2;                 // '<' (0x3C), symbole indiquant la fin de la trame
        uint8_t  f3;                 // '<' (0x3C), symbole indiquant la fin de la trame
        uint8_t  f4;                 // '<' (0x3C), symbole indiquant la fin de la trame
        uint8_t  f5;                 // '<' (0x3C), symbole indiquant la fin de la trame
    } field;                        // field access
} FRAME_VAL;


//typedef struct READ_FRAME_VAL
//{
//    FRAME_VAL * p_Read_Frame;
//    UINT8 Status;
//};


/** E X T E R N S *************************************************************/
// Global Variables
extern volatile FRAME_VAL * p_Frame_RF_Read;
extern volatile FRAME_VAL * p_Frame_RF_Write;
extern volatile FRAME_VAL Frame_RF_Buffer1;
extern volatile FRAME_VAL Frame_RF_Buffer2;
extern volatile FRAME_VAL Frame_RF_Send;
extern char Frame_String[FRAME_STRING_LENGHT];
//extern PUBLIC volatile UINT16 CheckSum;


/** P R I V A T E   P R O T O T Y P E S ***************************************/
// Local Function Prototypes:
void Frame_Init(void);              // initialiser les paramètes de la trame à envoyer
//void Frame_Clear(FRAME_VAL * p_Frame_To_Clear); // Effacer du contenu d'une trame
void Frame_To_String(FRAME_VAL * p_Frame); // Ecrire une trame dans une chaine de cartactère
uint16_t FrameChecksumCalculator(FRAME_VAL * p_Frame);  // Calcul du Checksum de la trame


#endif	/* TRAME_H */
