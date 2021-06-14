#ifndef Logger_H
#define Logger_H
#include <fstream>
#include "CmdReader.h"

class ConsoleLogger : public Observer, public std::enable_shared_from_this<ConsoleLogger> {
public:    
     static std::shared_ptr<Observer> Create(std::shared_ptr<CmdReader>& reader) {
        auto ptr = std::shared_ptr<ConsoleLogger>{ new ConsoleLogger{}};
        ptr->SetCmdReader(reader);
        return ptr;
    }
     virtual ~ConsoleLogger() {
         std::cout<<"Destroy ConsoleLogger"<<std::endl;
     };

    void Update(std::stringstream& ss) override {
        std::cout << ss.str() << std::endl;
    };
private:
    ConsoleLogger() {};
    void SetCmdReader(std::shared_ptr<CmdReader>& _reader) {
        m_reader = _reader;
        auto ptr = m_reader.lock();
        if (ptr) {
            ptr->Subscribe(shared_from_this());
        }
    }
    std::weak_ptr<CmdReader> m_reader;
};
class FileLogger : public Observer, public std::enable_shared_from_this<FileLogger> {
public:    
     static std::shared_ptr<Observer> Create(std::shared_ptr<CmdReader>& reader) {
        auto ptr = std::shared_ptr<FileLogger>{ new FileLogger{}};
        ptr->SetCmdReader(reader);
        return ptr;
    }
     virtual ~FileLogger() {
         std::cout<<"Destroy FileLogger"<<std::endl;
     };

    void Update(std::stringstream& ss) override {
        auto time = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        auto fileName{"bulk"+time+".log"};
        std::ofstream log(fileName,std::ios::out);
        log << ss.str() << std::endl;
        log.close();
    };
private:
    FileLogger() {};
    void SetCmdReader(std::shared_ptr<CmdReader>& _reader) {
        m_reader = _reader;
        auto ptr = m_reader.lock();
        if (ptr) {
            ptr->Subscribe(shared_from_this());
        }
    }
    std::weak_ptr<CmdReader> m_reader;
};
#endif