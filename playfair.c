/*********************
 * name: playfair.c
 * time: 2015.04.07
 * author: diefrom
 ********************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum {
    SUCCESS = 0,
    INTERNAL_ERROR = -1,
    INTERNAL_ERROR_MALLOC = -100,
    PARAM_ERROR = -2,
    PARAM_ERROR_KEY = -200,
};

#define DEBUG
#define ASCCII_LEN 128

char alphabet[5][5] = {0};

/*
 * name:print_error
 * function:print the error informations.
 * param[0]:input: line number
 * param[1]:input: -1:display internal error; -2:display param error.
 */
void print_error(int line, int info);

/*
 * name:create_key
 * function:create a special alphabet by the key.
 * param[0]:output: a special alphabet.
 * param[1]:input:  the key used to create a special alphabet.
 * return:0 SUCCESS, -2 PARAM_ERROR, -200 PARAM_ERROR_KEY.
 */
int create_key(char alphabet[][5], const char *key);

/* name:encode_word
 * function:encode plaintext for a word
 * notice:  ciphertext must be large.
 * param[0]:output: the ciphertext.
 * param[1]:input:  the plaintext.
 * return:
 */
int encode_word(char *ciphertext, char *plaintext);

/* name:encode_sentence
 * function:encode plaintext for a sentence.
 * notice:  ciphertext must be large.
 * param[0]:output: the ciphertext.
 * param[1]:input:  the plaintext.
 * return:
 */
int encode_sentence(char *ciphertext, char *plaintext);

/* name:get_coordinate
 * function:get the coordinate of the first letter and second letter.
 * param[0]:output: the coordinate: first.x first.y second.x second.y.
 * param[1]:input:  the first letter.
 * param[2]:input:  the second letter. 
 * return:
 */
int get_coordinate(int coordinate[4], char first, char second);

int main(int argc, char **argv)
{
    char *old_text = "It is not a problem. It is a challenge. Enjoy facing it.";
    char *new_text = NULL;
    int create_key_ret = 0;
    int encode_sentence_ret= 0;
    int i = 0;
    int j = 0;

    create_key_ret = create_key(alphabet, "boys and girls are students");
    if (create_key_ret != SUCCESS){
        return create_key_ret;
    }

    new_text = (char *)malloc(strlen(old_text) * 2 + 1);
    if (new_text == NULL){
        print_error(__LINE__-1, INTERNAL_ERROR_MALLOC);
        return INTERNAL_ERROR_MALLOC;
    }

    printf("plaintext:%s\n", old_text);
    encode_sentence_ret = encode_sentence(new_text, old_text);
    if (encode_sentence_ret != SUCCESS){
        return encode_sentence_ret;
    }
    printf("ciphertext:%s\n", new_text);

    free(new_text);

    return SUCCESS;
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
            printf("Error! Lack of space\n");
            break;
        case PARAM_ERROR:
            printf("PARAMTER Error!\n");
            break;
        case PARAM_ERROR_KEY:
            printf("Error! please input the key from 'a'-'z' or 'A'-'Z'\n");
            break;
        default:
            break;
    }
}

int create_key(char alphabet[][5], const char *key)
{
    if (key == NULL){
#ifdef DEBUG
        print_error(__LINE__-1, PARAM_ERROR);
#endif
        return PARAM_ERROR;
    }

    char ascii_flag[ASCCII_LEN] = {0};
    char a[27] = "abcdefghijklmnopqrstuvwxyz";
    unsigned int i = 0;
    int index_i = 0;
    int index_j = 0;

    for (i=0; i<strlen(key); i++){
        if (ascii_flag[key[i]] == 0){
            if (key[i]=='z' || key[i]==' ' || key[i]==',' || key[i]=='.'){
                continue;
            }
            if (key[i]<'a' || key[i]>'z'){
                print_error(__LINE__-1, PARAM_ERROR_KEY);
                return PARAM_ERROR_KEY;
            } 

            ascii_flag[key[i]] = 1;
            alphabet[index_i][index_j]= key[i];
            index_j++;

            if (index_j == 5){
                index_j = 0;
                index_i++;
            }
            if (index_i == 5){
                break;
            }
        }
        else{
            continue;
        }
    }

    for (i=0; i<strlen(a); i++){
        if (ascii_flag[a[i]] == 0){
            ascii_flag[a[i]] = 1;
            alphabet[index_i][index_j] = a[i];
            index_j++;

            if (index_j == 5){
                index_j = 0;
                index_i++;
            }
            if (index_i == 5){
                break;
            }
        }
    }
    return SUCCESS;
}

int encode_word(char *ciphertext, char *plaintext)
{
    if (ciphertext == NULL || plaintext == NULL){
        print_error(__LINE__-1, PARAM_ERROR);
        return PARAM_ERROR;
    }

    int ciphertext_len = 0;
    int coordinate[4];
    int get_coordinate_ret;
    int first_x = 0;
    int first_y = 0;
    int second_x = 0;
    int second_y = 0;
    int signle_coordinate = 0;

    //create the pair of letters
    while (*(plaintext) != '\0'){
        if (*(plaintext+1) == '\0'){
            *ciphertext = *plaintext;
            *(ciphertext+1) = 'x';
            ciphertext += 2;
            ciphertext_len += 2;
            break;
        }
        if (*plaintext == *(plaintext+1)){
            *ciphertext = *plaintext;
            *(ciphertext+1) = 'x';
            ciphertext += 2;
            ciphertext_len += 2;
            plaintext += 1;
        }
        else{
            *ciphertext = *plaintext;
            *(ciphertext+1) = *(plaintext+1);
            ciphertext += 2;
            ciphertext_len += 2;
            plaintext += 2;
        }
    }
    *(ciphertext+1) = '\0';
    ciphertext -= ciphertext_len;

    //encode
    while(*ciphertext != '\0'){
        get_coordinate_ret = 
            get_coordinate(coordinate, *ciphertext, *(ciphertext+1));
        if (get_coordinate_ret != SUCCESS){
            return get_coordinate_ret;
        }

        first_x = coordinate[0];
        first_y = coordinate[1];
        second_x = coordinate[2];
        second_y = coordinate[3];

        if (first_x == second_x){
            signle_coordinate = (first_y+1) % 5;
            *ciphertext = alphabet[first_x][signle_coordinate];
            signle_coordinate = (second_y+1) % 5;
            *(ciphertext+1) = alphabet[second_x][signle_coordinate];
            ciphertext += 2;
        }
        else if(first_y == second_y){
            signle_coordinate = (first_x+1) % 5;
            *ciphertext = alphabet[signle_coordinate][first_y];
            signle_coordinate = (second_x+1) % 5;
            *(ciphertext+1) = alphabet[signle_coordinate][second_y];
            ciphertext += 2;
        }
        else {
            *ciphertext = alphabet[first_x][second_y];
            *(ciphertext+1) = alphabet[second_x][first_y];
            ciphertext += 2;
        }
    }

    return SUCCESS;
}

int encode_sentence(char *ciphertext, char *plaintext)
{
    if (ciphertext == NULL || plaintext == NULL){
        print_error(__LINE__-1, PARAM_ERROR);
        return PARAM_ERROR;
    }

    int plaintext_len = strlen(plaintext);
    int new_plain_len = 0;
    char *new_plaintext = NULL;
    int encode_word_ret;

    new_plaintext = (char *)malloc(plaintext_len+1);
    if (new_plaintext == NULL){
        print_error(__LINE__, INTERNAL_ERROR_MALLOC);
        return INTERNAL_ERROR_MALLOC;
    }

    //encode with the whole sentece
#if 1
    while(*plaintext != '\0'){
        if (*plaintext == ' ' || *plaintext == '.' || *plaintext == ','){
            plaintext++;
            continue;
        }
        else {
            if (*plaintext>'A' && *plaintext<'Z'){
                *new_plaintext = *plaintext + 32;
            }
            else {
                *new_plaintext = *plaintext;
            }
            new_plaintext++;
            new_plain_len++;
            plaintext++;
        }
    }
    *new_plaintext = '\0';
    new_plaintext -= new_plain_len;

    encode_word_ret = encode_word(ciphertext, new_plaintext);
    if (encode_word_ret != SUCCESS){
        return encode_word_ret;
    }
#endif

    //encode with a word
#if 0
    while (*plaintext!= '\0'){
        if (*plaintext == ' ' || *plaintext == '.' || *plaintext == ','){
            *new_plaintext = '\0';
            new_plaintext -= new_plain_len;

            encode_word_ret = encode_word(ciphertext, new_plaintext);
            if (encode_word_ret != SUCCESS){
                return encode_word_ret;
            }
            ciphertext += strlen(ciphertext);
            *ciphertext = *plaintext;
            ciphertext ++;
            plaintext++;
            new_plain_len = 0;
        }
        else{
            *new_plaintext = *plaintext;
            new_plaintext++;
            plaintext++;
            new_plain_len++;
        }
    }
    //new_plaintext -= new_plain_len;
#endif

    free(new_plaintext);
    new_plaintext = NULL;
    

    return SUCCESS;
}

int get_coordinate(int coordinate[4], char first, char second)
{
    int i = 0;
    int j = 0;
    int flag = 0;

    for (i=0; i<5; i++){
        for (j=0; j<5; j++){
            if (first == alphabet[i][j]){
                coordinate[0] = i;
                coordinate[1] = j;
                flag += 1;
            }
            if (second == alphabet[i][j]){
                coordinate[2] = i;
                coordinate[3] = j;
                flag += 1;
            }
        }
    }

    if (flag != 2){
        print_error(__LINE__-1, INTERNAL_ERROR);
        return INTERNAL_ERROR;
    }

    return SUCCESS;
}

