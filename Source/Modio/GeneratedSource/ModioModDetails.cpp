#ifdef MODIO_SEPARATE_COMPILATION
	#include "modio/core/entities/ModioModDetails.h"
#endif

#include "modio/detail/ModioJsonHelpers.h"

namespace Modio
{
	void from_json(const nlohmann::json& Json, Modio::ModDetails& ModDetails)
	{
		from_json(Json, static_cast<Modio::PagedResult&>(ModDetails));

		Modio::Detail::ParseSafe(Json, ModDetails.InternalList, "data");
	}
}