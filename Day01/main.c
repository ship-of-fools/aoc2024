#include <stdio.h>
#include <stdlib.h>

int count_occur(unsigned int x, unsigned int *arr, size_t len){
    int result = 0;
    int count = 0;
    for (size_t i = 0; i < len; i++){
        if (arr[i] == x){
            count++;
        }
    }
    return count * x;
}

int compare (const void* p1, const void* p2){
    return ( *(unsigned int *)p1 - *(unsigned int *)p2);
}

int main(int argc, char** argv){

    FILE *fp;
    int c;
    char s[1024];

    const size_t list_length = 1000;

    unsigned int a[list_length];
    unsigned int b[list_length];
    unsigned int x;
    unsigned int y;

    fp = fopen("input.txt", "r");
    int i = 0;
    while (fscanf(fp, "%u %u", &x, &y) != EOF){
        a[i] = x;
        b[i] = y;
        i++;
    }
    fclose(fp);
    qsort(a, list_length, sizeof(unsigned int), compare);
    qsort(b, list_length, sizeof(unsigned int), compare);
    int sum1 = 0;
    for (size_t i = 0; i < list_length; i++){
        sum1 += abs(a[i] - b[i]);
    }
    // Part 2: Calculate similarity score
    int sum2 = 0;
    for (size_t i = 0; i < list_length; i++){
        sum2 += count_occur(a[i], b, list_length);
    }
    printf("sum1: %u\n", sum1);
    printf("sum2: %u\n", sum2);
    return 0;
}