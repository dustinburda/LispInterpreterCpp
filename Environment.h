#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <memory>
#include <unordered_map>
#include "MalType.h"

class Env;

using env_ptr = std::shared_ptr<Env>;

class SymbolNotFoundException : public std::exception {
public:
    explicit SymbolNotFoundException(const std::string& symbol) : error_message{symbol + " not found"} {}

    const char* what() const noexcept override {
        return error_message.c_str();
    }
private:
    std::string error_message;
};



class Env {
public:
    explicit Env(Env* outer, const std::vector<std::string>& binds, const std::vector<mal_t_ptr>& exprs ) : outer_ {outer} {

        // TODO: add more& support

        for(size_t i = 0; i < binds.size(); i++) {
            if(binds[i] == "&") {
                auto remaining_args = std::make_shared<MalList>();
                for(size_t j = i; j < exprs.size(); j++) {
                    remaining_args->add(exprs[j]);
                }

                data_[binds[i+1]] = remaining_args;
                break;
            } else {
                data_[binds[i]] = exprs[i];
            }

        }
    }

    mal_t_ptr set(const std::string& key, mal_t_ptr val) {
        data_[key] = val;
        return val;
    }

    Env* find(const std::string& key) {
        if(data_.count(key) > 0) {
            return this;
        } else if (outer_ != nullptr ){
            return outer_->find(key);
        }

        return nullptr;
    }

    mal_t_ptr get(const std::string& key) {
        auto env = find(key);

        if(env == nullptr) {
            throw SymbolNotFoundException(key);
        }

        return env->data_[key];
    }

private:
    Env* outer_;
    std::unordered_map<std::string, mal_t_ptr> data_;
};

#endif // ENVIRONMENT_H_