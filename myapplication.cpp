#include "myapplication.h"

MyApplication::MyApplication(int &argc, char **argv)
    :QApplication(argc, argv)
{
    this->screenAdded()
}
