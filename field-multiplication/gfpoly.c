#include <x86intrin.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_DEGREE	512

#if !defined (ALIGN16)
# if defined (__GNUC__)
#  define ALIGN16  __attribute__  ( (aligned (16)))
# else
#  define ALIGN16 __declspec (align (16))
# endif
#endif

void print_m128i_with_string(const char* string,__m128i data) {
	unsigned char *pointer = (unsigned char*)&data;
	int i;
	printf("%-40s[0x",string);
	for (i=15; i>=0; i--)
		printf("%02x",pointer[i]);
	printf("]\n");
}

int get_int_from_hex(char c) {
	if (c>='0' && c<='9')
		return c-'0';
	if (c>='a' && c<='f')
		return 10+c-'a';
	if (c>='A' && c<='F')
		return 10+c-'A';
	return '\0';
}

void get_m128i_from_string(char* hex,__m128i *data) {
	ALIGN16 uint8_t a[16];
	int j = 0;
	int len = strlen(hex);
	int i=len-1;
	int flag=0;
	while (j<16) {
		int lsb, msb;
		if (flag==0)
			lsb = get_int_from_hex(hex[i]);
		else
			lsb = 0;
		i--;
		if (i<0 || hex[i]=='x')
			flag=1;
		if (flag==0)
			msb = get_int_from_hex(hex[i]);
		else
			msb = 0;
		i--;
		if (i<0 || hex[i]=='x')
			flag=1;
		a[j] = msb*16+lsb;
		j++;
	}
	__m128i h1 = _mm_set_epi8(a[15],a[14],a[13],a[12],a[11],a[10],a[9],a[8],a[7],a[6],a[5],a[4],a[3],a[2],a[1],a[0]);
	*data = h1;
}

__m128i mm_bitshift_left(__m128i x, unsigned count) {
    __m128i carry = _mm_bslli_si128(x, 8);   // old compilers only have the confusingly named _mm_slli_si128 synonym
    if (count >= 64)
        return _mm_slli_epi64(carry, count-64);  // the non-carry part is all zero, so return early
    // else
    carry = _mm_srli_epi64(carry, 64-count);  // After bslli shifted left by 64b

    x = _mm_slli_epi64(x, count);
    return _mm_or_si128(x, carry);
}

// c1c0 = a * b
// karatsuba multiplication
void GF2m_mul_2x2_xmm(__m128i *c1, __m128i *c0, __m128i b, __m128i a) {
    __m128i t1, t2;
    *c0 = _mm_clmulepi64_si128(a, b, 0x00);
    *c1 = _mm_clmulepi64_si128(a, b, 0x11);
    t1  = _mm_shuffle_epi32(a, 0xEE);
    t1  = _mm_xor_si128(a, t1);
    t2  = _mm_shuffle_epi32(b, 0xEE);
    t2  = _mm_xor_si128(b, t2);
    t1  = _mm_clmulepi64_si128(t1, t2, 0x00);
    t1  = _mm_xor_si128(*c0, t1);
    t1  = _mm_xor_si128(*c1, t1);
    t2  = t1;
    t1  = _mm_slli_si128(t1, 8);
    t2  = _mm_srli_si128(t2, 8);
    *c0 = _mm_xor_si128(*c0, t1);
    *c1 = _mm_xor_si128(*c1, t2);
}

__m128i gfmul(__m128i a, __m128i b) {
    __m128i c0, c1;
    __m128i x0, x1, x2, x3;
    __m128i A, B, C, D;
    __m128i x3_upper, e, f, g;
    GF2m_mul_2x2_xmm(&c1, &c0, b, a);
    x0 = _mm_loadl_epi64(&c0);
    x1 = _mm_srli_si128(c0,8);
    x2 = _mm_loadl_epi64(&c1);
    x3 = _mm_srli_si128(c1,8);

    A = _mm_srli_epi64(x3, 63);
    B = _mm_srli_epi64(x3, 62);
    C = _mm_srli_epi64(x3, 57);

    A = _mm_xor_si128(A, B);
    x2 = _mm_xor_si128(C, x2);
    D = _mm_xor_si128(A, x2);

    x3_upper = _mm_slli_si128(x3,8);
    x3_upper = _mm_or_si128(x3_upper,D);

    e = mm_bitshift_left(x3_upper, 1);
    f = mm_bitshift_left(x3_upper, 2);
    g = mm_bitshift_left(x3_upper, 7);

    e = _mm_xor_si128(e, f);
    g = _mm_xor_si128(g, x3_upper);
    e = _mm_xor_si128(e, g);

    x1 = _mm_slli_si128(x1,8);
    x1 = _mm_or_si128(x1,x0);
    x1 = _mm_xor_si128(e, x1);
    return x1;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Incorrect number of arguments. Usage:\n%s <FILENAME>\n", argv[0]);
		return 1;
	}
	
	int n;		// degree of f
	char hexstring[40];
	__m128i f_coefficients[MAX_DEGREE];
	__m128i h;
	__m128i result;

	FILE *fp;
	fp = fopen(argv[1], "r");
	if (fp==NULL) {
		fprintf(stderr, "Unable to open file: %s. Aborting!\n", argv[1]);
		return 1;
	}
	fscanf (fp, "%d", &n);
	n = n+1;
	// read coefficients
	for (int i=0; i<n; i++) {
		fscanf(fp, "%s", hexstring);
		get_m128i_from_string(hexstring, &f_coefficients[i]);
	}
	// read h
	fscanf(fp, "%s", hexstring);
	get_m128i_from_string(hexstring, &h);
	print_m128i_with_string("h: ", h);

	result = f_coefficients[n-1];
	for (int i=n-1; i>0; i--) {
		result = gfmul(result, h);
		result = _mm_xor_si128(result, f_coefficients[i-1]);
	}

	print_m128i_with_string("Result: ", result);
	return 0;
}