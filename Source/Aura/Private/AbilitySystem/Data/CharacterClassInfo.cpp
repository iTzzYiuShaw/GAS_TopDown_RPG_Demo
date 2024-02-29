// Copyright Shawwy


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfomation(ECharacterClass CharacterClass)
{
	return CharacterClassInfomation.FindChecked(CharacterClass);
}
