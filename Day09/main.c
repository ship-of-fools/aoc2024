#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')
#define ismarker(c) ((c) == '.' || (c) == '#' || (c) == '^')

int main(int argc, char** argv){

    int fd = open("input.txt", O_RDONLY);
    // int fd = open("example.txt", O_RDONLY);

	struct stat st;
	size_t size;
	int status = fstat(fd, &st);
	size = st.st_size;
	char *ptr = (char*) mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	size_t idx = 0;

    int16_t array[262144]; // hope this is big enough
    memset(array, -1, sizeof(int16_t)*262144);
    // char array[1024]; // hope this is big enough
    // memset(array, -1, sizeof(char)*1024);
    size_t array_idx = 0;
    unsigned int id_number = 0;
    char file = 1;

    int32_t free_spaces[10][2048]; // can probably make this smaller
    int16_t free_spaces_len[10];
    memset(free_spaces_len, 0, sizeof(int16_t)*9);
    int16_t free_spaces_used[10];
    memset(free_spaces_used, 0, sizeof(int16_t)*9);
    int32_t numbers[10000][2]; // index + span for each number
    memset(numbers, 0, sizeof(int32_t)*10000*2); // prob remove this as we set everything anyway

	while(idx < size){
        if (isdigit((unsigned char)ptr[idx])){
            unsigned char num = (unsigned char)ptr[idx]-'0';
            
            if (file){
                uint8_t span = num;
                numbers[id_number][0] = array_idx;
                numbers[id_number][1] = span;
                for (unsigned char i = 0; i < num; i++){
                    array[array_idx++] = id_number;
                    // printf("%u\n", id_number);
                    // array_idx++;
                }
                id_number++;
                file = 0;
            } else {
                uint8_t span = num;
                free_spaces[span][free_spaces_len[span]++] = array_idx;
                for (unsigned char i = 0; i < num; i++){
                    array[array_idx++] = -1; // maybe dont set this to -1 as it already is
                    // array_idx++;
                }
                file = 1;
            }
        }
        idx++;
    }
    // could store free space indeces in a seperate array, might be faster?

    // copy array for part 2 later
    int16_t array_p2[262144];
    memcpy(array_p2, array, sizeof(array[0])*262144);
    // size_t end_idx_p2 = array_idx - 1;

    size_t end_idx = array_idx - 1;
    size_t free_space_idx = 0;

    while (end_idx > free_space_idx){
        while(array[free_space_idx] != -1){free_space_idx++;}
        while(array[end_idx] == -1){end_idx--;}

        array[free_space_idx++] = array[end_idx];
        array[end_idx--] = -1;
    }
    // Hack to sort out the end, sometimes it needs a shuffle
    if (array[end_idx+2] != -1){
        array[end_idx+1] = array[end_idx+2];
        array[end_idx+2] = -1;
        end_idx+=1;
    }

    uint64_t sum1 = 0;

    for (size_t i = 0; i <= end_idx; i++){
        sum1 += (array[i] * i);
    }

    // Debug print start and end
    // for (size_t i = 0; i <= 200; i++){
    //     if (array_p2[i] == -1){
    //         printf("@ ");
    //     }else{
    //         printf("%d ", array_p2[i]);
    //     }
    // }
    // printf("\n\n\n");
    // for (size_t i = end_idx_p2-200; i <= end_idx_p2; i++){
    //     if (array_p2[i] == -1){
    //         printf("@ ");
    //     }else{
    //         printf("%d ", array_p2[i]);
    //     }
    // }
    // printf("\n\n");

    int32_t largest_number = id_number-1;

    for (int32_t i = largest_number; i >= 0; i--){
        uint32_t idx = numbers[i][0];
        uint8_t span = numbers[i][1];
        uint32_t leftmost_space_idx = -1;
        uint8_t leftmost_space_span = 0;
        for (uint8_t j = span; j <=9; j++){ // we need to find left most index of space that is large enough
            
            if (free_spaces_len[j] > free_spaces_used[j]){
                uint32_t possible_free_space = free_spaces[j][free_spaces_used[j]];
                if (possible_free_space < leftmost_space_idx){
                    leftmost_space_idx = possible_free_space;
                    leftmost_space_span = j;
                }
            }
        }
        uint32_t possible_free_space = free_spaces[leftmost_space_span][free_spaces_used[leftmost_space_span]];
        if (possible_free_space < idx){
            // space created by us moving, search free spaces to find where to insert
            int32_t s = 0;
            for (s = free_spaces_len[span]-1; s > 0; s--){
                if (free_spaces[span][s] > idx){
                    free_spaces[span][s+1] = free_spaces[span][s];
                } else {
                    break;
                }
            }
            free_spaces[span][s+1] = idx;
            free_spaces_len[span]++;


            numbers[i][0] = possible_free_space; // move this number index to found space
            
            free_spaces_used[leftmost_space_span]++; // we took up an empty space, so update used len
            if (span != leftmost_space_span){ // the space was larger than we needed, so we've created a new space with the remainer
                uint8_t remaining_space_span = leftmost_space_span - span;
                uint32_t remaining_space_idx = possible_free_space + (leftmost_space_span-remaining_space_span);
                free_spaces_len[remaining_space_span]++; // increase spaces len to include new space
                for (uint32_t k = 0; k < free_spaces_len[remaining_space_span]; k++){
                    if (free_spaces[remaining_space_span][k] > remaining_space_idx) // find index to insert new space index
                    {
                        // printf("into size %u at %u, leaves size %u at %u\n", leftmost_space_span, possible_free_space, remaining_space_span, remaining_space_idx);
                        for (uint32_t w = free_spaces_len[remaining_space_span]-1; w > k; w--){ // insert new space index
                            free_spaces[remaining_space_span][w] = free_spaces[remaining_space_span][w-1];
                        }
                        free_spaces[remaining_space_span][k] = remaining_space_idx;
                        break; // we've inserted so get out
                    }
                }

            }
            // printf("%u: %u, %u\n", i, span, numbers[i][0]);
        } else {
            // printf("%u: %u, %u*\n", i, span, numbers[i][0]);
            // We need to check if we can just shift left though
                // Check all gap sizes that are closest (but to the left side) to this idx
                // Check the span of that gap reaches our idx
                // Move to that gap
                // update indeces
            // uint32_t closest_free_idx = 0;
            // uint8_t closest_free_span = 0;
            // uint32_t free_spaces_idx = 0;
            // for (uint8_t c = 1; c <=9; c++){
            //     for (int64_t cc=free_spaces_used[c]; cc < free_spaces_len[c]; cc++){ // maybe cc>=?
            //         int32_t free_idx = free_spaces[c][cc];
            //         if (free_idx < idx){
            //             if (free_idx > closest_free_idx){
            //                 closest_free_idx = free_idx;
            //                 closest_free_span = c;
            //                 free_spaces_idx = cc;
                            
            //             }
            //         } else {
            //             break;
            //         }
            //     }
            // }
            // if ((closest_free_idx + closest_free_span)==idx){
            //     printf("we shifting\n");
            //     free_spaces[closest_free_span][free_spaces_idx] += span;
            //     numbers[i][0] = closest_free_idx; // move this number index to found space




            // } else if ((closest_free_idx + closest_free_span)>idx){
            //     printf("should never get here\n"); // REMOVE THIS, NOT NEEDED
            // }

        }
        
        // break; 
            
    }

    // Lets print out the entire new map, get rid of this once we've fixed problems:
    // int16_t array_p2_dbg[262144];
    // memset(array_p2_dbg, -1, sizeof(int16_t)*262144);
    // for (uint32_t i = 0; i <= largest_number; i++){
    //     uint32_t idx = numbers[i][0];
    //     uint32_t span = numbers[i][1];
    //     for (uint16_t j = 0; j < span; j++){
    //         // sum2 += (i * (idx+j));
    //         array_p2_dbg[idx+j] = i;
    //     }
    // }
    // printf("\n\n\n");
    // int count = 0;
    // for (size_t i = 0; i <= end_idx_p2; i++){
    //     if (array_p2_dbg[i] == -1){
    //         printf(".");
    //     }else{
    //         printf("[%d]", array_p2_dbg[i]);
    //     }
    //     count++;
    //     if (count== 99){
    //         printf("\n");
    //         count = 0;
    //     }
    // }
    // printf("\n\n");

    uint64_t sum2 = 0;
    for (uint32_t i = 0; i <= largest_number; i++){
        uint32_t idx = numbers[i][0];
        uint32_t span = numbers[i][1];
        for (uint16_t j = 0; j < span; j++){
            sum2 += (i * (idx+j));
        }

    }

    // uint64_t sum3 = 0;
    // count = 0;
    // for (size_t i = 0; i <= end_idx_p2; i++){
    //     if (array_p2_dbg[i] == -1){
    //     }else{
    //         int16_t dgb = array_p2_dbg[i];
    //         sum3 += (((uint64_t)array_p2_dbg[i])*i);
    //         if (count >= 37514){
    //         // if (count < 200){
    //             printf("[%lu * %d]: %lu \n", i, array_p2_dbg[i], sum3);   
    //         }
    //     }
    //     count++;
    // }    


    printf("sum1: %lu\nsum2: %lu\n", sum1, sum2); // 7396973209083 too high

    //37515 = -1 followed by 3938, but should be start of 3938

}