#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_BINS 256
#define BLOCK_BIT_SIZE 8

#ifndef TOTAL_BYTES
#define TOTAL_BYTES 1048576
#endif

#ifndef SIGNIFICANCE_LEVEL
#define SIGNIFICANCE_LEVEL 10
#endif


void create_histogram(
/*
 * Stores a histogram of byte value frequences into a given array.
 *
 * Different byte values are represented by array indexes.
 * The value at an index is the frequence of a given byte in the random sample.
 * */
    unsigned char* array,       // array with data to analyze
    uint64_t array_num_bytes,   // size of the array
    uint32_t* histogram         // histogram array
    ) {
    int num_blocks;
    int byte_shift;
    int value_idx;

    uint32_t* block_32bit;
    uint32_t mask;

    uint64_t bits_read;
    uint64_t byte_offset;

    num_blocks = array_num_bytes * 8 / BLOCK_BIT_SIZE;
    mask = (1 << BLOCK_BIT_SIZE) - 1;
    bits_read = 0;

    for (int i = 0; i < num_blocks; i++) {
        byte_offset = bits_read / 8;
        block_32bit = (uint32_t *) (void *) (array + byte_offset);
        byte_shift = bits_read & 7;  // equivalent to bits_written % 8
        value_idx = (block_32bit[0] >> byte_shift) & mask;
        histogram[value_idx]++;
        bits_read += BLOCK_BIT_SIZE;
    }
}


int chi_squared_test(
    uint32_t* histogram
    ) {
    float chi_squared = 0;
    float expected_freq = TOTAL_BYTES / NUM_BINS;
    float critical_value;
    float tmp;

    printf("\nPerforming chi-squared goodness of fit test ...\n");

    for (int i = 0; i < NUM_BINS; i++) {
	tmp = histogram[i] - expected_freq;
	chi_squared += (tmp * tmp) / expected_freq;
    }

    switch (SIGNIFICANCE_LEVEL) {
        case 2: {
            critical_value = 303.4960850;
	    break;
        };
        case 5: {
	    critical_value = 293.24783508;
	    break;
	};
	case 10: {
            critical_value = 284.33590782;
            break;
        };
        case 20: {
            critical_value = 273.78615917;
            break;
        };
	default: {
	    fprintf(stderr, "unsupported significance level\n");
	    return 1;
	}
    }

    printf("critical value: %.8f\nchi-squared: %.8f\nsignificance level: %.2f\n", critical_value, chi_squared, SIGNIFICANCE_LEVEL / 100.f);

    if (chi_squared > critical_value) {
	printf("Test failed!\n");
	return 1;
    }

    return 0;
}


int main(int argc, char *argv[]) {
    int retval = 1;
    FILE* in_file;

    if (argc != 2) {
        fprintf(stderr, "USAGE: ./<binary for this source file> <file>\n");
        return retval;
    }

    if (!(in_file = fopen(argv[1], "rb"))) {
        fprintf(stderr, "could not open file %s\n", argv[1]);
        return retval;
    }

    uint32_t* histogram;
    unsigned char* buffer;

    if(!(histogram = calloc(sizeof(uint32_t), NUM_BINS))) {
        fprintf(stderr, "allocation error\n");
        goto cleanup_file;
    }

    size_t chunk_size = BLOCK_BIT_SIZE * 128;

    if(!(buffer = malloc(chunk_size))) {
        fprintf(stderr, "allocation error\n");
        goto cleanup_alloc;
    }

    size_t read_bytes;
    char previous_read_incomplete = 0;

    // read data file by chunks stored in buffer
    // count_bitblocks() updates the histogram according to the buffer
    while ((read_bytes = fread(buffer, sizeof(unsigned char), chunk_size, in_file)) > 0) {
        // only the last read can be incomplete, i.e., read_bytes != chunk_size
        // otherwise an incomplete buffer would be given to count_bitblocks(), causing an incorrect histogram
        if (previous_read_incomplete) {
            fprintf(stderr, "incomplete read from data file occurred, aborting\n");
            goto cleanup_full;
        }

        if (read_bytes != chunk_size)
            previous_read_incomplete = 1;

        create_histogram(buffer, read_bytes, histogram);
    }

    if (chi_squared_test(histogram)) {
	goto cleanup_full;
    }

    retval = 0;

cleanup_full:
    free(buffer);
cleanup_alloc:
    free(histogram);
cleanup_file:
    fclose(in_file);
    return retval;
}

