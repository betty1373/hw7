#ifndef Observer_H
#define Observer_H
#include <sstream>
#include <memory>

class Observer {
 public: 
    virtual ~Observer(){};  
    virtual void Update(std::stringstream& ss)=0;
};

class Observable {
public:
    virtual ~Observable()= default;
    virtual void Subscribe(const std::shared_ptr<Observer>& obs) = 0;
   // virtual void Unsubscribe(const std::shared_ptr<Observer>& obs) = 0;
    virtual void Notify() = 0;
};
#endif