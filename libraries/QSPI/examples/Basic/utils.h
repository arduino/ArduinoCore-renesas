
#define BIT_SHIFT_8  (8u)
#define SIZE_64      (64u)

#define LVL_ERR      (1u)       /* error conditions   */

#define RESET_VALUE             (0x00)

#define EP_VERSION              ("1.0")
#define MODULE_NAME             "r_qspi"
#define BANNER_INFO              "\r\n******************************************************************"\
                "\r\n*   Renesas FSP Example Project for "MODULE_NAME" Module                *"\
                "\r\n*   Example Project Version %s                                  *"\
                "\r\n*   Flex Software Pack Version  %d.%d.%d                            *"\
                "\r\n******************************************************************"\
                "\r\nRefer to readme.txt file for more details on Example Project and" \
                    "\r\nFSP User's Manual for more information about "MODULE_NAME" driver\r\n"

#define SEGGER_INDEX            (0)

#define APP_PRINT(...)      printf (__VA_ARGS__);

#define APP_ERR_PRINT(...)  if(LVL_ERR)\
        printf ("[ERR] In Function: %s(), %s",__FUNCTION__,##__VA_ARGS__);

#define APP_ERR_TRAP(err)        if(err) {\
        printf("\r\nReturned Error Code: 0x%x  \r\n", err);\
        __asm("BKPT #0\n");} /* trap upon the error  */
