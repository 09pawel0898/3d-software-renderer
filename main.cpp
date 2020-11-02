#include "3dstuff.hpp"

/*
*       A very basic 3d engine created from scratch with C++ and SFML library
*       whcich allows to display 3d models form *.obj files with the simplest form of lighting
*/

static void hideConsole(void)
{
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, 0);
}

static std::string getObjFileName(void)
{
    std::string fname;
    printf(" \"filename.obj\" \n enter file name according to the template : \n ");
    std::cin >> fname;
    return fname;
}

int main(void)
{

    Stuff3d myApp(getObjFileName());
    //hideConsole();
    myApp.run();

    return 0;
}