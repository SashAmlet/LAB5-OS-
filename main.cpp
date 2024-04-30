#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <sys/wait.h>

bool checkValidInput(const char* str) {
    // Перевірка на валідність вводу
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2 || !checkValidInput(argv[1])) {
        std::cerr << "Невірний ввод. Використання: " << argv[0] << " <число>\n";
        return 1;
    }

    int sv[2]; // Socket pair
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) != 0) {
        std::cerr << "Не вдалося створити пару сокетів\n";
        return 1;
    }

    pid_t pid_f = fork();
    if (pid_f == 0) { // Дочірній процес для f
        close(sv[0]);
        execlp("./f", "f", argv[1], NULL);
        exit(1); // execlp поверне управління тільки у випадку помилки
    }

    pid_t pid_g = fork();
    if (pid_g == 0) { // Дочірній процес для g
        close(sv[1]);
        execlp("./g", "g", argv[1], NULL);
        exit(1); // execlp поверне управління тільки у випадку помилки
    }

    // Основний процес
    int status_f, status_g, wait_time = 0;
    bool b1, b2;
    while (true)
    {
        b1 = waitpid(pid_f, &status_f, WNOHANG) == 0;
        b2 = waitpid(pid_g, &status_g, WNOHANG) == 0;
        if (b1 || b2) // Чекаємо завершення f та g
        {
            if (wait_time >= 10) {
                std::cout << "Програма виконується більше 10 секунд. Чи бажаєте ви чекати далі? (y/n): ";
                char response;
                std::cin >> response;
                if (response == 'n' || response == 'N') {
                    if (b1){kill(pid_f, SIGKILL); }
                    if (b2){kill(pid_g, SIGKILL); }
                    
                    std::cout << "Програму було прервано.\n";
                    break;
                }
                wait_time = 0;
            }
        }
        else break;

        ++wait_time;
        sleep(1);
    }

    bool result_f = WIFEXITED(status_f) && WEXITSTATUS(status_f) == 1;
    bool result_g = WIFEXITED(status_g) && WEXITSTATUS(status_g) == 1;

    std::cout << "Результат: " << (result_f && result_g) << std::endl;
    return 0;
}