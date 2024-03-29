/*
 * pass-gen.c - pronouncable password generator
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

int generator(char *buf, int n, int d, int s);
int syllable(char *syll, int n);

char *short_help = "Usage: pass-gen [-n length] [-d numbers] [-s symbols]\n";
char *options = "Options:\n"
                " -h\tShow this text.\n"
                " -n\tLength of password. [default: 16]\n"
                " -d\tMinimum numbers in password.\n"
                " -s\tMinimum symbols in password.";

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
  int n = 16, d = 0, s = 0;

  int opt;
  while((opt = getopt(argc, argv, ":l:s:n:d:h")) != -1){
    switch(opt){
    case 'h':
      puts(short_help);
      puts(options);
      return 0;
    case 's':
      s = atoi(optarg);
      break;
    case 'n':
      n = atoi(optarg);
      break;
    case 'd':
      d = atoi(optarg);
      break;
    case ':':
      fprintf(stderr, "Missing argument for option '-%c'\n", optopt);
      fputs(short_help, stderr);
      return 1;
    case '?':
      fprintf(stderr, "Invalid option '-%c'\n", optopt);
      fputs(short_help, stderr);
      return 1;
    }
  }

  char pass[n + 1];  // n + 1 for '\0'
  int len = generator(pass, n + 1, d, s);
  if (len < 0){
    fprintf(stderr, "generator error\n");
    return 1;
  }

  fprintf(stdout, "%s\n", pass);
  return 0;
}

// returns length of generated segment, negative on failure
int generator(char *pass, int n, int d, int s) {
  unsigned int type;
  int ret, len;
  int d_count, s_count;
  unsigned char buf[RAND_BYTES];

  // Keep generating passwords until syllable/digit requirements met
  do {
    d_count = 0, s_count = 0;
    len = 0;

    while (len < n) {

      ret = RAND_priv_bytes(buf, RAND_BYTES);
      if (ret != 1) {
        return -1;
      }
      type = *(unsigned int *) buf % 4;

      if (type == 0) {                          // number
        d_count++;
        ret = RAND_priv_bytes(buf, RAND_BYTES);
        if (ret != 1)
          return -1;
        pass[len++] = 48 + *(unsigned int *) buf % 10;
      } else if (type == 1) {                   // symbol
        s_count++;
        ret = RAND_priv_bytes(buf, RAND_BYTES);
        if (ret != 1)
          return -1;
        pass[len++] = syms[*(unsigned int *) buf % MAX_SYMS][0];
      } else {                                  // syllable
        int b = syllable(&pass[len], n - len);
        if (b > 0)
          len += b; // only advance len if remaining space was big enough
      }
    }

    pass[len] = '\0';

  } while (d_count < d || s_count < s);

  return len;
}

// generate alpha syllable
// params: syll (recommended min length 7)
// returns length of syllable, negative on failure
int syllable(char *syll, int n) {
  unsigned char buf[RAND_BYTES];

  // beginning
  int ret = RAND_priv_bytes(buf, RAND_BYTES);
  if (ret != 1) {
    perror("RAND_priv_bytes");
    return -1;
  }

  int begi = *(unsigned int *) buf % MAX_BEGS;
  int begz = strlen(begs[begi]);

  // middle
  ret = RAND_priv_bytes(buf, RAND_BYTES);
  if (ret != 1) {
    perror("RAND_priv_bytes");
    return -1;
  }

  int midi = *(unsigned int *) buf % MAX_MIDS;
  int midz = strlen(mids[midi]);

  // end
  ret = RAND_priv_bytes(buf, RAND_BYTES);
  if (ret != 1) {
    perror("RAND_priv_bytes");
    return -1;
  }

  int endi = *(unsigned int *) buf % MAX_ENDS;
  int endz = strlen(ends[endi]);

  // assemble
  if (begz + midz + endz > n - 1) {
    return -1;
  }

  int len = 0;
  strncpy(&syll[len], begs[begi], begz);
  len += begz;
  strncpy(&syll[len], mids[midi], midz);
  len += midz;
  strncpy(&syll[len], ends[endi], endz);
  len += endz;

  syll[0] = toupper(syll[0]);
  syll[len] = '\0';

  return len;
}
