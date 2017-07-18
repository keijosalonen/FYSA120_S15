/**
 * @file exercise1_2.cc
 * @brief FYSA120 harjoitustyö 1.2
 * @author keijo.k.a.salonen@student.jyu.fi
 * @date 5.11.2015
 *
 * Funktio joka ratkaisee f(x)=0 mille tahansa jatkuvalle f(x):lle.
 *
 */
#include <iostream>
#include <cmath>

/**
 * fysa120 nimiavaruus
 */
namespace fysa120
{

    /**
     * Jatkuva funktio f(x), jolle etsitään nollakohtaa.
     *
     * @param x reaaliluku
     * @return palauttaa alkeisfunktion arvon x:llä
     */
    double f(double x)
    {
        //return std::sin(x);
        return (std::sin(x) * (std::pow(x,2) + 2 * x));
    }

   /**
    * Jatkuvan funktion derivaatta f'(x), jolle etsitään nollakohtaa.
    *
    * @param x reaaliluku
    * @return palauttaa alkeisfunktion derivaatan arvon x:llä
    */
   double fder(double x)
   {
       //return std::cos(x);
       return (std::cos(x) * (std::pow(x,2) + 2 * x) + std::sin(x) * (2 * x + 2));
   }


    /**
     * Etsii jatkuvan funktion nollakohdan mid-point menetelmä.
     *
     * @param f funktio jonka nollakohtaa etsitään
     * @param a alaraja
     * @param b yläraja
     * @param eps etsittävän lopputuloksen tarkkuus
     * @param x löydetty nollakohta
     * @return true jos nollakohtaa ei löydy
     */
    bool findroot(const std::function<double(double)> &f,const double &a,const double &b, const double &eps, double &x )
    {
        // Tarkistetaan, että nollakohta on olemassa etsityllä alueella
        double x1 = a;
        double x2 = b;
        if( (f(x1)*f(x2)) < 0)
        {
            // nollakohta on olemassa
            double p = (x1+x2)/2.0;
            while(!(std::abs(f(p)) < eps))
            {
                if(f(p)*f(x2) < 0)
                {
                    x1 = p;
                }
                else
                {
                    x2 = p;
                }

                p = (x1+x2)/2.0;
                // tarkistetaan ettei ajauduta rajojen ulkopuolelle
                if( p < a || p > b)
                {
                    return true;
                }
            }
            x = p;
            return false;
        }
        return true;
    }
	
   /**
    * Etsii jatkuvan funktion nollakohdan Newton–Raphson menetelmällä.
    *
    * @param f funktio jonka nollakohtaa etsitään
	* @param fder funktion derivaatta
    * @param a alaraja
    * @param b yläraja
    * @param eps etsittävän lopputuloksen tarkkuus
    * @param x löydetty nollakohta
    * @return true jos nollakohtaa ei löydy
    */
   bool findroot(const std::function<double(double)> &f, const std::function<double(double)> &fder,const double &a,const double &b, const double &eps, double &x )
   {
       // Tarkistetaan, että nollakohta on olemassa etsityllä alueella
       double x1 = a;
       double x2 = b;
       if( (f(x1)*f(x2)) < 0)
       {
           // nollakohta on olemassa
           double p = (x1+x2)/2.0;
           while(!(std::abs(f(p)) < eps))
           {
               p = p - f(p)/fder(p);
               // tarkistetaan ettei ajauduta rajojen ulkopuolelle
               if( p < a || p > b)
               {
                   return true;
               }
           }
           x = p;
           return false;
       }
       return true;
   }
	
}

/**
 * Pääohjelma testaamista varten
 */
int main(void)
{
    std::cout << "FYSA120 Exercise1_2" << std::endl;
    std::cout << "f(x) = sin(x)(x^2+2x)" << std::endl;
    std::cout << "f'(x) = cos(x)(x^2+2x)+sin(x)(2x+2)" << std::endl;
    std::cout << "f(1) = 2.52441 => " << fysa120::f(1) << std::endl;
    std::cout << "f'(1) = 4.98679 => " << fysa120::fder(1) << std::endl;
    double x;
    fysa120::findroot(fysa120::f, -0.9, 0.8, 0.00001, x);
    std::cout << "1a: x = " << x << " f(x) = " << fysa120::f(x) << std::endl;
    fysa120::findroot(fysa120::f, fysa120::fder, -0.9, 0.8, 0.00001, x);
    std::cout << "1b: x = " << x << " f(x) = " << fysa120::f(x) << std::endl;
    
    fysa120::findroot(fysa120::f, 2.5, 4.0, 0.00001, x);
    std::cout << "2a: x = " << x << " f(x) = " << fysa120::f(x) << std::endl;
    fysa120::findroot(fysa120::f, fysa120::fder, 2.5, 4.0, 0.00001, x);
    std::cout << "2b: x = " << x << " f(x) = " << fysa120::f(x) << std::endl;
    
    fysa120::findroot(fysa120::f, -3.0, -1.5, 0.00001, x);
    std::cout << "3a: x = " << x << " f(x) = " << fysa120::f(x) << std::endl;
    fysa120::findroot(fysa120::f, fysa120::fder, -3.0, -1.5, 0.00001, x);
    std::cout << "3b: x = " << x << " f(x) = " << fysa120::f(x) << std::endl;
    
    fysa120::findroot(fysa120::f, 6.0, 7.0, 0.00001, x);
    std::cout << "4a: x = " << x << " f(x) = " << fysa120::f(x) << std::endl;
    fysa120::findroot(fysa120::f, fysa120::fder, 6.0, 7.0, 0.00001, x);
    std::cout << "4b: x = " << x << " f(x) = " << fysa120::f(x) << std::endl;
    
    return 0;
}

