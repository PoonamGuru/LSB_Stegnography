#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"decode.h"
#include"types.h"
#include"common.h"
char *old_magic_string = MAGIC_STRING;

/*Read and Validate the arguemtns
 *Input : argv, encInfo
 *Output : Status
 *Description:read and validate the command line argument
 */
Status read_and_validate_decode_args(char *argv[], DecodeInfo * decInfo)
{
    char buffer[4] = {"\0"};
    uint i,j=0;


    while(argv[2][i] != '.')      //increment the i till i point to dot
    {
	i++;
    }

    while(argv[2][i] != '\0')
    {

	buffer[j] = argv[2][i]; //store the file extension part to another buffer
	j++;
	i++;
    }

    buffer[j] = '\0';
    if(strcmp(buffer,".bmp") != 0)

    {
	printf("ERROR : enter commmand in proper order\n");
	printf("USAGE : ./a.out -e  xxx.bmp xxx.txt");
	exit(1);
    }

    decInfo->stego_image_fname = argv[2];  //assign steg file name to pointer
    printf("stego_image = %s\n", decInfo->stego_image_fname); 

    if(argv[3] != NULL) //check whether user entered output file name
    {
	printf("INFO : User defined output file name will be assigned to output file\n");
	decInfo->decode_secret_fname = argv[3];
    }
    else
    {
	printf("INFO : user didn't defined the output filename\n");
	decInfo->decode_secret_fname = "decode.txt";
	printf("INFO : Hence the default file name (%s) will be assigned to output file\n",decInfo->stego_image_fname);
    }



    return e_success;
}
/* Get File pointers for i/p and o/p files
 * Inputs: Stego Image file,decode Secret file and
 * Stego Image file
 * Output: FILE pointer for above file
 * Return Value: e_success or e_failure, on file errors
 */
Status open_decode_files(DecodeInfo *decInfo)
{
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    //Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
	return e_failure;
    }
    else
	printf("INFO : Opened %s \n",decInfo->stego_image_fname);


    //decode  Secret file
    decInfo->fptr_decode_secret = fopen(decInfo->decode_secret_fname, "w");
    //Do Error handling
    if (decInfo->fptr_decode_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->decode_secret_fname);
	return e_failure;
    }
    else
	printf("INFO : Opened %s \n",decInfo->decode_secret_fname);

    // No failure return e_success
    return e_success;
}

Status do_decoding(DecodeInfo * decInfo)
{
    printf("---------------------------------------------Task 4 : Start the Decoding Prcedure--------------------------------\n");
    printf("---------------------------------------------Task 4b : Decode the magic string-----------------------------------\n");
    if(decode_magic_string(decInfo) == e_success)
    {
	printf("INFO : Magic is decoded\n");
	printf("INFO : DONE\n");
	//return e_success;
    }
    else
    {
	printf("ERROR : magic string is not decoded\n");
	return e_failure;
    }

    printf("---------------------------------------TASK 4b : Decode the file extension size--------------------------------\n");
    if(decode_file_extn_size(decInfo) == e_success)
    {
	printf("INFO : file extension size  is decoded\n");
	printf("INFO : DONE\n");
	//	return e_success;
    }
    else
    {
	printf("ERROR : file extension size is not decoded\n");
	return e_failure;
    }

    printf("---------------------------------------TASK 4c : Decode the file extension data--------------------------------\n");
    if(decode_file_extn_data(decInfo) == e_success)
    {
	printf("INFO : file extension data  is decoded\n");
	printf("INFO : DONE\n");
	//	return e_success;
    }
    else
    {
	printf("ERROR : file extension data is not decoded\n");
	return e_failure;
    }
    printf("---------------------------------------TASK 4d : Decode the file  size--------------------------------\n");
    if(decode_file_size(decInfo) == e_success)
    {
	printf("INFO : file size  is decoded\n");
	printf("INFO : DONE\n");
	//	return e_success;
    }
    else
    {
	printf("ERROR : file size is not decoded\n");
	return e_failure;
    }

    printf("---------------------------------------TASK 4d : Decode the file  data--------------------------------\n");
    if(decode_file_data(decInfo) == e_success)
    {
	printf("INFO : file data  is decoded\n");
	printf("INFO : DONE\n");
	//	return e_success;
    }
    else
    {
	printf("ERROR : file data is not decoded\n");
	return e_failure;
    }

    /*close all the files*/
    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_decode_secret);
    return e_success;


}
/*Decode the magic string
 *Input : Magic string , decInfo
 *Output: Status
 *Description : Decode the magicstring to the steg image
 */
Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic_string;

    fseek(decInfo->fptr_stego_image ,54L,SEEK_SET); //shift the file pointer of  dest image  to 55 position

    magic_string  =  decode_image_to_data(8,decInfo->fptr_stego_image);
    if(*old_magic_string == magic_string) 
    {
	printf("both the magic string is matched\n");
	//putchar(magic_string);
	return e_success;
    }
    else
	return e_failure;
    //	printf("pos:%ld\n",ftell(decInfo->fptr_stego_image));
}
/*Decode file extension size
 *Input : decInfo
 *Output: Status
 *Description : decode the sizeo f file extension
 */
Status decode_file_extn_size(DecodeInfo * decInfo)
{
    //uint file_extn_len = 0;
    unsigned long position = ftell(decInfo->fptr_stego_image);
    //	printf("pos:%ld\n",ftell(decInfo->fptr_stego_image));

    fseek(decInfo->fptr_stego_image ,position,SEEK_SET); //shift the file pointer of  dest image  to 62 position
    //fseek(decInfo->fptr_stego_image ,62L,SEEK_SET); //shift the file pointer of  dest image  to 62 position

    decInfo->file_extn_len  =  decode_image_to_data(32,decInfo->fptr_stego_image);
    //printf("%d\n",file_extn_len);
    //	printf("%d\n",decInfo->file_extn_len);
    //	printf("pos:%ld\n",ftell(decInfo->fptr_stego_image));
    return e_success;
}

/*Decode file extension character
 *Input : decInfo
 *Output: Status
 *Description : decode the character of file extension
 */
Status decode_file_extn_data(DecodeInfo * decInfo)
{
    char file_extn_data[5] = {'\0'};
    int i;

    unsigned long position = ftell(decInfo->fptr_stego_image);
    //	printf("pos:%ld\n",ftell(decInfo->fptr_stego_image));


    fseek(decInfo->fptr_stego_image ,position,SEEK_SET); //shift the file pointer of  dest image  to 94 position
    //fseek(decInfo->fptr_stego_image ,94L,SEEK_SET); //shift the file pointer of  dest image  to 94 position

    for(i = 0 ; i < decInfo->file_extn_len; i++)
    {
	file_extn_data[i]  =  decode_image_to_data(8,decInfo->fptr_stego_image);
	//	    printf("%c\n",file_extn_data[i]);
    }
    //	printf("pos:%ld\n",ftell(decInfo->fptr_stego_image));
    return e_success;
}
/*Decode file  size
 *Input : decInfo
 *Output: Status
 *Description : decode the sizeo of file 
 */
Status decode_file_size(DecodeInfo * decInfo)
{
    //   uint file_size = 0;


    unsigned long position = ftell(decInfo->fptr_stego_image);
    //	printf("pos:%ld\n",ftell(decInfo->fptr_stego_image));


    fseek(decInfo->fptr_stego_image ,position,SEEK_SET); //shift the file pointer of  dest image  to 62 position
    //    fseek(decInfo->fptr_stego_image ,126L,SEEK_SET); //shift the file pointer of  dest image  to 62 position

    decInfo->file_size  =  decode_image_to_data(32,decInfo->fptr_stego_image);
    //printf("\n%d\n",file_size);
    //	printf("\n%d\n",decInfo->file_size);


    //	printf("pos:%ld\n",ftell(decInfo->fptr_stego_image));
    return e_success;
}

/*Decode file  data
 *Input : decInfo
 *Output: Status
 *Description : decode the data of secret file 
 */
Status decode_file_data(DecodeInfo * decInfo)
{
    char secret_buffer;
    int i, ch;
    unsigned long position = ftell(decInfo->fptr_stego_image);
    //	printf("pos:%ld\n",ftell(decInfo->fptr_stego_image));


    fseek(decInfo->fptr_stego_image ,position,SEEK_SET); //shift the file pointer of  dest image  to 62 position
    //   fseek(decInfo->fptr_stego_image ,158L,SEEK_SET); //shift the file pointer of  dest image  to 62 position

    for(i = 0; i< decInfo->file_size ; i++)
    {
	secret_buffer =  decode_image_to_data(8,decInfo->fptr_stego_image);
	//	    printf("%c",secret_buffer);
	ch = secret_buffer;
	fputc(ch,decInfo->fptr_decode_secret);   //writee into file
    }
    //	printf("pos:%ld\n",ftell(decInfo->fptr_stego_image));

    return e_success;
}



/*Decode the image to data
 *Input : decInfo
 *Output: Status
 *Description : Decode data from steg image
 */


uint  decode_image_to_data(int n,FILE *fptr_image)
{

    char data[1];
    char *buffer;
    int i, j = 0;
    uint  num = 0;
    /*dynamic memory allocation*/
    buffer = malloc((n+1) * sizeof(char));
    memset(buffer , '\0',n+1);

    /*extract the lsb from stego image byte*/
    for(i = 0; i < n; i++)
    {
	if(fread(data, sizeof(char), 1, fptr_image) != 1) //read the byte from the steg image
	{
	    if(ferror(fptr_image) != 0) //check for the error
	    {
		perror("fread:");
		clearerr(fptr_image);  //clear the fptr
	    }
	}

	buffer[i] = *data & 0x01; // extract the lsb from the byte of image
	//		printf("%d",buffer[i]);
    }


    /*convert each char to integer*/
    for(i = (n-1);i >= 0 ; i--)
    {
	if(buffer[i] & 0x01)
	{
	    num += pow(2,j);
	}
	j++;
	//	printf("%d",buffer[i]);
    }

    // printf(" \n %d %x %c\n",num,num,num);

    return num;
}














