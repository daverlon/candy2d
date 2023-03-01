#include "Game.h"


int main(int argc, char *argv[]) {
    std::cout << "Game started." << std::endl;
    {
        Game mygame;
        mygame.Run();
    }
    std::cout << "Game ended." << std::endl;
    return 0;
}