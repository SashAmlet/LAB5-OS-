#include <cstdlib>

bool f(int x) {
    return x % 2 == 0; // Повертає true, якщо x - парне
}

int main(int argc, char* argv[]) {

    int x = std::atoi(argv[1]);
    return f(x); // Повертає 1 якщо true, інакше 0
}