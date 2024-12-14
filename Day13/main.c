#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <math.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')
// #define map_w 10
#define map_w 140

typedef struct
{
    uint8_t row;
    uint8_t col;
} cord;

int main(int argc, char** argv){
    // int fd = open("example.txt", O_RDONLY);
    int fd = open("input.txt", O_RDONLY);

	struct stat st;
	size_t size;
	int status = fstat(fd, &st);
	size = st.st_size;
	char *ptr = (char*) mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	size_t idx = 0;

    uint64_t sum1 = 0;
    uint64_t sum2 = 0;

    uint32_t count = 0;

	while(idx < size){

        while ((char)ptr[idx] != '\n' & (char)ptr[idx] != 0){
            double x00 = (((char)ptr[idx+12]-'0')*10)+((char)ptr[idx+13]-'0');
            double x10 = (((char)ptr[idx+18]-'0')*10)+((char)ptr[idx+19]-'0');
            double x01 = (((char)ptr[idx+33]-'0')*10)+((char)ptr[idx+34]-'0');
            double x11 = (((char)ptr[idx+39]-'0')*10)+((char)ptr[idx+40]-'0');
            idx += 51;
            double y00 = 0;
            char dbg = (char)ptr[idx];
            while (isdigit((char)ptr[idx])){
                y00 = y00 * 10 + ((char)ptr[idx++]-'0');
            }
            idx += 4;
            double y10 = 0;
            while (isdigit((char)ptr[idx])){
                y10 = y10 * 10 + ((char)ptr[idx++]-'0');
            }
            double p2_y00 = y00 + 10000000000000;
            double p2_y10 = y10 + 10000000000000;

            // Lets do some math!
            // Matrix invert:
            double det = (1.0 / (x00*x11 - x01*x10));
            double i00 = x11 * det;
            double i01 = -x01 * det;
            double i10 = -x10 * det;
            double i11 = x00 * det;
            // Matmul
            double a00 = (y00 * i00) + (y10 * i01);
            double a10 = (y00 * i10) + (y10 * i11);

            // printf("| %.2f  %.2f | = | %.2f | = | %.2f |\n| %.2f  %.2f |   | %.2f |   | %.2f |\n\n", x00, x01, y00, a00, x10, x11, y10, a10);
            if (fabsf(roundf(a00)- a00) <= 0.0001f){
                if (fabsf(roundf(a10)- a10) <= 0.0001f){
                sum1 += (roundf(a00) * 3 + roundf(a10));
                }
            }

            double p2_a00 = (p2_y00 * i00) + (p2_y10 * i01);
            double p2_a10 = (p2_y00 * i10) + (p2_y10 * i11);

            if (fabsf(round(p2_a00)- p2_a00) <= 0.0001){
                if (fabsf(round(p2_a10)- p2_a10) <= 0.0001f){
                    sum2 += (round(p2_a00) * 3 + round(p2_a10));
                }
            }
            idx += 2;
            count++;
        }

    }
    printf("sum1: %lu\nsum2: %lu\n", sum1, sum2);

}