#include "planet.hpp"

/* Planet */

Citizen::id_t Planet::next_id = 0;

Citizen::id_t Planet::getNextId() {
    return next_id++;
}

/* Earth */

Human& Earth::registerCitizen(const std::string& name) {
    Human new_citizen = Human(Planet::getNextId(), name);
    citizens.insert(std::make_pair(new_citizen.id(), new_citizen));
    return new_citizen;
}

Human& Earth::findCitizen(Citizen::id_t id) const {
    auto it = citizens.find(id);
    if (it == citizens.end())
        throw CitizenNotFoundException();
    return it->second;
}

Human& Earth::findCitizen(const std::string& id) const {
    try {
        Citizen::id_t cid = std::stoi(id);
        return Earth::findCitizen(cid);
    } 
    catch (...) {
        throw CitizenNotFoundException();
    }
}

Earth& earth() {
    static Earth earth_instance;
    return earth_instance;
}

/* Qonos */

Klingon& Qonos::registerCitizen(const std::string& name) {
    Klingon new_citizen = Klingon(Planet::getNextId(), name);
    citizens.insert(std::make_pair(new_citizen.id(), new_citizen));
    return new_citizen;
}

Klingon& Qonos::findCitizen(Citizen::id_t id) const {
    auto it = citizens.find(id);
    if (it == citizens.end())
        throw CitizenNotFoundException();
    return it->second;
}

Klingon& Qonos::findCitizen(const std::string& id) const {
    try {
        Citizen::id_t cid = std::stoi(id);
        return Qonos::findCitizen(cid);
    } 
    catch (...) {
        throw CitizenNotFoundException();
    }
}

Qonos& qonos() {
    static Qonos qonos_instance;
    return qonos_instance;
}

/* Bynaus */

Bynar& Bynaus::registerCitizen(const std::string& name) {
    Bynar new_citizen = Bynar(Planet::getNextId(), name);
    citizens.insert(std::make_pair(new_citizen.id(), new_citizen));
    return new_citizen;
}


Bynar& Bynaus::registerCitizen(const Bynar& first, const Bynar& second) {
    std::string name = first.name() + "&" + second.name();
    return registerCitizen(name);
}

Bynar& Bynaus::findCitizen(Citizen::id_t id) const {
    auto it = citizens.find(id);
    if (it == citizens.end())
        throw CitizenNotFoundException();
    return it->second;
}

Bynar& Bynaus::findCitizen(const std::string& id) const {
    try {
        Citizen::id_t cid = std::stoi(id);
        return Bynaus::findCitizen(cid);
    } 
    catch (...) {
        throw CitizenNotFoundException();
    }
}

Bynaus& bynaus() {
    static Bynaus bynaus_instance;
    return bynaus_instance;
}
