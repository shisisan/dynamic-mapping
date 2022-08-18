#if defined _inc_mapping_function
    #undef _inc_mapping_function
#endif

#if defined _mapping_function_included
    #endinput
#endif
#define _mapping_function_included

forward OnObjectCreated(index);
public OnObjectCreated(index)
{
	ObjectData[index][objectID] = cache_insert_id();

	Object_Save(index);
	Object_Sync(index);
	return 1;
}

forward Object_Load();
public Object_Load()
{
    if(cache_num_rows())
	{
        for(new i = 0; i != cache_num_rows(); i++)
		{
            Iter_Add(DynamicObject, i);

            cache_get_value_name_int(0, "objectID", ObjectData[i][objectID]);
		    cache_get_value_name_int(0, "objectModel", ObjectData[i][objectModel]);
		    cache_get_value_name_float(0, "objectX", ObjectData[i][objectPos][0]);
		    cache_get_value_name_float(0, "objectY", ObjectData[i][objectPos][1]);
		    cache_get_value_name_float(0, "objectZ", ObjectData[i][objectPos][2]);
		    cache_get_value_name_float(0, "objectRX", ObjectData[i][objectPos][3]);
		    cache_get_value_name_float(0, "objectRY", ObjectData[i][objectPos][4]);
		    cache_get_value_name_float(0, "objectRZ", ObjectData[i][objectPos][5]);
            cache_get_value_name_int(0, "objectWorld", ObjectData[i][objectVW]);
            cache_get_value_name_int(0, "objectInterior", ObjectData[i][objectInterior]);

            Object_Sync(i);
        }
        printf(">>> Loaded %d dynamic object.", cache_num_rows());
    }
    return 1;
}

Object_IsExists(index)
{
	if(Iter_Contains(DynamicObject, index))
		return 1;
    
	return 0;
}

Object_Create(playerid, modelid)
{
    static
	    Float:x,
	    Float:y,
	    Float:z,
	    Float:angle,
        index;

    if((index = Iter_Free(DynamicObject)) != cellmin)
    {
        if (GetPlayerPos(playerid, x, y, z) && GetPlayerFacingAngle(playerid, angle))
	    {
            Iter_Add(DynamicObject, index);

            x += 1.0 * floatsin(-angle, degrees);
			y += 1.0 * floatcos(-angle, degrees);

            ObjectData[index][objectModel] = modelid;
            ObjectData[index][objectPos][0] = x;
            ObjectData[index][objectPos][1] = y;
            ObjectData[index][objectPos][2] = z;
			ObjectData[index][objectPos][3] = 0.0;
			ObjectData[index][objectPos][4] = 0.0;
            ObjectData[index][objectPos][5] = angle;
            ObjectData[index][objectInterior] = GetPlayerInterior(playerid);
            ObjectData[index][objectVW] = GetPlayerVirtualWorld(playerid);

			Object_Sync(index);
			mysql_tquery(MySQL_ReturnHandle(), sprintf("INSERT INTO `object` (`objectInterior`) VALUES('%d')", ObjectData[index][objectInterior]), "OnObjectCreated", "d", index);
            return index;
        }
    }
    return -1;
}

Object_Save(index)
{
    if(Object_IsExists(index))
	{
        new
	        query[512];

        format(query, sizeof(query), "UPDATE `object` SET `objectModel` = '%d', `objectX` = '%.4f', `objectY` = '%.4f', `objectZ` = '%.4f', `objectRX` = '%.4f', `objectRY` = '%.4f', `objectRZ` = '%.4f', `objectInterior` = '%d', `objectWorld` = '%d' WHERE `objectID` = '%d'",
            ObjectData[index][objectModel],
            ObjectData[index][objectPos][0],
            ObjectData[index][objectPos][1],
            ObjectData[index][objectPos][2],
            ObjectData[index][objectPos][3],
            ObjectData[index][objectPos][4],
            ObjectData[index][objectPos][5],
            ObjectData[index][objectInterior],
            ObjectData[index][objectVW],
            ObjectData[index][objectID]
        );
        return mysql_tquery(MySQL_ReturnHandle(), query);
    }
    return 1;
}

Object_Delete(index)
{
    if(index != -1 && Object_IsExists(index))
    {
        mysql_tquery(MySQL_ReturnHandle(), sprintf("DELETE FROM `object` WHERE `objectID` = '%d'", ObjectData[index][objectID]));

        if(IsValidDynamicObject(ObjectData[index][objectCreated]))
	        DestroyDynamicObject(ObjectData[index][objectCreated]);

        new tmp_ObjectData[E_OBJECT_DATA];
		ObjectData[index] = tmp_ObjectData;

        Iter_Remove(DynamicObject, index);
    }
    return 1;
}

Object_Sync(index)
{
    if (index != -1 && Object_IsExists(index))
	{
	    if (IsValidDynamicObject(ObjectData[index][objectCreated]))
	        DestroyDynamicObject(ObjectData[index][objectCreated]);

	 	ObjectData[index][objectCreated] = CreateDynamicObject(ObjectData[index][objectModel], ObjectData[index][objectPos][0], ObjectData[index][objectPos][1], ObjectData[index][objectPos][2] - 0.0, ObjectData[index][objectPos][3], ObjectData[index][objectPos][4], ObjectData[index][objectPos][5], ObjectData[index][objectVW], ObjectData[index][objectInterior]);
		return 1;
	}
	return 0;
}

/*
Object_GetCount()
{
    static
        count = 0;

    foreach(new i : DynamicObject) if(Object_IsExists(i))
    {
        count++;
    }
    return count;
}
*/

Object_GetModelName(modelid) 
{
	new temp[50] = "N/A";
	for(new i = 0; i < sizeof(ModelInfo); i++)
	{
	    if(ModelInfo[i][oID] == modelid) 
		{
			format(temp, 50, "%s", ModelInfo[i][oName]);
			break;
		}
	}
	
	return temp;
}

stock IsValidObjectModel(modelid)
{
    if(modelid < 0 || modelid > 20000)
        return 0;

    switch (modelid)
    {
        case 18632..18645, 18646..18658, 18659..18667, 18668..19299, 19301..19515, 18631, 331, 333..339, 318..321, 325, 326, 341..344, 346..353, 355..370, 372:
        return 1;
    }
    new const g_arrModelData[] =
    {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128,
                -515899393, -134217729, -1, -1, 33554431, -1, -1, -1, -14337, -1, -33,
                127, 0, 0, 0, 0, 0, -8388608, -1, -1, -1, -16385, -1, -1, -1, -1, -1,
             -1, -1, -33, -1, -771751937, -1, -9, -1, -1, -1, -1, -1, -1, -1, -1, -1,
             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
             -1, -1, -1, -1, -1, -1, -1, -1, 33554431, -25, -1, -1, -1, -1, -1, -1,
             -1073676289, -2147483648, 34079999, 2113536, -4825600, -5, -1, -3145729,
             -1, -16777217, -63, -1, -1, -1, -1, -201326593, -1, -1, -1, -1, -1,
             -257, -1, 1073741823, -133122, -1, -1, -65, -1, -1, -1, -1, -1, -1,
             -2146435073, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1073741823, -64, -1,
             -1, -1, -1, -2635777, 134086663, 0, -64, -1, -1, -1, -1, -1, -1, -1,
             -536870927, -131069, -1, -1, -1, -1, -1, -1, -1, -1, -16384, -1,
             -33554433, -1, -1, -1, -1, -1, -1610612737, 524285, -128, -1,
             2080309247, -1, -1, -1114113, -1, -1, -1, 66977343, -524288, -1, -1, -1,
             -1, -2031617, -1, 114687, -256, -1, -4097, -1, -4097, -1, -1,
             1010827263, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -32768, -1, -1, -1, -1, -1,
             2147483647, -33554434, -1, -1, -49153, -1148191169, 2147483647,
             -100781080, -262145, -57, 134217727, -8388608, -1, -1, -1, -1, -1, -1,
             -1, -1, -1, -1, -1, -1, -1, -1, -1048577, -1, -449, -1017, -1, -1, -1,
             -1, -1, -1, -1, -1, -1, -1, -1, -1835009, -2049, -1, -1, -1, -1, -1, -1,
             -8193, -1, -536870913, -1, -1, -1, -1, -1, -87041, -1, -1, -1, -1, -1,
             -1, -209860, -1023, -8388609, -2096897, -1, -1048577, -1, -1, -1, -1,
             -1, -1, -897, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1610612737,
             -3073, -28673, -1, -1, -1, -1537, -1, -1, -13, -1, -1, -1, -1, -1985,
             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1056964609, -1, -1, -1,
             -1, -1, -1, -1, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
             -236716037, -1, -1, -1, -1, -1, -1, -1, -536870913, 3, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
             -1, -1, -1, -1, -1, -2097153, -2109441, -1, 201326591, -4194304, -1, -1,
             -241, -1, -1, -1, -1, -1, -1, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, -32768, -1, -1, -1, -2, -671096835, -1, -8388609, -66323585, -13,
             -1793, -32257, -247809, -1, -1, -513, 16252911, 0, 0, 0, -131072,
             33554383, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 8356095, 0, 0, 0, 0, 0,
             0, -256, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
             -268435449, -1, -1, -2049, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
             92274627, -65536, -2097153, -268435457, 591191935, 1, 0, -16777216, -1,
             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 127
    };
    return ((modelid >= 0) && ((modelid / 32) < sizeof(g_arrModelData)) && (g_arrModelData[modelid / 32] & (1 << (modelid % 32))));
}