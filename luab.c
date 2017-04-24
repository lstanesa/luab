#include <stdlib.h>
#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

//#include "counter.c"
#include "card.c"
#include "conditions.c"

#define LENGTH(X) (sizeof X / sizeof X[0])

int set_lua_path(lua_State *L, char *path);

int main(int argc, char **argv) {
    
    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_card(L);
    luaopen_conditions(L);

    luaL_loadfile(L, "test.lua");
    int res = lua_pcall(L, 0, LUA_MULTRET, 0);

    if(res) {
        fprintf(stderr,"Failed to run script: %s\n", lua_tostring(L,-1));
        return 1;
    }

    lua_close(L);

    return 0;
}

int lua_path_append(lua_State *L, char *path) {
    lua_getglobal( L, "package" );
    lua_getfield( L, -1, "path" ); // get field "path" from table at top of stack (-1)
    char* cur_path = lua_tostring( L, -1 ); // grab path string from top of stack
    sprintf(cur_path, ";%s", path);
    lua_pop( L, 1 ); // get rid of the string on the stack we just pushed on line 5
    lua_pushstring( L, cur_path ); // push the new one
    lua_setfield( L, -2, "path" ); // set the field "path" in table at -2 with value at top of stack
    lua_pop( L, 1 ); // get rid of package table from top of stack
    return 0; // all done!
}
