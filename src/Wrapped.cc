//
// Created by user on 11/6/2021.
//

#include "Wrapped.h"

using namespace Napi;

Napi::Object Wrapped::Init(Napi::Env env, Napi::Object exports)
{
  // This method is used to hook the accessor and method callbacks
  Napi::Function func = DefineClass(env, "Wrapped", {
                                                               InstanceMethod<&Wrapped::Start>("start"),
                                                               InstanceMethod<&Wrapped::Stop>("stop"),
                                                           });

  Napi::FunctionReference *constructor = new Napi::FunctionReference();

  // Create a persistent reference to the class constructor. This will allow
  // a function called on a class prototype and a function
  // called on instance of a class to be distinguished from each other.
  *constructor = Napi::Persistent(func);
  exports.Set("Wrapped", func);

  // Store the constructor as the add-on instance data. This will allow this
  // add-on to support multiple instances of itself running on multiple worker
  // threads, as well as multiple instances of itself running in different
  // contexts on the same thread.
  //
  // By default, the value set on the environment here will be destroyed when
  // the add-on is unloaded using the `delete` operator, but it is also
  // possible to supply a custom deleter.
  env.SetInstanceData<Napi::FunctionReference>(constructor);

  return exports;
}

Wrapped::Wrapped(const Napi::CallbackInfo &info): Napi::ObjectWrap<Wrapped>(info)
{
    Napi::Env env = info.Env();
    if(info.Length() < 2){
        throw TypeError::New(env, "Expected two arguments");
    }
    else if(!info[0].IsNumber()){
        throw Napi::TypeError::New(env, "Expected first argument to be number");
    }
    else if(!info[1].IsNumber()){
        throw Napi::TypeError::New(env, "Expected first argument to be number");
    }
    this->length = info[0].As<Number>().Int32Value();
    this->delay = info[1].As<Number>().Int32Value();
}



Napi::Value Wrapped::Start(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if(info.Length()<1){
        throw TypeError::New(env, "Expected one arg");
    }

    tsfn = ThreadSafeFunction::New(
            env,
            info[0].As<Function>(),  // JavaScript function called asynchronously
            "Resource Name",         // Name
            0,                       // Unlimited queue
            1,                       // Only one thread will use this initially
            [this]( Napi::Env ) {        // Finalizer used to clean threads up
                nativeThread.join();
            }
    );
    nativeThread = std::thread([this]{
        auto callback = [this]( Napi::Env env, Function jsCallback, char* data) {
            // Transform native data into JS data, passing it to the provided
            // 'jsCallback' -- the TSFN's JavaScript function.
            fprintf(stderr, "CB:%x\n", data);
            jsCallback.Call( {ArrayBuffer::New(env, data, length, [](void *hint, void* data){
                fprintf(stderr, "Free:%x\n", data);
                free(data);
            })});
            // We are finished with the data.
        };

        tsfn.Acquire();
        int i=0;
        int *test;
        while(true){
            char* buffer = (char*)calloc(length,1);
            fprintf(stderr, "Alloc:%x\n", buffer);
            test = (int*)buffer;
            test[0] = i++;
            napi_status status = tsfn.BlockingCall( buffer, callback );
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            if ( status != napi_ok )
            {
                // Handle error
                break;
            }
        }
    });
    return Napi::Number::New(env, this->length);
}

Napi::Value Wrapped::Stop(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  return env.Undefined();
}
