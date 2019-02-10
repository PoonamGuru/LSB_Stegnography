#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"types.h"
#include"common.h"
#include"encode.h"
#include"decode.h"
int main(int argc, char *argv[])
{
    EncodeInfo encInfo;          //encode structure variable
    DecodeInfo decInfo;         //decode structure variable
    OperationType op_type;      //enum type
/*minimum  commond line arg*/
	if(argc == 1)
	{
	    printf("ERROR : enter the arguements properly\n");
	    printf("USAGE : <./a.out> -e <xxx.bmp> <xxxx.txt> <xxx.bmp/optional>\n");
	    exit(1);
	}

    if(strcmp(argv[1],"-e") == 0)
    {
	printf("%d\n",argc);
		/*minimum  commond line arg*/
	if(argc < 4)
	{
	    printf("ERROR : enter the arguements properly\n");
	    printf("USAGE : <./a.out> -e <xxx.bmp> <xxxx.txt> <xxx.bmp/optional>\n");
	    exit(1);
	}
    }
    else if(strcmp(argv[1],"-d") == 0)

    {
	printf("%d\n",argc);
	if(argc < 3)
	{
	    printf("ERROR : enter the arguements properly\n");
	    printf("USAGE : <./a.out> -d <xxx.bmp> <xxxx.txt/optional>\n");
	    exit(1);
	}
    }

    /*to check the type of operation*/
    printf("----------------------------------------TASK 1 : checking the type of the operation to be performed----------------------------\n\n");
    op_type  =  check_operation_type(argv);

    switch(op_type)
    {
	case e_encode:

	    printf("INFO:User selected Encryption Operation\n");
	    printf("DONE\n");


	    /*read and validate the arguements*/
	    printf("---------------------------------TASK 2:read and validate the arguements-------------------------------------\n\n");
	    if((read_and_validate_encode_args(argv, &encInfo)) == e_success)
	    {
		printf("INFO : The arguements are validated successfully\n");
		printf("DONE\n");
	    }
	    /*open the files*/
	    printf("------------------------------------TASK 3: Opening the related files-------------------------------------------------\n\n");
	    if(open_files(&encInfo) == e_success)
		printf("DONE\n");
	    /*calucate the sizeof both files*/

	    printf("-----------------------------------TASK 4 : check encryption is possible or not ---------------------------------------\n\n");
	    if(check_capacity(&encInfo) == e_success)
		printf("INFO : Encryption is possible\n");
	    else
	    {
		printf("ERROR : Encryption is not possible\n");
		exit(1);
	    }


	    printf("------------------------------TASK 5: start the process of Encodeing-------------------------------------------\n\n");
	    if(do_encoding(&encInfo, argv) == e_success)

		printf("##ENCODE is done successfully##\n");

	    else
		printf("##ENCODE is not done ,failed ##\n");
	    break;

	case e_decode:

	    printf("INFO : User selected Decryption Operation\n");
	    printf("DONE\n");


	    /*read and validate the arguements*/
	    printf("---------------------------------TASK 2:read and validate the arguements-------------------------------------\n\n");
	    if((read_and_validate_decode_args(argv, &decInfo)) == e_success)
	    {
		printf("INFO : The arguements are validated successfully\n");
		printf("DONE\n");
	    }


	    /*open the decode files*/
	    printf("------------------------------------TASK 3: Opening the related files-------------------------------------------------\n\n");
	    if(open_decode_files(&decInfo) == e_success)
		printf("DONE\n");

	    /*start the decoding*/
	    if(do_decoding(&decInfo) == e_success)
	    
		printf("INFO : DECODED SUCCESSFULLY\n");
	    else
		printf("ERROR : DECODE FAILED\n");



	    break;

	default:
	    printf("ERROR : please select the appropriate operation\n");
	    printf("USAGE : <./a.out> -e/-d <xxx.bmp> <xxxx.txt> <xxx.bmp/optional>\n");
	    exit(1);

    }


    return 0;
}
