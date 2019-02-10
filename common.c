#include<stdio.h>
#include<string.h>
#include"types.h"

/*Check the operation type
 *Input : argv[1]
 *Output : OperationType
 *Description : check the type of operation to be performed
*/
 
OperationType check_operation_type(char *argv[])
{
     if(strcmp(argv[1] , "-e") == 0)  //encode
     {
         printf("INFO : User selected an Encryption operation\n");
         return e_encode;
     }

    else if(strcmp(argv[1],"-d") == 0) //decode
    {
        printf("INFO : User selected Decryption operation\n");
       return e_decode;
     }
 
     else
     {
         printf("ERROR : ENTER the correct option\n");
         return e_unsupported;
     }
}

