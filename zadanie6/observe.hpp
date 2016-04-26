#ifndef __gsb__observe__
#define __gsb__observe__


/* Observer interface */
class Observer {
public:
    // Destructor
    virtual ~Observer() {}

    // Reports an event to a observer
    virtual void notify() const = 0;
};


/* Observable interface */
class Observable {
public:
    // Destructor
    virtual ~Observable() {}

    // Registers a new observer
    virtual void registerObserver(const Observer& observer) = 0;
};


#endif /* defined(__gsb__observe__) */
