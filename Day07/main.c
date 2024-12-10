#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')

unsigned int numbers_tens[1024][16];

unsigned int power(unsigned int x, unsigned int y){
    unsigned int result = x;
    for (unsigned int i=0; i < y-1; i++){
        result *= x;
    }
    return result;
}

unsigned char part_1(unsigned long int calc, unsigned long int value, unsigned int numbers[16], unsigned char idx, unsigned char len){
        if (idx == len){
            return (calc == value);
        }
        if (calc > value){
            return 0; // too high answer
        }
        unsigned int next_number = numbers[idx++];
        return (part_1(calc + next_number, value, numbers, idx, len) | part_1(calc * next_number, value, numbers, idx, len));
    }

unsigned char part_2(unsigned long int calc, unsigned long int value, unsigned int numbers[16], unsigned char idx, unsigned char len, const unsigned int i){
        if (idx == len){
            return (calc == value);
        }
        if (calc > value){
            return 0; // too high answer
        }
        unsigned int next_number = numbers[idx++];
        return (part_2(calc + next_number, value, numbers, idx, len, i) | part_2(calc * next_number, value, numbers, idx, len, i) |
                part_2(calc*numbers_tens[i][idx-1] + next_number, value, numbers, idx, len, i));
    }

int main(int argc, char** argv){

    int fd = open("input.txt", O_RDONLY);
    // int fd = open("example.txt", O_RDONLY);
	struct stat st;
	size_t size;
	int status = fstat(fd, &st);
	size = st.st_size;
	char *ptr = (char*) mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	size_t idx = 0;

    unsigned long int values[1024];
    memset(values, 0, sizeof(unsigned int)*1024);
    unsigned int numbers[1024][16];
    memset(numbers, 0, sizeof(unsigned int)*1024*16);
    
    unsigned int row = 0;
    unsigned char len_numbers[1024];
    memset(len_numbers, 0, sizeof(unsigned char)*1024);
	while(idx < size){
        unsigned long int answer = 0;
        while(isdigit((unsigned char)ptr[idx])){
            answer *= 10;
            answer += (unsigned char)ptr[idx++]-'0';
        }
        values[row] = answer;
        if ((unsigned char)ptr[idx]==':'){
            idx++;
        }
        if ((unsigned char)ptr[idx]==' '){
            idx++;
        }
        // idx += 2; // ": "
        unsigned char i = 0;
        while(((unsigned char)ptr[idx] != '\n' )&((unsigned char)ptr[idx] != '\0')){
            unsigned int number = 0;
            numbers_tens[row][i] = 1;
            while(isdigit((unsigned char)ptr[idx])){
                number *= 10;
                number += (unsigned char)ptr[idx++]-'0';
                numbers_tens[row][i] *= 10;
            }
            numbers[row][i++] = number;
            while((unsigned char)ptr[idx] == ' '){
                idx++;
            }
        }
        len_numbers[row] = i;
        row++;
        idx++;
    }

    unsigned long int sum1 = 0;
    unsigned long int sum2 = 0;
    unsigned int len_input = row;

    for (unsigned int i = 0; i < len_input; i++){

        unsigned int *number = &numbers[i][0];
        unsigned long int value = values[i];
        unsigned char len_number = len_numbers[i];
        char correct = part_1(number[0], value, number, 1, len_number);
        if (correct){
            // printf("%u\n", i);
            sum1 += value;
            // printf("%llu: %llu\n", i, sum1);
        }
        char correct_p2 = part_2(number[0], value, number, 1, len_number, i);
        if (correct_p2){
            // printf("%u\n", i);
            sum2 += value;
            // printf("%llu: %llu\n", i, sum1);
        }
    }
    printf("sum1: %lu\nsum2: %lu\n", sum1, sum2);

    // Fast part 1 but going to be bad for part 2
    // for (unsigned int i = 0; i < len_input; i++){
    //     unsigned int x = (power(2,(len_numbers[i]-1))-1);
    //     for (unsigned int j = 0; j <= x; j++){
    //         unsigned int xx = j;
    //         len_input = row;
    //         unsigned long int res = numbers[i][0];
    //         for (unsigned int k = 1; k < len_numbers[i]; k++){
    //             if (xx & 0x01){
    //                 res *= (numbers[i][k]);
    //             } else {
    //                 res += (numbers[i][k]);
    //             }
    //             xx = xx >> 1;
    //         }
    //         if (res == values[i]){
    //             // printf("%u\n", i);
    //             sum1 += res;
    //             // printf("%llu: %llu\n", i, sum1);
    //             break;
    //         }
    //     }
    //     // printf("%llu: %llu\n", i, sum1);
    //     len_input = row;
    // }

}