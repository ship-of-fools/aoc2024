#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')

char rules[256][256];
unsigned char rules_len[256];

char updates[256][256];

char incorrect_updates[256][256]; // for part 2
int num_incorrect_updates; // for part 2

unsigned char incorrect_ids[256];
unsigned char correct_ids[256];
unsigned char incorrect_ids_idx = 0;
unsigned char correct_ids_idx = 0;

int compare(const void *p, const void *q){
    char x = *(char *)p;
    char y = *(char *)q;
    int i = 0;
    while(1){
        char not_allowed = rules[y][i++];
        if (not_allowed == -1){
            return 0;
        }
        if (not_allowed == x){
            return 1;
        }
    }
}

char check_against_rules(char *update){
    char check[256];
    char check_idx = 1;
    char c;
    char i;
    while(c = update[check_idx], c != -1){
        char current_char = update[check_idx];
        char previous_char = update[check_idx-1];
        while(1){
            char not_allowed = rules[current_char][i++];
            if (not_allowed == -1){
                i = 0;
                break;
            }
            if (not_allowed == previous_char){
                return 0;
            }
        }
        check_idx++;
    }
    return 1;
}

char check_against_rules_p2(char *update){
    char check[256];
    char check_idx = 1;
    char c;
    char i;
    while(c = update[check_idx], c != -1){
loop:
        char current_char = update[check_idx];
        char previous_char = update[check_idx-1];
        while(1){
            char not_allowed = rules[current_char][i++];
            if (not_allowed == -1){
                i = 0;
                break;
            }
            if (not_allowed == previous_char){
                char swap = update[check_idx];
                update[check_idx] = update[check_idx-1];
                update[check_idx-1] = swap;
                check_idx = 1;
                goto loop;
            }
        }
        check_idx++;
    }
    return 1;
}


int main(int argc, char** argv){
    FILE *fp;
    fp = fopen("input.txt", "r");
    const unsigned char rows = 140;
    const unsigned char columns = 140; 
    
    memset(rules, -1, 256*256);
    memset(rules_len, 0, 256);
    memset(updates, -1, 256*256);

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
        rules[n][rules_len[n]++] = n2; 
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
    char check[256];
    char check_idx = 1;
    
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
        // qsort(&incorrect_updates[i][0], len, sizeof(char), compare);
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
    printf("sum1: %d\n", sum1);
    printf("sum2: %d\n", sum2);
}