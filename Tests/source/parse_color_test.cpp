#include <string>
#include "gtest/gtest.h"
#include "color.h"

using namespace std;


TEST(parse_color, edge_cases_all_zeroes)
{
	// Arrange
	const string &s = "00000000";

	// Act
	Color c = ParseColor(s);
	Color three_components = ParseColor(s.substr(2));

	// Assert
	ASSERT_EQ(0x00, c.r);
	ASSERT_EQ(0x00, c.g);
	ASSERT_EQ(0x00, c.b);
	ASSERT_EQ(0x00, c.a);

	ASSERT_EQ(0x00, three_components.r);
	ASSERT_EQ(0x00, three_components.g);
	ASSERT_EQ(0x00, three_components.b);
	ASSERT_EQ(0xFF, three_components.a);
}

TEST(parse_color, edge_cases_all_bits_set)
{
	// Arrange
	const string &s = "FFFFFFFF";

	// Act
	Color c = ParseColor(s);
	Color three_components = ParseColor(s.substr(2));

	// Assert
	ASSERT_EQ(0xFF, c.r);
	ASSERT_EQ(0xFF, c.g);
	ASSERT_EQ(0xFF, c.b);
	ASSERT_EQ(0xFF, c.a);

	ASSERT_EQ(0xFF, three_components.r);
	ASSERT_EQ(0xFF, three_components.g);
	ASSERT_EQ(0xFF, three_components.b);
	ASSERT_EQ(0xFF, three_components.a);

}

TEST(parse_color, different_values)
{
	// Arrange
	const string &s = "01020304";

	// Act
	Color c = ParseColor(s);
	Color three_components = ParseColor(s.substr(2));

	// Assert
	ASSERT_EQ(0x01, c.r);
	ASSERT_EQ(0x02, c.g);
	ASSERT_EQ(0x03, c.b);
	ASSERT_EQ(0x04, c.a);

	ASSERT_EQ(0x02, three_components.r);
	ASSERT_EQ(0x03, three_components.g);
	ASSERT_EQ(0x04, three_components.b);
	ASSERT_EQ(0xFF, three_components.a);
}