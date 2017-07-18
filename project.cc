/**
 * @file project.cc
 * @brief FYSA120 Harjoitusprojekti
 * @author keijo.k.a.salonen@student.jyu.fi
 * @date 22.12.2015
 *
 * Linear algebra with armadillo library combined with GSL integration.
 *
 * @note
 *
 * clang++ -std=c++11 -I/usr/local/include ./project.cc -o ./prj -L/usr/local/lib -lgsl -lgslcblas -larmadillo
 *
 */
#include <iostream>
#include <cmath>
#include <gsl/gsl_integration.h>
#include <armadillo>
#include <complex>

// Integroinnille varattu työtilan koko
#define LIMIT_SIZE 1000

/**
 * fysa120 nimiavaruus
 */
namespace fysa120
{
    /**
     * Parametrit GSL:lle
     */
    struct parametrit
    {
        double l;
        double k;
    };
    
    
    /**
     * Integroitava funktio 1.
     *
     * @param x Riippumaton funktio
     * @param cparam Osoitin parametreihin l ja k
     * @return Funktion arvo x^2/(x+k+l)
     */
    double f1(double x, void* cparam)
    {
        parametrit &par = *reinterpret_cast<parametrit *>(cparam);
        double l = par.l;
        double k = par.k;
        return std::pow(x,2)/(x+k+l);
    }
    
    
    /**
     * Integroitava funktio 2 reaaliosa.
     *
     * @param x Riippumaton funktio
     * @param cparam Osoitin parametreihin l ja k
     * @return Funktion arvo (x+2)*exp(x)/(x^2+4*x+4+l^2)
     */
    double f2_real(double x, void* cparam)
    {
        parametrit &par = *reinterpret_cast<parametrit *>(cparam);
        double l = par.l;
        double k = par.k;
        return (x+2)*std::exp(x)/(std::pow(x,2)+4*x+4+std::pow(l,2));
    }
    
    
    /**
     * Integroitava funktio 2 imaginääriosa.
     *
     * @param x Riippumaton funktio
     * @param cparam Osoitin parametreihin l ja k
     * @return Funktion arvo [l*exp(x)/(x^2+4*x+4+l^2)]*i
     */
    double f2_img(double x, void* cparam)
    {
        parametrit &par = *reinterpret_cast<parametrit *>(cparam);
        double l = par.l;
        double k = par.k;
        return (-l)*std::exp(x)/(std::pow(x,2)+4*x+4+std::pow(l,2));
    }
    
    
    /**
     * Suorittaa varsinaisen integroinnin funktiolle f1 (vain reaaliarvoinen).
     * @param l Parametri l
     * @param k Parametri k
     * @return Integroinnin \int_{k}^{l+1}\frac{x^2}{x+k+l}dx tulos kompleksilukuna
     */
    std::complex<double> integroi_f1(double l, double k)
    {
        gsl_integration_workspace *work_ptr = gsl_integration_workspace_alloc(LIMIT_SIZE);
        
        double alaraja = k;
        double ylaraja = l+1.0;
        double abs_virhe = 1.0e-8;
        double suht_virhe = 1.0e-8;
        double vastaus;
        double virhe;
        
        parametrit p;
        p.l = l;
        p.k = k;
        void *cparam = reinterpret_cast<void *>(&p);
        
        gsl_function funktio;
        
        funktio.function = &f1;
        funktio.params = cparam;
        
        gsl_integration_qags(&funktio, alaraja, ylaraja, abs_virhe, suht_virhe, 
                                     LIMIT_SIZE, work_ptr, &vastaus, &virhe);
        
        gsl_integration_workspace_free(work_ptr);
        std::complex<double> z(vastaus,0.0); 
        return z;
    }
    
    
    /**
     * Suorittaa varsinaisen integroinnin funktion f2 reaaliosalle.
     * @param l Parametri l
     * @param k Parametri k
     * @return Integroinnin \int_{k}^{k+1}\frac{(x+2)e^x}{x^2+4x+4+l^2}dx tulos
     */
    double integroi_f2_real(double l, double k)
    {
        gsl_integration_workspace *work_ptr = gsl_integration_workspace_alloc(LIMIT_SIZE);
        
        double alaraja = k;
        double ylaraja = k+1.0;
        double abs_virhe = 1.0e-8;
        double suht_virhe = 1.0e-8;
        double vastaus;
        double virhe;
        
        parametrit p;
        p.l = l;
        p.k = k;
        void *cparam = reinterpret_cast<void *>(&p);
        
        gsl_function funktio;
        
        funktio.function = &f2_real;
        funktio.params = cparam;
        
        gsl_integration_qags(&funktio, alaraja, ylaraja, abs_virhe, suht_virhe, 
                                     LIMIT_SIZE, work_ptr, &vastaus, &virhe);
        
        gsl_integration_workspace_free(work_ptr);
        
        return vastaus;
    }
   
   
    /**
     * Suorittaa varsinaisen integroinnin funktion f2 imaginääriosalle.
     * @param l Parametri l
     * @param k Parametri k
     * @return Integroinnin \int_{k}^{k+1}\frac{le^x}{x^2+4x+4+l^2}idx tulos
     */
    double integroi_f2_img(double l, double k)
    {
        gsl_integration_workspace *work_ptr = gsl_integration_workspace_alloc(LIMIT_SIZE);
        
        double alaraja = k;
        double ylaraja = k+1.0;
        double abs_virhe = 1.0e-8;
        double suht_virhe = 1.0e-8;
        double vastaus;
        double virhe;
        
        parametrit p;
        p.l = l;
        p.k = k;
        void *cparam = reinterpret_cast<void *>(&p);
        
        gsl_function funktio;
        
        funktio.function = &f2_img;
        funktio.params = cparam;
        
        gsl_integration_qags(&funktio, alaraja, ylaraja, abs_virhe, suht_virhe, 
                                     LIMIT_SIZE, work_ptr, &vastaus, &virhe);
        
        gsl_integration_workspace_free(work_ptr);
        
        return vastaus;
    }


    /**
     * Suorittaa varsinaisen integroinnin kompleksiselle funktiolle f2.
     * @param l Parametri l
     * @param k Parametri k
     * @return Integroinnin \int_{k}^{k+1}\frac{e^x}{x+li+2}dx tulos
     */
    std::complex<double> integroi_f2(double l, double k)
    {
        double real = integroi_f2_real(l,k);
        double img = integroi_f2_img(l,k);
        std::complex<double> z(real,img);
        return z;
    }

    /**
     * Tehdään pyydetyt laskutoimitukset
     */
    void suorita_laskenta(void)
    {
        std::complex<double> z;
        
        // Luodaan 5x5 kompleksi matriisi
        arma::cx_mat A(5,5, arma::fill::zeros);

        // Lasketaan arvot ja talletetaan ne matriisiin
        for(int k = 0 ; k < A.n_rows ; ++k)
        {
            for(int l = 0 ; l < A.n_cols ; ++l)
            {
                if( k == l)
                {
                    A(k,l) = integroi_f2(l,k);
                }
                else
                {
                    A(k,l) = integroi_f1(l,k);
                }
            }
        }
        // Tulostetaan matriisi ja talletetaan se A.mat tiedostoon
        A.print("A = ");
        A.save("A.mat", arma::arma_ascii);
        
        // Lasketaan ominaisarvot ja ominaisvektorit
        arma::cx_vec eigval;
        arma::cx_mat eigvec;
        arma::eig_gen(eigval, eigvec, A);
        std::cout << "eigval = \n" << eigval << std::endl;
        std::cout << "eigvec = \n" << eigvec << std::endl;
        
        // Tarkistus
        arma::cx_mat Q = eigvec;
        arma::cx_mat Y = Q.i()*A*Q; 
        std::cout << "(Q^-1AQ).diag = \n" << Y.diag() << std::endl;
         
        // Lasketaan determinatti
        z = arma::det(A);
        std::cout << "det A = " << z << std::endl;
    }
}


/**
 * Pääohjelma suorittamista varten
 */
int main(void)
{   
    fysa120::suorita_laskenta();
    return 0;
}

