#ifndef __STUDY_SRC_UTILS_H__
#define __STUDY_SRC_UTILS_H__
namespace aris {



template <typename T>
class Singleton {
public:
    Singleton() = delete;
    template<typename... Args>
    static T* getInstance(Args ...args) {
        if (instance_ == nullptr) {
            instance_ = new T(args...);
        }
    }

public:
    static T* instance_;

};


template<typename T>
static T* Singleton<T>::instance_ = nullptr;


};


#endif