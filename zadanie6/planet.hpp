#ifndef __gsb__planet__
#define __gsb__planet__

#include <string>
#include <map>
#include "citizen.hpp"

/* Planet abstract class */
class Planet {
public:
    // Destructor
    virtual ~Planet() {}

    // Returns a next interstellar citizen ID
    static Citizen::id_t getNextId();
    
    // Registers a new citizen
    virtual Citizen& registerCitizen(const std::string& name = "") = 0;
    
    // Finds a citizen
    virtual Citizen& findCitizen(Citizen::id_t id) const = 0;
    virtual Citizen& findCitizen(const std::string& id) const = 0;

private:
    // An interstellar ID of a next citizen
    static Citizen::id_t next_id;
};


/* Earth class */
class Earth : public Planet {
private:
    // A map of all human on the Earth
    std::map<Citizen::id_t, Human&> citizens;

public:
    // Registers a new human
    Human& registerCitizen(const std::string& name = "");
    
    // Finds a citizen
    Human& findCitizen(Citizen::id_t id) const;
    Human& findCitizen(const std::string& id) const;
};

/* Returns an instance of the Earth class */
Earth& earth();


/* Qunos class */
class Qonos : public Planet {
public:
    // Registers a new klingon
    Klingon& registerCitizen(const std::string& name = "");
    
    // Finds a citizen
    Klingon& findCitizen(Citizen::id_t id) const;
    Klingon& findCitizen(const std::string& id) const;

private:
    // A map of all klingons on the Quonos
    std::map<Citizen::id_t, Klingon&> citizens;
};

/* Returns an instance of the Quonos class */
Qonos& qonos();


/* Bynaus class */
class Bynaus : public Planet {
public:
    // Registers a new bynar 
    Bynar& registerCitizen(const std::string& name = "");
    Bynar& registerCitizen(const Bynar& first, const Bynar& second);
    
    // Finds a citizen 
    Bynar& findCitizen(Citizen::id_t id) const;
    Bynar& findCitizen(const std::string& id) const;

private:
    // A map of all bynars on the Bynaus
    std::map<Citizen::id_t, Bynar&> citizens;
};

/* Returns an instance of the Bynaus class */
Bynaus& bynaus();


#include <exception>

/* Hierarchy of planet exceptions */
class PlanetException : public std::exception {};
class CitizenNotFoundException : public PlanetException {};


#endif /* defined(__gsb__planet__) */
