#include "./mpc.h"
#include <stdio.h>

#define STRUCTURE \
" number : /-?[0-9]+/;                             \n" \
" factor : '(' <lexp> ')'                          \n" \
"        | <number>;                               \n" \
" term   : <factor> (('*'|'/'|'%') <factor>)*;     \n" \
" lexp   : <term> (('+'|'-') <term>)*;             \n"

#define is_a(t, a) (strstr(t->tag, a) != NULL)

int eval(mpc_ast_t* t) {
  if (is_a(t, "number")) {
    return atoi(t -> contents);
  }
  if (is_a(t, "factor")) {
    return eval(t -> children[1]);
  }
  if (t -> children_num >= 1) {
    int x = eval(t -> children[0]), i;
    for (i = 1; i < t -> children_num; i += 2) {
      char* op = t -> children[i] -> contents;
      int rhs = eval(t -> children[i+1]);
      if (strcmp(op, "+") == 0) { x += rhs; }
      if (strcmp(op, "-") == 0) { x -= rhs; }
      if (strcmp(op, "*") == 0) { x *= rhs; }
      if (strcmp(op, "/") == 0) { x /= rhs; }
      if (strcmp(op, "%") == 0) { x %= rhs; }
    }
    return x;
  }
  return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage of %s: expr\n", argv[0]);
  }
  mpc_result_t result;
  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Factor = mpc_new("factor");
  mpc_parser_t* Term   = mpc_new("term");
  mpc_parser_t* Lexp   = mpc_new("lexp");

  mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT, STRUCTURE, Number, Factor, Term, Lexp);

  if (err != NULL) {
    mpc_err_print(err);
    mpc_err_delete(err);
    goto leave;
  }

  if (!mpc_parse("<argument>", argv[1], Lexp, &result)) {
    mpc_err_print(result.error);
    mpc_err_delete(result.error);
    goto leave;
  }

  //mpc_ast_print(result.output);
  printf("%d\n", eval(result.output));
  mpc_ast_delete(result.output);

leave:
  mpc_cleanup(4, Number, Factor, Term, Lexp);
}
