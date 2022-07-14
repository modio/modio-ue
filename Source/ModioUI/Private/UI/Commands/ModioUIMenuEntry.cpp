// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Commands/ModioUIMenuEntry.h"

uint32 GetTypeHash(const FModioUIMenuEntry& Object)
{
	uint32 Hash = FCrc::MemCrc32(&Object, sizeof(FModioUIMenuEntry));
	return Hash;
}
