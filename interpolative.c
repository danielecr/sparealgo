#include <stdlib.h>
#include <stdio.h>

int outbufpos = 0;
unsigned char *outbuf;

void emit(int n, int bitcnt) {
   int blk1 = outbufpos / 8;
   int blk2 = blk1 +1;
   int offs = outbufpos % 8;
   if ( bitcnt + offs < 8) {
	   outbuf[blk1] |= (n<<(8-offs));
   } else {
	   outbuf[blk1] |= n>>(bitcnt -(8 - offs));
	   outbuf[blk2] |= n<<((8 -bitcnt) + (8 - offs));
   }
   outbufpos += bitcnt;
   printf("blk:%d %d %d\n", outbuf[blk1],n, bitcnt);
}

int bitcnt ( int n) {
	int l = 1;
	while ( (n = n>>1) > 0) l++;
	return l;
}
void int_code(int *msg, int l, int h) {
	int m_i = (l + h) / 2;
	//printf("code: %d %d %d\n", l, h, m_i);
	int v_m = msg[m_i];
	int u_b = msg[h] - (h - m_i);
	int l_b = msg[l] + (m_i - l);
	int bits = bitcnt(u_b - l_b);
	int m_c = msg[m_i] - l_b;
	emit(m_c, bits);
	if(m_i - l > 2) {
		// encode left side
		int_code(msg, l, m_i-1);
	}
	if (h - m_i > 2) {
		// encode right side
		int_code(msg, m_i+1, h);
	}
}

char consume_bits(unsigned char *code, int consumed, char base, char l_b) {

}

int int_decode_consume(char *msg, int l, int h, unsigned char *code, int consumed, int codelen) {
	int m_i = (l + h) /2;
	int u_b = msg[h] - (h - m_i);
	int l_b = msg[l] + (m_i - l);
	int bits = bitcnt(u_b - l_b);
	msg[m_i] = consume_bits(code, consumed, bits, msg[l], l_b);
	consumed += bits;
	if(consumed>=codelen) return;
	if(m_i - l > 2) {
		// encode left side
		int con = int_decode_consume(msg, l, m_i-1, code, consumed, codelen);
		consumed-=con;
	}
	if (h - m_i > 2) {
		// encode right side
		int con = int_decode_consune(msg, m_i+1, h, code, consumed, codelen);
		consumed -= con;
	}
	return consumed;
}

// size, first, last, bitcode
int int_decode(int s, int f, int l, unsigned char *code) {
	int msg[] = calloc(s);
	msg[0] = f;
	msg[s-1] = l;
	int_decode_consume(msg, 0, s-1, code, 0);
}

int main(){

	int msg[] = { 2,  3, 4, 5, 6, 8, 10, 11 };
	outbuf = malloc(100);
	int s = sizeof(msg) / sizeof(msg[0]);
	int_code(msg, 0, s-1);
	int last = outbufpos / 8 + 1;
	printf("outbufpos: %d last: %d", outbufpos, last);
	outbuf[last] = '\0';
	printf("\n%d|%d|%d_", s, msg[0], msg[s-1]);
	for (int i=0; i< last; i++) {
		printf("%X", outbuf[i]);
	}
	for (int i=0; i< s; i++) {
		//printf("is it %d\n", msg[i]);
	}
	printf("[\ndone\n");
}
