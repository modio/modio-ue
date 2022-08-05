/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Internationalization/Regex.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/Interface.h"

#include "IModioUITextValidator.generated.h"

UENUM(BlueprintType)
enum class EModioTextValidationRule : uint8
{
	ETVR_ValidateEmailAddress,
	ETVR_ValidateAllLetters,
	ETVR_ValidateAllNumeric,
	ETVR_ValidateAlphanumeric,
	ETVR_ValidateLength,
	ETVR_ValidateNotEmpty,
	ETVR_ValidateNoWhitespace
};

USTRUCT(BlueprintType)
struct MODIOUI_API FModioTextValidationRule
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Rules")
	EModioTextValidationRule RuleToUse;

	///@brief Message to display if validation fails. Use "{InputText} as a placeholder for the text being validated"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Rules")
	FText ValidationMessage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "RuleToUse==EModioTextValidationRule::ETVR_ValidateLength", EditConditionHides), Category="Rules")
	int32 MinimumLength;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "RuleToUse==EModioTextValidationRule::ETVR_ValidateLength", EditConditionHides), Category="Rules")
	int32 MaximumLength;
};

UCLASS()
class MODIOUI_API UModioUIInputValidationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="ModioUIInputValidationLibrary")
	static bool ValidateEmailAddress(const FText& TextToValidate)
	{
		// Regex for validating email adress found here: https://mylittledevblog.com/2018/02/15/ue4-email-validation/
		// I would prefer to use https://stackoverflow.com/a/201378/12018052, but that doesn't work
		const FRegexPattern Pattern(TEXT("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+"));
		FRegexMatcher Matcher(Pattern, TextToValidate.ToString());

		return Matcher.FindNext();
	};

	UFUNCTION(BlueprintCallable, Category="ModioUIInputValidationLibrary")
	static bool ValidateAllLetters(const FText& TextToValidate)
	{
		FString TmpString = TextToValidate.ToString();
		for (TCHAR CurrentCharacter : TmpString)
		{
			if (!FChar::IsAlpha(CurrentCharacter))
			{
				return false;
			}
		}
		return true;
	};

	UFUNCTION(BlueprintCallable, Category="ModioUIInputValidationLibrary")
	static bool ValidateAllNumeric(const FText& TextToValidate)
	{
		return TextToValidate.IsNumeric();
	};

	UFUNCTION(BlueprintCallable, Category="ModioUIInputValidationLibrary")
	static bool ValidateAlphanumeric(const FText& TextToValidate)
	{
		FString TmpString = TextToValidate.ToString();
		for (TCHAR CurrentCharacter : TmpString)
		{
			if (!FChar::IsAlnum(CurrentCharacter))
			{
				return false;
			}
		}
		return true;
	};

	UFUNCTION(BlueprintCallable, Category="ModioUIInputValidationLibrary")
	static bool ValidateLength(const FText& TextToValidate, int32 DesiredMaximumLength, int32 DesiredMinimumLength)
	{
		return TextToValidate.ToString().Len() <= DesiredMaximumLength &&
			TextToValidate.ToString().Len() >= DesiredMinimumLength;
	};

	UFUNCTION(BlueprintCallable, Category="ModioUIInputValidationLibrary")
	static bool ValidateNotEmpty(const FText& TextToValidate)
	{
		return !TextToValidate.IsEmpty();
	};
	UFUNCTION(BlueprintCallable, Category="ModioUIInputValidationLibrary")
	static bool ValidateNoWhitespace(const FText& TextToValidate)
	{
		FString TmpString = TextToValidate.ToString();
		for (TCHAR CurrentCharacter : TmpString)
		{
			if (FChar::IsWhitespace(CurrentCharacter))
			{
				return false;
			}
		}
		return true;
	}

	UFUNCTION(BlueprintCallable, Category="ModioUIInputValidationLibrary")
	static bool ValidateUsingRule(const FModioTextValidationRule& Rule, const FText& TextToValidate,
								  FText& ValidationMessageText)
	{
		switch (Rule.RuleToUse)
		{
			case EModioTextValidationRule::ETVR_ValidateEmailAddress:
				if (!ValidateEmailAddress(TextToValidate))
				{
					ValidationMessageText =
						FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
					return false;
				}
				else
				{
					return true;
				}
				break;
			case EModioTextValidationRule::ETVR_ValidateAllLetters:
				if (!ValidateAllLetters(TextToValidate))
				{
					ValidationMessageText =
						FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
					return false;
				}
				else
				{
					return true;
				}
				break;
			case EModioTextValidationRule::ETVR_ValidateAllNumeric:
				if (!ValidateAllNumeric(TextToValidate))
				{
					ValidationMessageText =
						FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
					return false;
				}
				else
				{
					return true;
				}
				break;
			case EModioTextValidationRule::ETVR_ValidateAlphanumeric:
				if (!ValidateAlphanumeric(TextToValidate))
				{
					ValidationMessageText =
						FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
					return false;
				}
				else
				{
					return true;
				}
				break;
			case EModioTextValidationRule::ETVR_ValidateLength:
				if (!ValidateLength(TextToValidate, Rule.MaximumLength, Rule.MinimumLength))
				{
					ValidationMessageText =
						FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
					return false;
				}
				else
				{
					return true;
				}
				break;
			case EModioTextValidationRule::ETVR_ValidateNotEmpty:
				if (!ValidateNotEmpty(TextToValidate))
				{
					ValidationMessageText =
						FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
					return false;
				}
				else
				{
					return true;
				}
				break;
			case EModioTextValidationRule::ETVR_ValidateNoWhitespace:
				if (!ValidateNoWhitespace(TextToValidate))
				{
					ValidationMessageText =
						FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
					return false;
				}
				else
				{
					return true;
				}
				break;
		}
		return true;
	}
};

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUITextValidator : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUITextValidator : public IInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeGetTextValidationRules(TArray<FModioTextValidationRule>& Rules)
	{
		static TArray<FModioTextValidationRule> DefaultRules;
		Rules = DefaultRules;
	}

	bool NativeValidateText(const FText& InputText, FText& ValidationMessageText)
	{
		TArray<FModioTextValidationRule> Rules;
		NativeGetTextValidationRules(Rules);
		for (const FModioTextValidationRule& CurrentRule : Rules)
		{
			if (!UModioUIInputValidationLibrary::ValidateUsingRule(CurrentRule, InputText, ValidationMessageText))
			{
				SetValidationError_Implementation(ValidationMessageText);
				return false;
			}
		}

		FText Empty;
		
		SetValidationError_Implementation(Empty);
		return true;
	}

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIInputValidationLibrary")
	void SetValidationError(FText &ErrorText);

	void SetValidationError_Implementation(FText &ErrorText)
	{
		NativeSetValidationError(ErrorText);
	}

	virtual void NativeSetValidationError(FText ErrorText) {};
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIInputValidationLibrary")
	void GetTextValidationRules(TArray<FModioTextValidationRule>& Rules);

	void GetTextValidationRules_Implementation(TArray<FModioTextValidationRule>& Rules)
	{
		NativeGetTextValidationRules(Rules);
	}

	/// @brief performs validation on the text passed in, storing the validation message of the first rule that failed
	/// validation in ValidationMessageText if validation failed
	/// @param InputText the text to validate
	/// @param ValidationMessageText FText containing the localised failure message
	/// @returns true if the text passed all the validation rules
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIInputValidationLibrary")
	bool ValidateText(const FText& InputText, FText& ValidationMessageText);

	bool ValidateText_Implementation(const FText& InputText, FText& ValidationMessageText)
	{
		return NativeValidateText(InputText, ValidationMessageText);
	}
};