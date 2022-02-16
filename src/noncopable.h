/**
 * @file noncopable.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __STUDY_SRC_NONCOPABLE_H__
#define __STUDY_SRC_NONCOPABLE_H__

namespace aris {

class Noncopable {
public:
    Noncopable() = default;
    virtual~Noncopable() = default;

    Noncopable(const Noncopable&) = delete;
    Noncopable& operator=(const Noncopable&) = delete; 
};

}

#endif