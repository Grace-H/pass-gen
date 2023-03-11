/* pass-gen.c - pronouncable password generator
 *
 * author: Grace-H
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rand.h>

#define RAND_BYTES 4
#define MAX_BEGS 46
#define MAX_MIDS 12
#define MAX_ENDS 45

char begs[MAX_BEGS][3] = {
  "b\0", "c\0","d\0", "f\0", "g\0", "h\0", "j\0", "k\0", "l\0", "m\0",
  "n\0", "p\0", "q\0", "r\0", "s\0", "t\0", "v\0", "w\0", "x\0", "y\0",
  "z\0","sh\0", "bl\0", "fl\0", "gl\0", "pl\0", "sl\0", "br\0", "cr\0", "dr\0",
  "fr\0", "gr\0", "pr\0", "tr\0", "sc\0", "sk\0", "sm\0", "sn\0", "sw\0", "tw\0",
  "ch\0", "th\0", "cl\0", "sp\0", "st\0", "dw\0"  };
char mids[MAX_MIDS][3] = {
  "a\0", "e\0", "i\0", "o\0", "u\0", "ai\0", "ee\0", "oo\0", "ow\0", "ou\0",
  "ea\0", "ie\0"  };
char ends[MAX_ENDS][3] = {
    "b\0", "c\0", "d\0", "f\0", "g\0", "h\0", "j\0", "k\0", "l\0", "m\0",
    "n\0", "p\0", "r\0", "s\0", "t\0", "v\0", "w\0", "x\0", "z\0", "sh\0",
    "rb\0", "rd\0", "rd\0", "sk\0", "sh\0", "ch\0", "th\0", "rm\0", "en\0", "st\0",
    "lk\0", "ll\0", "ck\0", "ft\0", "lt\0", "nt\0", "xt\0", "lb\0", "ld\0", "lp\0",
    "nk\0", "sp\0", "mp\0", "zz\0", "ff\0" };

int generator(char *syll, int n);
int syllable(char *syll, int n);

int main(int argc, char **argv){
  int l = 0, s = 2, n = 2, m = 8;
  int c = 0; // copy to clipboard
  char pass[16];
  
  int opt;
  while((opt = getopt(argc, argv, ":l:s:n:ch")) != -1){
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
      break;
    }
  }

  generator(pass, 16);
  printf("%s\n", pass);
  return 0;
}

// returns length of generated segment, negative on failure
int generator(char *syll, int n) {
  int len = 0;
  unsigned char buf[RAND_BYTES];
  
  int ret = RAND_bytes(buf, RAND_BYTES);
  if (ret < 1)
    return -1;

  int type = atoi(buf);
  
  if(type == 0){
    len = syllable(syll);
  } else if (type == 1) {
    len = 1;
    syll[0] = rand() % 10;
  } else if (type == 2) {
    len = 1;
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
  if (ret < 1)
    return -1;

  int begi = (int) *buf % MAX_BEGS;
  int begz = strlen(begs[begi]);

  // middle
  ret = RAND_bytes(buf, RAND_BYTES);
  if (ret < 1)
    return -1;

  int midi = (int) *buf % MAX_MIDS;
  int midz = strlen(mids[midi]);

  // end
  ret = RAND_bytes(buf, RAND_BYTES);
  if (ret < 1)
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
  syll[len] = '\0';

  return len;
}
