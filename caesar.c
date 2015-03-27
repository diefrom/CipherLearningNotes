#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ENCODE 'e'
#define DECODE 'd'

enum {
    INTERNAL_ERROR = -1,
    PARAM_ERROR = -2,
};

enum {
    WAY = 1,
    KEY,
    PLAINTEXT
};

/* name:encode_decode
 * function:encode plaintext or decode ciphertext
 * param[0]:input:  the text need to encrypted or deciphered (not null, check it)
 * param[1]:output: the enciphered or deciphered text (not null check it)
 * param[2]:input:  the key value(0-25, not check it)
 * param[3]:input:  the way value('d' or 'e', check it)
 * return: 0 success; PARAM_ERROR(-2) parameters error
*/
int encode_decode(char const *, char *, int, char );

int main(int argc, char **argv){

    //check paramters
    if (argc != 4){
        printf("Error! please input 2 paramater.\n");
        printf("./caesar w k p\n");
        printf("w: [d | e], d means decode, e means encode\n");
        printf("k: key[1, 25]\n");
        printf("p: Plaintext, Plaintext are letters from a to z\n\n");
        exit(PARAM_ERROR);
    }

    int key = 1;
    int ret = 0;
    char *ciphertext;

    if (strlen(argv[WAY]) != 1){
        printf("Error way(len)! please input w 'd' or 'e'\n");
        exit(PARAM_ERROR);
    }

    if (argv[WAY][0] != ENCODE && argv[WAY][0] != DECODE){
        printf("Error way(content)! please input w 'd' or 'e'\n");
        return PARAM_ERROR;
    }
    
    key = atoi(argv[KEY]);
    if (key < 1 || key > 25){
        printf("Error key! please input k between 1 to 25.\n");
        exit(PARAM_ERROR);
    }

    //encode or decode
    ciphertext = (char *)malloc(strlen(argv[PLAINTEXT])+ 1);
    if (ciphertext == NULL){
        printf("Error malloc!\n");
        exit(INTERNAL_ERROR);
    }

    ret = encode_decode(argv[PLAINTEXT], ciphertext, key, argv[WAY][0]);
    if (ret == PARAM_ERROR){
        printf("Error parse!\n");
        exit(PARAM_ERROR);
    }

    //print the result
    if (argv[WAY][0] == ENCODE){
       printf("Plaintext:%s\n", argv[PLAINTEXT]);
       printf("Ciphertext:%s\n", ciphertext);
    }
    else{
       printf("Ciphertext:%s\n", argv[PLAINTEXT]);
       printf("Plaintext:%s\n", ciphertext);
    }

    free(ciphertext);
    return 0;
}

int encode_decode(char const *input_text, char *out_text, int key, char type){
    if (input_text == NULL || out_text == NULL){
        return PARAM_ERROR;
    }

    if (type == ENCODE){
        for (; *input_text != '\0'; out_text++, input_text++){

            //notice: Here needs to subtract the 'a' or 'A'.
            if (*input_text >= 'a' && *input_text <= 'z'){
                *out_text = 'a' + (*input_text - 'a' + key) % 26;
            }
            else if (*input_text >= 'A' && *input_text <= 'Z'){
                *out_text = 'A' + (*input_text - 'A' + key) % 26;
            }
            else{
                return PARAM_ERROR;
            }
        }
    }
    else if (type == DECODE){
        for (; *input_text != '\0'; out_text++, input_text++){

            //notice: Here needs to subtract the 'z' or 'Z'.
            if (*input_text >= 'a' && *input_text <= 'z'){
                *out_text = 'z' + (*input_text - 'z' - key) % 26;
            }
            else if (*input_text >= 'A' && *input_text <= 'Z'){
                *out_text = 'Z' + (*input_text - 'Z' - key) % 26;
            }
            else{
                return PARAM_ERROR;
            }
        }
    }
    else {
        return PARAM_ERROR;
    }
    out_text = '\0';
    
    return 0;
}
