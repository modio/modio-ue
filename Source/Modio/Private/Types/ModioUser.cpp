#include "Types/ModioUser.h"
#include "Internal/ModioConvert.h"

FModioUser::FModioUser(const Modio::User& User)
	: UserId(ToUnreal(User.UserId)),
	  Username(ToUnreal(User.Username)),
	  DateOnline(ToUnreal(User.DateOnline)),
	  ProfileUrl(ToUnreal(User.ProfileUrl))
{}

FModioOptionalUser::FModioOptionalUser(TOptional<FModioUser>&& ModInfoList) : Internal(ModInfoList) {}
