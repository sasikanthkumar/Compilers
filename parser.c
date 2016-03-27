#include <stdio.h>
#include <stdlib.h>

int main(){
    char input[200];
    system("gcc -o lex Scanner.c");
    system("./lex");
    
    FILE* fin = NULL;
    fin = fopen("output.txt","r");
    
    char temp;
    temp = fgetc(fin);
    int i = 0;
    while(temp != EOF)
    {
        if (temp != '\n')
        {
            input[i] = temp;
            i++;
            temp = fgetc(fin);
        }else{
            input[i] = '\0';
            
            printf("%s\n",input);
            i = 0;
            temp = fgetc(fin);
        }
    }
    input[i] = '\0';
    printf("%s\n",input );
    fclose(fin);
    return 0;
}
