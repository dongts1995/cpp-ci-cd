#include "Calculator.h"
#include <iostream>

int main()
{
    Calculator calculator;

    std::cout << "2 + 3 = "
              << calculator.add(2, 3)
              << std::endl;

    return 0;
}