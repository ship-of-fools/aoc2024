#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

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

    uint8_t map[map_w+2][map_w+2];

    for (uint8_t i = 0; i < map_w+2; ++i){
        map[0][i] = '*';
        map[map_w+1][i] = '*';
    }
    uint8_t row = 1;
    uint8_t col = 1;
	while(idx < size){
        map[row][0] = '*';
        while ((char)ptr[idx] != '\n' & (char)ptr[idx] != 0 ){
            map[row][col++] = (char)ptr[idx++];
        }
        map[row++][col] = '*';
        idx++;
        col = 1;
    }
    uint8_t seen[map_w+2][map_w+2];
    memset(seen, 0, sizeof(uint8_t)*(map_w+2)*(map_w+2));
    uint32_t sum1 = 0;
    uint32_t sum2 = 0;

    for (uint8_t r = 1; r <= map_w; ++r){
        for (uint8_t c = 1; c <= map_w; ++c){
            if (seen[r][c] > 0)continue;
            seen[r][c] = 1;

            uint8_t region[map_w+2][map_w+2];
            memset(region, 0, sizeof(uint8_t)*(map_w+2)*(map_w+2));
            region[r][c] = 1;
            
            uint8_t crop = map[r][c];
            cord queue[256];
            queue[1].row = r;
            queue[1].col = c;
            uint8_t queue_idx = 1;
            uint32_t region_sum1 = 0;
            uint32_t region_sum2 = 0;

            uint32_t fences = 0;
            uint32_t corners = 0;
            uint32_t region_size = 1;

            while(queue_idx > 0){
                cord current = queue[queue_idx--];
                cord u_cord = {.row=current.row-1, .col=current.col};
                cord d_cord = {.row=current.row+1, .col=current.col};
                cord l_cord = {.row=current.row, .col=current.col-1};
                cord r_cord = {.row=current.row, .col=current.col+1};
                cord ul_cord = {.row=current.row-1, .col=current.col-1};
                cord ur_cord = {.row=current.row-1, .col=current.col+1};
                cord dl_cord = {.row=current.row+1, .col=current.col-1};
                cord dr_cord = {.row=current.row+1, .col=current.col+1};

                uint8_t u_crop = map[u_cord.row][u_cord.col];
                uint8_t d_crop = map[d_cord.row][d_cord.col];
                uint8_t l_crop = map[l_cord.row][l_cord.col];
                uint8_t r_crop = map[r_cord.row][r_cord.col];
                uint8_t ul_crop = map[ul_cord.row][ul_cord.col];
                uint8_t ur_crop = map[ur_cord.row][ur_cord.col];
                uint8_t dl_crop = map[dl_cord.row][dl_cord.col];
                uint8_t dr_crop = map[dr_cord.row][dr_cord.col];

                uint8_t u_crop_diff = (u_crop != crop);
                uint8_t d_crop_diff = (d_crop != crop);
                uint8_t l_crop_diff = (l_crop != crop);
                uint8_t r_crop_diff = (r_crop != crop);
                uint8_t ul_crop_diff = (ul_crop != crop);
                uint8_t ur_crop_diff = (ur_crop != crop);
                uint8_t dl_crop_diff = (dl_crop != crop);
                uint8_t dr_crop_diff = (dr_crop != crop);

                fences += u_crop_diff + d_crop_diff + l_crop_diff + r_crop_diff;

                // Check for inside corners
                if (u_crop_diff & l_crop_diff){
                    corners += 1;
                }
                if (u_crop_diff & r_crop_diff){
                    corners += 1;
                }
                if (d_crop_diff & l_crop_diff){
                    corners += 1;
                }
                if (d_crop_diff & r_crop_diff){
                    corners += 1;
                }
                // Check for outside corners
                if (!l_crop_diff & !u_crop_diff & ul_crop_diff){
                    corners += 1;
                }
                if (!r_crop_diff & !u_crop_diff & ur_crop_diff){
                    corners += 1;
                }
                if (!l_crop_diff & !d_crop_diff & dl_crop_diff){
                    corners += 1;
                }
                if (!r_crop_diff & !d_crop_diff & dr_crop_diff){
                    corners += 1;
                }

                if (!u_crop_diff){
                    if (region[u_cord.row][u_cord.col] == 0){
                        region[u_cord.row][u_cord.col] = 1;
                        region_size += 1;
                        seen[u_cord.row][u_cord.col] = 1;
                        queue[++queue_idx] = u_cord;
                    }
                }
                if (!d_crop_diff){
                    if (region[d_cord.row][d_cord.col] == 0){
                        region[d_cord.row][d_cord.col] = 1;
                        region_size += 1;
                        seen[d_cord.row][d_cord.col] = 1;
                        queue[++queue_idx] = d_cord;
                    }
                }
                if (!l_crop_diff){
                    if (region[l_cord.row][l_cord.col] == 0){
                        region[l_cord.row][l_cord.col] = 1;
                        region_size += 1;
                        seen[l_cord.row][l_cord.col] = 1;
                        queue[++queue_idx] = l_cord;
                    }
                }
                if (!r_crop_diff){
                    if (region[r_cord.row][r_cord.col] == 0){
                        region[r_cord.row][r_cord.col] = 1;
                        region_size += 1;
                        seen[r_cord.row][r_cord.col] = 1;
                        queue[++queue_idx] = r_cord;
                    }
                }
            }
            region_sum1 = region_size * fences;
            region_sum2 = region_size * corners;
            sum1 += region_sum1;
            sum2 += region_sum2;
        }
    }
    printf("sum1: %u\nsum2: %u\n", sum1, sum2); 
}