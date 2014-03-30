#include <algorithm>

inline bool NextPointInternal(const unsigned char *data, int x_pixels, int y_pixels, int pixel_length, 
						int *position, bool *eof)
{
	const int kBytesInColor = 3;
	bool is_white = true;

	for(; !*eof && is_white; *position += pixel_length)
	{
		const unsigned char *end = data + *position + kBytesInColor;
		is_white = std::find_if(
			data + *position, 
			end, 
			[](unsigned char p){return p != 0xFF;}) == end;

		*eof = !(*position < (x_pixels * y_pixels * pixel_length));
	}

	return !*eof;
}