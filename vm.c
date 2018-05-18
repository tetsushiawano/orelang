#include "./mpc.h"
#include <stdio.h>

#define STRUCTURE \
" number : /-?[0-9]+/;                             \n" \
" factor : '(' <lexp> ')'                          \n" \
"        | <number>;                               \n" \
" term   : <factor> (('*'|'/'|'%') <factor>)*;     \n" \
" lext   : <term> (('+'|'-'))*;                    \n"

int main(int argc, char **argv) {
  printf("%d", argc);
}
