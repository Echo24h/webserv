#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <nombre de fois où la commande doit être exécutée>" << std::endl;
        return 1;
    }

    int n = std::stoi(argv[1]);
    for (int i = 0; i < n; ++i) {
        int result = std::system("./request test/delete.txt");
    }

    return 0;
}
