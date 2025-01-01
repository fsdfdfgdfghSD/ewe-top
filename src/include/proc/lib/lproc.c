/* TODO: Get mem/cpu usage */

/*
 * gcc -shared -fpic lproc.c -o lproc.so -I/usr/include/luajit-2.1 -lprocps
*/

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdlib.h>
#include <string.h>
#include <pwd.h>

#include <proc/readproc.h>

/* Utility */

struct proc_t *create_proc_info(lua_State *L) {
    struct proc_t *p = malloc(sizeof(struct proc_t));
    if (p == NULL)
        luaL_error(L, "malloc");

    /* Initialize p struct with zeros */
    memset(p, 0, sizeof(struct proc_t));

    return p;
}

const char *uid_to_str(lua_State *L, uid_t uid)
{
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL)
        luaL_error(L, "getpwuid");

    return pw->pw_name;
}

/* ---------------------- */

static int l_openproc(lua_State *L)
{
    int flags = luaL_checknumber(L, 1);
    PROCTAB *proc = openproc(flags);

    if (proc == NULL)
        luaL_error(L, "openproc");

    lua_pushlightuserdata(L, proc);
    return 1;
}

static int l_readproc(lua_State *L)
{
    PROCTAB *proc = (PROCTAB*)lua_touserdata(L, 1);
    if (proc == NULL)
        luaL_error(L, "lua_touserdata");

    struct proc_t *p = create_proc_info(L);

    if (readproc(proc, p) == NULL) {
        free(p);
        lua_pushnil(L);

        return 1;
    }

    /* Push every element to the table */

    lua_newtable(L);

    lua_pushstring(L, "pid");
    lua_pushinteger(L, p->tid);
    lua_settable(L, -3);

    lua_pushstring(L, "cmd");
    lua_pushstring(L, p->cmd);
    lua_settable(L, -3);

    lua_pushstring(L, "user");
    lua_pushstring(L, uid_to_str(L, p->euid));
    lua_settable(L, -3);

    /* CPU usage calculation */
    lua_pushstring(L, "cpu");
    lua_pushnumber(L, 0.0);
    lua_settable(L, -3);

    /* Memory usage calculation */
    lua_pushstring(L, "mem");
    lua_pushnumber(L, 0.0);
    lua_settable(L, -3);

    lua_pushstring(L, "state");
    lua_pushstring(L, &p->state);
    lua_settable(L, -3);

    free(p);
    return 1;
}

static int l_closeproc(lua_State *L)
{
    PROCTAB *proc = (PROCTAB*)lua_touserdata(L, 1);
    if (proc == NULL)
        luaL_error(L, "lua_touserdata");

    closeproc(proc);
    return 0;
}

LUALIB_API int luaopen_include_proc_lib_lproc_lib(lua_State *L)
{
    struct luaL_Reg lproc_lib[] = {
        {"openproc",    l_openproc  },
        {"readproc",    l_readproc  },
        {"closeproc",   l_closeproc },
        {NULL,          NULL        }
    };

    luaL_newlib(L, lproc_lib);
    return 1;
}
