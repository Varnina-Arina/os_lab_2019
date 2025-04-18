#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char **argv) {
    pid_t pid = fork();
    
    if (pid == 0)
    {
        // Мы в дочернем процессе
        // Вдруг нужный исполняемый файл лежит в PATH
        int errcode;
        execvp("seq_min_max", argv);
        // "The exec() functions return only if an error has occurred"
        errcode = errno;
        perror("seq_min_max");
        if (errcode != ENOENT)
            return 1;
        printf("Поиск в PATH не удался, пробуем найти в текущей директории\n");
        // Или в текущей директории
        execv("./seq_min_max", argv);
        errcode = errno;
        perror("./seq_min_max");
        if (errcode != ENOENT)
            return 1;
        printf("Поиск в текущей директории не удался, пробуем найти по абсолютному пути\n");
        // На худой конец, попробуем абсолютный путь
        execv("/workspaces/os_lab_2019/lab3/bin/seq_min_max", argv);
        errcode = errno;
        perror("/workspaces/os_lab_2019/lab3/bin/seq_min_max");
        if (errcode == ENOENT) printf("Файла нигде нет.\n");
        return 1;
    }
    else
    {
        int status;
        wait(&status);
        printf("Процесс заверешен с кодом %d\n", status);
    }
    return 0;
}
