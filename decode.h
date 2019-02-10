#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* Structure to store information required for
 * decoding secret file from  steg Image
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    

    /* decode File Info */
    char *decode_secret_fname;
    FILE *fptr_decode_secret;
    uint file_size;
    uint file_extn_len;
   } DecodeInfo;


/* Decoding function prototype */


/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo); 

/*start doing decode*/
Status do_decoding(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/*extract the data from the image*/
 uint decode_image_to_data(int n, FILE *fptr_image);
 
 
 /* Encode secret file extenstion size */
Status decode_file_extn_size(DecodeInfo *decInfo);

/* Encode secret file extenstion data */
Status decode_file_extn_data(DecodeInfo *decInfo);


/* Encode secret  file  size */
Status decode_file_size(DecodeInfo *decInfo);


/* Encode secret file  data */
Status decode_file_data(DecodeInfo *decInfo);


#endif
