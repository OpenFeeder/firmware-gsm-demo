/**
 * @file app_config.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#include "app.h"
#include "app_config.h"
#include "framework/AlphaTRX/radio_alpha_trx_slave_api.h"

#define _DEBUG (1) // à effacer 

bool config_set(void) {

    INI_READ_STATE read_ini_status;
    char buf[50];

    /* Search for the CONFIG.INI file. */
    if (FILEIO_RESULT_FAILURE == config_find_ini()) {
#if defined(_DEBUG)
        printf("ERRO ==> FILEIO_RESULT_FAILURE == config_find_ini( )\n");
#endif
        strcpy(appError.message, "CONFIG.INI not found");
        appError.current_line_number = __LINE__;
        sprintf(appError.current_file_name, "%s", __FILE__);
        appError.number = ERROR_INI_FILE_NOT_FOUND;
        return false;
    }

    /* Read the CONFIG.INI file. */
    read_ini_status = config_read_ini();

    if (INI_READ_OK != read_ini_status) {
#if defined(_DEBUG)
        printf("ERRO ==> INI_READ_OK %d != %d read_ini_status\n", INI_READ_OK, read_ini_status);
#endif
        getIniPbChar(read_ini_status, buf, sizearray(buf));

        sprintf(appError.message, "Wrong parameters in CONFIG.INI: %s (%d)", buf, read_ini_status);
        appError.current_line_number = __LINE__;
        sprintf(appError.current_file_name, "%s", __FILE__);
        appError.number = ERROR_INI_FILE_READ;
        return false;
    }

    return true;
}

FILEIO_RESULT read_reward_probabilities(void) {

    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[4];
    int i;

    /* Log event if required */
    if (true == appDataLog.log_events) {
        store_event(OF_READ_REWARD_PROBABILITIES);
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf("Read reward probabilities file\n");
#endif 

    if (appDataPitTag.num_pit_tag_accepted_or_color_B > 0) {

        if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, "PROBA.TXT", FILEIO_OPEN_READ)) {
            errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
            printf("unable to open reward probabilities file (%u)", errF);
#endif 
            sprintf(appError.message, "Unable to open reward probabilities file (%u)", errF);
            appError.current_line_number = __LINE__;
            sprintf(appError.current_file_name, "%s", __FILE__);
            FILEIO_ErrorClear('A');
            appError.number = ERROR_REWARD_PROBABILITIES_FILE_OPEN;
            return FILEIO_RESULT_FAILURE;
        }

        buf[4] = '\0';

        for (i = 0; i < appDataPitTag.num_pit_tag_accepted_or_color_B; i++) {
            FILEIO_Read(buf, 1, 3, &file);
            appDataPitTag.reward_probability[i + appDataPitTag.num_pit_tag_denied_or_color_A] = atoi(buf);
        }

        if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file)) {
            errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
            printf("unable to close reward probabilities file (%u)", errF);
#endif 
            sprintf(appError.message, "Unable to close reward probabilities file (%u)", errF);
            appError.current_line_number = __LINE__;
            sprintf(appError.current_file_name, "%s", __FILE__);
            FILEIO_ErrorClear('A');
            appError.number = ERROR_REWARD_PROBABILITIES_FILE_CLOSE;
            return FILEIO_RESULT_FAILURE;
        }
    }

    return FILEIO_RESULT_SUCCESS;

}

FILEIO_RESULT read_PIT_tags(void) {

    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    uint16_t i, j, cumulative_sum;
    char buf[13];

    /* Log event if required */
    if (true == appDataLog.log_events) {
        store_event(OF_READ_PIT_TAGS);
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf("Read PIT tags files\n");
#endif 

    appDataPitTag.num_pit_tag_stored = 0;

    if (GO_NO_GO == appData.scenario_number) {
        if (LEFT_RIGHT_LEDS == appDataAttractiveLeds.pattern_number) {
            if (ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_denied_or_color_A && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_denied_or_color_A) {

                if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, "PTLEFT.TXT", FILEIO_OPEN_READ)) {
                    errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf("unable to open PIT tags left file (%u)", errF);
#endif 
                    sprintf(appError.message, "Unable to open PIT tags left file (%u)", errF);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    FILEIO_ErrorClear('A');
                    appError.number = ERROR_PIT_TAGS_LEFT_FILE_OPEN;
                    return FILEIO_RESULT_FAILURE;
                }

                for (i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; i++) {
                    FILEIO_Read(appDataPitTag.pit_tags_list[i], 1, 10, &file);
                    appDataPitTag.pit_tags_list[i][11] = '\0';
                    appDataPitTag.num_pit_tag_stored += 1;
                }

                if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file)) {
                    errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf("unable to close PIT tags left file (%u)", errF);
#endif 
                    sprintf(appError.message, "Unable to close PIT tags left file (%u)", errF);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    FILEIO_ErrorClear('A');
                    appError.number = ERROR_PIT_TAGS_LEFT_FILE_CLOSE;
                    return FILEIO_RESULT_FAILURE;
                }

            }

            if (ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_accepted_or_color_B && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_accepted_or_color_B) {

                if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, "PTRIGHT.TXT", FILEIO_OPEN_READ)) {
                    errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf("unable to open PIT tags right file (%u)", errF);
#endif 
                    sprintf(appError.message, "Unable to open PIT tags right file (%u)", errF);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    FILEIO_ErrorClear('A');
                    appError.number = ERROR_PIT_TAGS_RIGHT_FILE_OPEN;
                    return FILEIO_RESULT_FAILURE;
                }

                for (i = 0; i < appDataPitTag.num_pit_tag_accepted_or_color_B; i++) {
                    FILEIO_Read(appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A], 1, 10, &file);
                    appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A][11] = '\0';
                    appDataPitTag.num_pit_tag_stored += 1;
                }

                if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file)) {
                    errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf("unable to close PIT tags right file (%u)", errF);
#endif 
                    sprintf(appError.message, "Unable to close PIT tags right file (%u)", errF);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    FILEIO_ErrorClear('A');
                    appError.number = ERROR_PIT_TAGS_RIGHT_FILE_CLOSE;
                    return FILEIO_RESULT_FAILURE;
                }
            }
        } else if (TOP_BOTTOM_LEDS == appDataAttractiveLeds.pattern_number) {
            if (ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_denied_or_color_A && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_denied_or_color_A) {

                if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, "PTTOP.TXT", FILEIO_OPEN_READ)) {
                    errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf("unable to open PIT tags top file (%u)", errF);
#endif 
                    sprintf(appError.message, "Unable to open PIT tags top file (%u)", errF);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    FILEIO_ErrorClear('A');
                    appError.number = ERROR_PIT_TAGS_TOP_FILE_OPEN;
                    return FILEIO_RESULT_FAILURE;
                }

                for (i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; i++) {
                    FILEIO_Read(appDataPitTag.pit_tags_list[i], 1, 10, &file);
                    appDataPitTag.pit_tags_list[i][11] = '\0';
                    appDataPitTag.num_pit_tag_stored += 1;
                }

                if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file)) {
                    errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf("unable to close PIT tags top file (%u)", errF);
#endif 
                    sprintf(appError.message, "Unable to close PIT tags top file (%u)", errF);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    FILEIO_ErrorClear('A');
                    appError.number = ERROR_PIT_TAGS_TOP_FILE_CLOSE;
                    return FILEIO_RESULT_FAILURE;
                }

            }

            if (ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_accepted_or_color_B && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_accepted_or_color_B) {

                if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, "PTBOTTOM.TXT", FILEIO_OPEN_READ)) {
                    errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf("unable to open PIT tags bottom file (%u)", errF);
#endif 
                    sprintf(appError.message, "Unable to open PIT tags bottom file (%u)", errF);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    FILEIO_ErrorClear('A');
                    appError.number = ERROR_PIT_TAGS_BOTTOM_FILE_OPEN;
                    return FILEIO_RESULT_FAILURE;
                }

                for (i = 0; i < appDataPitTag.num_pit_tag_accepted_or_color_B; i++) {
                    FILEIO_Read(appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A], 1, 10, &file);
                    appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A][11] = '\0';
                    appDataPitTag.num_pit_tag_stored += 1;
                }

                if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file)) {
                    errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf("unable to close PIT tags bottom file (%u)", errF);
#endif 
                    sprintf(appError.message, "Unable to close PIT tags bottom file (%u)", errF);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    FILEIO_ErrorClear('A');
                    appError.number = ERROR_PIT_TAGS_BOTTOM_FILE_CLOSE;
                    return FILEIO_RESULT_FAILURE;
                }
            }
        } else if (ONE_LED == appDataAttractiveLeds.pattern_number) {
            memset(buf, 0, sizeof (buf));

            /* Cumulative sum of PIT tags per group */
            for (j = 0; j < 4; j++) {
                /* If no PIT tag in the current group, no need to search and read corresponding file => skip the current iteration */
                if (0 == appDataPitTag.num_pit_tag_per_group[j] || ALL_PIT_TAG_DENIED == appDataPitTag.num_pit_tag_per_group[j] || ALL_PIT_TAG_ACCEPTED == appDataPitTag.num_pit_tag_per_group[j]) {
                    /* If it's the first group*/
                    if (0 == j) {
                        cumulative_sum = 0;
                        appDataAttractiveLeds.pattern_one_led_groups[j] = 0;
                    }                        /* Otherwise */
                    else {
                        cumulative_sum += appDataPitTag.num_pit_tag_per_group[j - 1];
                        appDataAttractiveLeds.pattern_one_led_groups[j] = appDataAttractiveLeds.pattern_one_led_groups[j - 1];
                    }
                    continue;
                }

                /* Set the name of the file to read (e.g. PTONE1.TXT, PTONE2.TXT, PTONE3.TXT? */
                snprintf(buf, 13, "PTONE%d.TXT", j + 1);

                /* Open file */
                if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, buf, FILEIO_OPEN_READ)) {
                    errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf("unable to open PIT tags group %d file (%u)", j + 1, errF);
#endif 
                    sprintf(appError.message, "Unable to open PIT tags group %d file (%u)", j + 1, errF);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    FILEIO_ErrorClear('A');
                    if (0 == j) {
                        appError.number = ERROR_PIT_TAGS_PTONE1_FILE_OPEN;
                    } else if (1 == j) {
                        appError.number = ERROR_PIT_TAGS_PTONE2_FILE_OPEN;
                    } else if (2 == j) {
                        appError.number = ERROR_PIT_TAGS_PTONE3_FILE_OPEN;
                    } else {
                        appError.number = ERROR_PIT_TAGS_PTONE4_FILE_OPEN;
                    }
                    return FILEIO_RESULT_FAILURE;
                }

                if (0 == j) {
                    cumulative_sum = 0;
                    appDataAttractiveLeds.pattern_one_led_groups[j] = appDataPitTag.num_pit_tag_per_group[j];
                } else {
                    cumulative_sum += appDataPitTag.num_pit_tag_per_group[j - 1];
                    appDataAttractiveLeds.pattern_one_led_groups[j] = appDataAttractiveLeds.pattern_one_led_groups[j - 1] + appDataPitTag.num_pit_tag_per_group[j];
                }

                for (i = 0; i < appDataPitTag.num_pit_tag_per_group[j]; i++) {
                    FILEIO_Read(appDataPitTag.pit_tags_list[i + cumulative_sum], 1, 10, &file);
                    appDataPitTag.pit_tags_list[i + cumulative_sum][11] = '\0';
                    appDataPitTag.num_pit_tag_stored += 1;
                }

                if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file)) {
                    errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf("unable to close PIT tags group %d file (%u)", j + 1, errF);
#endif 
                    sprintf(appError.message, "Unable to close PIT tags group %d file (%u)", j + 1, errF);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    FILEIO_ErrorClear('A');
                    if (0 == j) {
                        appError.number = ERROR_PIT_TAGS_PTONE1_FILE_CLOSE;
                    } else if (1 == j) {
                        appError.number = ERROR_PIT_TAGS_PTONE2_FILE_CLOSE;
                    } else if (2 == j) {
                        appError.number = ERROR_PIT_TAGS_PTONE3_FILE_CLOSE;
                    } else {
                        appError.number = ERROR_PIT_TAGS_PTONE4_FILE_CLOSE;
                    }
                    return FILEIO_RESULT_FAILURE;
                }

            }
        }
    } else if (COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number) {
        if (ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_denied_or_color_A && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_denied_or_color_A) {

            if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, "PTCOLORA.TXT", FILEIO_OPEN_READ)) {
                errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf("unable to open PIT tags color A file (%u)", errF);
#endif 
                sprintf(appError.message, "Unable to open PIT tags color A file (%u)", errF);
                appError.current_line_number = __LINE__;
                sprintf(appError.current_file_name, "%s", __FILE__);
                FILEIO_ErrorClear('A');
                appError.number = ERROR_PIT_TAGS_COLOR_A_FILE_OPEN;
                return FILEIO_RESULT_FAILURE;
            }

            for (i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; i++) {
                FILEIO_Read(appDataPitTag.pit_tags_list[i], 1, 10, &file);
                appDataPitTag.pit_tags_list[i][11] = '\0';
                appDataPitTag.num_pit_tag_stored += 1;
            }

            if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file)) {
                errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf("unable to close PIT tags color A file (%u)", errF);
#endif 
                sprintf(appError.message, "Unable to close PIT tags color A file (%u)", errF);
                appError.current_line_number = __LINE__;
                sprintf(appError.current_file_name, "%s", __FILE__);
                FILEIO_ErrorClear('A');
                appError.number = ERROR_PIT_TAGS_COLOR_A_FILE_CLOSE;
                return FILEIO_RESULT_FAILURE;
            }

        }

        if (ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_accepted_or_color_B && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_accepted_or_color_B) {

            if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, "PTCOLORB.TXT", FILEIO_OPEN_READ)) {
                errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf("unable to open PIT tags color B file (%u)", errF);
#endif 
                sprintf(appError.message, "Unable to open PIT tags color B file (%u)", errF);
                appError.current_line_number = __LINE__;
                sprintf(appError.current_file_name, "%s", __FILE__);
                FILEIO_ErrorClear('A');
                appError.number = ERROR_PIT_TAGS_COLOR_B_FILE_OPEN;
                return FILEIO_RESULT_FAILURE;
            }

            for (i = 0; i < appDataPitTag.num_pit_tag_accepted_or_color_B; i++) {
                FILEIO_Read(appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A], 1, 10, &file);
                appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A][11] = '\0';
                appDataPitTag.num_pit_tag_stored += 1;
            }

            if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file)) {
                errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf("unable to close PIT tags color B file (%u)", errF);
#endif 
                sprintf(appError.message, "Unable to close PIT tags color B file (%u)", errF);
                appError.current_line_number = __LINE__;
                sprintf(appError.current_file_name, "%s", __FILE__);
                FILEIO_ErrorClear('A');
                appError.number = ERROR_PIT_TAGS_COLOR_B_FILE_CLOSE;
                return FILEIO_RESULT_FAILURE;
            }
        }
    } else {
        if (ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_denied_or_color_A && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_denied_or_color_A) {

            if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, "PTDENIED.TXT", FILEIO_OPEN_READ)) {
                errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf("unable to open PIT tags denied file (%u)", errF);
#endif 
                sprintf(appError.message, "Unable to open PIT tags denied file (%u)", errF);
                appError.current_line_number = __LINE__;
                sprintf(appError.current_file_name, "%s", __FILE__);
                FILEIO_ErrorClear('A');
                appError.number = ERROR_PIT_TAGS_DENIED_FILE_OPEN;
                return FILEIO_RESULT_FAILURE;
            }

            for (i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; i++) {
                FILEIO_Read(appDataPitTag.pit_tags_list[i], 1, 10, &file);
                appDataPitTag.pit_tags_list[i][11] = '\0';
                appDataPitTag.num_pit_tag_stored += 1;
            }

            if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file)) {
                errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf("unable to close PIT tags denied file (%u)", errF);
#endif 
                sprintf(appError.message, "Unable to close PIT tags denied file (%u)", errF);
                appError.current_line_number = __LINE__;
                sprintf(appError.current_file_name, "%s", __FILE__);
                FILEIO_ErrorClear('A');
                appError.number = ERROR_PIT_TAGS_DENIED_FILE_CLOSE;
                return FILEIO_RESULT_FAILURE;
            }

        }

        if (ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_accepted_or_color_B && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_accepted_or_color_B) {

            if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, "PTACCEPT.TXT", FILEIO_OPEN_READ)) {
                errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf("unable to open PIT tags accepted file (%u)", errF);
#endif 
                sprintf(appError.message, "Unable to open PIT tags accepted file (%u)", errF);
                appError.current_line_number = __LINE__;
                sprintf(appError.current_file_name, "%s", __FILE__);
                FILEIO_ErrorClear('A');
                appError.number = ERROR_PIT_TAGS_ACCEPTED_FILE_OPEN;
                return FILEIO_RESULT_FAILURE;
            }

            for (i = 0; i < appDataPitTag.num_pit_tag_accepted_or_color_B; i++) {
                FILEIO_Read(appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A], 1, 10, &file);
                appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A][11] = '\0';
                appDataPitTag.num_pit_tag_stored += 1;
            }

            if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file)) {
                errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf("unable to close PIT tags accepted file (%u)", errF);
#endif 
                sprintf(appError.message, "Unable to close PIT tags accepted file (%u)", errF);
                appError.current_line_number = __LINE__;
                sprintf(appError.current_file_name, "%s", __FILE__);
                FILEIO_ErrorClear('A');
                appError.number = ERROR_PIT_TAGS_ACCEPTED_FILE_CLOSE;
                return FILEIO_RESULT_FAILURE;
            }
        }
    }

    return FILEIO_RESULT_SUCCESS;

}

FILEIO_RESULT config_find_ini(void) {

    FILEIO_SEARCH_RECORD searchRecord;

    /* Log event if required */
    if (true == appDataLog.log_events) {
        store_event(OF_FIND_INI);
    }

    return FILEIO_Find("CONFIG.INI", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true);
}

INI_READ_STATE config_read_ini(void) {

    int32_t read_parameter;
    int s, i;
    char str[20];
    bool security_found = false;
    bool logs_found = false;
    bool check_found = false;
    bool attractiveleds_found = false;
    bool stimuli_found = false;
    //modif anzilane
    bool local_id_found = false;

    /* Log event if required */
    if (true == appDataLog.log_events) {
        store_event(OF_READ_INI);
    }

    /* Clear watch-dog timer because INI read take time */
    ClrWdt();

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
    printf("\tReading CONFIG.INI.\n");
#endif 

    for (s = 0; ini_getsection(s, str, 20, "CONFIG.INI") > 0; s++) {
        /* Check if "security" section is present in the INI file */
        if (0 == strcmp(str, "security")) {
            security_found = true;
        }
        /* Check if "logs" section is present in the INI file */
        if (0 == strcmp(str, "logs")) {
            logs_found = true;
        }
        /* Check if "check" section is present in the INI file */
        if (0 == strcmp(str, "check")) {
            check_found = true;
        }
        /* Check if "attractiveleds" is present in the INI file */
        if (0 == strcmp(str, "attractiveleds")) {
            attractiveleds_found = true;
        }
        /* Check if "stimuli" is present in the INI file */
        if (0 == strcmp(str, "stimuli")) {
            stimuli_found = true;
        }

        // anzilane modif
        if (0 == strcmp(str, "localId")) {
            local_id_found = true;
        }
    }

    /* Scenario number */
    read_parameter = ini_getl("scenario", "num", -1, "CONFIG.INI");
    if (-1 == read_parameter) {
        return INI_PB_SCENARIO_NUM;
    } else {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf("\tScenario number... read.\n");
#endif 
        appData.scenario_number = (uint8_t) read_parameter;
    }

    /* Site identification. */
    s = ini_gets("siteid", "zone", "XXXX", appData.siteid, sizearray(appData.siteid), "CONFIG.INI");
    for (i = s; i < 4; i++) {
        appData.siteid[i] = 'X';
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
    printf("\tSite ID... read.\n");
#endif 

    /* Wake up time. */
    read_parameter = ini_getl("time", "wakeup_hour", -1, "CONFIG.INI");
    if (-1 == read_parameter) {
        return INI_PB_TIME_WAKEUP_HOUR;
    } else {
        appDataAlarmWakeup.time.tm_hour = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf("\tTime Wakeup hour... read.\n");
#endif     
    }
    read_parameter = ini_getl("time", "wakeup_minute", -1, "CONFIG.INI");
    if (-1 == read_parameter) {
        return INI_PB_TIME_WAKEUP_MINUTE;
    } else {
        appDataAlarmWakeup.time.tm_min = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf("\tTime Wakeup minute... read.\n");
#endif
    }
    appDataAlarmWakeup.time.tm_sec = 0;

    /* Sleep time. */
    read_parameter = ini_getl("time", "sleep_hour", -1, "CONFIG.INI");
    if (-1 == read_parameter) {
        return INI_PB_TIME_SLEEP_HOUR;
    } else {
        appDataAlarmSleep.time.tm_hour = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf("\tTime sleep hour... read.\n");
#endif
    }
    read_parameter = ini_getl("time", "sleep_minute", -1, "CONFIG.INI");
    if (-1 == read_parameter) {
        return INI_PB_TIME_SLEEP_MINUTE;
    } else {
        appDataAlarmSleep.time.tm_min = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf("\tTime sleep minute... read.\n");
#endif
    }
    appDataAlarmSleep.time.tm_sec = 0;

    /* Clear watch-dog timer because INI read take time */
    ClrWdt();

    if (true == attractiveleds_found) {
        appData.flags.bit_value.attractive_leds_status = true;

        read_parameter = ini_getl("attractiveleds", "red_a", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_ATTRACTIVE_LEDS_RED_A;
        } else {
            appDataAttractiveLeds.red[0] = (uint8_t) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs red A... read.\n");
#endif
        }
        read_parameter = ini_getl("attractiveleds", "green_a", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_ATTRACTIVE_LEDS_GREEN_A;
        } else {
            appDataAttractiveLeds.green[0] = (uint8_t) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs green A... read.\n");
#endif
        }
        read_parameter = ini_getl("attractiveleds", "blue_a", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_ATTRACTIVE_LEDS_BLUE_A;
        } else {
            appDataAttractiveLeds.blue[0] = (uint8_t) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs blue A... read.\n");
#endif
        }

        if (appData.scenario_number == COLOR_ASSOCIATIVE_LEARNING) {
            read_parameter = ini_getl("attractiveleds", "red_b", -1, "CONFIG.INI");
            if (-1 == read_parameter) {
                return INI_PB_ATTRACTIVE_LEDS_RED_B;
            } else {
                appDataAttractiveLeds.red[1] = (uint8_t) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf("\tAttractive LEDs red B... read.\n");
#endif
            }
            read_parameter = ini_getl("attractiveleds", "green_b", -1, "CONFIG.INI");
            if (-1 == read_parameter) {
                return INI_PB_ATTRACTIVE_LEDS_GREEN_B;
            } else {
                appDataAttractiveLeds.green[1] = (uint8_t) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf("\tAttractive LEDs green B... read.\n");
#endif
            }
            read_parameter = ini_getl("attractiveleds", "blue_b", -1, "CONFIG.INI");
            if (-1 == read_parameter) {
                return INI_PB_ATTRACTIVE_LEDS_BLUE_B;
            } else {
                appDataAttractiveLeds.blue[1] = (uint8_t) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf("\tAttractive LEDs blue B... read.\n");
#endif
            }
        }

        if (GO_NO_GO == appData.scenario_number || COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number) {
            /* Attractive LEDs alternate delay. */
            read_parameter = ini_getl("attractiveleds", "alt_delay", -1, "CONFIG.INI");
            if (-1 == read_parameter) {
                return INI_PB_ATTRACTIVE_LEDS_ALT_DELAY;
            } else {
                appDataAttractiveLeds.alt_delay = (uint8_t) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf("\tAttractive LEDs alternate delay... read.\n");
#endif
            }
        }

        /* Attractive LEDs wake up time. */
        read_parameter = ini_getl("attractiveleds", "on_hour", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_ATTRACTIVE_LEDS_ON_HOUR;
        } else {
            appDataAttractiveLeds.wake_up_time.tm_hour = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs on hour... read.\n");
#endif
        }
        read_parameter = ini_getl("attractiveleds", "on_minute", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_ATTRACTIVE_LEDS_ON_MINUTE;
        } else {
            appDataAttractiveLeds.wake_up_time.tm_min = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs on minute... read.\n");
#endif
        }

        appDataAttractiveLeds.wake_up_time.tm_sec = 0;

        /* Attractive LEDs sleep time. */
        read_parameter = ini_getl("attractiveleds", "off_hour", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_ATTRACTIVE_LEDS_OFF_HOUR;
        } else {
            appDataAttractiveLeds.sleep_time.tm_hour = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs off hour... read.\n");
#endif
        }
        read_parameter = ini_getl("attractiveleds", "off_minute", -1, "CONFIG.INI");
        if (read_parameter == -1) {
            return INI_PB_ATTRACTIVE_LEDS_OFF_MINUTE;
        } else {
            appDataAttractiveLeds.sleep_time.tm_min = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs off minute... read.\n");
#endif
        }
        appDataAttractiveLeds.sleep_time.tm_sec = 0;

        if (GO_NO_GO == appData.scenario_number) {
            read_parameter = ini_getl("attractiveleds", "pattern", -1, "CONFIG.INI");
            if (read_parameter == -1) {
                return INI_PB_ATTRACTIVE_LEDS_PATTERN;
            } else {
                appDataAttractiveLeds.pattern_number = (uint8_t) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf("\tAttractive LEDs pattern number... read.\n");
#endif
            }

            if (ALL_LEDS == appDataAttractiveLeds.pattern_number) {

                read_parameter = ini_getl("attractiveleds", "pattern_percent", -1, "CONFIG.INI");
                if (read_parameter == -1) {
                    return INI_PB_ATTRACTIVE_LEDS_PATTERN_PERCENT;
                } else {
                    appDataAttractiveLeds.pattern_percent = (uint8_t) read_parameter;
                    appDataAttractiveLeds.pattern_percent_bak = appDataAttractiveLeds.pattern_percent;

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                    printf("\tAttractive LEDs pattern percent... read.\n");
#endif
                }

                //                ini_gets( "attractiveleds", "pattern_percent", "1.0", str, sizearray( str ), "CONFIG.INI" );
                //                appDataAttractiveLeds.pattern_percent = atof(str);
                //#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                //                printf( "\tAttractive LEDs pattern percent... read.\n" );
                //#endif
            }

        }
    }
    /* Logs */
    if (logs_found) {
        /* Data separator in the log file. */
        ini_gets("logs", "separator", DEFAULT_LOG_SEPARATOR, appDataLog.separator, sizearray(appDataLog.separator), "CONFIG.INI");
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf("\tLogs separator... read.\n");
#endif
//                read_parameter = ini_getl( "logs", "birds", -1, "CONFIG.INI" );
//                if ( -1 == read_parameter )
//                {
//                    return INI_PB_LOGS_BIRDS;
//                }
//                else
//                {
//                    appDataLog.log_data = ( bool ) read_parameter;
//                }
        read_parameter = ini_getl("logs", "udid", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            appDataLog.log_udid = true;
            //            return INI_PB_LOGS_UDID;
        } else {
            appDataLog.log_udid = (bool) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tLogs UDID... read.\n");
#endif
        }
        read_parameter = ini_getl("logs", "events", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            appDataLog.log_events = true;
            //            return INI_PB_LOGS_EVENTS;
        } else {
            appDataLog.log_events = (bool) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tLogs events... read.\n");
#endif
        }
        read_parameter = ini_getl("logs", "errors", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            appDataLog.log_errors = true;
            //            return INI_PB_LOGS_ERRORS;
        } else {
            appDataLog.log_errors = (bool) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tLogs errors... read.\n");
#endif
        }
        read_parameter = ini_getl("logs", "battery", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            appDataLog.log_battery = true;
            //            return INI_PB_LOGS_BATTERY;
        } else {
            appDataLog.log_battery = (bool) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tLogs battery... read.\n");
#endif
        }
        read_parameter = ini_getl("logs", "temperature", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            //            return INI_PB_LOGS_RFID;
            appDataLog.log_temp = true;
        } else {
            appDataLog.log_temp = (bool) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tLogs temperature... read.\n");
#endif
        }
        
    } else {
        /* Data separator in the log file. */
        ini_gets("logfile", "separator", DEFAULT_LOG_SEPARATOR, appDataLog.separator, sizearray(appDataLog.separator), "CONFIG.INI");
        appDataLog.log_birds = true;
        appDataLog.log_udid = true;
        appDataLog.log_events = true;
        appDataLog.log_errors = true;
        appDataLog.log_battery = true;
        appDataLog.log_rfid = true;
        appDataLog.log_temp = true;
        appDataLog.log_calibration = true;
    }

    /* Reward. */
    read_parameter = ini_getl("reward", "enable", -1, "CONFIG.INI");
    if (-1 == read_parameter) {
        return INI_PB_REWARD_ENABLE;
    } else {
        appData.reward_enable = (uint8_t) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf("\tReward enable... read.\n");
#endif
    }

    if (1 == appData.reward_enable) {
        /* Timeout taking reward. */
        read_parameter = ini_getl("reward", "timeout", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_REWARD_TIMEOUT;
        } else {
            appData.timeout_taking_reward = (uint16_t) read_parameter * 1000;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tReward timeout... read.\n");
#endif
        }
    } else {
        appData.timeout_taking_reward = 0;
    }

    /* Clear watch-dog timer because INI read take time */
    ClrWdt();

    return INI_READ_OK;
}

void config_print(void) {
    int i, j;

    printf("Configuration parameters\n");

    printf("\tSite ID\n\t\tZone: %s\n",
           appData.siteid);

    printf("\tTime\n");
    printf("\t\tWake up: %02d:%02d\n",
           appDataAlarmWakeup.time.tm_hour,
           appDataAlarmWakeup.time.tm_min);
    printf("\t\tSleep: %02d:%02d\n",
           appDataAlarmSleep.time.tm_hour,
           appDataAlarmSleep.time.tm_min);

    printf("\tLoggers\n");
    printf("\t\tSeparator: %s\n", appDataLog.separator);
    
    if (true == appDataLog.log_battery) {
        printf("\t\tBattery: enable - %s\n", BATTERY_LOG_FILE);
    } else {
        printf("\t\tBattery: disable\n");
    }

    if (true == appDataLog.log_temp) {
        printf("\t\tTemperature: enable - %s\n", EXT_TEMP_LOG_FILE);
    } else {
        printf("\t\tTemperature: disable\n");
    }
    if (true == appDataLog.log_calibration) {
        printf("\t\tCalibration: enable - %s\n", CALIBRATION_LOG_FILE);
    } else {
        printf("\t\tTemperature: disable\n");
    }
    if (true == appDataLog.log_udid) {
        printf("\t\tUdid: enable - %s\n", UDID_LOG_FILE);
    } else {
        printf("\t\tUdid: disable\n");
    }
    if (true == appDataLog.log_errors) {
        printf("\t\tErrors: enable - %s\n", ERRORS_LOG_FILE);
    } else {
        printf("\t\tErrors: disable\n");
    }
    if (true == appDataLog.log_events) {
        if (EVENT_FILE_BINARY == appDataEvent.file_type) {
            printf("\t\tEvents: enable - %s\n", appDataEvent.bin_file_name);
        } else if (EVENT_FILE_TEXT == appDataEvent.file_type) {
            printf("\t\tEvents: enable - %s\n", appDataEvent.txt_file_name);
        } else {
            printf("\t\tEvents: enable - %s - %s\n", appDataEvent.txt_file_name, appDataEvent.bin_file_name);
        }
    } else {
        printf("\t\tEvents: disable\n");
    }

    printf("\tChecks\n");
    if (true == appData.chk_food_level) {
        printf("\t\tFood level: enable\n");
    } else {
        printf("\t\tFood level: disable\n");
    }

    if (true == appData.flags.bit_value.attractive_leds_status) {

        printf("\tAttractive LEDs\n");

        printf("\t\tLEDs order: %u %u %u %u\n",
               appDataAttractiveLeds.leds_order[0], appDataAttractiveLeds.leds_order[1],
               appDataAttractiveLeds.leds_order[2], appDataAttractiveLeds.leds_order[3]);

        if (COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number) {

            printf("\t\tColor A: RGB(%d, %d, %d)\n",
                   appDataAttractiveLeds.red[0],
                   appDataAttractiveLeds.green[0],
                   appDataAttractiveLeds.blue[0]);
            printf("\t\tColor B: RGB(%d, %d, %d)\n",
                   appDataAttractiveLeds.red[1],
                   appDataAttractiveLeds.green[1],
                   appDataAttractiveLeds.blue[1]);
            printf("\t\tAlternate delay: %us\n", appDataAttractiveLeds.alt_delay);
        } else if (GO_NO_GO == appData.scenario_number) {
            printf("\t\tColor: RGB(%d, %d, %d)\n",
                   appDataAttractiveLeds.red[0],
                   appDataAttractiveLeds.green[0],
                   appDataAttractiveLeds.blue[0]);
            printf("\t\tAlternate delay: %us\n", appDataAttractiveLeds.alt_delay);

            if (ALL_LEDS == appDataAttractiveLeds.pattern_number) {
                printf("\t\tOn pattern percent: %u%%\n", appDataAttractiveLeds.pattern_percent_bak);
            }
        }
        
        printf("\t\tOn time: %02d:%02d\n",
               appDataAttractiveLeds.wake_up_time.tm_hour,
               appDataAttractiveLeds.wake_up_time.tm_min);
        printf("\t\tOff time: %02d:%02d\n",
               appDataAttractiveLeds.sleep_time.tm_hour,
               appDataAttractiveLeds.sleep_time.tm_min);
    }
    
    printf("\n");
}

void getIniPbChar(INI_READ_STATE state, char *buf, uint8_t n) {

    switch (state) {

        case INI_PB_SCENARIO_NUM:
            snprintf(buf, n, "Scenario: number");
            break;
        case INI_PB_SITEID_ZONE:
            snprintf(buf, n, "Site ID: zone");
            break;
        case INI_PB_TIME_WAKEUP_HOUR:
            snprintf(buf, n, "Wake-up: hour");
            break;
        case INI_PB_TIME_WAKEUP_MINUTE:
            snprintf(buf, n, "Wake-up: minute");
            break;
        case INI_PB_TIME_SLEEP_HOUR:
            snprintf(buf, n, "Sleep: hour");
            break;
        case INI_PB_TIME_SLEEP_MINUTE:
            snprintf(buf, n, "Sleep: minute");
            break;
        case INI_PB_LOGS_BIRDS:
            snprintf(buf, n, "Logs: birds");
            break;
        case INI_PB_LOGS_UDID:
            snprintf(buf, n, "Logs: udid");
            break;
        case INI_PB_LOGS_EVENTS:
            snprintf(buf, n, "Logs: events");
            break;
        case INI_PB_LOGS_ERRORS:
            snprintf(buf, n, "Logs: errors");
            break;
        case INI_PB_LOGS_BATTERY:
            snprintf(buf, n, "Logs: battery");
            break;
        case INI_PB_LOGS_RFID:
            snprintf(buf, n, "Logs: rfid");
            break;
        case INI_PB_ATTRACTIVE_LEDS_RED_A:
            snprintf(buf, n, "Attractive LEDs: red A");
            break;
        case INI_PB_ATTRACTIVE_LEDS_GREEN_A:
            snprintf(buf, n, "Attractive LEDs: green A");
            break;
        case INI_PB_ATTRACTIVE_LEDS_BLUE_A:
            snprintf(buf, n, "Attractive LEDs: blue A");
            break;
        case INI_PB_ATTRACTIVE_LEDS_RED_B:
            snprintf(buf, n, "Attractive LEDs: red B");
            break;
        case INI_PB_ATTRACTIVE_LEDS_GREEN_B:
            snprintf(buf, n, "Attractive LEDs: green B");
            break;
        case INI_PB_ATTRACTIVE_LEDS_BLUE_B:
            snprintf(buf, n, "Attractive LEDs: blue B");
            break;
        case INI_PB_ATTRACTIVE_LEDS_ALT_DELAY:
            snprintf(buf, n, "Attractive LEDs: alternate delay");
            break;
        case INI_PB_ATTRACTIVE_LEDS_ON_HOUR:
            snprintf(buf, n, "Attractive LEDs: off minute");
            break;
        case INI_PB_ATTRACTIVE_LEDS_ON_MINUTE:
            snprintf(buf, n, "Attractive LEDs: on minute");
            break;
        case INI_PB_ATTRACTIVE_LEDS_OFF_HOUR:
            snprintf(buf, n, "Attractive LEDs: off hour");
            break;
        case INI_PB_ATTRACTIVE_LEDS_OFF_MINUTE:
            snprintf(buf, n, "Attractive LEDs: off minute");
            break;
        case INI_PB_ATTRACTIVE_LEDS_PATTERN:
            snprintf(buf, n, "Attractive LEDs: pattern");
            break;
        case INI_PB_ATTRACTIVE_LEDS_PATTERN_PERCENT:
            snprintf(buf, n, "Attractive LEDs: pattern percent");
            break;
        default:
            snprintf(buf, n, "Error not listed");
            break;
    }


}

/*******************************************************************************
 End of File
 */
