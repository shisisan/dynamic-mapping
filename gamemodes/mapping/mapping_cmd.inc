#if defined _inc_mapping_cmd
    #undef _inc_mapping_cmd
#endif

#if defined _mapping_cmd_included
    #endinput
#endif
#define _mapping_cmd_included

SSCANF:ObjectMenu(string[]) 
{
	if(!strcmp(string,"create",true)) return 1;
	else if(!strcmp(string,"delete",true)) return 2;
	return 0;
}

YCMD:om(playerid. params[], help) = objectmenu;

YCMD:objectmenu(playerid, params[], help)
{
    static 
        index, action, nextParams[128];

	if (!IsPlayerAdmin(playerid))
        return SendErrorMessage(playerid, "You are no admin");

    if(sscanf(params, "k<ObjectMenu>S()[128]", action, nextParams))
	{
		SendSyntaxMessage(playerid, "/objectmenu [entity]");
		SendSyntaxMessage(playerid, "ENTITY: [create/delete]");
		return 1;
	}

    switch(action)
    {
        case 1: //Create
        {
            static modelid;
			if(sscanf(nextParams, "d", modelid))
			{
				SendSyntaxMessage(playerid, "/objectmenu create [modelid]");
				return 1;
			}

            index = Object_Create(playerid, modelid);

            if (index == -1)
	            return SendErrorMessage(playerid, "Dynamic Object Has been limit ("#MAX_DYNAMIC_OBJECT" objects)");
			
			if(!IsValidObjectModel(modelid))
            	return SendErrorMessage(playerid, "Invalid object model.");

            SendCustomMessage(playerid, "OBJECT", "Succesfully created object ID: "YELLOW"%d "GREEN"(%s).", index, Object_GetModelName(modelid));
            EditDynamicObject(playerid, ObjectData[index][objectCreated]);
            SetPVarInt(playerid, "EditingObject", index);
        }
        case 2: //Delete
        {
            static objectid;
            if (sscanf(nextParams, "d", objectid))
	            return SendSyntaxMessage(playerid, "/objectmenu delete [object id]");

            if(Object_Delete(objectid)) SendCustomMessage(playerid, "OBJECT", "Succesfully deleted object ID:"YELLOW" %d.", objectid);
			else SendErrorMessage(playerid, "Object ID is not valid!");
        }
        default: 
        {
            SendSyntaxMessage(playerid, "/objectmenu [entity]");
			SendSyntaxMessage(playerid, "ENTITY: [create/delete]");
        }
    }
    return 1;
}