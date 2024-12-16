#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include "uthash.h"

#define isdigit(c) ((c) >= '0' && (c) <= '9')
#define ismarker(c) ((c) == '.' || (c) == '#' || (c) == '^')

struct stone_single{
    uint64_t number;
    uint8_t steps_remaining;
    uint64_t result;
    UT_hash_handle hh;
} stone_single;

struct lookup_key{
    uint64_t number;
    uint8_t steps_remaining;
} lookup_key;

struct stone_single *stones_cache = NULL;

void add_stone(uint64_t number, uint8_t steps_remaining, uint64_t result){
    struct stone_single *s;
    s = malloc(sizeof *s);
    s->number = number;
    s->steps_remaining = steps_remaining;
    s->result = result;
    // struct stone_single *check;
    // struct lookup_key compound_key={.number=number,.steps_remaining=steps_remaining};
    // HASH_FIND(hh, stones_cache, &compound_key, 9, check);
    // if (check != NULL){
    //     printf("we have a problem!\n");
    // }
    HASH_ADD(hh, stones_cache, number, 9, s);
}

struct stone_single *find_stone(uint64_t number, uint8_t steps_remaining){
    struct lookup_key compound_key={.number=number,.steps_remaining=steps_remaining};
    struct stone_single *s;
    HASH_FIND(hh, stones_cache, &compound_key, 9, s);
    return s;
}

uint64_t num_digits_mod(uint64_t n){
    if (n<10) return 0;
    if (n<100) return 10;
    if (n<1000) return 0;
    if (n<10000) return 100;
    if (n<100000) return 0;
    if (n<1000000) return 1000;
    if (n<10000000) return 0;
    if (n<100000000) return 10000;
    if (n<1000000000) return 0;
    if (n<10000000000) return 100000;
    if (n<100000000000) return 0;
    if (n<1000000000000) return 1000000;
    if (n<10000000000000) return 0;
    if (n<100000000000000) return 10000000;
    printf("\n\nwe dont handle more than these digits!!!\n\n");
    return 0;
}

uint64_t count_stones_with_cache(uint64_t stone, uint8_t steps_remaining){

    struct stone_single *s;
    s = find_stone(stone, steps_remaining);
    if (s != NULL){
        return s->result;
    }

    if (steps_remaining==0){
        return 1;
    }
    if (stone == 0){
        uint64_t result = count_stones_with_cache(1, steps_remaining-1);

        add_stone(stone, steps_remaining, result);
        return result;
    }
    if (num_digits_mod(stone)){
        uint64_t right_split_stone = stone % num_digits_mod(stone);
        uint64_t left_split_stone = (stone - right_split_stone) / num_digits_mod(stone);
        uint64_t result = count_stones_with_cache(left_split_stone, steps_remaining-1) + count_stones_with_cache(right_split_stone, steps_remaining-1);
        add_stone(stone, steps_remaining, result);
        return result;
    } 
    uint64_t result = count_stones_with_cache(stone*2024, steps_remaining-1);

    add_stone(stone, steps_remaining, result);
    return result;
}

uint64_t count_stones(uint64_t stone, uint8_t steps_remaining){
    if (steps_remaining==0){
        return 1;
    }
    if (stone == 0){

        return count_stones(1, steps_remaining-1);
    }
    if (num_digits_mod(stone)){
        uint64_t split_stone = stone % num_digits_mod(stone);
        stone = (stone - split_stone) / num_digits_mod(stone);
        return count_stones(stone, steps_remaining-1) + count_stones(split_stone, steps_remaining-1);
    } 
    return count_stones(stone*2024, steps_remaining-1);
}

int main(int argc, char** argv){
    
    int fd = open("input.txt", O_RDONLY);

	struct stat st;
	size_t size;
	int status = fstat(fd, &st);
	size = st.st_size;
	char *ptr = (char*) mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	size_t idx = 0;

    uint64_t input_stone[16];
    uint64_t input_stone_idx = 0;

	while(idx < size){
        uint64_t stone = 0;
        while (isdigit((char)ptr[idx])){
                stone = (stone * 10) + (char)ptr[idx++]-'0';
        }
        input_stone[input_stone_idx++] = stone;
        idx++;
    }
    uint64_t sum1 = 0;
    uint64_t sum2 = 0;
    for (uint64_t i=0; i<input_stone_idx;i++){
        uint64_t stones_count_1 = count_stones_with_cache(input_stone[i], 25);
        uint64_t stones_count_2 = count_stones_with_cache(input_stone[i], 75);
        sum1 += stones_count_1;
        sum2 += stones_count_2;
    }
    
    printf("sum1: %lu\nsum2: %lu\n", sum1, sum2);

}