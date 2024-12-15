// Copyright by person HDD  


#include "Game/LoadScreenSaveGame.h"

FSaveMap ULoadScreenSaveGame::GetSavedMapWithMapName(const FString& InMapName)
{
    for (const FSaveMap& Map : SavedMaps)
    {
        if(Map.MapAssetName == InMapName)
        {
            return Map;
        }
    }

    return FSaveMap();
}

bool ULoadScreenSaveGame::HasMap(const FString& InMapName)
{
    for (const FSaveMap& Map : SavedMaps)
    {
        if(Map.MapAssetName == InMapName)
        {
            return true;
        }
    }

    return false;
}
