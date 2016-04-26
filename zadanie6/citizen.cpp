#include "citizen.hpp"

/* Human */

Human::Human(const id_t id, const std::string& name) 
    : _id(id), _name(name)
{}

const Citizen::id_t Human::id() const {
    return _id;
}

const std::string& Human::name() const {
    return _name;
}

/* Klingon */

Klingon::Klingon(const id_t id, const std::string& name)
    : _id(id), _name(name)
{}

const Citizen::id_t Klingon::id() const {
    return _id;
}

const std::string& Klingon::name() const {
    return _name;
}

/* Bynar */

Bynar::Bynar(const id_t id, const std::string& name)
    : _id(id), _name(name)
{}

const Citizen::id_t Bynar::id() const {
    return _id;
}

const std::string& Bynar::name() const {
    return _name;
}
