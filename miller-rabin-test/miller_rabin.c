#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WORD_SIZE	256
#define DWORD_SIZE	512

int* get_bit_string (int size) {
	int *c = (int*) calloc(sizeof(int), size);
	if (c==NULL) {
		fprintf(stderr, "Error allocating memory. Aborting!\n");
		exit(1);
	}
	return c;
}

void hex_to_binary(char hex[], int bin[]) {
	int j = 0;
	int len = strlen(hex);
	for (int i=len-1; i>=0 && hex[i]!='x'; i--) {
		if (hex[i] == '0' || hex[i] == '2' || hex[i] == '4' || hex[i] == '6' || hex[i] == '8' ||
			hex[i] == 'A' || hex[i] == 'C' || hex[i] == 'E' || hex[i] == 'a' || hex[i] == 'c' || hex[i] == 'e')
			bin[j] = 0;
		else
			bin[j] = 1;
		j++;
		if (hex[i] == '0' || hex[i] == '1' || hex[i] == '4' || hex[i] == '5' || hex[i] == '8' ||
			hex[i] == '9' || hex[i] == 'C' || hex[i] == 'D' || hex[i] == 'c' || hex[i] == 'd')
			bin[j] = 0;
		else
			bin[j] = 1;
		j++;
		if (hex[i] == '0' || hex[i] == '1' || hex[i] == '2' || hex[i] == '3' || hex[i] == '8' ||
			hex[i] == '9' || hex[i] == 'A' || hex[i] == 'B' || hex[i] == 'a' || hex[i] == 'b')
			bin[j] = 0;
		else
			bin[j] = 1;
		j++;
		if (hex[i] == '0' || hex[i] == '1' || hex[i] == '2' || hex[i] == '3' || hex[i] == '4' ||
			hex[i] == '5' || hex[i] == '6' || hex[i] == '7')
			bin[j] = 0;
		else
			bin[j] = 1;
		j++;
	}
	while (j<WORD_SIZE) {
		bin[j]=0;
		j++;
	}
}

void subtract_one(int n[], int n_1[]) {
	for (int i=1; i<WORD_SIZE; i++)
		n_1[i] = n[i];
	n_1[0] = 0;
}

void print_number (int arr[], int size) {
	for (int i=size-1; i>=0; i--)
		printf ("%d", arr[i]);
	printf("\n\n");
}

// both are of WORD_SIZE
int is_equal (int a[], int b[]) {
	for (int i=WORD_SIZE-1; i>=0; i--) {
		if (a[i]!=b[i])
			return 0;
	}
	return 1;
}

// return whether a is greater than or equal to b
int is_greater_or_equal(int a[], int b[], int size_a, int size_b) {
	int i=size_a-1;
	if (size_a>size_b) {
		for (i=size_a-1; i>=size_b; i--) {
			if (a[i]==1)
				return 1;
		}
	}
	else if (size_b>size_a) {
		for (i=size_b-1; i>=size_a; i--) {
			if (b[i]==1)
				return 0;
		}
	}
	while (i>=0) {
		if (a[i]<b[i]) 
			return 0;
		if (a[i]>b[i])
			return 1;
		i--;
	}
	return 1;
}

// subtract the numbers a and b and puts difference in diff
void diff(int a[], int b[], int size_a, int size_b, int diff[], int size_diff) {
	int carry = 0;
	int bit1, bit2;
	for (int i=0; i<size_diff; i++) {
		if (i>=size_a)
			bit1 = 0;
		else
			bit1 = a[i];
		if (i>=size_b)
			bit2 = 0;
		else
			bit2 = b[i];
		if (bit1==bit2)
			diff[i]=carry;
		else
			diff[i]=1-carry;
		if (carry==1 && bit1==1 && bit2==0)
			carry=0;
		if (carry==0 && bit1==0 && bit2==1)
			carry=1;
	}
}

// adds the numbers a and b and puts it in sum
void add(int a[], int b[], int sum[], int size) {
	int carry = 0;
	int bit1, bit2;
	for (int i=0; i<size; i++) {
		bit1 = a[i];
		bit2 = b[i];
		if (bit1+bit2+carry == 0) 
			sum[i]=carry=0;
		else if (bit1+bit2+carry==1) {
			sum[i]=1;
			carry=0;
		}
		else if (bit1+bit2+carry==2) {
			sum[i]=0;
			carry=1;
		}
		else 
			sum[i] = carry = 1;
	}
}

// left shifts the number a by n bits
void bit_shift (int a[], int shifted[], int n) {
	int i=0;
	for (i=0; i<n; i++)
		shifted[i]=0;
	while (i<DWORD_SIZE && i-n<WORD_SIZE) { 
		shifted[i] = a[i-n];
		i++;
	}
	while (i<DWORD_SIZE) {
		shifted[i] = 0;
		i++;
	}
}

// right shifts x by 1 bit
void right_shift (int x[], int size) {
	for (int i=1; i<size; i++)
		x[i-1] = x[i];
	x[size-1] = 0;
}

int is_zero(int x[], int size) {
	for (int i=0; i<size; i++) {
		if (x[i]==1)
			return 0;
	}
	return 1;
}

void set_zero(int a[], int size) {
	for (int i=0; i<size; i++) 
		a[i] = 0;
}

// multiplies a and b and stores in product
void multiply (int a[], int b[], int product[]) {
	set_zero(product, DWORD_SIZE);
	for (int i=WORD_SIZE-1; i>=0; i--) {
		if (b[i]==1) {
			int arr[DWORD_SIZE];
			bit_shift(a, arr, i);
			add(product, arr, product, DWORD_SIZE);
		}
	}
}

// copies the number a to b
void copy (int a[], int b[], int size_a, int size_b) {
	int i;
	for (i=0; i<size_a && i<size_b; i++)
		b[i] = a[i];
	while (i<size_b) {
		b[i] = 0;
		i++;
	}
}

// calculate a mod b (remainder when a is divided by b)
// a is DWORD_SIZE 512 bit number
// b (and rem) are always WORD_SIZE (256 bits)
int* modulo (int a[], int b[]) {
	int *rem = get_bit_string(WORD_SIZE);
	// a is the remainder if a is less than b
	if (is_greater_or_equal(a, b, DWORD_SIZE, WORD_SIZE) == 0) {
		copy(a, rem, DWORD_SIZE, WORD_SIZE);
	}
	else {
		int *half = get_bit_string(DWORD_SIZE);
		for (int i=1; i<DWORD_SIZE-1; i++) 
			half[i-1] = a[i];
		half[DWORD_SIZE-1] = 0;
		int *half_rem = modulo(half, b);
		for (int i=1; i<WORD_SIZE; i++) {
			rem[i] = half_rem[i-1];
		}
		rem[0] = a[0];
		if (half_rem[WORD_SIZE-1] == 1 || is_greater_or_equal(rem, b, WORD_SIZE, WORD_SIZE)==1) {
			diff(rem, b, WORD_SIZE, WORD_SIZE, rem, WORD_SIZE);
		}
		free(half_rem);
		free(half);
	}
	return rem;
}

// returns x^n mod m
// x, n, m, result are all WORD_SIZE 
void exponentiation (int x[], int n[], int m[], int result[]) {
	int temp[DWORD_SIZE];
	int *mod;
	copy(x, temp, WORD_SIZE, DWORD_SIZE);
	int *ctr = modulo(temp, m);

	// set initial value of remainder = 1
	result[0] = 1;
	for (int i=1; i<WORD_SIZE; i++)
		result[i]=0;

	for (int i=0; i<WORD_SIZE; i++) {
		if (n[i] == 1) {
			multiply (result, ctr, temp);
			mod = modulo (temp, m);
			copy(mod, result, WORD_SIZE, WORD_SIZE);
			free(mod);
		}
		multiply (ctr, ctr, temp);
		free(ctr);
		ctr = modulo (temp, m);
	}
	free(ctr);
}

// Generates a random bit string less that or equal to n and saves it in rand
// Flips a coin for each bit, ensuring that the random number does not exceed n at any time
void generate_random(int n[], int random[]) {
	int flag = 1;
	int i;
	time_t t;
	srand((unsigned) time(&t));
	for (i=WORD_SIZE-1; i>=0 && n[i]==0; i--)
		random[i]=0;
	while (i>=0) {
		if (flag==0 || n[i]==1) {
			int toss = rand()%2;
			random[i] = toss;
			if (toss==0)
				flag = 0;
		}
		else
			random[i]=0;
		i--;
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Incorrect number of arguments. Usage:\n%s <FILENAME>\n", argv[0]);
		return 1;
	}
	char input[10+ WORD_SIZE/4];
	int inputBitString[WORD_SIZE];
	int inputMinusOne[WORD_SIZE];
	int m[WORD_SIZE];
	int a[WORD_SIZE];
	int b[WORD_SIZE];
	int one[WORD_SIZE];
	int temp_double[DWORD_SIZE];
	int *temp;
	int k;

	FILE *fp;
	fp = fopen(argv[1], "r");
	if (fp==NULL) {
		fprintf(stderr, "Unable to open file: %s. Aborting!\n", argv[1]);
		return 1;
	}
	fscanf (fp, "%s", input);

	for (int i=0; i<WORD_SIZE; i++)
		one[i]=0;
	one[0] = 1;

	hex_to_binary(input, inputBitString);
	printf ("Input number N = \n");
	print_number(inputBitString, WORD_SIZE);
	if (inputBitString[0] == 0) {			// Trivial case when N is even
		printf ("Input number is even. Composite!\n");
		return 0;
	}
	subtract_one(inputBitString, inputMinusOne);
	// express N-1 as (2^k) * m
	k = 0;
	while (inputMinusOne[k]==0)
		k++;
	for (int i=0; i<WORD_SIZE; i++) {
		if (i+k<WORD_SIZE) 
			m[i] = inputMinusOne[i+k];
		else
			m[i] = 0;
	}

	printf ("Value of k = %d.\n", k);
	printf ("Value of m = \n");
	print_number(m, WORD_SIZE);
	do {
		generate_random(inputMinusOne, a);
	} while (is_zero(a, WORD_SIZE)==1);
	printf ("\nRandom number generated: A = \n");
	print_number(a, WORD_SIZE);

	exponentiation (a, m, inputBitString, b);

	if (is_equal(b, one)==1) {
		printf("Prime.\n");
		return 0;
	}

	for (int i=0; i<k; i++) {
		printf("b is: \n");
		print_number(b, WORD_SIZE);
		if (is_equal(b, inputMinusOne) == 1) {
			printf("Prime.\n");
			return 0;
		}
		multiply (b, b, temp_double);
		temp = modulo(temp_double, inputBitString);
		copy(temp, b, WORD_SIZE, WORD_SIZE);
		free(temp);
	}
	printf ("Composite.\n");
	return 0;
}