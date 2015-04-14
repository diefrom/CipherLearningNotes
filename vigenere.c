/*******************************
 * name: vigenere.c
 * time: 2015.01.14
 * author: diefrom_hdu@126.com
 *******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG
enum {
    SUCCESS = 0,
    INTERNAL_ERROR = -1,
    INTERNAL_ERROR_MALLOC = -100,
    PARAM_ERROR = -2,
    PARAM_ERROR_KEY = -200,
    PARAM_ERROR_KEY_LEN = -201,
    PARAM_ERROR_TEXT= -202,
    PARAM_ERROR_HELP = - 203,
};

/* name:free_set_null
 * function:free pointer and set it null.
 * param[0]:output: the pointer.
 * param[0]:input:  the pointer.
 */
void free_set_null(char **pointer);

/* name:print_error
 * function:print the error informations.
 * param[0]:input: line number
 * param[1]:input: -1:display internal error; -2:display param error.
 */
void print_error(int line, int info);

/* name:to_lowercase
 * function:converts the string to lowercase.
 * param[0]:output: the lowercase string.
 * param[1]:input:  the old string.
 * return: 0 success; not 0 faild.
 */
int to_lowercase(char *new_text, char *old_text);

/* name:encode_decode
 * function:encode plaintext or decode ciphertext
 * param[0]:output: the ciphertext
 * param[1]:input:  the plaintext 
 * param[2]:input:  the key value
 * param[3]:input:  the type of encode or decode
 * return: 0 success; PARAM_ERROR(-2) parameters error
 */
int encode_decode(char *new_text, char *old_text, char *key, char *type);

int main(int argc, char **argv){

    //check paramters
    if (argc != 4){
        print_error(__LINE__-1, PARAM_ERROR_HELP);
        return PARAM_ERROR_HELP;
    }

    char *new_text = NULL;
    char *old_text = argv[1];
    char *key = argv[2];
    char *type = argv[3];
    int ret = 0;


    //encode or decode
    new_text = (char *)malloc(strlen(old_text)+ 1);
    if (new_text == NULL){
        print_error(__LINE__-1, INTERNAL_ERROR);
        return INTERNAL_ERROR;
    }

    ret = encode_decode(new_text, old_text, key, type);
    if (ret == PARAM_ERROR){
        free_set_null(&new_text);
        return PARAM_ERROR;
    }
    printf("old_text: %s\n", old_text);
    printf("key:      %s\n", key);
    printf("new_text: %s\n", new_text);

    free_set_null(&new_text);

    return 0;
}

void free_set_null(char **pointer)
{
    free(*pointer);
    if (*pointer != NULL){
        *pointer = NULL;
    }
}

/*undone*/
void print_error(int line, int info)
{   
#ifdef DEBUG
    printf("LINE: %d | ", line);
#endif
    switch(info){
        case INTERNAL_ERROR:
            printf("INTERNAL Error!\n");
            break;
        case INTERNAL_ERROR_MALLOC:
            printf("Error! Lack of space.\n");
            break;
        case PARAM_ERROR:
            printf("PARAMTER Error!\n");
            break;
        case PARAM_ERROR_KEY:
            printf("Error! please input the key from 'a'-'z' or 'A'-'Z'.\n");
            break;
        case PARAM_ERROR_KEY_LEN:
            printf("Error! The length of key and the plaintext is not equal.\n");
            break;
        case PARAM_ERROR_TEXT:
            printf("Error! please input the text from 'a'-'z' or 'A'-'Z'\n");
        case PARAM_ERROR_HELP:
            printf("HELP:./vigener plaintext key type(e/d)\n");
        default:
            break;
    }
}

int to_lowercase(char *new_text, char *old_text)
{
    if (new_text==NULL || old_text==NULL){
        print_error(__LINE__-1, PARAM_ERROR);
        return PARAM_ERROR;
    }

    while (*old_text != '\0'){
        if (*old_text>='a' && *old_text<='z'){
            *new_text = *old_text;
        }
        else if (*old_text>='A' && *old_text<='Z'){
            *new_text = *old_text + 32;
        }
        else {
            print_error(__LINE__, PARAM_ERROR_TEXT);
            return PARAM_ERROR;
        }
        old_text++;
        new_text++;
    }

    return SUCCESS;
}

int encode_decode(char *new_text, char *old_text, char *key, char *type){
    if (new_text==NULL || old_text==NULL || key==NULL){
        print_error(__LINE__-1, PARAM_ERROR);
        return PARAM_ERROR;
    }

    if (strlen(type) != 1){
        print_error(__LINE__-1, PARAM_ERROR);
        return PARAM_ERROR;
    }
    if (*type != 'e' && *type != 'd'){
        print_error(__LINE__-1, PARAM_ERROR);
            return PARAM_ERROR;
    }

    size_t old_len = strlen(old_text);
    int i_ret = 0;
    char *tmp_old_text = NULL;
    char *tmp_key = NULL;

    if ((old_len) != strlen(key)){
        print_error(__LINE__-1, PARAM_ERROR_KEY_LEN);
        return PARAM_ERROR_KEY_LEN;
    }

    tmp_old_text = (char *)malloc(old_len + 1);
    tmp_key = (char *)malloc(old_len + 1);
    if (tmp_old_text == NULL || tmp_key == NULL){
        print_error(__LINE__-1, INTERNAL_ERROR_MALLOC);
        return INTERNAL_ERROR_MALLOC;
    }

    //to lowercase
    i_ret = to_lowercase(tmp_old_text, old_text);
    if (i_ret != SUCCESS){
        free_set_null(&tmp_old_text);
        return i_ret;
    }

    i_ret = to_lowercase(tmp_key, key);
    if (i_ret != SUCCESS){
        free_set_null(&tmp_key);
        return i_ret;
    }

    //encode
    while (*tmp_old_text != '\0'){
        if (*type == 'e'){
            *new_text = 'a' + (*tmp_old_text- 'a' + (*tmp_key-'a')) % 26;
        }
        else {
            *new_text = 'z' + (*tmp_old_text- 'z' - (*tmp_key-'a')) % 26;
        }
        tmp_old_text++;
        tmp_key++;
        new_text++;
    }

    *new_text = '\0';

    return SUCCESS;
}
