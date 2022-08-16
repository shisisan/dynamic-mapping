#define YSI_NO_HEAP_MALLOC

#include <a_samp>
#include <a_mysql>
#include <sscanf2>
#include <streamer>

#include <YSI_Coding\y_hooks>
#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_va>
#include <YSI_Coding\y_inline>
#include <YSI_Visual\y_commands>
#include <YSI_Server\y_colours>

#include <strlib>
#include <easyDialog>

/** Macro */
new MySQL: m_Handle = MYSQL_INVALID_HANDLE;
#define MySQL_ReturnHandle() m_Handle

#define SendServerMessage(%0,%1)        SendClientMessageEx(%0, X11_LIGHT_SKY_BLUE_1, "SERVER: "WHITE""%1)
#define SendCustomMessage(%0,%1,%2)     SendClientMessageEx(%0, X11_LIGHT_SKY_BLUE_1, %1": "WHITE""%2)
#define SendSyntaxMessage(%0,%1)        SendClientMessageEx(%0, X11_GREY_80, "USAGE: "%1)
#define SendErrorMessage(%0,%1)         SendClientMessageEx(%0, X11_GREY_80, "ERROR: "%1)


/** Module */
#include <mapping_main>

main() {
    printf("Starting Initialized Gamemode.");
}

public OnGameModeInit()
{
    CreateDatabaseConnection();
    printf("Initialized Gamemode succesfully.");
    return 1;
}

public OnGameModeExit()
{
    mysql_close(MySQL_ReturnHandle());
    return 1;
}

public OnPlayerSpawn(playerid)
{
    SetPlayerPos(playerid, 1642.6522,-2331.9292,13.5469);
    SetPlayerFacingAngle(playerid, 359.7889);
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    return 1;
}

static CreateDatabaseConnection()
{
    MySQL_ReturnHandle() = mysql_connect_file();

    if (mysql_errno(MySQL_ReturnHandle()))
    {
        new error[128];
        mysql_error(error, sizeof(error), MySQL_ReturnHandle());
        printf("[MySQL Connection] Failed! Error: [%d] %s", mysql_errno(MySQL_ReturnHandle()), error);
        return 0;
    }

    printf("MySQL Has been Connected.");
    return 1;
}

stock SendClientMessageEx(playerid, color, const text[], {Float, _}:...)
{
	static
	    args,
	    str[144];
	if ((args = numargs()) == 3)
	{
	    SendClientMessage(playerid, color, text);
	}
	else
	{
		while (--args >= 3)
		{
			#emit LCTRL 5
			#emit LOAD.alt args
			#emit SHL.C.alt 2
			#emit ADD.C 12
			#emit ADD
			#emit LOAD.I
			#emit PUSH.pri
		}
		#emit PUSH.S text
		#emit PUSH.C 144
		#emit PUSH.C str
		#emit PUSH.S 8
		#emit SYSREQ.C format
		#emit LCTRL 5
		#emit SCTRL 4
		SendClientMessage(playerid, color, str);
		#emit RETN
	}
	return 1;
}