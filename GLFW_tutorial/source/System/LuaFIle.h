#ifndef LUA_FILE_H
#define LUA_FILE_H

#include "lua/include/lua.hpp"
#include <LuaBridge.h>
#include "System/Exception.h"
#include <iostream>
#pragma comment( lib, "lib/lua/liblua54.a")

namespace luke {
    inline void what(const std::exception& e) {
        ::LOG("%s\n", e.what());
        abort();
    }
    #define LUA_TRY try {
    #define LUA_CATCH } catch (const std::exception& e) { luke::what(e); }

    typedef lua_State LuaState;
    typedef luabridge::LuaRef LuaRef;
    
    ////Lua-----------------------------------------------
    class Lua {
    public:
        inline static void init() {
            pState = luaL_newstate();
            luaL_openlibs(pState);
        }
        inline static LuaState* state() { return pState; }
        //inline static LuaState* getPrivateState() { return luaL_newstate(); }

    private:

        static LuaState* pState;

        Lua() {}
        ~Lua() {}

    };
    // This template wraps a member function into a C-style "free" function compatible with lua.
    template <typename T, T* obj, void (T::*mem_func)()>
    static void dispatch() {
        obj->mem_func();
    }
    ////LuaInterface-----------------------------------------------
    class LuaInterface {
    public:
        LuaInterface() {
            
        }
        bool openB(const char* path) {
            if (luaL_loadfile(Lua::state(), path) || lua_pcall(Lua::state(), 0, 0, 0)) {
                LOG(LogError, "Lua::Failed to open or compile file:%s\n", path);
                return 0;
            }
            return 1;
        }

        void open(const char* path) {
            luaL_dofile(Lua::state(), path);
            lua_pcall(Lua::state(), 0, 0, 0);
        }
        /* static bool isNill() {
             if (s.isNil()) {
                 std::cout << "Variable not found!" << std::endl;
             }
         }*/
         /*if (s.isString()) {
             luaString = s.cast<std::string>();
         }*/
        inline luabridge::Namespace global() { return luabridge::getGlobalNamespace(Lua::state()); }
        inline luabridge::Namespace beginNamespace(const char* name) { return global().beginNamespace(name); }

        inline LuaRef get(const char* name) { return luabridge::getGlobal(Lua::state(), name);}
        template<typename T> T get(const char* name, const T& default_value) {  return get(name).cast<T>();}
        template<typename T> T get(const char* name) {                          return get(name).cast<T>(); }
    private:
    };
}

#endif

