/**
 * @file exercise2.cc
 * @brief FYSA120 harjoitustyö 2
 * @author keijo.k.a.salonen@student.jyu.fi
 * @date 12.11.2015
 *
 * Kinetic Monte Carlo simulaatio.
 * 
 * @note
 * 
 * clang++ -std=c++11 exercise2.cc -o ex2
 *
 */
#include <iostream>
#include <random>
#include <functional>
#include <vector>
#include <cmath>
#include <queue>

/**
 * fysa120 nimiavaruus
 */
namespace fysa120
{
    /**
     * Event tietorakenne tapahtuman tallentamiseen.
     */
    struct event
    {
        double queue_time;
        double execution_time;
        int process_number;
        
        // kuormitetaan < operaattori std::priority_queue varten
        // tarkoitus on järjestää nousevaan järjestykseen suoritusajan mukaan
        bool operator<(const struct event& e) const
        {
            return execution_time > e.execution_time;
        }
    };


    /**
    * Tapahtuman tulostaminen virtaan kuormittamalla << operaattoria.
    */
    std::ostream& operator<<(std::ostream& os, const event& e)
    {
        return os << e.queue_time << " " << e.execution_time << " " << e.process_number;
    }


    /**
     * Generoidaan tapahtumia.
     *
     * @param events Vektori johon tallenetaan tapahtumat
     * @param n Luotavien tapahtumien lukumäärä
     */
    void generate_events(std::vector<event> &events, std::size_t n)
    {
        double k = 0.1;
        double t;       ///< queue_time
        double tx;      ///< execution_time
        int p;          ///< process_number
        
        // Alustetaan tarvittavat satunnaislukujen luojat
        std::mt19937 gena{std::random_device{}()};
        std::mt19937 genb{std::random_device{}()};
        std::mt19937 genc{std::random_device{}()};
        
        std::uniform_real_distribution<double> unif_dist_10(0,10); 
        std::uniform_real_distribution<double> unif_dist_1(0,1);
        std::uniform_int_distribution<int> unif_int_dist(0,12);

        auto random_t = std::bind(unif_dist_10, gena);
        auto random_r = std::bind(unif_dist_1, genb);
        auto random_p = std::bind(unif_int_dist, genc);
        
        // Luodaan n kpl:ta tapahtumia
        for(int i = 0; i < n ; i++)
        {
            t = random_t();
            tx = t - ((1/k) * std::log(random_r()));
            p = random_p();
            event tmp = {t,tx,p};
            events.push_back(tmp);
        }
    }
    
    /**
    * Poistaa tapahtumia execution_time:n perusteella väliltä [min - max].
    *
    * @param events Vektori josta poistetaan tapahtumia
    * @param min execution_time alaraja poistolle
    * @param max execution_time yläraja poistolle
    */
    void remove_events(std::vector<event> &events, const double min, const double max)
    {
        events.erase(std::remove_if(events.begin(), events.end(),
                                    [=](const event e){ return e.execution_time <= max && e.execution_time >= min; }), 
                                    events.end());
                                    ///< käytetään Lambda-lausetta selvittämään täyttyykö poiston ehdot
    }
    
    
    /**
    * Tallettaa tapahtumat priority_queue -jonoon.
    * 
    * @param queue Prioriteettijono jonne talletetaan vektorin sisältämät tapahtumat järjestykseen
    * @param events Vektori jossa on tapahtumat
    */
    void insert_events_priority_queue(std::priority_queue<event> &queue, std::vector<event> &events)
    {
        for(auto e : events)
        {
            queue.push(e);
        }
    }
    
    
    /**
    * Simuloidaan jonossa olevien tapahtumien suorittamista.
    *
    * @param q Prioriteettijono jossa on tapahtumat
    */
    void run_simulation(std::priority_queue<event> &q)
    {
        while(!q.empty())
        {
            std::cout << q.top() << std::endl;
            q.pop(); ///< poistetaan jonon ylin elementti
        }
    }

}


/**
 * Pääohjelma testaamista varten.
 */
int main(void)
{
    std::cout << "Exercise 2: Kinetic Monte Carlo" << std::endl;
    
    std::vector<fysa120::event> events;
    
    fysa120::generate_events(events,100);
    std::cout << "Events: " << events.size() << std::endl;
    for(auto e : events)
    {
        std::cout << e << std::endl;
    }
    
    std::cout << "Poistetaan tapahtumia..." << std::endl;
    fysa120::remove_events(events,6.0,7.0);
    std::cout << "Events: " << events.size() << std::endl;
    for(auto e : events)
    {
        std::cout << e << std::endl;
    }
    
    std::priority_queue<fysa120::event> queue;
    fysa120::insert_events_priority_queue(queue,events);
    
    std::cout << "Tapahtumia jonossa: " << queue.size() << std::endl;
    fysa120::run_simulation(queue);
    std::cout << "Tapahtumia jonossa: " << queue.size() << std::endl;

    return 0;
}

