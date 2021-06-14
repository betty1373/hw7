#ifndef CmdReader_H
#define CmdReader_H
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <chrono>
#include "Observer.h"
#include <thread>

class CmdReader : public Observable {
public:
    static std::shared_ptr<CmdReader> Create(const size_t num_cmds,std::istream& istream=std::cin) {
        auto ptr = std::shared_ptr<CmdReader>{ new CmdReader{num_cmds,istream}};
        return ptr;
    }
    virtual ~CmdReader() {
        std::cout<<"Destroy CmdReader"<<std::endl;
    };
    void Subscribe(const std::shared_ptr<Observer>& obs) override {
        m_observers.emplace_back(obs);
    }
    // void Unsubscribe(const std::shared_ptr<Observer>& observer ) override {
    //     std::cout<<m_observers.size()<<std::endl;
    //     m_observers.erase(
    //         std::remove_if(
    //             m_observers.begin(),
    //             m_observers.end(),
    //             [&](const std::weak_ptr<Observer>& wptr)
    //             {
    //                 return wptr.expired() || wptr.lock() == observer;
    //             }
    //         ),
    //         m_observers.end()
    //     );
    //     std::cout<<"Unsubscribe ConsoleLogger"<<std::endl;
    //     std::cout<<m_observers.size()<<std::endl;
    // }
    
    void Notify() override {
        if (m_cmds.empty()) return;   
        
        for (auto it=m_observers.begin();it!=m_observers.end();++it) {
            auto ptr = it->lock();
            if (ptr) {
                std::stringstream ss = FormBatch();
                ptr->Update(ss);
            }
            else
                m_observers.erase(it);
        }
        m_cmds.resize(0);
    };
    void Work() {
        m_cmds.reserve(m_num_cmds);
        std::string cmd;
        size_t cnt_braces=0;
        std::chrono::milliseconds dura(500);
        while(std::getline(m_istream,cmd)) {          
            if (cmd=="{") {
               if ((cnt_braces==0) && !m_cmds.empty()) Notify();
               ++cnt_braces;
            }
            else {
                if (cmd=="}") {
                    if (cnt_braces>0) {
                        --cnt_braces;
                        if (cnt_braces==0 && !m_cmds.empty()) Notify();
                    }
                    else {
                        throw std::runtime_error("Unnecessary symbol '}' in input stream");
                    }
                }
                else {
                    m_cmds.emplace_back(cmd);
                    if (cnt_braces==0 && m_cmds.size()==m_num_cmds) Notify();
                }
            }
            std::this_thread::sleep_for(dura);
        }
        if ((cnt_braces==0) && !m_cmds.empty()) Notify();
    };
private:
    CmdReader(const size_t num_cmds,std::istream& istream=std::cin) 
        : m_num_cmds(num_cmds),
          m_istream(istream)   {};
    
    std::stringstream FormBatch() {
        std::stringstream ss;
        for (auto it_cmd = m_cmds.cbegin();it_cmd!=m_cmds.cend();it_cmd++) {
            if (it_cmd !=m_cmds.cbegin())
                ss<< ", "<<*it_cmd;
            else
                ss<<"bulk: "<<*it_cmd;
        }
        return ss;     
    }
    const size_t m_num_cmds;
    std::istream& m_istream;
    std::vector<std::string> m_cmds;
    std::list<std::weak_ptr<Observer>> m_observers;
};

#endif