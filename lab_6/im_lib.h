#ifndef IM_LIB_H
#define IM_LIB_H

#include <iostream>
#include <fstream>
#include <string.h>

typedef struct {
      char type1;               /* Two chars "BM" in little-endian order  */
      char type2;
    __int16 size1;               /* File size                                 */
    __int16 size2;               /* File size                              */
    __int16 reserved1;
    __int16 reserved2;
    __int16 offset1;             /* Bytes from start of file to pixel data */
    __int16 offset2;             /* Bytes from start of file to pixel data */
} BmpFileHeader;

typedef struct {
    __int32 size;               /* Size of this header  (40 bytes)        */
    __int32  width;              /* Bitmap width in pixels                 */
    __int32  height;             /* Bitmap height in pixels                */
    __int16 planes;             /* Must be 1                              */
    __int16 bits;               /* Bits per pixel. (8 for 256-color bmps) */
    __int32 compression;        /* 0 means no compression                 */
    __int32 imagesize;          /* Number of bytes in pixel data section  */
    __int32  xresolution;        /* Not used by any of my functions        */
    __int32  yresolution;        /* Not used by any of my functions        */

    __int32 ncolors;            /* 0 means use all colors                 */
    __int32 importantcolors;    /* 0 means all are important              */
} BmpInfoHeader;

#define PI 3.1415926535897932384626433832795 // the value of PI

unsigned __int8* imread(char *file_name, BmpInfoHeader &im_info, BmpFileHeader &im_header, unsigned __int8* color_map);
int imwrite(char *file_name,unsigned __int8* image, BmpInfoHeader &im_info, BmpFileHeader &im_header, unsigned __int8* color_map);

#endif
 