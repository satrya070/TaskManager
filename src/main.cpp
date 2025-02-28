#include <iostream>

int main() {
    int choice;
    //std::cin >> choice;
    choice = 1;

    switch (choice) {
        case 0:
            std::cout << "show tasks"; break;
        case 1:
            std::cout << "add task"; break;
        case 2:
            std::cout << "delete task"; break;
    } 

    return 0;
}