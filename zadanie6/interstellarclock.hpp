#ifndef __gsb__interstellar_clock__
#define __gsb__interstellar_clock__

#include <map>
#include "observe.hpp"

using Date = unsigned long long;
using Time = unsigned short;

// Czas międzyplanetarny liczymy z dokładnością do godziny.
// Nowoutworzony zegar międzyplanetarny będzie wskazywał czas 0 i dzień 0.
class InterstellarClock : public Observable {
public:
    //Konstruktor
    InterstellarClock();
    
    // Doba międzyplanetarna ma 20 godzin (0-19).
    static const unsigned HOURS_IN_DAY = 20;
    
    // Miesiąc c międzyplanetarny ma 30 dni.
    static const unsigned DAYS_IN_MONTH = 30;
    
    //Destruktor
    virtual ~InterstellarClock() {}
    
    // Zwraca liczbę godzin od początku założenia ZFP.
    unsigned long long hours() const;
    
    // Zwraca bieżącą godzinę (0-19).
    Time time() const;
    
    // Zwraca bieżącą datę, tj. dzień od początku założenia ZFP.
    Date date() const;
    
    // Przesuwa zegar o godzinę.
    virtual InterstellarClock& nextHour();
    
    // Przesuwa zegar o dobę.
    virtual InterstellarClock& nextDay();
    
    // Przesuwa zegar o miesiącc (miesiąc ma 30 dni).
    virtual InterstellarClock& nextMonth();
    
    // Rejestruje obserwatora
    void registerObserver(const Observer& observer);
    
private:
    // Liczba godzin od początku założenia ZFP
    unsigned long long _hours;
    
    // Mapa (id obserwatora -> obserwator)
    std::map<int, const Observer*> _observers;
    
    // Powiadamia wszystkich obserwatorow
    void notifyAllObservers() const;
};

/* Funkcja statyczna zwracajaca zegar */
InterstellarClock& interstellarClock();

#endif /* defined(__gsb__interstellar_clock__) */
