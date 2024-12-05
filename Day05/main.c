#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')

unsigned char big_rules[9999];
char updates[256][256];

char incorrect_updates[256][256]; // for part 2
int num_incorrect_updates; // for part 2

unsigned char incorrect_ids[256];
unsigned char correct_ids[256];
unsigned char incorrect_ids_idx = 0;
unsigned char correct_ids_idx = 0;

char check_against_rules(char *update){
    unsigned char check_idx = 1;
    char c;
    while(c = update[check_idx], c != -1){
        char current_char = update[check_idx];
        char previous_char = update[check_idx-1];
        unsigned int big_char = current_char *100 + previous_char;
        char not_allowed = big_rules[big_char];
        if (not_allowed){
            return 0;
        }
        check_idx++;
    }
    return 1;
}

char check_against_rules_p2(char *update){
    unsigned char check_idx = 1;
    char c;
    while(c = update[check_idx], c != -1){
loop:
        char current_char = update[check_idx];
        char previous_char = update[check_idx-1];
        unsigned int big_char = current_char *100 + previous_char;
        char not_allowed = big_rules[big_char];
        if (not_allowed){
            char swap = update[check_idx];
            update[check_idx] = update[check_idx-1];
            update[check_idx-1] = swap;
            check_idx = 1;
            goto loop;
        }
        check_idx++;
    }
    return 1;
}


int main(int argc, char** argv){
    FILE *fp;
    fp = fopen("input.txt", "r");
    
    memset(updates, -1, 256*256);
    memset(big_rules, 0, 99*99);

    // Read data
    char c = 0;
    char cc = 0;
    char n = 0;
    char n2 = 0;
    while(1){
        c = fgetc(fp);
        if (c == '\n')break;
        cc = fgetc(fp);
        n = (c-'0')*10 + (cc-'0');
        c = fgetc(fp);
        
        c = fgetc(fp);
        cc = fgetc(fp);
        n2 = (c-'0')*10 + (cc-'0');
        unsigned int idx = n*100+n2;
        big_rules[idx]=1;
        c = fgetc(fp); // \n 
    }
    int i = 0;
    int j = 0;
    while(1){
        c = fgetc(fp);
        if (c == '\n' || c == EOF)break;
        cc = fgetc(fp);
        n = (c-'0')*10 + (cc-'0');
        updates[i][j++] = n;
        c = fgetc(fp); // ',' or '\n'
        if (c == '\n'){
            i++;
            j = 0;
        }
    }

    int num_updates = i+1;
    
    for(int i=0; i<num_updates; i++){
        char passed = check_against_rules(&updates[i][0]);
        if (passed){
            correct_ids[correct_ids_idx++] = i;
        } else {
            incorrect_ids[incorrect_ids_idx++] = i;
            memcpy(&incorrect_updates[num_incorrect_updates++], &updates[i], 256);
        }
    }
    incorrect_ids[incorrect_ids_idx] = -1;
    num_incorrect_updates = incorrect_ids_idx-1;

    int sum1 = 0;
    // Add up all middle numbers of correct updates
    for (int i = 0; i < correct_ids_idx-1; i++){
        for (int len = 0; len < 255; len++){ // Calculate size of this update 
            if (updates[correct_ids[i]][len] == -1){
                sum1 += updates[correct_ids[i]][(len-1)/2];
                break;
            }
        }
    }

    //Part 2
    for (int i = 0; i <= num_incorrect_updates; i++){
        int len = 0;
        for (len = 0; len < 255; len++){ // Calculate size of this update
            if (incorrect_updates[i][len] == -1)break;
        }
        check_against_rules_p2(&incorrect_updates[i][0]);
    }
    int sum2 = 0;
    for (int i = 0; i <= num_incorrect_updates; i++){
        for (int len = 0; len < 255; len++){ // Calculate size of this update
            if (incorrect_updates[i][len] == -1){
                sum2 += incorrect_updates[i][(len-1)/2];
                break;
            }
        }
    }
    printf("sum1: %d\nsum2: %d\n", sum1, sum2);
}