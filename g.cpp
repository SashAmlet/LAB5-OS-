#include <cstdlib>

bool g(int x) {
    return x > 5; // Повертає true, якщо x більше 5
}

int main(int argc, char* argv[]) {

    int x = std::atoi(argv[1]);
    return g(x); // Повертає 1 якщо true, інакше 0
}