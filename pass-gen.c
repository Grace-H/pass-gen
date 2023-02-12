/* pass-gen.c - pronouncable password generator
 *
 * author: Grace-H
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int generator(char syll[4]);
int syllable(char syll[4]);

int main(int argc, char **argv){
  int l = 0, s = 2, n = 2, m = 8;
  int c = 0; // copy to clipboard
  
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
  
  printf("password123\n");
  return 0;
}

int generator(char syll[4]){
  int len = 0;
  
  int type = rand() * 3;

  if(type == 0){
    len = syllable(&syll);
  } else if (type == 1) {
    len = 1;
    char[0] = rand() * 10;
  } else if (type == 2) {
    len = 1;
  }

  syll[len] = '\0';

  return len;
}
