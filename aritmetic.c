
#include <stdlib.h>

unsigned long bit_a[] = {0,0,0,0};

void add_ipt(unsigned char n) {
	unsigned char x = n>>5;
	unsigned char y = n<<3; y >>=6; y<<=3; //otherwise compiler will choose SAL and then SAR (and not SHR)
	unsigned char z = n<<5; z>>=5;
	// ba[x](y)(z) : xnd int, ynd byte, znd bit
	printf("inserting %d:%c with x: %d, y: %d z:%d ....%d / %d\n", n, n,  x, y, z, n&3, n<<3>>6);
	bit_a[x] = bit_a[x] | (1<<(8*y + z));
}

// count the universe of symb
unsigned char count_uni() {
	unsigned long bac[] = {bit_a[0], bit_a[1], bit_a[2], bit_a[3]};
	unsigned char x = 0, c = 0;
	while(x<4) {
		int y= 0;
		while (bac[x]>>y > 0 || bac[x]>>y > 0) {
			if(bac[x]>>y & 1) c++;
			y++;
		}
		x++;
	}
	return c;
}

struct symprob {
	double p;
	char c;
};


int cmpprob(const void *a, const void *b) {
	struct symprob* a1 = (struct symprob*) a;
	struct symprob* b1 = (struct symprob*) b;
	return a1->c>=b1->c?-1:1;
}

struct symprob** calcprob(char* ch) {
	// esempio di errore: pensare che la dimensione dell'array
	// di simboli-probabilita sia n, la lunghezza dell'input
	// e non dipendente dal contenuto dell'input
	int l = sizeof(ch);
	struct symprob** sym = malloc(l);
	int iptl = l;
	for (int i=0;i<sizeof(ch);i++) {
		int c= 0;
		struct symprob* pa = malloc(sizeof(struct symprob));
		pa->c = ch[i];
		for( int j = 0; j <= l; j++) {
			if (ch[j] == ch[i]) c++;
		}
		pa->p = (float) c / iptl;
		sym[i] = pa;
	}
	qsort(sym, l, sizeof(struct symprob), cmpprob);
	return sym;
}

void arith_code(struct symprob **prob, char *ipt) {
}

char * build_nth(unsigned char l) {
	unsigned char *nth = malloc(l);
	unsigned long bac[] = {bit_a[0], bit_a[1], bit_a[2], bit_a[3]};
	unsigned char x = 0, c = 0;
	while(x<4) {
		int y = 0;
		while (bac[x]>>y > 0 || bac[x]>>y > 0) {
			if(bac[x]>>y & 1) {
				nth[c] = (x*32)+y;
				c++;
			}
			y++;
		}
		x++;
	}
	return nth;
}

char * bitstring(int x) {

}

int main() {
	char * input = "aabbc\0";
	//struct symprob **prob = calcprob(input);
	for (int i = 0; i< strlen(input); i++) {
		add_ipt(input[i]);
	}
	printf("%d %d %d %d %d\n", input[0], input[1], input[2], input[3], input[4]);
	printf("bitarr: %lu %lu %lu %lu\n", bit_a[0], bit_a[1], bit_a[2], bit_a[3]);
	unsigned char cnt = count_uni();
	printf("count:: %d\n", cnt);
	// now the map is for the 1st, the 2nd, the 3rd, ... the cnt-th element
	// so build:
	//  * nth_s[i] = i-th element value
	//  * fth[i] = i-th element frequency
	char * nth_s = build_nth(cnt);
	for( int i=0; i<cnt; i++) {
		printf("%c", nth_s[i]);
	}
	//
	//arith_code(prob, input);

}
