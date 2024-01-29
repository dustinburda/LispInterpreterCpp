#ifndef MALTYPE_H_
#define MALTYPE_H_

#include <memory>
#include <vector>


struct MalType;
using mal_t_ptr = std::shared_ptr<MalType>;

struct MalType {

}

struct MalList : MalType {
    void add(mal_t_ptr& mal_t) {
        mal_list_.push_back(mal_t);
    }

    std::vector<mal_t_ptr> mal_list_;
}


struct Number : MalType {
    int number;
}

struct Symbol : MalType {
    std::string symbol;
}



#endif // MALTYPE_H_