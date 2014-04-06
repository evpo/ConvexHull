#include <vector>
#include "gtest/gtest.h"
#include "color.h"
#include "color_evaluator.h"

typedef ColorEvaluator<GreyColor> ColorEvaluatorT;

TEST(color_evaluator, When_Empty_List_Then_All_Colors_Test)
{
	// Arrange
	std::vector<GreyColor> v;
	ColorEvaluatorT evaluator(v.begin(), v.end());

	// Act
	ASSERT_TRUE(evaluator.TestColor(GreyColor(0x10, 0xFF)));
}

TEST(color_evaluator, When_One_Color_Then_It_Is_Not_Tested)
{
	// Arrange
	std::vector<GreyColor> v;
	v.push_back(GreyColor(0x80, 0xFF));

	ColorEvaluatorT evaluator(v.begin(), v.end());

	// Act
	ASSERT_TRUE(evaluator.TestColor(GreyColor(0x80, 0xFF)));
}

TEST(color_evaluator, When_Black_And_White_Then_Only_Black_Tested)
{
	// Arrange
	std::vector<GreyColor> v;
	v.push_back(GreyColor(0x00, 0xFF));
	v.push_back(GreyColor(0xFF, 0xFF));

	ColorEvaluatorT evaluator(v.begin(), v.end());

	// Act
	ASSERT_TRUE(evaluator.TestColor(GreyColor(0x00, 0xFF)));
	ASSERT_FALSE(evaluator.TestColor(GreyColor(0xFF, 0xFF)));
}

TEST(color_evaluator, When_Black_Gray_And_White_Then_Black_And_Gray_Tested)
{
	// Arrange
	std::vector<GreyColor> v;
	v.push_back(GreyColor(0x00, 0xFF));
	v.push_back(GreyColor(0x80, 0xFF));
	v.push_back(GreyColor(0xFF, 0xFF));

	ColorEvaluatorT evaluator(v.begin(), v.end());

	// Act
	ASSERT_TRUE(evaluator.TestColor(GreyColor(0x00, 0xFF)));
	ASSERT_TRUE(evaluator.TestColor(GreyColor(0x80, 0xFF)));
	ASSERT_FALSE(evaluator.TestColor(GreyColor(0xFF, 0xFF)));
}

TEST(color_evaluator, When_Four_Colors_Then_Low_Edge_Color_Tested_And_Upper_Edge_Not_Tested)
{
	// Arrange
	std::vector<GreyColor> v;
	v.push_back(GreyColor(0x00, 0xFF));
	v.push_back(GreyColor(0x80, 0xFF));
	v.push_back(GreyColor(0x85, 0xFF));
	v.push_back(GreyColor(0xFF, 0xFF));

	ColorEvaluatorT evaluator(v.begin(), v.end());

	// Act
	ASSERT_TRUE(evaluator.TestColor(GreyColor(0x00, 0xFF)));
	ASSERT_FALSE(evaluator.TestColor(GreyColor(0xFF, 0xFF)));
}

TEST(color_evaluator, When_All_Colors_Then_0x7E_Tested_And_0x82_Not_Tested)
{
	// Arrange
	std::vector<GreyColor> v;
	for(unsigned char i = 0x00; i < 0xFF; i ++)
		v.push_back(GreyColor(i, 0xFF));

	v.push_back(GreyColor(0xFF, 0xFF));

	ColorEvaluatorT evaluator(v.begin(), v.end());

	// Act
	ASSERT_TRUE(evaluator.TestColor(GreyColor(0x00, 0xFF)));
	ASSERT_TRUE(evaluator.TestColor(GreyColor(0x7E, 0xFF)));
	ASSERT_FALSE(evaluator.TestColor(GreyColor(0x82, 0xFF)));
	ASSERT_FALSE(evaluator.TestColor(GreyColor(0xFF, 0xFF)));
}


