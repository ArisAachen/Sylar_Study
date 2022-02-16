/**
 * @file singelton.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef __STUDY_SRC_SINGELTON_H__
#define __STUDY_SRC_SINGELTON_H__

#include "noncopable.h"

#include <memory>

namespace aris {

template <typename T>
class Singelton : Noncopable {
public:
    template<typename... Args>
    static T* get_instance(Args... args) {
        if (instance_ == nullptr) 
            instance_ = new T(args...);
        return instance_;    
    }

private:
    static T* instance_ {nullptr};
};

template <typename T>
class SingeltonPtr : Noncopable {
public: 
    template<typename... Args>
    static std::shared_ptr<T> get_instance(Args... args) {
        if (instance_ == nullptr) 
            instance_ = std::shared_ptr<T>(new T(args...));
        return instance_;
    }
private:
    static std::shared_ptr<T> instance_ {nullptr};    
};

}



#endif