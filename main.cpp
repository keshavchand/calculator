#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "stack.cpp"
#define DEFAULT_CHAR_EQ_LEN 1024

struct EquationBuffer{
  // NULL terminated string
  char *equation;
  int capacity;
};

enum Operator{
  PARAN_LEFT,
  PARAN_RIGHT,
  ADD, SUB,
  MUL, DIV,
  COUNT
};


struct Elements{
  int isNum;  
  union{
    int num;
    Operator op;
  };
};

void stripWhiteSpace(EquationBuffer *eq){
  // Not null and is whitespace
  while (eq->equation && isspace(*(eq->equation))){
    eq->equation++;
  }
}

Elements Tokenize(EquationBuffer *eq){
  char current_char = *(eq->equation);
  Elements e = {0};
  if (current_char){
    // in case eq->equation is NULL then
    // we should'nt increase the pointer
    eq->equation++;
    if (current_char == '(') e.op = PARAN_LEFT; 
    else if (current_char == ')') e.op = PARAN_RIGHT; 
    else if (current_char == '/') e.op = DIV; 
    else if (current_char == '*') e.op = MUL;
    else if (current_char == '+') e.op = ADD;
    else if (current_char == '-') e.op = SUB;
    else if (isdigit(current_char)){
      e.isNum = 1;
      int number = current_char - '0';
      int continueParsing = isdigit(*(eq->equation + 1));
      while(continueParsing){
        current_char = *(eq->equation++);
        int digit = current_char - '0';
        number *= 10;
        number += digit;
        if (!isdigit(*(eq->equation))){
          continueParsing = 0;
        }
      }
      e.num = number;
    }else{
      assert(0 && "BAD Equation");
    }
  }

  return e;
}

// This take elements and puts it into stack to 
// create reverse polish notation
void
processElementsIntoStack(Elements e, Stack<Elements> *output, Stack<Elements> *ops){
  // TODO ERROR CHECKING
  if (e.isNum) {
    PushStack(output, e);
    return;
  }

  if (e.op == PARAN_RIGHT){
    StackTop<Elements> top = PopStack(ops);
    while(top.hasValue && !(top.value.op == PARAN_LEFT)){
      PushStack(output, top.value);
      top = PopStack(ops);
    }  
    // if top.hasValue is true then then last one must be paran_left
    if (top.hasValue) return;
    else assert(0 && "BAD EQUATION");
  }
  if (e.op == PARAN_LEFT){
    PushStack(ops, e);
  }else{
    StackTop<Elements> top = PopStack(ops);
    while (top.hasValue && (top.value.op != PARAN_LEFT) && (top.value.op >= e.op)) {
      PushStack(output, top.value);
      top = PopStack(ops);
    }
    if (top.hasValue) PushStack(ops, top.value);
    PushStack(ops, e);
  }
}

Elements
SolveRPN(Elements* es, int size){
  Stack<Elements> result = InitStack( (Elements *) calloc(size, sizeof(Elements)) , size);
  for (int i = 0 ; i < size; i++){
    if (es[i].isNum) PushStack(&result, es[i]);
    else if (es[i].op == MUL) {
      StackTop<Elements> a = PopStack(&result);
      StackTop<Elements> b = PopStack(&result);
      assert((a.hasValue && b.hasValue) && "NOT MANY VALUES IN THE EQUATION");

      Elements e = {
        1, a.value.num * b.value.num
      };
      PushStack(&result, e);
    } else if (es[i].op == DIV) {
      StackTop<Elements> a = PopStack(&result);
      StackTop<Elements> b = PopStack(&result);
      assert((a.hasValue && b.hasValue) && "NOT MANY VALUES IN THE EQUATION");

      Elements e = {
        1, b.value.num / a.value.num
      };
      PushStack(&result, e);
    } else if (es[i].op == ADD) {
      StackTop<Elements> a = PopStack(&result);
      StackTop<Elements> b = PopStack(&result);
      assert((a.hasValue && b.hasValue) && "NOT MANY VALUES IN THE EQUATION");

      Elements e = {
        1, a.value.num + b.value.num
      };
      PushStack(&result, e);
    } else if (es[i].op == SUB) {
      StackTop<Elements> a = PopStack(&result);
      StackTop<Elements> b = PopStack(&result);
      assert((a.hasValue && b.hasValue) && "NOT MANY VALUES IN THE EQUATION");

      Elements e = {
        1, b.value.num - a.value.num
      };
      PushStack(&result, e);
    } 
  }  
  StackTop<Elements> res = PopStack(&result);
  free(result.buffer); 

  assert(res.hasValue && "VALUES LEFT IN STACK");
  Elements e = res.value;
  assert(e.isNum && "FINAL VALUE ISNT A NUMBER");
  return e;
}

int calculate(char *calc_eq){
  Stack<Elements> output = InitStack((Elements *) calloc(1024, sizeof(Elements)), 1024);
  Stack<Elements> ops = InitStack((Elements *) calloc(1024 , sizeof(Elements)), 1024);
  EquationBuffer eqBuffer;
  eqBuffer.equation = calc_eq;
  eqBuffer.capacity = DEFAULT_CHAR_EQ_LEN;
  while (*(eqBuffer.equation)){
    stripWhiteSpace(&eqBuffer);
    Elements el = Tokenize(&eqBuffer);
    processElementsIntoStack(el, &output, &ops);
  }
  MoveStack(&output, &ops);

  free(ops.buffer);
  // NOTE: Rpn = ReversePolishNotation
  Elements* rpn = output.buffer;
  int size = output.stackPointer;
  {
    void* ops;
    void* output;

    Elements e = SolveRPN(rpn, size);
    return e.num;
  }
}

int main(){
  char *calc_eq = (char *)malloc(sizeof(*calc_eq) * DEFAULT_CHAR_EQ_LEN );
#if 0
  if (calc_eq){
    fgets(calc_eq, DEFAULT_CHAR_EQ_LEN, stdin);
    calculate(calc_eq);
    free(calc_eq);
  }
#else
  snprintf(calc_eq, DEFAULT_CHAR_EQ_LEN, "(1 * 2 + 2 * 3) / 2 * 4");
  int result = calculate(calc_eq);
  snprintf(calc_eq, DEFAULT_CHAR_EQ_LEN, "1 + 2 *(2 / 2)");
  result = calculate(calc_eq);
#endif
  return 0;
}
