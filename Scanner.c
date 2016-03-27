#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>

// file pointers
FILE* in_file;
FILE* out_file;

char input[200];
char* ch;
char* start = NULL;
char* current = NULL;

int state = 0;
int single_token_flag = 0;
int keyword_token_flag = 0;
int id_int_float_flag = 0;


enum{
    TKN_LB,TKN_RB,TKN_PLUS,TKN_MINUS,TKN_MULTI,TKN_EQEQ,TKN_AND, TKN_DEFVAR,TKN_DECF,TKN_FORMAT,TKN_IF,TKN_INCF,TKN_LOOP,TKN_MAX,TKN_MIN,TKN_MOD,TKN_NOT,TKN_NIL,TKN_OR,TKN_PROGN,TKN_REM,TKN_RET,TKN_SETQ,TKN_TRUE,TKN_WHEN,TKN_GT,TKN_GTEQ,TKN_LT,TKN_LTEQ,TKN_DIV,TKN_NTEQ,TKN_INT,TKN_FLOAT,TKN_ID
};


void retract(){
    current--;
}

// file initializations
void init_file(){
    
    in_file = fopen("input.lisp","r");
    out_file = fopen("output.txt","w");
    
    if (in_file == NULL || out_file == NULL) {
        printf("file initialization error\n");
    }
}


int check_for_id(char* ch){
    if (*ch == '(' || *ch == ')' || *ch == ' ' || *ch == '+' || *ch == '-' || *ch == '/' || *ch == '*' || *ch == EOF || *ch == '>' || *ch == '<' || *ch == '=') {
        return 1;
    }else{
        return 0;
    }
}


void print_single_token(int token_case){
    switch (token_case) {
        case 1:{fprintf(out_file,"TKN_LB       %d      \n",TKN_LB);break;}
        case 2:{fprintf(out_file,"TKN_RB       %d      \n",TKN_RB);break;}
        case 3:{fprintf(out_file,"TKN_PLUS     %d      \n",TKN_PLUS);break;}
        case 4:{fprintf(out_file,"TKN_MINUS    %d      \n",TKN_MINUS);break;}
        case 5:{fprintf(out_file,"TKN_MULTI    %d      \n",TKN_MULTI);break;}
        case 6:{fprintf(out_file,"TKN_EQEQ     %d      \n",TKN_EQEQ);break;}
        case 7:{fprintf(out_file,"TKN_GTEQ     %d      \n",TKN_GTEQ);break;}
        case 8:{fprintf(out_file,"TKN_GT     %d      \n",TKN_GT);break;}
        case 9:{fprintf(out_file,"TKN_LTEQ     %d      \n",TKN_LTEQ);break;}
        case 10:{fprintf(out_file,"TKN_LT     %d      \n",TKN_LT);break;}
        case 11:{fprintf(out_file,"TKN_NTEQ     %d      \n",TKN_NTEQ);break;}
        case 12:{fprintf(out_file,"TKN_DIV     %d      \n",TKN_DIV);break;}

    }
    single_token_flag = 1;
}


void print_keyword_token(int token_case){
    switch (token_case) {
        case 1:{fprintf(out_file,"TKN_AND           %d      \n",TKN_AND);break;}
        case 2:{fprintf(out_file,"TKN_DEFVAR        %d      \n",TKN_DEFVAR);break;}
        case 3:{fprintf(out_file,"TKN_DECF          %d      \n",TKN_DECF);break;}
        case 4:{fprintf(out_file,"TKN_FORMAT        %d      \n",TKN_FORMAT);break;}
        case 5:{fprintf(out_file,"TKN_IF            %d      \n",TKN_IF);break;}
        case 6:{fprintf(out_file,"TKN_INCF          %d      \n",TKN_INCF);break;}
        case 7:{fprintf(out_file,"TKN_LOOP          %d      \n",TKN_LOOP);break;}
        case 8:{fprintf(out_file,"TKN_MAX           %d      \n",TKN_MAX);break;}
        case 9:{fprintf(out_file,"TKN_MIN           %d      \n",TKN_MIN);break;}
        case 10:{fprintf(out_file,"TKN_MOD           %d      \n",TKN_MOD);break;}
        case 11:{fprintf(out_file,"TKN_NOT           %d      \n",TKN_NOT);break;}
        case 12:{fprintf(out_file,"TKN_NIL           %d      \n",TKN_NIL);break;}
        case 13:{fprintf(out_file,"TKN_OR           %d      \n",TKN_OR);break;}
        case 14:{fprintf(out_file,"TKN_PROGN           %d      \n",TKN_PROGN);break;}
        case 15:{fprintf(out_file,"TKN_REM           %d      \n",TKN_REM);break;}
        case 16:{fprintf(out_file,"TKN_RET           %d      \n",TKN_RET);break;}
        case 17:{fprintf(out_file,"TKN_SETQ           %d      \n",TKN_SETQ);break;}
        case 18:{fprintf(out_file,"TKN_TRUE           %d      \n",TKN_TRUE);break;}
        case 19:{fprintf(out_file,"TKN_WHEN           %d      \n",TKN_WHEN);break;}


    }
    keyword_token_flag = 1;
}

int check_alphabet(char* ch){
    if ((*ch>=97 && *ch<=122) ||(*ch >=65 && *ch<=90)) {
        return 1;
    }else{
        return 0;
    }
    return 0;
}

void print_int_token(){
    fprintf(out_file,"TKN_INT           %d         \n",TKN_INT);
    id_int_float_flag = 1;
}

void print_float_token(){
    fprintf(out_file,"TKN_FLOAT           %d         \n",TKN_FLOAT);
    id_int_float_flag = 1;
}

void print_id_token(){
    fprintf(out_file,"TKN_ID             %d        \n",TKN_ID);
    id_int_float_flag = 1;
}

int check_numeric(char* ch){
    if (*ch >=48 && *ch<=57) {
        return 1;
    }else{
        return 0;
    }
    return 0;
}

int get_next_state(char* ch, int state){
    int next_state = 0;
    switch (state) {
        case 0:{
            switch (*ch) {
                    
                // single
                // for all these single cases single_token flag will be set to 1
                case '(':{ print_single_token(1); next_state = 0; start = current; break;}
                case ')':{ print_single_token(2); next_state = 0; start = current; break;}
                case '+':{ print_single_token(3); next_state = 0; start = current; break;}
                case '-':{ print_single_token(4); next_state = 0; start = current; break;}
                case '*':{ print_single_token(5); next_state = 0; start = current; break;}
                case '=':{ print_single_token(6); next_state = 0; start = current; break;}
                //single
                
                // keywords
                case 'a':{ next_state = 1; break;}
                case 'd':{ next_state = 2; break;}
                case 'f':{ next_state = 3; break;}
                case 'i':{ next_state = 4; break;}
                case 'l':{ next_state = 5; break;}
                case 'm':{ next_state = 6; break;}
                case 'n':{ next_state = 7; break;}
                case 'o':{ next_state = 8; break;}
                case 'p':{ next_state = 9; break;}
                case 'r':{ next_state = 10; break;}
                case 's':{ next_state = 11; break;}
                case 't':{ next_state = 12; break;}
                case 'w':{ next_state = 13; break;}
                // keywords
                
                // space case
                case ' ':{ next_state = 69; break;}
                // space case
                    
                    
                case '>':{ next_state = 62; break;}
                case '<':{ next_state = 63; break;}
                case '/':{ next_state = 64; break;}
                case '"':{ next_state = 65; break;}
                default:{
                    if (check_alphabet(ch)) {
                        next_state = 998;
                    }else if(check_numeric(ch)){
                        next_state = 66;
                    }else{
                        fprintf(out_file,"TKN_ERROR         %d", -1);
                        while (*current != EOF && *current != 0 && *current != '\0') {
                            fprintf(out_file,"%c",*current);
                            current++;
                        }
                        fprintf(out_file,"\n");
                        retract();
                        break;
                    }
                    break;
                }
            }
            break;
        }
            
        case 69:{
            if (*ch == ' ') {
                next_state = 69;
                break;
            }else{
                retract();
                next_state = 0;
                break;
            }
            break;
        }
        case 999:{
            retract();
            print_id_token();
            next_state = 0;
            break;
        }
        case 998:{
            if (check_for_id(ch)) {
                retract();
                print_id_token();
                next_state = 0;
                break;
            }else if(*ch == '_' || (*ch >= '0' && *ch <= '9') || check_alphabet(ch)){
                next_state = 998;
                break;
            }
            break;
        }
        case 66:{
            if (*ch == '.') {
                next_state = 67;
                break;
            }else if(check_numeric(ch)){
                next_state = 66;
                break;
            }else{
                retract();
                next_state = 68;
                break;
            }
        }
        case 67:{
            if (!check_numeric(ch)) {
                retract();
                print_float_token();
                next_state = 0;
                break;
            }else{
                next_state = 67;
                break;
            }
        }
        case 68:{
            retract();
            print_int_token();
            next_state = 0;
            break;
        }
        case 65:{
            if (*(ch-1) == '"') {
                fprintf(out_file,"TKN_STR       ");
            }
            if (*ch == '"') {
                fprintf(out_file,"\n");
                next_state = 0;
                break;
            }else{
                fprintf(out_file,"%c",*ch);
                next_state = 65;
                break;
            }
        }
        case 62:{
            if (*ch == '=') {
                print_single_token(7);   // GTEQ
                next_state = 0;
                break;
            }else{
                print_single_token(8);   // GT
                retract();
                next_state = 0;
                break;
            }
            break;
        }
        case 63:{
            if (*ch == '=') {
                print_single_token(9);  // LTEQ
                next_state = 0;
                break;
            }else{
                print_single_token(10);   // LT
                retract();
                next_state = 0;
                break;
            }
            break;
        }
        case 64:{
            if (*ch == '=') {
                print_single_token(11);  // NOTEQ
                next_state = 0;
                break;
            }else{
                print_single_token(12);   // DIV
                retract();
                next_state = 0;
                break;
            }
            break;
        }
        case 1:{
            if (*ch == 'n') {
                next_state  = 14;
                break;
            }else if (check_for_id(ch)) {
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 2:{
            if (*ch == 'e') {
                next_state = 16;
                break;
            }else if (check_for_id(ch)) {
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 3:{
            if (*ch == 'o') {
                next_state  = 23;
                break;
            }else if (check_for_id(ch)) {
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 4:{
            if (*ch == 'f') {
                next_state = 28;
                break;
            }else if(*ch == 'n'){
                next_state = 29;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 5:{
            if (*ch == 'o') {
                next_state  = 14;
                break;
            }else if (check_for_id(ch)) {
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 6:{
            if (*ch == 'a') {
                next_state = 35;
                break;
            }else if(*ch == 'i'){
                next_state = 37;
                break;
            }else if(*ch == 'o'){
                next_state = 39;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 7:{
            if (*ch == 'o') {
                next_state = 41;
                break;
            }else if(*ch == 'i'){
                next_state = 43;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
 
        }
        case 8:{
            if (*ch == 'r') {
                next_state = 45;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
            
        }
        case 9:{
            if (*ch == 'r') {
                next_state = 46;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
            
        }
        case 10:{
            if (*ch == 'e') {
                next_state = 50;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
            
        }
        case 11:{
            if (*ch == 'e') {
                next_state = 56;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
            
        }
        case 12:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(18);  // true keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 13:{
            if (*ch == 'h') {
                next_state = 59;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
            
        }
        case 14:{
            if (*ch == 'd') {
                next_state = 15;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 15:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(1);  // and keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 16:{
            if (*ch == 'f') {
                next_state = 17;
                break;
            }else if(*ch == 'c'){
                next_state = 18;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 17:{
            if (*ch == 'v') {
                next_state = 19;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 19:{
            if (*ch == 'a') {
                next_state = 20;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 20:{
            if (*ch == 'r') {
                next_state = 21;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 21:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(2);  // defvar keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 18:{
            if (*ch == 'f') {
                next_state = 22;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 22:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(3);  // decf keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 23:{
            if (*ch == 'r') {
                next_state  = 24;
                break;
            }else if (check_for_id(ch)) {
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 24:{
            if (*ch == 'm') {
                next_state  = 25;
                break;
            }else if (check_for_id(ch)) {
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 25:{
            if (*ch == 'a') {
                next_state  = 26;
                break;
            }else if (check_for_id(ch)) {
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 26:{
            if (*ch == 't') {
                next_state  = 27;
                break;
            }else if (check_for_id(ch)) {
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 27:{
            if (check_for_id(ch)) {
                retract();
                //printf("Case 27\n");
                print_keyword_token(4); // format keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 28:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(5); // if keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 29:{
            if (*ch == 'c') {
                next_state  = 30;
                break;
            }else if (check_for_id(ch)) {
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 30:{
            if (*ch == 'f') {
                next_state  = 31;
                break;
            }else if (check_for_id(ch)) {
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 31:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(6); // incf keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 32:{
            if (*ch == 'o') {
                next_state  = 33;
                break;
            }else if (check_for_id(ch)) {
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 33:{
            if (*ch == 'p') {
                next_state  = 34;
                break;
            }else if (check_for_id(ch)) {
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 34:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(7); // loop keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 35:{
            if (*ch == 'x') {
                next_state = 36;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;

        }
        case 36:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(8); // max keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 37:{
            if (*ch == 'n') {
                next_state = 38;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 38:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(9); // min keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 39:{
            if (*ch == 'd') {
                next_state = 40;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 40:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(10); // mod keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 41:{
            if (*ch == 't') {
                next_state = 42;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 42:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(11); // not keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 43:{
            if (*ch == 'l') {
                next_state = 44;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 44:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(12); // nil keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 45:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(13); // or keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 46:{
            if (*ch == 'o') {
                next_state = 47;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
            
        }
        case 47:{
            if (*ch == 'g') {
                next_state = 48;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
            
        }
        case 48:{
            if (*ch == 'n') {
                next_state = 49;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
            
        }
        case 49:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(14); // progn keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 50:{
            if (*ch == 'm') {
                next_state = 51;
                break;
            }else if(*ch == 't'){
                next_state = 52;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 51:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(15); // rem keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 52:{
            if (*ch == 'u') {
                next_state = 53;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;

        }
        case 53:{
            if (*ch == 'r') {
                next_state = 54;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
            
        }
        case 54:{
            if (*ch == 'n') {
                next_state = 55;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
            
        }
        case 55:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(16); // return keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 56:{
            if (*ch == 't') {
                next_state = 57;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
            
        }
        case 57:{
            if (*ch == 'q') {
                next_state = 58;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
            
        }
        case 58:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(17); // setq keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 59:{
            if (*ch == 'e') {
                next_state = 60;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
            
        }
        case 60:{
            if (*ch == 'n') {
                next_state = 61;
                break;
            }else if(check_for_id(ch)){
                next_state = 999;
                retract();
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
        case 61:{
            if (check_for_id(ch)) {
                retract();
                print_keyword_token(19); // when keyword
                next_state = 0;
                break;
            }else{
                next_state = 998;
                break;
            }
            break;
        }
    }
    return next_state;
}

int main(){
    init_file();
    
    
    
    /**
    while ((fscanf(in_file,"%s",input)) != EOF) {
        ch = input;
        start = input;
        current = input;
        while (*current != '\0' || state != 0) {
            
            state = get_next_state(current,state);
            
            current++;
            
            // should implement flags
            if (id_int_float_flag == 1 || keyword_token_flag == 1 || single_token_flag == 1) {
                start = current;
                id_int_float_flag = 0;
                keyword_token_flag = 0;
                single_token_flag = 0;
            }
        }
    }
    **/
    
    char temp;
    temp = fgetc(in_file);
    int i = 0;
    while (temp != EOF) {
        if (temp != '\n') {
            input[i] = temp;
            i++;
            temp = fgetc(in_file);
        }else{
            input[i] = '\0';
            // our subroutine
            ch = input;
            start = input;
            current = input;
            while (*current != '\0' || state != 0) {
                state = get_next_state(current,state);
                current++;
                if (id_int_float_flag == 1 || keyword_token_flag == 1 || single_token_flag == 1) {
                    start = current;
                    id_int_float_flag = 0;
                    keyword_token_flag = 0;
                    single_token_flag = 0;
                }            }
            // end subroutine
            i = 0;
            temp = fgetc(in_file);
        }
    }
    input[i] = '\0';
    // our subroutine
    ch = input;
    start = input;
    current = input;
    while (*current != '\0' || state != 0) {
        //printf("before current = %d state = %d\n",*current,state);
        state = get_next_state(current,state);
        //printf("After current = %d state = %d\n",*current,state);
        current++;
        if (id_int_float_flag == 1 || keyword_token_flag == 1 || single_token_flag == 1) {
            start = current;
            id_int_float_flag = 0;
            keyword_token_flag = 0;
            single_token_flag = 0;
        }            }
    // end subroutine
    
    fclose(in_file);
    fclose(out_file);
    return 1;
}