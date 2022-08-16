#if defined _inc_mapping_callback
    #undef _inc_mapping_callback
#endif

#if defined _mapping_callback_included
    #endinput
#endif
#define _mapping_callback_included

#include <YSI_Coding\y_hooks>

hook OnGameModeInitEx()
{
	mysql_pquery(MySQL_ReturnHandle(), "SELECT * FROM `object` ORDER BY `objectID` ASC LIMIT "#MAX_DYNAMIC_OBJECT";", "Object_Load", "");
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEditDynObj(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	if(GetPVarInt(playerid, "EditingObject") != -1)
	{
		if(response == EDIT_RESPONSE_FINAL)
		{
			new id = GetPVarInt(playerid, "EditingObject");

			if(Object_IsExists(id))
			{
				ObjectData[id][objectPos][0] = x;
				ObjectData[id][objectPos][1] = y;
				ObjectData[id][objectPos][2] = z;
				ObjectData[id][objectPos][3] = rx;
				ObjectData[id][objectPos][4] = ry;
				ObjectData[id][objectPos][5] = rz;

				Object_Sync(id);
				Object_Save(id);

				SetPVarInt(playerid, "EditingObject", -1);
				SendCustomMessage(playerid, "OBJECT", "{FFFFFF}Succesfully Edited Object ID: "YELLOW"%d.", id);
			}
		}
		else if(response == EDIT_RESPONSE_CANCEL)
		{
			new id = GetPVarInt(playerid, "EditingObject");

			if(Object_IsExists(id))
			{
				Object_Sync(id);
				SetPVarInt(playerid, "EditingObject", -1);

				SendCustomMessage(playerid, "OBJECT", "Failed To edit object ID: "YELLOW"%d.", id);
			}
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{
	SetPVarInt(playerid, "EditingObject", -1);
    return Y_HOOKS_CONTINUE_RETURN_1;
}