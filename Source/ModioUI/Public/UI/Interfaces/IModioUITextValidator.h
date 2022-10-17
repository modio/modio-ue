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

/**
* Enumeration for the possible validation rules
**/
UENUM(BlueprintType)
enum class EModioTextValidationRule : uint8
{
	/** Validate as an email  **/
	ETVR_ValidateEmailAddress,

	/** Validate that every character is a letter **/
	ETVR_ValidateAllLetters,

	/** Validate that every character is a number **/
	ETVR_ValidateAllNumeric, 

	/** Validate characters as numbers or letters **/
	ETVR_ValidateAlphanumeric,

	/** Validate the lenght of the stream **/
	ETVR_ValidateLength,

	/** Validate that has some content **/
	ETVR_ValidateNotEmpty,

	/** Validate the lack of spaces **/
	ETVR_ValidateNoWhitespace
};

/**
* Strong type struct with the necessary details to create a text validation rule
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioTextValidationRule
{
	GENERATED_BODY()
	/**
	* The base rule to use in this instance. One of the many listed in EModioTextValidationRule
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Rules")
	EModioTextValidationRule RuleToUse;

	/**
	* @brief Message to display if validation fails.Use "{InputText} as a placeholder for the text being validated"
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Rules")
	FText ValidationMessage;

	/**
	* Apply the validation rule to at least minimum lenght
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "RuleToUse==EModioTextValidationRule::ETVR_ValidateLength", EditConditionHides), Category="Rules")
	int32 MinimumLength;
	
	/**
	* Apply the validation rule to a maximum lenght
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "RuleToUse==EModioTextValidationRule::ETVR_ValidateLength", EditConditionHides), Category="Rules")
	int32 MaximumLength;
};

/**
* Class with static functions to validate elements listed in the enum EModioTextValidationRule
**/
UCLASS()
class MODIOUI_API UModioUIInputValidationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	* Confirm the text provided matches an email address
	* @param TextToValidate String to check against email pattern
	* @return True when string contains a valid email, false otherwise
	**/
	UFUNCTION(BlueprintCallable, Category="ModioUIInputValidationLibrary")
	static bool ValidateEmailAddress(const FText& TextToValidate)
	{
		// Regex for validating email adress found here: https://mylittledevblog.com/2018/02/15/ue4-email-validation/
		// I would prefer to use https://stackoverflow.com/a/201378/12018052, but that doesn't work
		const FRegexPattern Pattern(TEXT("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+"));
		FRegexMatcher Matcher(Pattern, TextToValidate.ToString());

		return Matcher.FindNext();
	};

	/**
	* Confirm the text provided matches an all letters rule
	* @param TextToValidate String to check against all letters rule
	* @return True when string validation passes, false otherwise
	**/
	UFUNCTION(BlueprintCallable, Category = "ModioUIInputValidationLibrary")
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

	/**
	* Confirm the text provided matches an all numeric rule
	* @param TextToValidate String to check against all numeric rule
	* @return True when string validation passes, false otherwise
	**/
	UFUNCTION(BlueprintCallable, Category="ModioUIInputValidationLibrary")
	static bool ValidateAllNumeric(const FText& TextToValidate)
	{
		return TextToValidate.IsNumeric();
	};

	/**
	* Confirm the text provided matches the alphanumeric rule
	* @param TextToValidate String to check against the alphanumeric rule
	* @return True when string validation passes, false otherwise
	**/
	UFUNCTION(BlueprintCallable, Category = "ModioUIInputValidationLibrary")
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

	/**
	* Confirm the text provided matches the length rule
	* @param TextToValidate String to check against the length rule
	* @return True when string validation passes, false otherwise
	**/
	UFUNCTION(BlueprintCallable, Category="ModioUIInputValidationLibrary")
	static bool ValidateLength(const FText& TextToValidate, int32 DesiredMaximumLength, int32 DesiredMinimumLength)
	{
		return TextToValidate.ToString().Len() <= DesiredMaximumLength &&
			TextToValidate.ToString().Len() >= DesiredMinimumLength;
	};

	/**
	* Confirm the text provided matches the not-empty rule
	* @param TextToValidate String to check against the not-empty rule
	* @return True when string validation passes, false otherwise
	**/
	UFUNCTION(BlueprintCallable, Category="ModioUIInputValidationLibrary")
	static bool ValidateNotEmpty(const FText& TextToValidate)
	{
		return !TextToValidate.IsEmpty();
	};
	
	/**
	* Confirm the text provided matches the lack of whitespace rule
	* @param TextToValidate String to check against the lack of whitespace rule
	* @return True when string validation passes, false otherwise
	**/
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

	/**
	* Convenience function to check a rule against a text and returns a validation text
	* when the rule was not matched
	* @param Rule The text validation rule to apply
	* @param TextToValidate The string to apply a rule to
	* @param ValidationMessageText A string that receives a formatted value when rule checks fail
	* @return True when string validation passes, false otherwise
	**/
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

/**
* Modio UI interface that defines a validation actions over text, subclass of UInterface
**/
UINTERFACE(BlueprintType)
class MODIOUI_API UModioUITextValidator : public UInterface
{
	GENERATED_BODY()
};

/**
* Modio UI interface that defines a validation actions over text, subclass of IInterface
**/
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
	/**
	* Change the validation error with new text
	* @param ErrorText The new string to set on this instance
	**/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIInputValidationLibrary")
	void SetValidationError(FText &ErrorText);

	// SetValidationError_Implementation
	void SetValidationError_Implementation(FText &ErrorText)
	{
		NativeSetValidationError(ErrorText);
	}

	// NativeSetValidationError
	virtual void NativeSetValidationError(FText ErrorText) {};
	
	/**
	* Retrieve the validation rules stored in this instance
	* @param Rules Reference array where the rules will be stored
	**/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIInputValidationLibrary")
	void GetTextValidationRules(TArray<FModioTextValidationRule>& Rules);

	// GetTextValidationRules_Implementation
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

	// ValidateText_Implementation
	bool ValidateText_Implementation(const FText& InputText, FText& ValidationMessageText)
	{
		return NativeValidateText(InputText, ValidationMessageText);
	}
};