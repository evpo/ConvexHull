#include <vector>

#include "gtest/gtest.h"
#include "point_loader_nextpoint.h"

using namespace std;

namespace {

	class PointLoaderFxt : public ::testing::Test
	{
	protected:
		static const unsigned int white_data_[];
		static const unsigned int data_[];
		
		static const unsigned char *Convert(const unsigned int *p)
		{
			return reinterpret_cast<const unsigned char*>(p);
		}

		const int x_pixels_;
		const int y_pixels_;
		const int pixel_length_;
	public:
		PointLoaderFxt()
			:x_pixels_(2), y_pixels_(2), pixel_length_(4)
		{
		}
	};

	const unsigned int PointLoaderFxt::white_data_[] = 
	{
		0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF
	};

	const unsigned int PointLoaderFxt::data_[] = 
	{
		0xFFFF0000, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFF00FFFF
	};

	TEST_F(PointLoaderFxt, NextPointInternal_When_Buffer_Is_White_Then_EOF_FalseResult_OffsetAfterEnd)
	{
		// Arrange
		bool eof = false;
		int offset = 0;

		// Act
		bool result = NextPointInternal(Convert(white_data_), x_pixels_, y_pixels_, pixel_length_, &offset, &eof);

		// Assert
		ASSERT_FALSE(result);
		ASSERT_TRUE(eof);
		ASSERT_EQ(20, offset);
	}

	TEST_F(PointLoaderFxt, NextPointInternal_When_First_Point_Is_Not_White_Then_FalseEOF_TrueResult_OffsetZero)
	{
		// Arrange
		bool eof = false;
		int offset = 0;

		// Act
		bool result = NextPointInternal(Convert(data_), x_pixels_, y_pixels_, pixel_length_, &offset, &eof);

		// Assert
		ASSERT_FALSE(eof);
		ASSERT_TRUE(result);
		ASSERT_EQ(4, offset);
	}

	TEST_F(PointLoaderFxt, NextPointInternal_When_Next_Point_Is_Not_White_Then_FalseEOF_TrueResult_RightOffset)
	{
		// Arrange
		bool eof = false;
		int offset = 0;
		NextPointInternal(Convert(data_), x_pixels_, y_pixels_, pixel_length_, &offset, &eof);

		// Act
		bool result = NextPointInternal(Convert(data_), x_pixels_, y_pixels_, pixel_length_, &offset, &eof);

		// Assert
		ASSERT_FALSE(eof);
		ASSERT_TRUE(result);
		ASSERT_EQ(16, offset);
	}

	TEST_F(PointLoaderFxt, NextPointInternal_When_Having_Passed_Points_Then_EOF_FalseResult_RightOffset)
	{
		// Arrange
		bool eof = false;
		int offset = 0;
		NextPointInternal(Convert(data_), x_pixels_, y_pixels_, pixel_length_, &offset, &eof);
		NextPointInternal(Convert(data_), x_pixels_, y_pixels_, pixel_length_, &offset, &eof);

		// Act
		bool result = NextPointInternal(Convert(data_), x_pixels_, y_pixels_, pixel_length_, &offset, &eof);

		// Assert
		ASSERT_TRUE(eof);
		ASSERT_FALSE(result);
		ASSERT_EQ(20, offset);
	}



}