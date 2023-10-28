#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *A;
  // void **B;
  unsigned int *occurrencies;
  int last_used;
} Hash2direct;

Hash2direct *h2d_new(/* int unisize */) {
  Hash2direct *h2d = malloc(sizeof(Hash2direct));
  h2d->A = (int *)calloc(256, sizeof(int));
  h2d->occurrencies = NULL;
  h2d->last_used = 0;
  return h2d;
}

void h2d_free(Hash2direct **h2d) {
  free((*h2d)->A);
  free((*h2d)->occurrencies);
  free(*h2d);
}

void h2d_fill_occurrencies(Hash2direct *h2d, char *stream) {
  char c;
  int i = 0;
  while (0 != (c = stream[i++])) {
    printf("FILLING %c %d\n", c, h2d->A[c]);
    h2d->occurrencies[h2d->A[c] - 1]++;
  }
  // at the end, each h2d->occurrencies[h2d->A[c]] contains the occurency of
  // character c
}

void h2d_count_unique(Hash2direct *h2d, char *stream) {
  // count unique symbols
  char c;
  int i = 0;
  while (0 != (c = stream[i++])) {
    if (!h2d->A[c]) {
      h2d->last_used++;
      h2d->A[c] = h2d->last_used;
    }
  }
  // at the end all A[c] != 0 are parts of the universe
}

int h2d_alloc_occ(Hash2direct *h2d) {
  int unisize = h2d->last_used;
  // h2d->B = (void **)calloc(unisize, sizeof(void *));
  h2d->occurrencies = (unsigned int *)calloc(unisize, sizeof(unsigned int));
}

double h2d_get_freq(Hash2direct *h2d, char c) {
  // returns frequency for symbol c
  return (double)h2d->occurrencies[h2d->A[c] - 1] / ((double)h2d->last_used);
}

unsigned int h2d_get_occurency(Hash2direct *h2d, char c) {
  // returns number of occurencies for simbol c
  return h2d->occurrencies[h2d->A[c] - 1];
}

int main() {
  // ciao
  char *input = "aabbc\0";
  Hash2direct *h2d = h2d_new();
  h2d_count_unique(h2d, input);
  h2d_alloc_occ(h2d);
  h2d_fill_occurrencies(h2d, input);
  for (int i = 0; input[i] != 0; i++) {
    printf("%c occurs %d\n", input[i], h2d_get_occurency(h2d, input[i]));
  }
  h2d_free(&h2d);
}
