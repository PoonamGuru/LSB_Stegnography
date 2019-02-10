#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include"common.h"
uint  file_size;
char magic_string[10];

/*Read and Validate the arguemtns
 *Input : argv, encInfo
 *Output : Status
 *Description:read and validate the command line argument
 */
Status read_and_validate_encode_args(char *argv[], EncodeInfo * encInfo)
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




    encInfo->src_image_fname = argv[2];
    encInfo->secret_fname = argv[3];

    if(argv[4] != NULL)        //4th arg is optional , if it entered use that
    {
	printf("INFO : User defined output file name will be assigned to output file\n");
	encInfo->stego_image_fname = argv[4];
    }
    else //use default name
    {
	printf("INFO : user didn't defined the output filename\n");
	encInfo->stego_image_fname = "steg_image.bmp";
	printf("INFO : Hence the default file name (%s) will be assigned to output file\n",encInfo->stego_image_fname);
    }

    return e_success;
}
/* Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above file
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    //Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
	return e_failure;
    }
    else
    {
	printf("INFO : Opened %s \n",encInfo->src_image_fname);
    }
    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    //Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
	return e_failure;
    }
    else
    {
	printf("INFO : Opened %s \n",encInfo->secret_fname);
    }  
    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    //Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
	return e_failure;
    }
    else
    {
	printf("INFO : Opened %s \n",encInfo->stego_image_fname);
    }

    // No failure return e_success
    return e_success;
}

/*Check encryption is possible
 *Input : encInfo
 *Outptut : Status
 *Description :  check the capacity of the of  secret file and image  
 */

Status check_capacity(EncodeInfo *encInfo)
{
    uint  image_size, total_size = 0;

    image_size = get_image_size_for_bmp(encInfo->fptr_src_image); //get the image data part size
    file_size = get_file_size((encInfo->fptr_secret)); //get the file size

    if(file_size != 0) //to check file is empty or not
    {
	printf("INFO : %s file is not empty\n",encInfo->secret_fname);
    }
    else
    {
	printf("ERROR : %s file is empty\n",encInfo->secret_fname);
	return e_failure;
    }

//    printf("image size = %u\n", image_size);

    total_size = ( strlen(MAGIC_STRING) * 8 ) + ( 4 * 8 ) +( 32 ) + (4 * 8 )+ (file_size * 8 );  // total encoding capacity
  //  printf("total size = %u\n",total_size);

    /*check for the encoding possibility*/
    if(total_size > image_size)
	return e_failure;
    else
	return e_success;
}



/*Get file size
 *Input : file ptr
 *Output : file_size
 *Description : set the postion of file pointer to end of file , then by usinf ftell file size can be take easily
 */
uint get_file_size(FILE *fptr)
{
    uint file_size;
    fseek(fptr,0L,SEEK_END);  //fptr is shilfted to end of the file
    file_size = ftell(fptr);  //gives the current position which is equal to size of file
    rewind(fptr);             //reset to the fptr postion (back to begining of file)

    return file_size;
}


/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}
/*start the encodeing
 *Input : encinfo
 *Output : status
 *Description : 1 . copy the header of source image file to stego image file
 */
Status do_encoding(EncodeInfo *encInfo, char *argv[])
{

    char * file_extn;

    printf("``````````````````````````````````````````TASK 5a : checking magic string entered by user``````````````````````````````\n\n");
    if(argv[5] == "-p")
    {
	puts("Enter the magic string");
	scanf("%s",magic_string);
	printf("INFO : FOR encoding purpose using user entered magic string \" %s \" \n",magic_string);
    }
    else
    {
	printf("INFO : User didn't enter any magic string , default magic string will be used\n");
	strcpy(magic_string,MAGIC_STRING);
    } 

    printf("``````````````````````````````````````TASK 5b : copy the header part to output file````````````````````````````````````````````\n\n");
    if(copy_bmp_header(encInfo->fptr_src_image , encInfo->fptr_stego_image) == e_success)
    {
	printf("INFO : Header part of the image is successfully copied to steg image\n");
	//	return e_success;
    }

    else
    {
	printf("INFO : Header part of the image is not  copied to steg image\n");
	//	return e_failure;
    }

    printf("``````````````````````````````````````````````````TASK 5c: Encode the magic string``````````````````````````````````````````````\n\n");
    if(encode_magic_string(magic_string , encInfo)  == e_success)
    {
	printf("INFO : magic string is encoded successuly\n");
	printf("INFO : DONE\n");
	//	return e_success;
    }
    else
    {
	printf("ERROR : magic string is not encoded successfully\n");
	return e_failure;
    }

    printf("``````````````````````````````````````````````````TASK 5d: Encode the file extension size``````````````````````````````````````````````\n\n");

    /*count the number of character from dot in file file name and store the character from dot to till null character in file_extn string*/
    file_extn =  count_store_file_extn(encInfo->secret_fname);

    if( encode_secret_file_extension_size(file_extn, encInfo)  == e_success)
    {
	printf("INFO : file extension size is encoded successuly\n");
	printf("INFO : DONE\n");
	//	return e_success;
    }
    else
    {
	printf("ERROR : file extension is not encoded successfully\n");
	return e_failure;
    }

    printf("``````````````````````````````````````````````````TASK 5e: Encode the file extension ``````````````````````````````````````````````\n\n");


    if( encode_secret_file_extension(file_extn, encInfo)  == e_success)
    {
	printf("INFO : file extension character is encoded successuly\n");
	printf("INFO : DONE\n");
	//	return e_success;
    }
    else
    {
	printf("ERROR : file extension character is not encoded successfully\n");
	return e_failure;
    }
    printf("``````````````````````````````````````````````````TASK 5f: Encode the file size ``````````````````````````````````````````````\n\n");


    if( encode_secret_file_size(file_size, encInfo) == e_success)
    {
	printf("INFO : file size is encoded successuly\n");
	printf("INFO : DONE\n");
	//	return e_success;
    }
    else
    {
	printf("ERROR : file size  is not encoded successfully\n");
	return e_failure;
    }


    printf("``````````````````````````````````````````````````TASK 5g: Encode the characters of secret file ``````````````````````````````````````````````\n\n");


    if( encode_secret_file_data(encInfo) == e_success)
    {
	printf("INFO : secrate file data is encoded successuly\n");
	printf("INFO : DONE\n");
	//	return e_success;
    }
    else
    {
	printf("ERROR : secret file data is not encoded successfully\n");
	return e_failure;
    }


    printf("``````````````````````````````````````````````````TASK 5h: Copy the remaining data to output file ``````````````````````````````````````````````\n\n");
    if(  copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image)  == e_success)
    {
	printf("INFO : remaining data is copied successuly\n");
	printf("INFO : DONE\n");
	//	return e_success;
    }
    else
    {
	printf("ERROR : Remaining data is not copied successfully\n");
	return e_failure;
    }


    //printf("postion_s = %ld\n",ftell(encInfo->fptr_src_image));
    //printf("postion_d = %ld\n",ftell(encInfo->fptr_stego_image));

    /*close all the files*/
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);



    return e_success;

}


/*Copy the header part of src image to output iamge file
 *Input : fptr_src_image , fptr_stego_image
 *Output : Status
 *Description : read the first 54 bytes from original image file and write into the steg image file
 */
Status copy_bmp_header(FILE * fptr_src_image, FILE * fptr_stego_image)
{
    char image_header[54] = {0};
    fseek(fptr_stego_image, 0L, SEEK_SET);  //set fptr pointer to begininning of the file
    fseek(fptr_src_image, 0L, SEEK_SET);     //set fptr pointer to begininning of the file

//    printf("postion of the src image :%ld\n",ftell(fptr_src_image));


    //read 54 bytes of data from src_image to buffer(image_header)
    if(fread(image_header,  sizeof(char), 54 , fptr_src_image) != 54)
    {
	if(ferror(fptr_src_image) != 0)
	{
	    printf("ERROR: failed to read the file\n");
	    clearerr(fptr_src_image);
	    return e_failure;
	}
    }


    //write the 54 bytes of date from buffer to stego image file

    if(fwrite(image_header , sizeof(char), 54, fptr_stego_image) != 54)
    {
	if(ferror(fptr_src_image) != 0)
	{
	    printf("ERROR: failed to read the file\n");
	    clearerr(fptr_src_image);
	    return e_failure;
	}
    }

    return e_success;
}
/*Encode the magic string
 * Input : Magic string , encInfo
 *Output: Status
 *Description : Encode the magicstring to the src image
 */

Status encode_magic_string(const char * magic_string,EncodeInfo *encInfo)
{
    uint num = (uint )magic_string[0];     //store the ascii value
    printf("%x\n",num);

    fseek(encInfo->fptr_src_image ,54L,SEEK_SET); //shift the file pointer of  src image  to 55 position
    fseek(encInfo->fptr_stego_image ,54L,SEEK_SET); //shift the file pointer of  dest image  to 55 position

    if(encoding(8,num , encInfo) == e_success)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

/*get the count of file extn and store file extn character in a file
 *Input : secret_fname
 *Output : lenght
 *Description : get the count of file extn and store file extn character in a file
 */
char * count_store_file_extn(char *file_extn)
{
    int i = 0, j = 0;
    uint file_extn_len = 0;           //length of the file extension
    static char f_extn[10] = {'\0'};
    while(file_extn[i] != '.')      //increment the i till i point to dot
    {
	i++;
    }

    while(file_extn[i] != '\0')
    {
	file_extn_len++;          //count the number of character in file extension
	f_extn[j] = file_extn[i]; //store the file extension part to another buffer
	j++;
	i++;
    }
    f_extn[j] = '\0';
  //  puts("file extn:");
//    puts(f_extn);
    //printf("%d\n",file_extn_len);

    return f_extn;
}
/*Encode the file extension size
 *Input : file_extn , encInfo
 *Output : Status
 *Description : encode the file extension character with scr image and copy the encoded data to output file
 */

Status encode_secret_file_extension_size(const char *file_extn, EncodeInfo *encInfo)
{
    uint file_extn_len = strlen(file_extn);
   // printf("lenght = %d\n",file_extn_len);


    fseek(encInfo->fptr_src_image ,62L,SEEK_SET); //shift the file pointer of  src image  to 62 position
    fseek(encInfo->fptr_stego_image ,62L,SEEK_SET); //shift the file pointer of  dest image  to 62 position

    /*encode the file extension size*/
    if(encoding(32 , file_extn_len, encInfo) == e_success)
	return e_success;

    else
	return e_failure;

}


/*Encode the file extension characters
 *Input : file_extn , encInfo
 *Output : Status
 *Description : encode the file extension character with scr image and copy the encoded data to output file
 */

Status encode_secret_file_extension(const char *file_extn, EncodeInfo *encInfo)
{
    uint num, position, i;

    position = ftell(encInfo->fptr_src_image); //get the current position of the file
 //   printf("postion = %d\n",position);

    fseek(encInfo->fptr_src_image ,94L,SEEK_SET); //shift the file pointer of  src image  to 94 position
    fseek(encInfo->fptr_stego_image ,94L,SEEK_SET); //shift the file pointer of  dest image  to 94 position

    for(i = 0; i < 4; i++)
    {
	num = (uint)file_extn[i];
	/*encode the file extension*/
	if(encoding(8 , num, encInfo) != e_success)
	    return e_failure;
    }

    return e_success;

}



/*Encode secret file size 
 *Input : file_size , encInfo
 *Output : Status
 *Description : encode the secret file size to scr_image
 */ 
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{

    uint position;

    position = ftell(encInfo->fptr_src_image); //get the current position of the file
   // printf("postion = %d\n",position);


   // printf("size= %ld\n",file_size);
    fseek(encInfo->fptr_src_image ,126L,SEEK_SET); //shift the file pointer of  src image  to 126 position
    fseek(encInfo->fptr_stego_image ,126L,SEEK_SET); //shift the file pointer of  dest image  to 126 position

    /*encode the file extension size*/
    if(encoding(32 , (int)file_size, encInfo) == e_success)
	return e_success;

    else
	return e_failure;

}

/*Encode secret file data
 *Input : encInfo
 *Output : Status
 *Description : Encode the characters in secret file with src image
 */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    uint position, i;
    int ch;
    position = ftell(encInfo->fptr_src_image); //get the current position of the file
   // printf("postion = %d\n",position);


    fseek(encInfo->fptr_src_image ,158L,SEEK_SET); //shift the file pointer of  src image  to 158 position
    fseek(encInfo->fptr_stego_image ,158L,SEEK_SET); //shift the file pointer of  dest image  to 158 position

    for(i = 0; i < file_size; i++) 
    {
	/*read the character from the secret file*/
	if((ch =fgetc(encInfo->fptr_secret)) != EOF)
	    //num = (uint)(ch); //type cast char to unsinged int

//	    printf("ch =%c\n",ch);
	/*encode the file extension size*/
	if(encoding(8 , (uint)ch , encInfo) != e_success)
	    return e_failure;

    }
    position = ftell(encInfo->fptr_src_image); //get the current position of the file
  //  printf("postion = %d\n",position);

    return e_success;
}




/*endcode byte to lsb
 *Input : data , image_buffer
 *Output : Status
 *Description : replace lsb'ss of scr_image to magic string
 */
Status encode_byte_tolsb( char data, char *image_buffer)
{

    *image_buffer = ((*image_buffer& ~ 0x01) |( data & 0x01));
    return e_success;
}



/*Encode function, which does the real encoding 
 *Input: data,size,fptr_scr_image,fptr_stego_image
 *Output : Status
 *Description : copy the image_buffer data to stego_image
 */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    if(fwrite(data,sizeof(char),1,fptr_stego_image) != 1) //write data into the output file
    {
	if(ferror(fptr_stego_image) != 0)
	{
	    printf("ERROR : failed to write into a file\n");
	    clearerr(fptr_stego_image);
	    return e_failure;
	}
    }

    return e_success;
}



/*Do the actual encoding
 *Input : n, buffer , encInfo
 *Output : Status
 *Description : get the encoded keys,  reverse it , extract the bytes to be encoded , do the replace , copy the encoded data to output file
 */ 



Status encoding(uint n, uint buffer , EncodeInfo *encInfo)
{
    //char temp[n+1] = {'\0'};
    int i;
    char *temp = malloc((n+1) * sizeof(char));
    memset(temp,'\0',(n+1)); //set all the block with \0

    /*store the msb bit first*/
    for(i = n-1; i >= 0; i--)      
    {
	if(buffer  & 0x01)
	{
	    temp[i] = '1';
//	    printf("1");
	}
	else
	{
	    temp[i] = '0';
//	    printf("0");
	}
	buffer  >>= 1;
    }

  //  puts("");
    /*extract a byte from the scr image every time*/
    for(i = 0; i < n; i++)
    {

	if(fread(encInfo->image_data, sizeof(char), 1 , encInfo->fptr_src_image) != 1) //to read a byte from src image file
	{
	    if(ferror(encInfo->fptr_src_image) != 0) //check why fread failed
	    {
		printf("ERROR: failed to read the file\n");
		clearerr(encInfo->fptr_src_image); //clear the src image file pointer
		return e_failure;
	    }
	}


	/*pass the temp and image data to encode*/
	if(encode_byte_tolsb((char)temp[i],encInfo-> image_data) == e_success)
	{

	    if(encode_data_to_image(encInfo->image_data,1, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
		return e_failure;
	}

    }

    /*free the dma memory*/
    if(temp != NULL)
    {
	free(temp);
	temp = NULL;
    }
    return e_success;
}

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{

    char image_data[8];
    while(feof(fptr_src) == 0)  //untill end of the file        /*note : feof return non zero value if it encounter EOF else 0*/
    {
	if(fread(image_data, sizeof(char), 1 , fptr_src) != 1) //to read a byte from src image file
	{
	    if(ferror(fptr_src) != 0) //check why fread failed
	    {
		printf("ERROR: failed to read the file\n");
		clearerr(fptr_src); //clear the src image file pointer
		return e_failure;
	    }
	}

	/*write the data in image_data to dest file*/
	if(fwrite(image_data , sizeof(char),1,fptr_dest) != 1)
	{
	    if(ferror(fptr_dest) != 0)
	    {
		printf("ERROR : failed to write into the file\n");
		clearerr(fptr_dest);
		return e_failure;
	    }
	}
    }

    clearerr(fptr_src);

    return e_success;
}



