/**
 * @file exercise4.cc
 * @brief FYSA120 Harjoitustyö 4
 * @author keijo.k.a.salonen@student.jyu.fi
 * @date 25.11.2015
 *
 * Numeerinen integrointi GSL kirjastolla.
 *
 * @note
 *
 * clang++ -std=c++11 -I/usr/local/include ./exercise4.cc -o ./ex4 -L/usr/local/lib -lgsl -lgslcblas 
 *
 */
#include <iostream>
#include <string>
#include <cmath>
#include <gsl/gsl_integration.h>
#include <boost/math/constants/constants.hpp>

// Integroinnille varattu työtilan koko
#define LIMIT_SIZE 1000

/**
 * fysa120 nimiavaruus
 */
namespace fysa120
{
    /*
     * C++11 ei pidä sisällään pi:tä, joten käytetään boostia.
     */
    const double pi = boost::math::constants::pi<double>();

    /**
     * Tietorakenne parametrien välittämiseen GSL:lle.
     */
    struct parametrit
    {
        double alfa;
        double beeta;
    };
    

    /**
     * Integroitava funktio.
     *
     * @param x Riippumaton muuttuja 
     * @param cparam Osoitin parametreihin
     * @return Funktion arvo cos(sin(a*x+b)) 
     */
    double f(double x, void* cparam)
    {
        /* reinterpret_cast < new_type > ( expression )     
         *
         * reinterpret_cast expression does not compile to any CPU instructions. 
         * It is purely a compiler directive which instructs the compiler 
         * to treat the sequence of bits (object representation) of expression
         * as if it had the type new_type.
         */ 
        parametrit &par = *reinterpret_cast<parametrit *>(cparam);
        double a = par.alfa;
        double b = par.beeta;
        return std::cos(std::sin(a*x+b));
    }


    /**
     * Suorittaa varsinaisen integroinnin.
     */
    void integroi(void)
    {
        gsl_integration_workspace *work_ptr = gsl_integration_workspace_alloc(LIMIT_SIZE);
        
        double alaraja = 0.0;
        double ylaraja = pi/4.0;
        double abs_virhe = 1.0e-8;
        double suht_virhe = 1.0e-8;
        double vastaus;
        double virhe;

        parametrit p;
        p.alfa = 5.6;
        p.beeta = 2.2;
        void *cparam = reinterpret_cast<void *>(&p);

        gsl_function funktio;

        funktio.function = &f;
        funktio.params = cparam;

        gsl_integration_qags(&funktio, alaraja, ylaraja, abs_virhe, suht_virhe, 
                                        LIMIT_SIZE, work_ptr, &vastaus, &virhe);

        std::cout.setf(std::ios::fixed, std::ios::floatfield);
        std::cout.precision(18);
        std::cout << "Vastaus = " << vastaus << std::endl;

        gsl_integration_workspace_free(work_ptr);
    }
}

/**
 * Pääohjelma testaamista varten.
 */
int main(void)
{
    //
    fysa120::integroi();
    return 0;
}

