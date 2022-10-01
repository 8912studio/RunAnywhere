#include <gtest/gtest.h>
#include "module/tool/rgb/rgb_command_factory.h"

using namespace ra::module::tool::rgb;

TEST(RGBCommandFactoryTest, DifferentPartCount) {

	std::vector<std::pair<std::wstring, std::uint32_t>> inputs{
		{ L"rgb 1", 0xff010000 },
		{ L"rgb 1,", 0xff010000 },
		{ L"rgb 1,2", 0xff010200 },
		{ L"rgb 1,2,", 0xff010200 },
		{ L"rgb 1,2,3", 0xff010203 },
		{ L"rgb 1,2,3,", 0xff010203 },
		{ L"rgb 1,2,3,4", 0x04010203 },
	};

	for (const auto& each_input : inputs) {

		ra::utility::CommandLine command_line(each_input.first);
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->color, zaf::Color::FromARGB(each_input.second));
	}

	std::vector<std::wstring> fail_inputs{
		L"rgb 1,2,3,4,",
		L"rgb 1,2,3,4,5",
	};

	for (const auto& each_input : fail_inputs) {

		ra::utility::CommandLine command_line(each_input);
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_FALSE(result.has_value());
	}
}


TEST(RGBCommandFactoryTest, DecimalIntegerComponentFormat) {

	{
		ra::utility::CommandLine command_line(L"rgb 84,93,143");
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->color, zaf::Color::FromRGB(0x545d8f));
	}

	{
		ra::utility::CommandLine command_line(L"rgb 84,93,143,234");
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->color, zaf::Color::FromARGB(0xea545d8f));
	}

	std::vector<std::wstring> fail_inputs{
		L"rgb 10,0.34,4",
		L"rgb 10,0x4,4",
		L"rgb 10,2432,4",
		L"rgb -10,5,4",
	};

	for (const auto& each_input : fail_inputs) {

		ra::utility::CommandLine command_line(each_input);
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_FALSE(result.has_value());
	}
}


TEST(RGBCommandFactoryTest, DecimalFloatComponentFormat) {

	{
		ra::utility::CommandLine command_line(L"rgb 0.4,0.2,0.8");
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->color, zaf::Color::FromRGB(0x6633cc));
	}

	{
		ra::utility::CommandLine command_line(L"rgb 0.4,0.2,0.8,0.6");
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->color, zaf::Color::FromARGB(0x996633cc));
	}

	{
		ra::utility::CommandLine command_line(L"rgb 1.0,1,1");
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->color, zaf::Color::FromRGB(0xffffff));
	}

	std::vector<std::wstring> fail_inputs{
		L"rgb 1.0,24,5",
		L"rgb 1.0,0x24,5",
		L"rgb -0.4,0.4,0.4",
		L"rgb 0.4,1.1,0.4",
	};

	for (const auto& each_input : fail_inputs) {

		ra::utility::CommandLine command_line(each_input);
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_FALSE(result.has_value());
	}
}


TEST(RGBCommandFactoryTest, HexComponentFormat) {

	{
		ra::utility::CommandLine command_line(L"rgb 0x");
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->color, zaf::Color::FromRGB(0));
	}

	{
		ra::utility::CommandLine command_line(L"rgb 0x1,0");
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->color, zaf::Color::FromRGB(0x010000));
	}

	{
		ra::utility::CommandLine command_line(L"rgb 0x1,0,x");
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->color, zaf::Color::FromRGB(0x010000));
	}

	{
		ra::utility::CommandLine command_line(L"rgb 0x1,0x2,0x3");
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->color, zaf::Color::FromRGB(0x010203));
	}

	{
		ra::utility::CommandLine command_line(L"rgb 0x1,0x2,0x3,0x4");
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->color, zaf::Color::FromARGB(0x04010203));
	}
}


TEST(RGBCommandFactoryTest, ARGBFormat) {

	std::vector<std::pair<std::wstring, std::uint32_t>> inputs{
		{ L"rgb #", 0xff000000 },
		{ L"rgb #1", 0xff000001 },
		{ L"rgb #12", 0xff000012 },
		{ L"rgb #123", 0xff000123 },
		{ L"rgb #1234", 0xff001234 },
		{ L"rgb #12345", 0xff012345 },
		{ L"rgb #123456", 0xff123456 },
		{ L"rgb #1234567", 0x01234567 },
		{ L"rgb #12345678", 0x12345678 },
		{ L"rgb #a86cbb", 0xffa86cbb },
		{ L"rgb #7c9bef10", 0x7c9bef10 },
	};

	for (const auto& each_input : inputs) {

		ra::utility::CommandLine command_line(each_input.first);
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->color, zaf::Color::FromARGB(each_input.second));
	}

	std::vector<std::wstring> fail_inputs{
		L"rgb #1234567890",
		L"rgb #zdw23",
	};

	for (const auto& each_input : fail_inputs) {

		ra::utility::CommandLine command_line(each_input);
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_FALSE(result.has_value());
	}
}


TEST(RGBCommandFactoryTest, AdditionalAlpha) {

	std::vector<std::pair<std::wstring, std::uint32_t>> inputs{
		{ L"rgb $", 0xff000000 },
		{ L"rgb $0", 0x00000000 },
		{ L"rgb $200", 0xc8000000 },
		{ L"rgb $0.8", 0xcc000000 },
		{ L"rgb $xab", 0xab000000 },
		{ L"rgb $0xac", 0xac000000 },
		{ L"rgb $x", 0x00000000 },
		{ L"rgb $0x", 0x00000000 },
		{ L"rgb #ccbbaa $x78", 0x78ccbbaa },
		{ L"rgb 0x1,0x2,0x3,0x4 $0.5", 0x02010203 },
	};

	for (const auto& each_input : inputs) {

		ra::utility::CommandLine command_line(each_input.first);
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->color, zaf::Color::FromARGB(each_input.second));
	}

	std::vector<std::wstring> fail_inputs{
		L"rgb $-1",
		L"rgb $256",
		L"rgb $-0.1",
		L"rgb $1.1",
		L"rgb $0x100",
		L"rgb $b01",
		L"rgb $abc",
	};

	for (const auto& each_input : fail_inputs) {

		ra::utility::CommandLine command_line(each_input);
		auto result = RGBCommandFactory::Parse(command_line);
		ASSERT_FALSE(result.has_value());
	}
}