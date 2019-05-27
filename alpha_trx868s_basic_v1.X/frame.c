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
 * Rev      Date        Author      Comment
 * ----------------------------------------------------------------------------
 * v01r00   28/09/2013  AB          [EN] Initial release
 *    r01   30/09/2013  AB          Remplacement de la fonction Frame_Clear() par memset(Frame_RF_Buffer1.v, 0, FRAME_LENGTH);
 *
 * _____________________________________________________________________________
 */

// TODO: > Faire un calcul de validation des données transmise par CRC16


/** I N C L U D E S ***********************************************************/
#include <stdio.h>
#include <string.h>
//#include "HardwareProfile.h"
//#include "GenericTypeDefs.h"
#include "frame.h"


/** V A R I A B L E S *********************************************************/
// Global Variables
volatile FRAME_VAL * p_Frame_RF_Read;
volatile FRAME_VAL * p_Frame_RF_Write;
volatile FRAME_VAL Frame_RF_Buffer1;
volatile FRAME_VAL Frame_RF_Buffer2;
volatile FRAME_VAL Frame_RF_Send;
char Frame_String[FRAME_STRING_LENGHT];
char Frame_String_ALARM[FRAME_STRING_LENGHT];
char Frame_String_STOP[FRAME_STRING_LENGHT];


/** D E C L A R A T I O N S ***************************************************/
void Frame_Init(void)
{
    /* Effacer du contenu des trames d'émission/réception */
//    Frame_Clear(Frame_RF_Buffer1.v);
//    Frame_Clear(Frame_RF_Buffer2.v);
    memset(Frame_RF_Send.v,    0, FRAME_LENGTH);
    memset(Frame_RF_Buffer1.v, 0, FRAME_LENGTH);
    memset(Frame_RF_Buffer2.v, 0, FRAME_LENGTH);

    
    // Initialiser les paramètes de la trame à envoyer :
    Frame_RF_Send.field.Start      = START_CHAR;
    Frame_RF_Send.field.ID_Carte   = 0;//ID_CARTE_NUM;
    Frame_RF_Send.field.ID_Message = 0;
    Frame_RF_Send.field.End        = END_CHAR;

} // End of Frame_Init()


/**
 * @brief Ecrire une trame dans une chaine de cartactère
 * la chaine de cartactère Frame_String peut ensuite être utilisé, pour afficher la trame
 * @param p_Frame
 */
void Frame_To_String(FRAME_VAL * p_Frame)
{
    /* Ecrire une trame dans une chaine de cartactère */
    p_Frame->byte.Data_Message_CharNUL = NULL; // force Data_Message_CharNUL to 0
    sprintf(Frame_String, "%c0x%04X:0x%04X:%s:0x%04X:%u%c", p_Frame->field.Start, p_Frame->field.ID_Carte, p_Frame->field.ID_Message, p_Frame->field.Data_Message, p_Frame->field.Checksum, p_Frame->field.Ack, p_Frame->field.End);

} // End of Frame_To_String()


/** This fonction will calculate the FCS (Frame check sum) for the given FRAME_VAL string */
// Calcul du Checksum de la trame sur 16 bits
uint16_t FrameChecksumCalculator(FRAME_VAL * p_Frame)
{
    uint16_t ChkSum = 0;
    uint8_t i;

// Exemple de calcul de Checksum:
//    p_Frame->field.ID_Carte     = 0x0001
//    p_Frame->field.ID_Message   = 0x0001
//    p_Frame->field.Data_Message = 'S'+'Y'+'N'+'C'+'H'
//    1+1+83+89+78+67+72= 391 --> 0x0187
// !  0x0187 & 0x00FF = 0x87
    for (i=1; i < 10; i++)
    {
        ChkSum += p_Frame->v[i];
    }

    return (ChkSum);

} // End of Frame_Checksum()
