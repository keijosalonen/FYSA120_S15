/**
 * @file exercise3.cc
 * @brief FYSA120 Harjoitustyö 3
 * @author keijo.k.a.salonen@student.jyu.fi
 * @date 24.11.2015
 *
 * Tavallisen differentiaaliyhtälön ratkaiseminen Boost odeint kirjastolla
 *
 * @note
 *
 * clang++ -std=c++11 -I/usr/local/include exercise3.cc -o ex3 
 *
 */
#include <iostream>
#include <array>
#include <fstream>
#include <boost/numeric/odeint.hpp>


/**
 * fysa120 nimiavaruus
 */
namespace fysa120
{
    /**
     * Lyhenteitä käyttöön
     */
    namespace odeint = boost::numeric::odeint;
    
    typedef std::array<double,2> state_type;


    /**
     * Ratkaistava DY: y''(t)-6y'(t)+y(t)=0
     * Yleinen ratkaisu on: A*e^((3-2*sqrt(2))*t)+B*e^((3+2*sqrt(2))*t)
     * 
     * x[0] = y(t)
     * x[1] = y'(t)
     * dxdt[0] = y'(t)
     * dxdt[1] = y''(t) = 6y'(t)-y(t)
     * 
     * @param x     Taulukko, jossa 2 solua
     * @param dxdt  Taulukko, jossa 2 solua
     * @param t     Ei käytössä
     */
    void ratkaistava_dy(const state_type &x, state_type &dxdt, const double /*t*/)
    {
        //
        dxdt[0] = x[1];
        dxdt[1] = 6 * x[1] - x[0]; 
    }


    /**
     * Yrittää ratkaista DY:n käyttäen vakiomittaista askellusta.
     * Tulos talletetaan tiedostoon: ode_fixed.dat
     */
    void suorita_fixed_step_ratkasin(void)
    {
        //
        std::ofstream out("ode_fixed.dat");
        odeint::runge_kutta4<state_type> rk4;       ///< Käytettävä ratkaisumenetelmä
        state_type x = {{2.0, 5.0/2.0 }};           ///< alkuarvot y(0)=2 ja y'(0)=5/2
        const double dt = 0.01;                     ///< Vakioaskelluksen suuruus
        for(double t = 0.0 ; t < 20.0 ; t += dt)    ///< Haetaan ratkaisu välillä 0 < t < 20
        {
            rk4.do_step(ratkaistava_dy,x,t,dt);     ///< Haetaan yksi ratkaisu pisteessä t
            out << t << " " << x[0] << " " << x[1] << std::endl;    ///< t y(t) y'(t)
        }
        out.close();
    }


    /**
     * Yritää ratkaista DY:n käyttäen vaihtelevan mittaista askellusta.
     * Tulos talletetaan tiedostoon: ode_adaptive.dat
     */
    void suorita_adaptive_step_ratkaisin(void)
    {
        //
        std::ofstream out("ode_adaptive.dat");
        typedef odeint::runge_kutta_cash_karp54<state_type> stepper_type;       ///< Käytettävä ratkaisumenetelmä
        double abs_err = 1.0e-10 , rel_err = 1.0e-6;    ///< absoluuttinen virheraja , suhteellinen virheraja
        auto stepper = odeint::make_controlled<stepper_type>(abs_err, rel_err);  ///< Luodaan ohjattu askellin
        state_type x = {{2.0, 5.0/2.0 }};               ///< alkuarvot y(0)=2 ja y'(0)=5/2
        double dt = 0.01;   ///< 1. kierroksen askelluksen suuruus
        double t = 0.0;
        while(t < 20.0)
        {
            stepper.try_step(ratkaistava_dy,x,t,dt);
            out << t << " " << x[0] << " " << x[1] << std::endl;    ///< t y(t) y'(t)
            /**
             * try_step()
             * Yrittää hakea ratkaisua virherajojen puitteissa, kun askellus on dt. Jos onnistuu, niin vastaus kirjoitetaan x:ään
             * ja uusi aika talletetaan t:n ja dt sisältää uuden mahdollisesti suuremman askelluksen.
             * Jos virhe on kuitenkin liian suuri, arvo hylätään ja askelluksen dt kokoa pienennetään
             * seuraavaa yritystä varten.
             */
        }
        out.close();
    }
}


/**
 * Pääohjelma testaamista varten
 */
int main(void)
{
    fysa120::suorita_fixed_step_ratkasin();
    fysa120::suorita_adaptive_step_ratkaisin();
    return 0;
}
