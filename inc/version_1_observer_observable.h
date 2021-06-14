#ifndef CMD_PUBLISH_H
#define CMD_PUBLISH_H
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>

class Observer {
 public: 
    virtual ~Observer() {};  
    virtual void Update(std::stringstream& ss)=0;
};

class Observable {
public:
    virtual ~Observable() {};
    virtual void Subscribe(Observer *logger) = 0;
  // virtual void Unsubscribe(std::weak_ptr<ISubscriber> sbs) = 0;
    virtual void Notify() = 0;
    virtual void Work() = 0;
};
class CmdReader : public Observable {
public:
    explicit CmdReader(const size_t num_cmds) : m_num_cmds(num_cmds) {
        m_cmds.reserve(m_num_cmds);
    };
    virtual ~CmdReader() {
        std::cout<<"Destroy CmdReadr"<<std::endl;
    };
    void Subscribe(Observer *logger) override {
        m_loggers.push_back(logger);
    }
    void Notify() override {
        if (m_cmds.empty()) return;

        std::stringstream ss;
        
        for (auto it_cmd = m_cmds.cbegin();it_cmd!=m_cmds.cend();it_cmd++) {
            if (it_cmd !=m_cmds.cbegin())
                ss<< ","<<*it_cmd;
            else
                ss<<"Bulk :"<<*it_cmd;
        }
        
        ss<<std::endl;
        
        for (auto& logger : m_loggers) {
            
            logger->Update(ss);
        }
    };
    void Work() override {
        std::string cmd;
        size_t cnt_braces=0;

        while(std::cin>>cmd) {
            if (cmd=="{") {
               if ((cnt_braces==0) && !m_cmds.empty()) {
                    Notify();
                    m_cmds.resize(0);
                }  
                ++cnt_braces;
            }
            else {
                if (cmd=="}") {
                    if (cnt_braces>0) {
                        --cnt_braces;
                        if (cnt_braces==0 && !m_cmds.empty()) {
                            Notify();
                            m_cmds.resize(0);
                        }
                    }
                    else {
                        std::runtime_error("Unnecessary symbol '}' in input stream");
                    }
                }
                else {
                    m_cmds.emplace_back(cmd);
                    if (cnt_braces==0 && m_cmds.size()==m_num_cmds) {
                        Notify();
                        m_cmds.resize(0);
                    }
                }
            }
        }
    };
private:
    const size_t m_num_cmds;
    std::vector<std::string> m_cmds;
    std::vector<Observer *> m_loggers;
};

class ConsoleLogger : public Observer {
public:    
     ConsoleLogger(Observable *reader) {
         reader->Subscribe(this);
    };
     virtual ~ConsoleLogger() {
         std::cout<<"Destroy ConsoleLogger"<<std::endl;
     };

    void Update(std::stringstream& ss) override {
        std::cout << "bulk is " << ss.str() << std::endl;
    };
};
// class FileLogger : public Subscriber {
// public:
//      FileLogger() {};
//     virtual  ~FileLogger() {
//          std::cout<<"Destroy FileLogger"<<std::endl;
//      };

//     void Update(std::stringstream& ss) override {
//         std::cout << "bulk is " << ss.str() << std::endl;
//     };
// //private:
//    // std::shared_ptr<IPublisher> m_publisher;
// };

#endif
#include "src/Publisher.h"

/// @file

/// @brief Main function. Print Ip's from various types.
/// @author btv<example@example.com>
int main(int argc, char * argv[]) {
   if (argc < 2) {
      std::cerr<<"Use"<<argv[0]<<"<bulk_size>\n";
      return EXIT_FAILURE;
   }
   size_t num = std::atoi(argv[1]);
   if (num<=0) {
       std::cerr<<"Bulk size must be greater than 0\n";
      return EXIT_FAILURE;
   }
   CmdReader cmdReader(num);
   ConsoleLogger consoleLogger(&cmdReader);
  // cmdReader->Subscribe(consoleLogger);
  // auto fileLogger = FileLogger(cmdReader);
   cmdReader.Work();
   return 0;
}  
