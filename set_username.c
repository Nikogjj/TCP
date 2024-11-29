#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void* set_username(char* pseudo){

    printf("Choisissez votre pseudo 15 caracteres max: ");
    while (1)
    {
        fgets(pseudo,16,stdin);
        
        if (strcmp(pseudo,"\n")==0)
        {
            printf("Entrez un pseudo valide\n");
            continue;
        }
        int check_space=0;
        for (int i = 0; i < strlen(pseudo); i++)
        {
            if (pseudo[i]==' ')
            {
                check_space++;
            }            
        }
        if (check_space>0)
        {
            printf("Votre pseudo ne peut pas contenir d'espace. Entrez un pseudo valide\n");
            continue;
        }
        break;
    }
    pseudo[strlen(pseudo)-1]=0;

    return NULL ;
}