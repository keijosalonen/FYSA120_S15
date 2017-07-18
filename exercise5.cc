/**
 * @file exercise5.cc
 * @brief FYSA120 Harjoitustyö 5
 * @author keijo.k.a.salonen@student.jyu.fi
 * @date 10.12.2015
 *
 * B-spline sovitus
 *
 * @note
 *
 * clang++ -std=c++11 -I/usr/local/include ./exercise5.cc -o ./ex5 -L/usr/local/lib -larmadillo 
 *
 */
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <armadillo>
#include "Bspline.hpp"


/**
 * fysa120 nimiavaruus
 */
namespace fysa120
{
    
   /**
    * Generoi dataa johon tehdään b-spline sovitus.
    * Data talletetaan tiedostoon: xy_data.dat
    * Sovitettu käyrä talletetaan tiedostoon: fit_data.dat 
    */
    void suorita_bspline_sovitus(void)
    {
        // datapisteiden määrä
        const int N=100;
        
        // alustetaan b-spline
        std::vector<double> t{0,1,2,3,4,4,4,5,6,7,8,9,10};
        my::Bspline<decltype(t)> bs(t);
        
        // muodostetaan data välille 0-10
        arma::vec x(N);
        arma::vec y(N);
        for(int i = 0 ; i < N ; ++i)
        {
            double xx = i*10.0/(N-1);
            x[i] = xx;
            auto noise = 0.2*(std::rand()*1.0/RAND_MAX-0.5);
            y[i] = 3*std::exp(-std::abs(xx-4))+noise;
        }

        // muodostetaan kerroinmatriisi A: A_ij = B_j(x_i)
        arma::mat A;
        A.set_size(x.size(),bs.max_i());
        for(int j = 0 ; j < x.size() ; ++j)
        {
            for(int i = 0 ; i < bs.max_i() ; ++i)
            {
                A(j,i) = bs.B(i,x[j]);
            }
        }
    
        // ratkaistaan kerroinmatriisi c (vektori)
        arma::vec c = solve(A,y);

        // tehdään x pisteet sovituskäyrää varten (1000 pistettä välille 0-10)
        arma::vec X(1000); 
        for(int i = 0 ; i < 1000 ; ++i)
        {
            double xx = i*10.0/(1000-1);
            X[i] = xx;
        }

        // muodostetaan kerroinmatriisi B sovituskäyrää varten
        arma::mat B;
        B.set_size(X.size(),bs.max_i());
        for(int j = 0 ; j < X.size() ; ++j)
        {
            for(int i = 0 ; i < bs.max_i() ; ++i)
            {
                B(j,i) = bs.B(i,X[j]);
            }
        }

        // Muodostetaan sovituskäyrän y-koordinaatti
        arma::vec Y = B * c;
    
        // kirjoitetaan tulokset tiedostoihin
        // Data
        std::ofstream out("xy_data.dat");
        for(int i = 0 ; i < x.size() ; ++i)
        {
            out << x[i] << " " << y[i] << std::endl;
        }
        out.close();
    
        // sovituskäyrä
        std::ofstream out2("fit_data.dat");
        for(int i = 0 ; i < X.size() ; ++i)
        {
            out2 << X[i] << " " << Y[i] << std::endl;
        }
        out2.close();
    }
}


/**
 * Pääohjelma testaamista varten
 */
int main(void)
{
    fysa120::suorita_bspline_sovitus();
    return 0;
}

/*
gnuplot:

# gnuplot komentotiedosto
#
# load "piirra_bspline.gp"
# 10.12.2015
#
set term png
set output "kuvaaja.png"
set title "B-spline sovitus"
set xlabel "x"
set ylabel "y"
set xrange [0 to 10]
set yrange [-0.5 to 3.5]
plot "xy_data.dat" title "data", "fit_data.dat" title "B-spline fit" pt 7 ps .2


*/