#include "interstellarclock.hpp"

InterstellarClock::InterstellarClock() {
}

unsigned long long InterstellarClock::hours() const {
    return _hours;
}

Time InterstellarClock::time() const {
    return _hours % HOURS_IN_DAY;
}

Date InterstellarClock::date() const {
    return _hours / HOURS_IN_DAY;
}

InterstellarClock& InterstellarClock::nextHour() {
    _hours++;
    if (_hours % (HOURS_IN_DAY * DAYS_IN_MONTH) == 0)
        notifyAllObservers();
    return *this;
}

InterstellarClock& InterstellarClock::nextDay() {
    for (Time i = 0; i < HOURS_IN_DAY; i++)
        nextHour();
    return *this;
}

InterstellarClock& InterstellarClock::nextMonth() {
    for (Date i = 0; i < DAYS_IN_MONTH; i++)
        nextDay();
    return *this;
}

void InterstellarClock::registerObserver(const Observer& observer) {
    _observers.insert(std::make_pair(_observers.size(), &observer));
}

void InterstellarClock::notifyAllObservers() const {
    for (auto it = _observers.begin(); it != _observers.end(); ++it) {
        it->second->notify();
    }
}

InterstellarClock& interstellarClock() {
    static InterstellarClock interstellarClock;
    return interstellarClock;
}
