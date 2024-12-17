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

uint64_t reg_a = 0;
uint64_t reg_b = 0;
uint64_t reg_c = 0;
uint8_t pc = 0;

uint8_t program[32];
uint8_t program_len = 0;
uint8_t output[32];
uint8_t output_len = 0;

void adv(uint64_t *a, uint64_t *b, uint64_t *c, uint8_t operand){
    if (operand < 4){
        *a = *a >> operand;
    }
    if (operand == 4){
        *a = *a >> *a;
    }
    if (operand == 5){
        *a = *a >> *b;
    }
    if (operand == 6){
        *a = *a >> *c;
    }
}
void bdv(uint64_t *a, uint64_t *b, uint64_t *c, uint8_t operand){
    if (operand < 4){
        *b = *a >> operand;
    }
    if (operand == 4){
        *b = *a >> *a;
    }
    if (operand == 5){
        *b = *a >> *b;
    }
    if (operand == 6){
        *b = *a >> *c;
    }
}
void cdv(uint64_t *a, uint64_t *b, uint64_t *c, uint8_t operand){
    if (operand < 4){
        *c = *a >> operand;
    }
    if (operand == 4){
        *c = *a >> *a;
    }
    if (operand == 5){
        *c = *a >> *b;
    }
    if (operand == 6){
        *c = *a >> *c;
    }
}
void bxl(uint64_t *a, uint64_t *b, uint64_t *c, uint8_t operand){
    *b = *b ^ operand;
}
void bxc(uint64_t *a, uint64_t *b, uint64_t *c, uint8_t operand){
    *b = *b ^ *c;
}
void bst(uint64_t *a, uint64_t *b, uint64_t *c, uint8_t operand){
    if (operand < 4){
        *b = operand % 8;
    }
    if (operand == 4){
        *b = *a % 8;
    }
    if (operand == 5){
        *b = *b % 8;
    }
    if (operand == 6){
        *b = *c % 8;
    } 
}

uint8_t first = 1;
uint64_t p2;

uint64_t run_program(uint64_t a, uint8_t output_idx){ // lets assume for now that b and c only depend on a
    // lets assume last two operations are always a >> 3, jmp
    if (output_idx == 255){
        if (first){
            first = 0;
            p2 = a;
            printf("%lu\n", a);
        }
        return a;
    }
    if (!first){
        return 0;
    }
    uint64_t initial_a = a;
    // printf("a: %lu\n", a);
    for (uint8_t i=0; i<8; i++){
        if (!first){
            return 0;
        }
        a = (initial_a << 3) + i;
        uint64_t b = 0;
        uint64_t c = 0;
        uint8_t pc_idx = 0;
        while (1){ // do program once, should always reach output and either pass or fail
            uint8_t opcode = program[pc_idx++];
            uint8_t operand = program[pc_idx++];
            if (opcode == 0){
                adv(&a, &b, &c, operand);
                continue;
            }   
            if (opcode == 1){
                bxl(&a, &b, &c, operand);
                continue;
            }
            if (opcode == 2){
                bst(&a, &b, &c, operand);
                continue;
            }
            if (opcode == 3){
                continue;
            }
            if (opcode == 4){
                bxc(&a, &b, &c, operand);
                continue;
            }
            if (opcode == 5){
                uint64_t combo = 0;
                if (operand < 4){
                    combo = operand;
                }
                if (operand == 4){
                    combo = a;
                }
                if (operand == 5){
                    combo = b;
                }
                if (operand == 6){
                    combo = c;
                } 
                uint8_t output = combo % 8;
                if (output != program[output_idx])break;
                // printf("a: %lu\n", a);
                uint64_t res = run_program(a, output_idx-1);
                break;
            }
            if (opcode == 6){
                bdv(&a, &b, &c, operand);
                continue;
            }
            if (opcode == 7){
                cdv(&a, &b, &c, operand);
                continue;
            }

        }
    } 
}

int main(int argc, char** argv){
    // int fd = open("example.txt", O_RDONLY);
    int fd = open("input.txt", O_RDONLY);

	struct stat st;
	size_t size;
	int status = fstat(fd, &st);
	size = st.st_size;
	char *ptr = (char*) mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	size_t idx = 0;

    while(idx < size){
            idx+=12;
            while(isdigit((char)ptr[idx])){
                reg_a = reg_a * 10 + ((char)ptr[idx++]-'0');
            }
            idx+=13;
            while(isdigit((char)ptr[idx])){
                reg_b = reg_b * 10 + ((char)ptr[idx++]-'0');
            }
            idx+=13;
            while(isdigit((char)ptr[idx])){
                reg_c = reg_c * 10 + ((char)ptr[idx++]-'0');
            }
        
        idx+=11;
        while ((char)ptr[idx] != '\n' & (char)ptr[idx] != 0){
            program[program_len++] = ((char)ptr[idx++]-'0');
            idx++;
        }
    }

    while(1){
        if (pc >= program_len)break;
        uint8_t opcode = program[pc++];
        uint8_t operand = program[pc++];
        if (opcode == 0){
            adv(&reg_a, &reg_b, &reg_c, operand);
            continue;
        }   
        if (opcode == 1){
            bxl(&reg_a, &reg_b, &reg_c, operand);
            continue;
        }
        if (opcode == 2){
            bst(&reg_a, &reg_b, &reg_c, operand);
            continue;
        }
        if (opcode == 3){
            if (reg_a != 0){
                pc = operand;
            }else{
                // printf("no jump\n");
            }
            continue;
        }
        if (opcode == 4){
            bxc(&reg_a, &reg_b, &reg_c, operand);
            continue;
        }
        if (opcode == 5){
            uint64_t combo = 0;
            if (operand < 4){
                combo = operand;
            }
            if (operand == 4){
                combo = reg_a;
            }
            if (operand == 5){
                combo = reg_b;
            }
            if (operand == 6){
                combo = reg_c;
            } 
            output[output_len++] = combo % 8;
            continue;
        }
        if (opcode == 6){
            bdv(&reg_a, &reg_b, &reg_c, operand);
            continue;
        }
        if (opcode == 7){
            cdv(&reg_a, &reg_b, &reg_c, operand);
            continue;
        }
    }

    char sum1[32];
    uint8_t sum1_idx = 0;
    for (uint8_t i=0; i < output_len; i++){
        sum1[sum1_idx++] = output[i]+'0';
        sum1[sum1_idx++] = ',';

    }
    sum1[sum1_idx-1] = '\0';
    printf("%s\n", (char *)sum1);

    // Part 2
    run_program(0, program_len-1);
}