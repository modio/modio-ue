/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Engine/Engine.h"
#include "Libraries/ModioSDKLibrary.h"
#include "Misc/AutomationTest.h"
#include "Types/ModioUnsigned64.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioUnsigned64ConversionBoolZeroTest,
								 "Modio.ModioUnsigned64.Conversion.Bool.ZeroFalse",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioUnsigned64ConversionBoolZeroTest::RunTest(const FString& Parameters)
{
	FModioUnsigned64 Empty = FModioUnsigned64(0);
	TestEqual("ZeroIsFalse", (bool)Empty, false);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioUnsigned64ConversionBoolNonZeroTest,
								 "Modio.ModioUnsigned64.Conversion.Bool.NonZeroTrue",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)

bool FModioUnsigned64ConversionBoolNonZeroTest::RunTest(const FString& Parameters)
{
	FModioUnsigned64 NonZero = FModioUnsigned64(123);
	TestEqual("NonZeroIsTrue", (bool)NonZero, true);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioUnsigned64OperatorLessTest, "Modio.ModioUnsigned64.Arithmetic.LessThan",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)

bool FModioUnsigned64OperatorLessTest::RunTest(const FString& Parameters)
{
	FModioUnsigned64 First = FModioUnsigned64(1);
	FModioUnsigned64 Second = FModioUnsigned64(2);
	TestEqual("LessThanTrue", First < Second, true);
	TestEqual("LessThanFalse", Second < First, false);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioUnsigned64OperatorGreaterTest, "Modio.ModioUnsigned64.Arithmetic.GreaterThan",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)

bool FModioUnsigned64OperatorGreaterTest::RunTest(const FString& Parameters)
{
	FModioUnsigned64 First = FModioUnsigned64(1);
	FModioUnsigned64 Second = FModioUnsigned64(2);
	TestEqual("GreaterThanTrue", Second > First, true);
	TestEqual("GreaterThanFalse", First > Second, false);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioUnsigned64OperatorEqualTest, "Modio.ModioUnsigned64.Arithmetic.Equal",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)

bool FModioUnsigned64OperatorEqualTest::RunTest(const FString& Parameters)
{
	FModioUnsigned64 First = FModioUnsigned64(123456789);
	FModioUnsigned64 Second = FModioUnsigned64(123456789);
	FModioUnsigned64 Third = FModioUnsigned64(987654321);
	TestEqual("EqualsTrue", First == Second, true);
	TestEqual("EqualsFalse", First == Third, false);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioUnsigned64OperatorNotEqualTest, "Modio.ModioUnsigned64.Arithmetic.NotEqual",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)

bool FModioUnsigned64OperatorNotEqualTest::RunTest(const FString& Parameters)
{
	FModioUnsigned64 First = FModioUnsigned64(123456789);
	FModioUnsigned64 Second = FModioUnsigned64(123456789);
	FModioUnsigned64 Third = FModioUnsigned64(987654321);
	TestEqual("NotEqualsTrue", First != Third, true);
	TestEqual("NotEqualsFalse", First != Second, false);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioUnsigned64OperatorSubtractTest, "Modio.ModioUnsigned64.Arithmetic.Subtract",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)

bool FModioUnsigned64OperatorSubtractTest::RunTest(const FString& Parameters)
{
	FModioUnsigned64 First = FModioUnsigned64(10);
	FModioUnsigned64 Second = FModioUnsigned64(4);
	FModioUnsigned64 Result = FModioUnsigned64(6);
	TestEqual("SubtractionCorrectResult", First - Second, Result);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioUnsigned64OperatorAddTest, "Modio.ModioUnsigned64.Arithmetic.Add",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)

bool FModioUnsigned64OperatorAddTest::RunTest(const FString& Parameters)
{
	FModioUnsigned64 First = FModioUnsigned64(6);
	FModioUnsigned64 Second = FModioUnsigned64(4);
	FModioUnsigned64 Result = FModioUnsigned64(10);
	TestEqual("AdditionCorrectResult", First + Second, Result);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioUnsigned64ArithmeticPercentTest, "Modio.ModioUnsigned64.Arithmetic.Percent",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)

bool FModioUnsigned64ArithmeticPercentTest::RunTest(const FString& Parameters)
{
	uint64 Value = 0xabcdF00d;
	FModioUnsigned64 First = FModioUnsigned64(Value);
	FModioUnsigned64 Second = First * 2;
	float Result = UModioUnsigned64Library::Percentage_Unsigned64(First, Second);
	TestEqual("ResultCorrect1", FMath::IsNearlyEqual(Result, 50.f), true);
	Result = UModioUnsigned64Library::Percentage_Unsigned64(Second, First);
	TestEqual("ResultCorrect2", FMath::IsNearlyEqual(Result, 200.f), true);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioUnsigned64MakeBreakComponentTest, "Modio.ModioUnsigned64.MakeBreak",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)

bool FModioUnsigned64MakeBreakComponentTest::RunTest(const FString& Parameters)
{
	uint64 Value = 0xabcdF00d1234F00d;
	FModioUnsigned64 First = FModioUnsigned64(Value);
	int32 High;
	int32 Low;
	UModioUnsigned64Library::BreakToComponents(First, High, Low);
	TestEqual("HighComponentCorrect", static_cast<uint32>(High), 0xabcdf00d);
	TestEqual("LowComponentCorrect", static_cast<uint32>(Low), 0x1234f00d);
	FModioUnsigned64 Second = UModioUnsigned64Library::MakeFromComponents(High, Low);
	TestEqual("RoundTripCorrectResult", Second, First);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioUnsigned64ValueConversionTest, "Modio.ModioUnsigned64.Conversion.UInt64",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)

bool FModioUnsigned64ValueConversionTest::RunTest(const FString& Parameters)
{
	FModioUnsigned64 Value = FModioUnsigned64(0xabcdF00d1234F00d);
	uint64 ConvertedValue = (uint64) Value;
	TestEqual("ValueConversionCorrect", Value == ConvertedValue, true);
	return true;
}

#endif