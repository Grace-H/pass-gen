/* pass-gen.c - pronouncable password generator
 *
 * author: Grace-H
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <openssl/rand.h>

#define RAND_BYTES 4
#define MAX_BEGS 46
#define MAX_MIDS 10
#define MAX_ENDS 42
#define MAX_SYMS 32

int generator(char *syll, int n, int m);
int syllable(char *syll, int n);

char begs[46][3] = {
  "b\0", "c\0","d\0", "f\0", "g\0", "h\0", "j\0", "k\0", "l\0", "m\0",
  "n\0", "p\0", "q\0", "r\0", "s\0", "t\0", "v\0", "w\0", "x\0", "y\0",
  "z\0","sh\0", "bl\0", "fl\0", "gl\0", "pl\0", "sl\0", "br\0", "cr\0", "dr\0",
  "fr\0", "gr\0", "pr\0", "tr\0", "sc\0", "sk\0", "sm\0", "sn\0", "sw\0", "tw\0",
  "ch\0", "th\0", "cl\0", "sp\0", "st\0", "dw\0"  };
char mids[10][3] = {
  "a\0", "e\0", "i\0", "o\0", "u\0", "ai\0", "ee\0", "oo\0", "ou\0", "ea\0" };
char ends[42][3] = {
  "b\0", "c\0", "d\0", "f\0", "g\0", "h\0", "j\0", "k\0", "l\0", "m\0",
  "n\0", "p\0", "r\0", "s\0", "t\0", "v\0", "w\0", "x\0", "z\0", "sh\0",
  "rb\0", "rd\0", "rd\0", "sk\0", "sh\0", "ch\0", "th\0", "rm\0", "st\0", "lk\0",
  "ll\0", "ck\0", "ft\0", "lt\0", "nt\0", "ld\0", "lp\0", "nk\0", "sp\0", "mp\0",
  "zz\0", "ff\0" };
char syms[32][2] = {
  "!\0", "\"\0", "#\0", "$\0", "%\0", "&\0", "'\0", "(\0", ")\0", "*\0",
  "+\0", ",\0", "-\0", ".\0", "/\0", ":\0", ";\0", "<\0", "=\0", ">\0",
  "?\0", "@\0", "[\0", "\\\0", "]\0", "^\0", "_\0", "`\0", "{\0", "|\0",
  "}\0", "~\0" };

int main(int argc, char **argv){
  int l = 0, s = 2, m = 8, n = 24;
  int c = 0; // copy to clipboard
  
  int opt;
  while((opt = getopt(argc, argv, ":l:s:n:m:ch")) != -1){
    switch(opt){
    case 'h':
      printf("helping you...\n");
      break;
    case 'l':
      l = atoi(optarg);
      break;
    case 's':
      s = atoi(optarg);
      break;
    case 'n':
      n = atoi(optarg);
      break;
    case 'm':
      m = atoi(optarg);
      break;
    case ':':
      fprintf(stderr, "Missing argument for option '-%c'\n", optopt);
      return 1;
    case '?':
      fprintf(stderr, "Unexpected option '-%c'\n", optopt);
      return 1;
    }
  }

  char pass[n];
  if (generator(pass, n, m) < 0){
    fprintf(stderr, "generator\n");
  }
  fprintf(stdout, "%s\n", pass);
  return 0;
}

// returns length of generated segment, negative on failure
int generator(char *syll, int n, int m) {
  int type, ret, i = 0, len = 0;
  unsigned char buf[RAND_BYTES];

  while (i < m && len < n) {
    ret = RAND_bytes(buf, RAND_BYTES);
    if (ret < 0) {
      fprintf(stderr, "RAND_bytes: too few rand bytes\n");
      return -1;
    }
    
    // TODO casting to int only casts first byte
    type = (int) *buf % 3;
    fprintf(stderr, "type: %d\n", type);
    if (type == 0) {
      int b = syllable(&syll[len], n - len);
      if (b > 0)
	len += b; // only advance len if remaining space was big enough
    } else if (type == 1) {
      ret = RAND_bytes(buf, RAND_BYTES);
      if (ret < 0)
	return -1;
      syll[len++] = 48 + (int) *buf % 10;
    } else if (type == 2) { // symbol	
	ret = RAND_bytes(buf, RAND_BYTES);
	if (ret < 0)
	  return -1;
	syll[len++] = syms[(int) *buf % MAX_SYMS][0];
    }
    i++;
  }

  syll[len] = '\0';

  return len;
}

// generate alpha syllable
// params: syll (recommended min length 7)
// returns length of syllable, negative on failure
int syllable(char *syll, int n){
  int len = 0;
  unsigned char buf[RAND_BYTES];

  // beginning
  int ret = RAND_bytes(buf, RAND_BYTES);
  if (ret < 0)
    return -1;

  int begi = (int) *buf % MAX_BEGS;
  int begz = strlen(begs[begi]);

  // middle
  ret = RAND_bytes(buf, RAND_BYTES);
  if (ret < 0)
    return -1;

  int midi = (int) *buf % MAX_MIDS;
  int midz = strlen(mids[midi]);

  // end
  ret = RAND_bytes(buf, RAND_BYTES);
  if (ret < 0)
    return -1;

  int endi = (int) *buf % MAX_ENDS;
  int endz = strlen(ends[endi]);

  // assemble
  if (len + begz + midz + endz < n) {
    strncpy(&syll[len], begs[begi], begz);
    len += begz;
    strncpy(&syll[len], mids[midi], midz);
    len += midz;
    strncpy(&syll[len], ends[endi], endz);
    len += endz;
  } else {
    fprintf(stderr, "segment too short\n");
    return -1;
  }
  syll[0] = toupper(syll[0]);
  syll[len] = '\0';

  return len;
}
