#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

// A utility function to get maximum value in arr[]
int getMax(int arr[], int n)
{
	int mx = arr[0];
	for (int i = 1; i < n; i++)
		if (arr[i] > mx)
			mx = arr[i];
	return mx;
}

// A function to do counting sort of arr[] according to
// the digit represented by exp.
void countSort256(unsigned int arr[], unsigned int n, unsigned int exp)
{
	// output array
	unsigned int output[n];
	int i, count[256] = { 0 };

	// Store count of occurrences in count[]
	for (i = 0; i < n; i++)
		count[(arr[i] / exp) % 256]++;

	// Change count[i] so that count[i] now contains actual
	// position of this digit in output[]
	for (i = 1; i < 256; i++)
		count[i] += count[i - 1];

	// Build the output array
	for (i = n - 1; i >= 0; i--) {
		output[count[(arr[i] / exp) % 256] - 1] = arr[i];
		count[(arr[i] / exp) % 256]--;
	}

	// Copy the output array to arr[], so that arr[] now
	// contains sorted numbers according to current digit
	for (i = 0; i < n; i++)
		arr[i] = output[i];
}

// The main function to that sorts arr[] of size n using
// Radix Sort
void radixsort256(unsigned int arr[], unsigned int n)
{
	// Find the maximum number to know number of digits
	unsigned int m = getMax(arr, n);

	// Do counting sort for every digit. Note that instead
	// of passing digit number, exp is passed. exp is 10^i
	// where i is current digit number
	for (unsigned int exp = 1; m / exp > 0; exp *= 256)
		countSort256(arr, n, exp);
}

// A function to do counting sort of arr[] according to
// the digit represented by exp.
void countSort10(unsigned int arr[], unsigned int n, unsigned int exp)
{
	// output array
	unsigned int output[n];
	int i, count[10] = { 0 };

	// Store count of occurrences in count[]
	for (i = 0; i < n; i++)
		count[(arr[i] / exp) % 10]++;

	// Change count[i] so that count[i] now contains actual
	// position of this digit in output[]
	for (i = 1; i < 10; i++)
		count[i] += count[i - 1];

	// Build the output array
	for (i = n - 1; i >= 0; i--) {
		output[count[(arr[i] / exp) % 10] - 1] = arr[i];
		count[(arr[i] / exp) % 10]--;
	}

	// Copy the output array to arr[], so that arr[] now
	// contains sorted numbers according to current digit
	for (i = 0; i < n; i++)
		arr[i] = output[i];
}

// The main function to that sorts arr[] of size n using
// Radix Sort
void radixsort10(unsigned int arr[], unsigned int n)
{
	// Find the maximum number to know number of digits
	unsigned int m = getMax(arr, n);

	// Do counting sort for every digit. Note that instead
	// of passing digit number, exp is passed. exp is 10^i
	// where i is current digit number
	for (unsigned int exp = 1; m / exp > 0; exp *= 10)
		countSort10(arr, n, exp);
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
    radixsort256(a, list_length);
    radixsort256(b, list_length);
    // qsort(a, list_length, sizeof(unsigned int), compare);
    // qsort(b, list_length, sizeof(unsigned int), compare);
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