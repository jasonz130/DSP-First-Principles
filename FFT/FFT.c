#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 4 * atan(1)

typedef struct {
	double real, img;
} complex_num;

complex_num complex_addition(complex_num a, complex_num b){
	complex_num c;
	c.real = a.real + b.real;
	c.img = a.img + b.img;
	return c;
}

complex_num complex_subtract(complex_num a, complex_num b){
	complex_num c;
	c.real = a.real - b.real;
	c.img = a.img - b.img;
	return c;
}

complex_num complex_multiply(complex_num a, complex_num b){
	complex_num c;
	c.real = (a.real * b.real) + (-1 * (a.img * b.img));
	c.img = (a.real * b.img) + (a.img * b.real);
	return c;
}

complex_num* init_input(complex_num* input, int N){
	for (int i = 0; i < N; i++){
		input[i].real = i;
		input[i].img = 0;
	}
	return input;
}

complex_num twiddle_factor(int N, int k){
	complex_num twiddle;
	twiddle.real = cos((-1*2*PI*k)/N);
	twiddle.img = sin((-1*2*PI*k)/N);
	return twiddle;
}

void display_FFT(complex_num* output, int N){
	for (int i = 0; i < N; i++){
		if (output[i].img >= 0)
			printf("%.2lf+%.2lfj ", output[i].real, output[i].img);
		else
			printf("%.2lf%.2lfj ", output[i].real, output[i].img);
	}
}

complex_num* FFT_bit_reversal(complex_num* input, complex_num* output, int N, int* step, int size){
	/* Base case if length is 1 */ 
	if (N == 1){
		/* Set the size 1 input array into the appropriate index in the array output*/
		output[*step].real = input[0].real;
		output[*step].img = input[0].img;
		(*step)++;
		return output;
	}

	/* Recurse call to repeatedly reduce input size by half */
	N /= 2;
	complex_num* even = malloc(N * sizeof(complex_num));

	/* Split even indices into sub-array */
	for (int i = 0, j = 0; i < N*2, j < N; i+=2, j++)
		even[j] = input[i];
	FFT_bit_reversal(even, output, N, step, size);

	/* Split odd indices into sub-array */
	complex_num* odd = malloc(N * sizeof(complex_num));
	for (int i = 1, j = 0; i < N*2, j < N; i+=2, j++)
		odd[j] = input[i];
	FFT_bit_reversal(odd, output, N, step, size);

	return output;
}

complex_num* FFT_reconstruct(complex_num* input, complex_num* output, int N){
	/* Product of indices i and j equal to N_old */
	int i = 2;			// Groups of i
	int j = N / 2;		// j number of groups of i
	int c = 0;			// index for output array
	
	/* Reconstruct bit reversal ordered array: log(base2)N iterations */
	while (i <= N){		
		c = 0;
		
		/* Apply twiddle factor to every other groups of i */
		for (int b = i/2, idx = 0; idx < j; b+=i, idx++){
			for (int d = b, twid = 0; d < (i/2+b); d++, twid++){
				output[d] = complex_multiply(twiddle_factor(i, twid), output[d]);
			}
		}
		
		/* Apply addition and subtraction between groups of i */
		for (int a = 0; a < j; a++) {
			/* Addition */
			for (int b = 0; b < i/2; b++, c++){
				input[c] = complex_addition(output[(a*i) + b], output[(a*i) + (b + i/2)]);
			}

			/* Subtraction */
			for (int b = 0; b < i/2; b++, c++){
				input[c] = complex_subtract(output[(a*i) + b], output[(a*i) + (b + i/2)]);
			}
		}
		
		/* Copy input array contents over to output array contents */
		for (int b = 0; b < N; b++){
			output[b].real = input[b].real;
			output[b].img = input[b].img;
		}

		/* Increment index i by 2 and decrement index j by half: product of i and j is N (i*j = N) */
		i *= 2;
		j /= 2;
	}
	return output;
}

complex_num* FFT(complex_num* input, complex_num* output, int N){
	int step = 0;
	int size = N;
	output = FFT_bit_reversal(input, output, N, &step, size);
	output = FFT_reconstruct(input, output, size);
	return output;
}

int main(int argc, char** argv){
	/* Initialize time-domain input */
	int N = 8;
	complex_num* input = malloc(sizeof(complex_num) * N);
	input = init_input(input, N);
	
	/* Dynamic allocate array of struct complex_num for output */
	complex_num* output = malloc(sizeof(complex_num) * N);

	/* Call FFT function */
	output = FFT(input, output, N);

	/* Display final output of FFT */
	display_FFT(output, N);

	/* Deallocate memory */
	free(input);
	free(output);
	return 0;
}
