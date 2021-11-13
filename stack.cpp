#ifndef STACK_H
#define STACK_H

template <typename T>
struct Stack{
  T *buffer;
  int stackPointer;
  int capacity;
};

template <typename T>
struct StackTop{
  int hasValue;
  T value;
};

template <typename T>
Stack<T> InitStack(T *buffer, int capacity){
  int stackPointer = 0;
  return {
    buffer, stackPointer, capacity
  };
}

// if return value is 2 then that means stack if full
// if return value is 0 then no action is performed
template <typename T>
int PushStack(Stack<T> *stack, T item){
  if (stack->stackPointer >= stack->capacity){
    return 0;
  }
  T* location = stack->buffer + stack->stackPointer;  
  *location = item;
  stack->stackPointer += 1;
  if (stack->stackPointer >= stack->capacity) return 2;

  return 1;
}

template <typename T>
StackTop<T> PopStack(Stack<T> *stack){
  StackTop<T> result = {0};
  if (stack->stackPointer == 0){
    result.hasValue = 0; 
    return result;
  }
  result.hasValue = 1;
  result.value = *(stack->buffer + stack->stackPointer - 1);
  stack->stackPointer -= 1;
  return result;
}

template <typename T>
StackTop<T> PeekStack(Stack<T> *stack){
  StackTop<T> result = {0};
  if (stack->stackPointer == 0){
    result.hasValue = 0; 
    return result;
  }
  result.hasValue = 1;
  result.value = *(stack->buffer + stack->stackPointer - 1);
  return result;
}

template <typename T>
void MoveStack(Stack<T> *dst, Stack<T> *src){
  StackTop<T> stackItem = PopStack(src);
  while ( stackItem.hasValue ){
    PushStack(dst, stackItem.value);
    stackItem = PopStack(src);
  }

  return;
}

#endif //STACK_H
