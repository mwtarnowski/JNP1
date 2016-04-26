#ifndef __gsb__citizen__
#define __gsb__citizen__

#include <cstddef>
#include <string>

/* Citizen interface */
class Citizen {
public:
    // Member type
    typedef size_t id_t;
    
    // Destructor
    virtual ~Citizen() {};
    
    // Returns an ID of the citizen 
    virtual const id_t id() const = 0;
    
    // Returns a name of the citizen
    virtual const std::string& name() const = 0;
};


/* Human class */
class Human : public Citizen {
public:
    // Constructor
    Human(const id_t id, const std::string& name);
    
    // Returns ID of the human
    const id_t id() const;
    
    // Returns the name of the human
    const std::string& name() const;

private:
    // An ID of the human
    const id_t _id;

    // A name of the human
    const std::string& _name;
};


/* Klingon class */
class Klingon : public Citizen {
public:
    // Constructor
    Klingon(const id_t id, const std::string& name);
    
    // Returns ID of the klingon
    const id_t id() const;
    
    // Returns the name of the klingon
    const std::string& name() const;

private:
    // An ID of the klingon
    const id_t _id;

    // A name of the klingon
    const std::string& _name;
};


/* Bynar class */
class Bynar : public Citizen {
public:
    // Constructor
    Bynar(const id_t id, const std::string& name);
    
    // Returns ID of the bynar
    const id_t id() const;
    
    // Returns the name of the bynar
    const std::string& name() const;

private:
    // An ID of the bynar
    const id_t _id;

    // A name of the bynar
    const std::string& _name;
};


#endif /* defined(__gsb__citizen__) */
