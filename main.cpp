#include "./src/Image.h"
#include <iostream>

int main(int argc, char** argv)
{
    Image test("test.jpg");
    test.write("SecretMessage.png");

    test.encodeMessage("Toto je fero");

    char buffer[10000] = {0};
    std::size_t len = 0;
    test.decodeMessage(buffer, len);
    std::cout << "Message: " << buffer << " " << len << std::endl;

    return 0;
}