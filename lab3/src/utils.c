#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
void GenerateArray(int *array, unsigned int array_size, unsigned int seed) {
  // Запускаем генератор псевдослучайных чисел
  srand(seed);
  for (int i = 0; i < array_size; i++) {
    // Присваиваем случайное число от 0 до RAND_MAX
    array[i] = rand();
  }
}
