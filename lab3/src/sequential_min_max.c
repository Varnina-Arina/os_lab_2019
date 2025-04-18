#include <stdio.h>
#include <stdlib.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  // Если количество аргументов не то, что нам нужно (3), выводим подсказку
  if (argc != 3) {
    printf("Usage: %s seed arraysize\n", argv[0]);
    return 1;
  }

  // Проверяем аргументы
  // seed => int > 0
  int seed = atoi(argv[1]);
  if (seed <= 0) {
    printf("seed is a positive number\n");
    return 1;
  }
  // array_size => int > 0
  int array_size = atoi(argv[2]);
  if (array_size <= 0) {
    printf("array_size is a positive number\n");
    return 1;
  }

  // Выделим памяти под массив из array_size int'ов
  int *array = malloc(array_size * sizeof(int));
  // Сгенерируем массив псевдослучайных чисел
  GenerateArray(array, array_size, seed);
  struct MinMax min_max = GetMinMax(array, 0, array_size);
  // Освободим память
  free(array);

  printf("min: %d\n", min_max.min);
  printf("max: %d\n", min_max.max);

  return 0;
}
