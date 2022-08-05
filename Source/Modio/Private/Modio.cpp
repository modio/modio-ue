/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Modio.h"
#include "HAL/PlatformMisc.h"
#include "Math/Color.h"
#include "Stats/Stats.h"
#include "Containers/Array.h"

DEFINE_LOG_CATEGORY(LogModio)

DECLARE_STATS_GROUP(TEXT("Modio"), STATGROUP_Modio, STATCAT_Advanced);

#ifdef MODIO_UNREAL_PROFILING_SUPPORT

extern "C"
{
	static TArray<FName>& ProfilingScopes()
	{
		static TArray<FName> ScopeStorage;
		return ScopeStorage;
	}
	static TStatId& GetOrCreateStat(FName StatName) {
		static TMap<FName, TStatId> StatIDs;
		if (TStatId* FoundID = StatIDs.Find(StatName)) {
			return *FoundID;
		}
		else
		{
			StatIDs.Add(StatName,FDynamicStats::CreateStatId<FStatGroup_STATGROUP_Modio>(StatName));
			return StatIDs[StatName];
		}
	}
	PRAGMA_DISABLE_OPTIMIZATION
	void modio_profile_scope_start(const char* Name, void** Data)
	{
		if (FThreadStats::IsCollectingData())
		{
			FThreadStats::AddMessage(GetOrCreateStat(FName(Name)).GetName(), EStatOperation::CycleScopeStart);
		}
		FCpuProfilerTrace::OutputBeginEvent(FCpuProfilerTrace::OutputEventType(Name));
	}
	void modio_profile_scope_end(const char* Name, void* Data)
	{
		if (FThreadStats::IsCollectingData())
		{
			FThreadStats::AddMessage(GetOrCreateStat(FName(Name)).GetName(), EStatOperation::CycleScopeEnd);
		}
		FCpuProfilerTrace::OutputEndEvent();
	}
	void modio_profile_push(const char* Name)
	{
		//FCpuProfilerTrace::OutputBeginEvent(FCpuProfilerTrace::OutputEventType(Name));
	}
	void modio_profile_pop()
	{
		//FCpuProfilerTrace::OutputEndEvent();
	}
	PRAGMA_ENABLE_OPTIMIZATION
}

#endif
