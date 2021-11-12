//
// Created by user on 11/6/2021.
//

#ifndef UCS_WRAPPED_H
#define UCS_WRAPPED_H

#include <napi.h>
#include <mutex>
#include <queue>
using namespace Napi;





typedef struct{
    char* buffer;
    size_t size;
    uint64_t seq;
} Event;

class Wrapped : public Napi::ObjectWrap<Wrapped> {

public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Wrapped(const Napi::CallbackInfo& info);
    static Napi::Value CreateNewItem(const Napi::CallbackInfo& info);
private:
    Napi::Value Start(const Napi::CallbackInfo& info);
    Napi::Value Stop(const Napi::CallbackInfo& info);

    std::thread nativeThread;
    ThreadSafeFunction tsfn;

    uint32_t length;
    uint32_t delay;
};


#endif //UCS_WRAPPED_H
