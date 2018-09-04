#include <iostream>
#include "include/xsea.h"
using namespace std;

int main() {
    Xsea::Document doc("test.xml");
    bool isLoaded = doc.loadFile();
    cout << boolalpha << isLoaded << endl;
    return 0;
}