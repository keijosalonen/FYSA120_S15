/**
 * @file exercise1_1.cc
 * @brief FYSA120 Harjoitustyö 1.1
 * @author keijo.k.a.salonen@student.jyu.fi
 * @date 5.11.2015
 *
 * Ohjelmointi testi.
 *
 */
#include <iostream>
#include <cmath>

/**
 * Funktion esittely
 */
double f(double);

/**
 * Pääohjelma
 */
int main(void)
{
    double x;
    x = 1.12;
    std::cout << "f(x) = " << f(x) << std::endl;
    return 0;
}

/**
 * Funktio f(x)
 * @param x reaaliluku
 * @return funktion arvo 
 */
double f(double x)
{
    // x^3+2x^4+5x^5
    return std::pow(x,3) + 2.0 * std::pow(x,4) + 5.0 * pow(x,5);
}

