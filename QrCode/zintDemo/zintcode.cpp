/****************************************************************************
 * File       : zint_code.c
 ***************************************************************************/

// Standard Library
#include <string.h>
#include <stdio.h>

// so Library
#include "zint.h"

// Project Header
#include "zintcode.h"


/****************************************************************************
Descpribe: Create Qrcode API with C Code by calling zint lib.
Input    : pQrCodeData, the qrcode data buf
           QrcodeLen, the len of qrcode data, but it can be 0
           pQrCodeFile, the output file name of qrcode, it can be NULL
Output   : pZintRet, to store the ret code from linzint.
Return   : 0 is ok, and other values are fail. See the meanings in enum ZINT_RET_CODE
Notes    : pQrCodeFile, Must end in .png, .eps or .svg. when isn,t NULL string.
****************************************************************************/
ZINT_RET_CODE ZintCreateQrCode(uint8_t *pQrCodeData, int QrcodeLen, char *pQrCodeFile, int *pZintRet)
{
    struct zint_symbol *pMySymbol     = NULL;
    int RetCode                     = 0;

    if(!pQrCodeData) //check input pointer
    {
        return ZINT_ERR_INV_DATA;
    }

    if(QrcodeLen == 0)
    {
        QrcodeLen = strlen((char *)pQrCodeData);
    }
    if(QrcodeLen > QRCODE_MAX_LEN)//len is too long
    {
        return ZINT_ERR_TOO_LONG;
    }

    if(0 == ZBarcode_ValidID(BARCODE_QRCODE))
    {
        return ZINT_ERR_INV_CODE_ID;
    }

    pMySymbol = ZBarcode_Create();
    if(pMySymbol == NULL)
    {
        return ZINT_ERR_MEMORY;
    }

    if(pQrCodeFile)//when it's NULL, outfile will be "out.png"
    {
        if(strstr(pQrCodeFile, "png") || (strstr(pQrCodeFile, "eps")) || (strstr(pQrCodeFile, "svg")))
        {
            strcpy(pMySymbol->outfile, pQrCodeFile);
        }
        else
        {
            ZBarcode_Clear(pMySymbol);
            ZBarcode_Delete(pMySymbol); //release memory in zint lib
            return ZINT_ERR_FILE_NAME;
        }
    }
    pMySymbol->symbology     = BARCODE_QRCODE;
    pMySymbol->option_1     = 3; //ECC Level.It can be large when ECC Level is larger.(value:1-4)
    pMySymbol->scale         = 4; //contorl qrcode file size, default is 1, used to be 4
    pMySymbol->border_width = 2; //set white space width around your qrcode and 0 is for nothing

    RetCode = ZBarcode_Encode_and_Print(pMySymbol, pQrCodeData, QrcodeLen, 0);
    ZBarcode_Clear(pMySymbol);
    ZBarcode_Delete(pMySymbol); //release memory in zint lib

    if(pZintRet)
    {
        *pZintRet = RetCode; //save ret code from zint lib
    }

    return ((0 == RetCode) ? (ZINT_OK) : (ZINT_ERR_LIB_RET));
}
