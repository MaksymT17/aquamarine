/*
 *****************************************************************************
 *                                                                           *
 *                          Platform Independent                             *
 *                    Bitmap Image Reader Writer Library                     *
 *                                                                           *
 * Author: Arash Partow - 2002                                               *
 * URL: http://partow.net/programming/bitmap/index.html                      *
 *                                                                           *
 * Note: This library only supports 24-bits per pixel bitmap format files.   *
 *                                                                           *
 * Copyright notice:                                                         *
 * Free use of the Platform Independent Bitmap Image Reader Writer Library   *
 * is permitted under the guidelines and in accordance with the most current *
 * version of the MIT License.                                               *
 * http://www.opensource.org/licenses/MIT                                    *
 *                                                                           *
 *****************************************************************************
*/


#ifndef INCLUDE_BITMAP_IMAGE_HPP
#define INCLUDE_BITMAP_IMAGE_HPP

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>

class bitmap_image
{
public:

	enum channel_mode {
		rgb_mode = 0,
		bgr_mode = 1
	};

	enum color_plane {
		blue_plane = 0,
		green_plane = 1,
		red_plane = 2
	};

	struct rgb_t
	{
		unsigned char   red;
		unsigned char green;
		unsigned char  blue;
	};

	bitmap_image()
		: file_name_(""),
		width_(0),
		height_(0),
		row_increment_(0),
		bytes_per_pixel_(3),
		channel_mode_(bgr_mode)
	{}

	bitmap_image(const std::string& filename)
		: file_name_(filename),
		width_(0),
		height_(0),
		row_increment_(0),
		bytes_per_pixel_(0),
		channel_mode_(bgr_mode)
	{
		load_bitmap();
	}

	bitmap_image(const unsigned int width, const unsigned int height)
		: file_name_(""),
		width_(width),
		height_(height),
		row_increment_(0),
		bytes_per_pixel_(3),
		channel_mode_(bgr_mode)
	{
		create_bitmap();
	}

	bitmap_image(const bitmap_image& image)
		: file_name_(image.file_name_),
		width_(image.width_),
		height_(image.height_),
		row_increment_(0),
		bytes_per_pixel_(3),
		channel_mode_(bgr_mode)
	{
		create_bitmap();
		data_ = image.data_;
	}

	bitmap_image& operator=(const bitmap_image& image)
	{
		if (this != &image)
		{
			file_name_ = image.file_name_;
			bytes_per_pixel_ = image.bytes_per_pixel_;
			width_ = image.width_;
			height_ = image.height_;
			row_increment_ = 0;
			channel_mode_ = image.channel_mode_;
			create_bitmap();
			data_ = image.data_;
		}

		return *this;
	}

	inline bool operator!()
	{
		return (data_.size() == 0) ||
			(width_ == 0) ||
			(height_ == 0) ||
			(row_increment_ == 0);
	}

	inline void clear(const unsigned char v = 0x00)
	{
		std::fill(data_.begin(), data_.end(), v);
	}

	inline unsigned char red_channel(const unsigned int x, const unsigned int y) const
	{
		return data_[(y * row_increment_) + (x * bytes_per_pixel_ + 2)];
	}

	inline unsigned char green_channel(const unsigned int x, const unsigned int y) const
	{
		return data_[(y * row_increment_) + (x * bytes_per_pixel_ + 1)];
	}

	inline unsigned char blue_channel(const unsigned int x, const unsigned int y) const
	{
		return data_[(y * row_increment_) + (x * bytes_per_pixel_ + 0)];
	}

	inline void red_channel(const unsigned int x, const unsigned int y, const unsigned char value)
	{
		data_[(y * row_increment_) + (x * bytes_per_pixel_ + 2)] = value;
	}

	inline void green_channel(const unsigned int x, const unsigned int y, const unsigned char value)
	{
		data_[(y * row_increment_) + (x * bytes_per_pixel_ + 1)] = value;
	}

	inline void blue_channel(const unsigned int x, const unsigned int y, const unsigned char value)
	{
		data_[(y * row_increment_) + (x * bytes_per_pixel_ + 0)] = value;
	}

	inline unsigned char* row(unsigned int row_index) const
	{
		return const_cast<unsigned char*>(&data_[(row_index * row_increment_)]);
	}

	inline void get_pixel(const unsigned int x, const unsigned int y,
		unsigned char& red,
		unsigned char& green,
		unsigned char& blue) const
	{
		const unsigned int y_offset = y * row_increment_;
		const unsigned int x_offset = x * bytes_per_pixel_;
		const unsigned int offset = y_offset + x_offset;

		blue = data_[offset + 0];
		green = data_[offset + 1];
		red = data_[offset + 2];
	}

	template <typename RGB>
	inline void get_pixel(const unsigned int x, const unsigned int y, RGB& colour) const
	{
		get_pixel(x, y, colour.red, colour.green, colour.blue);
	}

	inline rgb_t get_pixel(const unsigned int x, const unsigned int y) const
	{
		rgb_t colour;
		get_pixel(x, y, colour.red, colour.green, colour.blue);
		return colour;
	}

	inline void set_pixel(const unsigned int x, const unsigned int y,
		const unsigned char red,
		const unsigned char green,
		const unsigned char blue)
	{
		const unsigned int y_offset = y * row_increment_;
		const unsigned int x_offset = x * bytes_per_pixel_;
		const unsigned int offset = y_offset + x_offset;

		data_[offset + 0] = blue;
		data_[offset + 1] = green;
		data_[offset + 2] = red;
	}

	template <typename RGB>
	inline void set_pixel(const unsigned int x, const unsigned int y, const RGB& colour)
	{
		set_pixel(x, y, colour.red, colour.green, colour.blue);
	}

	inline bool copy_from(const bitmap_image& image)
	{
		if (
			(image.height_ != height_) ||
			(image.width_ != width_)
			)
		{
			return false;
		}

		data_ = image.data_;

		return true;
	}

	inline bool copy_from(const bitmap_image& source_image,
		const unsigned int& x_offset,
		const unsigned int& y_offset)
	{
		if ((x_offset + source_image.width_) > width_) { return false; }
		if ((y_offset + source_image.height_) > height_) { return false; }

		for (unsigned int y = 0; y < source_image.height_; ++y)
		{
			unsigned char* itr1 = row(y + y_offset) + x_offset * bytes_per_pixel_;
			const unsigned char* itr2 = source_image.row(y);
			const unsigned char* itr2_end = itr2 + source_image.width_ * bytes_per_pixel_;

			std::copy(itr2, itr2_end, itr1);
		}

		return true;
	}

	inline bool region(const unsigned int& x,
		const unsigned int& y,
		const unsigned int& width,
		const unsigned int& height,
		bitmap_image& dest_image) const
	{
		if ((x + width) > width_) { return false; }
		if ((y + height) > height_) { return false; }

		if (
			(dest_image.width_ < width_) ||
			(dest_image.height_ < height_)
			)
		{
			dest_image.setwidth_height(width, height);
		}

		for (unsigned int r = 0; r < height; ++r)
		{
			unsigned char* itr1 = row(r + y) + x * bytes_per_pixel_;
			unsigned char* itr1_end = itr1 + (width * bytes_per_pixel_);
			unsigned char* itr2 = dest_image.row(r);

			std::copy(itr1, itr1_end, itr2);
		}

		return true;
	}

	inline bool roi_from_center(const unsigned int& cx,
		const unsigned int& cy,
		const unsigned int& width,
		const unsigned int& height,
		bitmap_image& dest_image) const
	{
		return region(cx - (width / 2), cy - (height / 2),
			width, height,
			dest_image);
	}

	inline bool set_region(const unsigned int&  x,
		const unsigned int&  y,
		const unsigned int&  width,
		const unsigned int&  height,
		const unsigned char& value)
	{
		if ((x + width) > width_) { return false; }
		if ((y + height) > height_) { return false; }

		for (unsigned int r = 0; r < height; ++r)
		{
			unsigned char* itr = row(r + y) + x * bytes_per_pixel_;
			unsigned char* itr_end = itr + (width * bytes_per_pixel_);

			std::fill(itr, itr_end, value);
		}

		return true;
	}

	inline bool set_region(const unsigned int&  x,
		const unsigned int&  y,
		const unsigned int&  width,
		const unsigned int&  height,
		const color_plane    color,
		const unsigned char& value)
	{
		if ((x + width) > width_) { return false; }
		if ((y + height) > height_) { return false; }

		const unsigned int color_plane_offset = offset(color);

		for (unsigned int r = 0; r < height; ++r)
		{
			unsigned char* itr = row(r + y) + x * bytes_per_pixel_ + color_plane_offset;
			unsigned char* itr_end = itr + (width * bytes_per_pixel_);

			while (itr != itr_end)
			{
				*itr = value;
				itr += bytes_per_pixel_;
			}
		}

		return true;
	}

	inline bool set_region(const unsigned int&  x,
		const unsigned int&  y,
		const unsigned int&  width,
		const unsigned int&  height,
		const unsigned char& red,
		const unsigned char& green,
		const unsigned char& blue)
	{
		if ((x + width) > width_) { return false; }
		if ((y + height) > height_) { return false; }

		for (unsigned int r = 0; r < height; ++r)
		{
			unsigned char* itr = row(r + y) + x * bytes_per_pixel_;
			unsigned char* itr_end = itr + (width * bytes_per_pixel_);

			while (itr != itr_end)
			{
				*(itr++) = blue;
				*(itr++) = green;
				*(itr++) = red;
			}
		}

		return true;
	}

	void reflective_image(bitmap_image& image, const bool include_diagnols = false)
	{
		image.setwidth_height(3 * width_, 3 * height_, true);

		image.copy_from(*this, width_, height_);

		vertical_flip();

		image.copy_from(*this, width_, 0);
		image.copy_from(*this, width_, 2 * height_);

		vertical_flip();
		horizontal_flip();

		image.copy_from(*this, 0, height_);
		image.copy_from(*this, 2 * width_, height_);

		horizontal_flip();

		if (include_diagnols)
		{
			bitmap_image tile = *this;

			tile.vertical_flip();
			tile.horizontal_flip();

			image.copy_from(tile, 0, 0);
			image.copy_from(tile, 2 * width_, 0);
			image.copy_from(tile, 2 * width_, 2 * height_);
			image.copy_from(tile, 0, 2 * height_);
		}
	}

	inline unsigned int width() const
	{
		return width_;
	}

	inline unsigned int height() const
	{
		return height_;
	}

	inline unsigned int bytes_per_pixel() const
	{
		return bytes_per_pixel_;
	}

	inline unsigned int pixel_count() const
	{
		return width_ * height_;
	}

	inline void setwidth_height(const unsigned int width,
		const unsigned int height,
		const bool clear = false)
	{
		data_.clear();
		width_ = width;
		height_ = height;

		create_bitmap();

		if (clear)
		{
			std::fill(data_.begin(), data_.end(), static_cast<unsigned char>(0x00));
		}
	}

	void save_image(const std::string& file_name) const
	{
		std::ofstream stream(file_name.c_str(), std::ios::binary);

		if (!stream)
		{
			std::cerr << "bitmap_image::save_image(): Error - Could not open file " << file_name << " for writing!" << std::endl;
			return;
		}

		bitmap_information_header bih;

		bih.width = width_;
		bih.height = height_;
		bih.bit_count = static_cast<unsigned short>(bytes_per_pixel_ << 3);
		bih.clr_important = 0;
		bih.clr_used = 0;
		bih.compression = 0;
		bih.planes = 1;
		bih.size = bih.struct_size();
		bih.x_pels_per_meter = 0;
		bih.y_pels_per_meter = 0;
		bih.size_image = (((bih.width * bytes_per_pixel_) + 3) & 0x0000FFFC) * bih.height;

		bitmap_file_header bfh;

		bfh.type = 19778;
		bfh.size = bfh.struct_size() + bih.struct_size() + bih.size_image;
		bfh.reserved1 = 0;
		bfh.reserved2 = 0;
		bfh.off_bits = bih.struct_size() + bfh.struct_size();

		write_bfh(stream, bfh);
		write_bih(stream, bih);

		unsigned int padding = (4 - ((3 * width_) % 4)) % 4;
		char padding_data[4] = { 0x00, 0x00, 0x00, 0x00 };

		for (unsigned int i = 0; i < height_; ++i)
		{
			const unsigned char* data_ptr = &data_[(row_increment_ * (height_ - i - 1))];

			stream.write(reinterpret_cast<const char*>(data_ptr), sizeof(unsigned char) * bytes_per_pixel_ * width_);
			stream.write(padding_data, padding);
		}

		stream.close();
	}

	inline void set_all_ith_bits_low(const unsigned int bitr_index)
	{
		unsigned char mask = static_cast<unsigned char>(~(1 << bitr_index));

		for (unsigned char* itr = data(); itr != end(); ++itr)
		{
			*itr &= mask;
		}
	}

	inline void set_all_ith_bits_high(const unsigned int bitr_index)
	{
		unsigned char mask = static_cast<unsigned char>(1 << bitr_index);

		for (unsigned char* itr = data(); itr != end(); ++itr)
		{
			*itr |= mask;
		}
	}

	inline void set_all_ith_channels(const unsigned int& channel, const unsigned char& value)
	{
		for (unsigned char* itr = (data() + channel); itr < end(); itr += bytes_per_pixel_)
		{
			*itr = value;
		}
	}

	inline void set_channel(const color_plane color, const unsigned char& value)
	{
		for (unsigned char* itr = (data() + offset(color)); itr < end(); itr += bytes_per_pixel_)
		{
			*itr = value;
		}
	}

	inline void ror_channel(const color_plane color, const unsigned int& ror)
	{
		for (unsigned char* itr = (data() + offset(color)); itr < end(); itr += bytes_per_pixel_)
		{
			*itr = static_cast<unsigned char>(((*itr) >> ror) | ((*itr) << (8 - ror)));
		}
	}

	inline void set_all_channels(const unsigned char& value)
	{
		for (unsigned char* itr = data(); itr < end(); )
		{
			*(itr++) = value;
		}
	}

	inline void set_all_channels(const unsigned char& r_value,
		const unsigned char& g_value,
		const unsigned char& b_value)
	{
		for (unsigned char* itr = (data() + 0); itr < end(); itr += bytes_per_pixel_)
		{
			*(itr + 0) = b_value;
			*(itr + 1) = g_value;
			*(itr + 2) = r_value;
		}
	}

	inline void invert_color_planes()
	{
		for (unsigned char* itr = data(); itr < end(); *itr = ~(*itr), ++itr);
	}

	inline void add_to_color_plane(const color_plane color, const unsigned char& value)
	{
		for (unsigned char* itr = (data() + offset(color)); itr < end(); itr += bytes_per_pixel_)
		{
			(*itr) += value;
		}
	}

	inline void convert_to_grayscale()
	{
		double r_scaler = 0.299;
		double g_scaler = 0.587;
		double b_scaler = 0.114;

		if (rgb_mode == channel_mode_)
		{
			std::swap(r_scaler, b_scaler);
		}

		for (unsigned char* itr = data(); itr < end(); )
		{
			unsigned char gray_value = static_cast<unsigned char>
				(
				(r_scaler * (*(itr + 2))) +
					(g_scaler * (*(itr + 1))) +
					(b_scaler * (*(itr + 0)))
					);

			*(itr++) = gray_value;
			*(itr++) = gray_value;
			*(itr++) = gray_value;
		}
	}

	inline const unsigned char* data() const
	{
		return data_.data();
	}

	inline unsigned char* data()
	{
		return const_cast<unsigned char*>(data_.data());
	}

	inline void bgr_to_rgb()
	{
		if ((bgr_mode == channel_mode_) && (3 == bytes_per_pixel_))
		{
			reverse_channels();
			channel_mode_ = rgb_mode;
		}
	}

	inline void rgb_to_bgr()
	{
		if ((rgb_mode == channel_mode_) && (3 == bytes_per_pixel_))
		{
			reverse_channels();
			channel_mode_ = bgr_mode;
		}
	}

	inline void reverse()
	{
		unsigned char* itr1 = data();
		unsigned char* itr2 = end() - bytes_per_pixel_;

		while (itr1 < itr2)
		{
			for (std::size_t i = 0; i < bytes_per_pixel_; ++i)
			{
				unsigned char* citr1 = itr1 + i;
				unsigned char* citr2 = itr2 + i;

				std::swap(*citr1, *citr2);
			}

			itr1 += bytes_per_pixel_;
			itr2 -= bytes_per_pixel_;
		}
	}

	inline void horizontal_flip()
	{
		for (unsigned int y = 0; y < height_; ++y)
		{
			unsigned char* itr1 = row(y);
			unsigned char* itr2 = itr1 + row_increment_ - bytes_per_pixel_;

			while (itr1 < itr2)
			{
				for (unsigned int i = 0; i < bytes_per_pixel_; ++i)
				{
					unsigned char* p1 = (itr1 + i);
					unsigned char* p2 = (itr2 + i);

					std::swap(*p1, *p2);
				}

				itr1 += bytes_per_pixel_;
				itr2 -= bytes_per_pixel_;
			}
		}
	}

	inline void vertical_flip()
	{
		for (unsigned int y = 0; y < (height_ / 2); ++y)
		{
			unsigned char* itr1 = row(y);
			unsigned char* itr2 = row(height_ - y - 1);

			for (std::size_t x = 0; x < row_increment_; ++x)
			{
				std::swap(*(itr1 + x), *(itr2 + x));
			}
		}
	}

	inline void export_color_plane(const color_plane color, unsigned char* image)
	{
		for (unsigned char* itr = (data() + offset(color)); itr < end(); ++image, itr += bytes_per_pixel_)
		{
			(*image) = (*itr);
		}
	}

	inline void export_color_plane(const color_plane color, bitmap_image& image)
	{
		if (
			(width_ != image.width_) ||
			(height_ != image.height_)
			)
		{
			image.setwidth_height(width_, height_);
		}

		image.clear();

		unsigned char* itr1 = (data() + offset(color));
		unsigned char* itr1_end = end();
		unsigned char* itr2 = (image.data() + offset(color));

		while (itr1 < itr1_end)
		{
			(*itr2) = (*itr1);

			itr1 += bytes_per_pixel_;
			itr2 += bytes_per_pixel_;
		}
	}

	inline void export_response_image(const color_plane color, double* response_image)
	{
		double* resp_itr = response_image;

		for (unsigned char* itr = (data() + offset(color)); itr < end(); ++response_image, itr += bytes_per_pixel_)
		{
			*(resp_itr++) = (1.0 * (*itr)) / 256.0;
		}
	}

	inline void export_gray_scale_response_image(double* response_image) const
	{
		double* resp_itr = response_image;

		for (const unsigned char* itr = data(); itr < end(); itr += bytes_per_pixel_)
		{
			unsigned char gray_value = static_cast<unsigned char>
				(
				(0.299 * (*(itr + 2))) +
					(0.587 * (*(itr + 1))) +
					(0.114 * (*(itr + 0)))
					);

			*(resp_itr++) = (1.0 * gray_value) / 256.0;
		}
	}

	inline void export_rgb(double* red, double* green, double* blue) const
	{
		if (bgr_mode != channel_mode_)
			return;

		for (const unsigned char* itr = data(); itr < end(); ++red, ++green, ++blue)
		{
			(*blue) = (1.0 * (*(itr++))) / 256.0;
			(*green) = (1.0 * (*(itr++))) / 256.0;
			(*red) = (1.0 * (*(itr++))) / 256.0;
		}
	}

	inline void export_rgb(float* red, float* green, float* blue) const
	{
		if (bgr_mode != channel_mode_)
			return;

		for (const unsigned char* itr = data(); itr < end(); ++red, ++green, ++blue)
		{
			(*blue) = (1.0f * (*(itr++))) / 256.0f;
			(*green) = (1.0f * (*(itr++))) / 256.0f;
			(*red) = (1.0f * (*(itr++))) / 256.0f;
		}
	}

	inline void export_rgb(unsigned char* red, unsigned char* green, unsigned char* blue) const
	{
		if (bgr_mode != channel_mode_)
			return;

		for (const unsigned char* itr = data(); itr < end(); ++red, ++green, ++blue)
		{
			(*blue) = *(itr++);
			(*green) = *(itr++);
			(*red) = *(itr++);
		}
	}

	inline void export_ycbcr(double* y, double* cb, double* cr) const
	{
		if (bgr_mode != channel_mode_)
			return;

		for (const unsigned char* itr = data(); itr < end(); ++y, ++cb, ++cr)
		{
			const double blue = (1.0 * (*(itr++)));
			const double green = (1.0 * (*(itr++)));
			const double red = (1.0 * (*(itr++)));

			(*y) = clamp<double>(16.0 + (1.0 / 256.0) * (65.738 * red + 129.057 * green + 25.064 * blue), 1.0, 254);
			(*cb) = clamp<double>(128.0 + (1.0 / 256.0) * (-37.945 * red - 74.494 * green + 112.439 * blue), 1.0, 254);
			(*cr) = clamp<double>(128.0 + (1.0 / 256.0) * (112.439 * red - 94.154 * green - 18.285 * blue), 1.0, 254);
		}
	}

	inline void export_rgb_normal(double* red, double* green, double* blue) const
	{
		if (bgr_mode != channel_mode_)
			return;

		for (const unsigned char* itr = data(); itr < end(); ++red, ++green, ++blue)
		{
			(*blue) = (1.0 * (*(itr++)));
			(*green) = (1.0 * (*(itr++)));
			(*red) = (1.0 * (*(itr++)));
		}
	}

	inline void export_rgb_normal(float* red, float* green, float* blue) const
	{
		if (bgr_mode != channel_mode_)
			return;

		for (const unsigned char* itr = data(); itr < end(); ++red, ++green, ++blue)
		{
			(*blue) = (1.0f * (*(itr++)));
			(*green) = (1.0f * (*(itr++)));
			(*red) = (1.0f * (*(itr++)));
		}
	}

	inline void import_rgb(double* red, double* green, double* blue)
	{
		if (bgr_mode != channel_mode_)
			return;

		for (unsigned char* itr = data(); itr < end(); ++red, ++green, ++blue)
		{
			*(itr++) = static_cast<unsigned char>(256.0 * (*blue));
			*(itr++) = static_cast<unsigned char>(256.0 * (*green));
			*(itr++) = static_cast<unsigned char>(256.0 * (*red));
		}
	}

	inline void import_rgb(float* red, float* green, float* blue)
	{
		if (bgr_mode != channel_mode_)
			return;

		for (unsigned char* itr = data(); itr < end(); ++red, ++green, ++blue)
		{
			*(itr++) = static_cast<unsigned char>(256.0f * (*blue));
			*(itr++) = static_cast<unsigned char>(256.0f * (*green));
			*(itr++) = static_cast<unsigned char>(256.0f * (*red));
		}
	}

	inline void import_rgb(unsigned char* red, unsigned char* green, unsigned char* blue)
	{
		if (bgr_mode != channel_mode_)
			return;

		for (unsigned char* itr = data(); itr < end(); ++red, ++green, ++blue)
		{
			*(itr++) = (*blue);
			*(itr++) = (*green);
			*(itr++) = (*red);
		}
	}

	inline void import_ycbcr(double* y, double* cb, double* cr)
	{
		if (bgr_mode != channel_mode_)
			return;

		for (unsigned char* itr = data(); itr < end(); ++y, ++cb, ++cr)
		{
			double y_ = (*y);
			double cb_ = (*cb);
			double cr_ = (*cr);

			*(itr++) = static_cast<unsigned char>(clamp((298.082 * y_ + 516.412 * cb_) / 256.0 - 276.836, 0.0, 255.0));
			*(itr++) = static_cast<unsigned char>(clamp((298.082 * y_ - 100.291 * cb_ - 208.120 * cr_) / 256.0 + 135.576, 0.0, 255.0));
			*(itr++) = static_cast<unsigned char>(clamp((298.082 * y_ + 408.583 * cr_) / 256.0 - 222.921, 0.0, 255.0));
		}
	}

	inline void import_gray_scale_clamped(double* gray)
	{
		if (bgr_mode != channel_mode_)
			return;

		for (unsigned char* itr = data(); itr < end(); ++gray)
		{
			unsigned char c = static_cast<unsigned char>(clamp<double>(256.0 * (*gray), 0.0, 255.0));

			*(itr + 0) = c;
			*(itr + 1) = c;
			*(itr + 2) = c;

			itr += 3;
		}
	}

	inline void import_rgb_clamped(double* red, double* green, double* blue)
	{
		if (bgr_mode != channel_mode_)
			return;

		for (unsigned char* itr = data(); itr < end(); ++red, ++green, ++blue)
		{
			*(itr++) = static_cast<unsigned char>(clamp<double>(256.0 * (*blue), 0.0, 255.0));
			*(itr++) = static_cast<unsigned char>(clamp<double>(256.0 * (*green), 0.0, 255.0));
			*(itr++) = static_cast<unsigned char>(clamp<double>(256.0 * (*red), 0.0, 255.0));
		}
	}

	inline void import_rgb_clamped(float* red, float* green, float* blue)
	{
		if (bgr_mode != channel_mode_)
			return;

		for (unsigned char* itr = data(); itr < end(); ++red, ++green, ++blue)
		{
			*(itr++) = static_cast<unsigned char>(clamp<double>(256.0f * (*blue), 0.0, 255.0));
			*(itr++) = static_cast<unsigned char>(clamp<double>(256.0f * (*green), 0.0, 255.0));
			*(itr++) = static_cast<unsigned char>(clamp<double>(256.0f * (*red), 0.0, 255.0));
		}
	}

	inline void import_rgb_normal(double* red, double* green, double* blue)
	{
		if (bgr_mode != channel_mode_)
			return;

		for (unsigned char* itr = data(); itr < end(); ++red, ++green, ++blue)
		{
			*(itr++) = static_cast<unsigned char>(*blue);
			*(itr++) = static_cast<unsigned char>(*green);
			*(itr++) = static_cast<unsigned char>(*red);
		}
	}

	inline void import_rgb_normal(float* red, float* green, float* blue)
	{
		if (bgr_mode != channel_mode_)
			return;

		for (unsigned char* itr = data(); itr < end(); ++red, ++green, ++blue)
		{
			*(itr++) = static_cast<unsigned char>(*blue);
			*(itr++) = static_cast<unsigned char>(*green);
			*(itr++) = static_cast<unsigned char>(*red);
		}
	}

	inline void subsample(bitmap_image& dest) const
	{
		/*
		   Half sub-sample of original image.
		*/
		unsigned int w = 0;
		unsigned int h = 0;

		bool odd_width = false;
		bool odd_height = false;

		if (0 == (width_ % 2))
			w = width_ / 2;
		else
		{
			w = 1 + (width_ / 2);
			odd_width = true;
		}

		if (0 == (height_ % 2))
			h = height_ / 2;
		else
		{
			h = 1 + (height_ / 2);
			odd_height = true;
		}

		unsigned int horizontal_upper = (odd_width) ? (w - 1) : w;
		unsigned int vertical_upper = (odd_height) ? (h - 1) : h;

		dest.setwidth_height(w, h);
		dest.clear();

		unsigned char* s_itr[3];
		const unsigned char*  itr1[3];
		const unsigned char*  itr2[3];

		s_itr[0] = dest.data() + 0;
		s_itr[1] = dest.data() + 1;
		s_itr[2] = dest.data() + 2;

		itr1[0] = data() + 0;
		itr1[1] = data() + 1;
		itr1[2] = data() + 2;

		itr2[0] = data() + row_increment_ + 0;
		itr2[1] = data() + row_increment_ + 1;
		itr2[2] = data() + row_increment_ + 2;

		unsigned int total = 0;

		for (unsigned int j = 0; j < vertical_upper; ++j)
		{
			for (unsigned int i = 0; i < horizontal_upper; ++i)
			{
				for (unsigned int k = 0; k < bytes_per_pixel_; s_itr[k] += bytes_per_pixel_, ++k)
				{
					total = 0;
					total += *(itr1[k]);
					total += *(itr1[k]);
					total += *(itr2[k]);
					total += *(itr2[k]);

					itr1[k] += bytes_per_pixel_;
					itr1[k] += bytes_per_pixel_;
					itr2[k] += bytes_per_pixel_;
					itr2[k] += bytes_per_pixel_;

					*(s_itr[k]) = static_cast<unsigned char>(total >> 2);
				}
			}

			if (odd_width)
			{
				for (unsigned int k = 0; k < bytes_per_pixel_; s_itr[k] += bytes_per_pixel_, ++k)
				{
					total = 0;
					total += *(itr1[k]);
					total += *(itr2[k]);

					itr1[k] += bytes_per_pixel_;
					itr2[k] += bytes_per_pixel_;

					*(s_itr[k]) = static_cast<unsigned char>(total >> 1);
				}
			}

			for (unsigned int k = 0; k < bytes_per_pixel_; ++k)
			{
				itr1[k] += row_increment_;
			}

			if (j != (vertical_upper - 1))
			{
				for (unsigned int k = 0; k < bytes_per_pixel_; ++k)
				{
					itr2[k] += row_increment_;
				}
			}
		}

		if (odd_height)
		{
			for (unsigned int i = 0; i < horizontal_upper; ++i)
			{
				for (unsigned int k = 0; k < bytes_per_pixel_; s_itr[k] += bytes_per_pixel_, ++k)
				{
					total = 0;
					total += *(itr1[k]);
					total += *(itr2[k]);

					itr1[k] += bytes_per_pixel_;
					itr2[k] += bytes_per_pixel_;

					*(s_itr[k]) = static_cast<unsigned char>(total >> 1);
				}
			}

			if (odd_width)
			{
				for (unsigned int k = 0; k < bytes_per_pixel_; ++k)
				{
					(*(s_itr[k])) = *(itr1[k]);
				}
			}
		}
	}

	inline void upsample(bitmap_image& dest) const
	{
		/*
		   2x up-sample of original image.
		*/

		dest.setwidth_height(2 * width_, 2 * height_);
		dest.clear();

		const unsigned char* s_itr[3];
		unsigned char*  itr1[3];
		unsigned char*  itr2[3];

		s_itr[0] = data() + 0;
		s_itr[1] = data() + 1;
		s_itr[2] = data() + 2;

		itr1[0] = dest.data() + 0;
		itr1[1] = dest.data() + 1;
		itr1[2] = dest.data() + 2;

		itr2[0] = dest.data() + dest.row_increment_ + 0;
		itr2[1] = dest.data() + dest.row_increment_ + 1;
		itr2[2] = dest.data() + dest.row_increment_ + 2;

		for (unsigned int j = 0; j < height_; ++j)
		{
			for (unsigned int i = 0; i < width_; ++i)
			{
				for (unsigned int k = 0; k < bytes_per_pixel_; s_itr[k] += bytes_per_pixel_, ++k)
				{
					*(itr1[k]) = *(s_itr[k]); itr1[k] += bytes_per_pixel_;
					*(itr1[k]) = *(s_itr[k]); itr1[k] += bytes_per_pixel_;

					*(itr2[k]) = *(s_itr[k]); itr2[k] += bytes_per_pixel_;
					*(itr2[k]) = *(s_itr[k]); itr2[k] += bytes_per_pixel_;
				}
			}

			for (unsigned int k = 0; k < bytes_per_pixel_; ++k)
			{
				itr1[k] += dest.row_increment_;
				itr2[k] += dest.row_increment_;
			}
		}
	}

	inline void alpha_blend(const double& alpha, const bitmap_image& image)
	{
		if (
			(image.width_ != width_) ||
			(image.height_ != height_)
			)
		{
			return;
		}

		if ((alpha < 0.0) || (alpha > 1.0))
		{
			return;
		}

		unsigned char* itr1 = data();
		const unsigned char* itr1_end = end();
		const unsigned char* itr2 = image.data();

		double alpha_compliment = 1.0 - alpha;

		while (itr1 != itr1_end)
		{
			*(itr1) = static_cast<unsigned char>((alpha * (*itr2)) + (alpha_compliment * (*itr1)));
			++itr1;
			++itr2;
		}
	}

	inline double psnr(const bitmap_image& image)
	{
		if (
			(image.width_ != width_) ||
			(image.height_ != height_)
			)
		{
			return 0.0;
		}

		const unsigned char* itr1 = data();
		const unsigned char* itr2 = image.data();

		double mse = 0.0;

		while (itr1 != end())
		{
			const double v = (static_cast<double>(*itr1) - static_cast<double>(*itr2));

			mse += v * v;
			++itr1;
			++itr2;
		}

		if (mse <= 0.0000001)
		{
			return 1000000.0;
		}
		else
		{
			mse /= (3.0 * width_ * height_);

			return 20.0 * std::log10(255.0 / std::sqrt(mse));
		}
	}

	inline double psnr(const unsigned int& x,
		const unsigned int& y,
		const bitmap_image& image)
	{
		if ((x + image.width()) > width_) { return 0.0; }
		if ((y + image.height()) > height_) { return 0.0; }

		double mse = 0.0;

		const unsigned int height = image.height();
		const unsigned int width = image.width();

		for (unsigned int r = 0; r < height; ++r)
		{
			const unsigned char* itr1 = row(r + y) + x * bytes_per_pixel_;
			const unsigned char* itr1_end = itr1 + (width * bytes_per_pixel_);
			const unsigned char* itr2 = image.row(r);

			while (itr1 != itr1_end)
			{
				double v = (static_cast<double>(*itr1) - static_cast<double>(*itr2));
				mse += v * v;
				++itr1;
				++itr2;
			}
		}

		if (mse <= 0.0000001)
		{
			return 1000000.0;
		}
		else
		{
			mse /= (3.0 * image.width() * image.height());
			return 20.0 * std::log10(255.0 / std::sqrt(mse));
		}
	}

	inline void histogram(const color_plane color, double hist[256]) const
	{
		std::fill(hist, hist + 256, 0.0);

		for (const unsigned char* itr = (data() + offset(color)); itr < end(); itr += bytes_per_pixel_)
		{
			++hist[(*itr)];
		}
	}

	inline void histogram_normalized(const color_plane color, double hist[256]) const
	{
		histogram(color, hist);

		double* h_itr = hist;
		const double* h_end = hist + 256;
		const double pixel_count = static_cast<double>(width_ * height_);

		while (h_end != h_itr)
		{
			*(h_itr++) /= pixel_count;
		}
	}

	inline unsigned int offset(const color_plane color) const
	{
		switch (channel_mode_)
		{
		case rgb_mode: {
			switch (color)
			{
			case red_plane: return 0;
			case green_plane: return 1;
			case blue_plane: return 2;
			default: return std::numeric_limits<unsigned int>::max();
			}
		}

		case bgr_mode: {
			switch (color)
			{
			case red_plane: return 2;
			case green_plane: return 1;
			case blue_plane: return 0;
			default: return std::numeric_limits<unsigned int>::max();
			}
		}

		default: return std::numeric_limits<unsigned int>::max();
		}
	}

	inline void incremental()
	{
		unsigned char current_color = 0;

		for (unsigned char* itr = data(); itr < end();)
		{
			(*itr++) = (current_color);
			(*itr++) = (current_color);
			(*itr++) = (current_color);

			++current_color;
		}
	}

	inline void reverse_channels()
	{
		if (3 != bytes_per_pixel_)
			return;

		for (unsigned char* itr = data(); itr < end(); itr += bytes_per_pixel_)
		{
			std::swap(*(itr + 0), *(itr + 2));
		}
	}

private:

	inline const unsigned char* end() const
	{
		return data_.data() + data_.size();
	}

	inline unsigned char* end()
	{
		return const_cast<unsigned char*>(data() + data_.size());
	}

	struct bitmap_file_header
	{
		unsigned short type;
		unsigned int   size;
		unsigned short reserved1;
		unsigned short reserved2;
		unsigned int   off_bits;

		unsigned int struct_size() const
		{
			return sizeof(type) +
				sizeof(size) +
				sizeof(reserved1) +
				sizeof(reserved2) +
				sizeof(off_bits);
		}

		void clear()
		{
			std::memset(this, 0x00, sizeof(bitmap_file_header));
		}
	};

	struct bitmap_information_header
	{
		unsigned int   size;
		unsigned int   width;
		unsigned int   height;
		unsigned short planes;
		unsigned short bit_count;
		unsigned int   compression;
		unsigned int   size_image;
		unsigned int   x_pels_per_meter;
		unsigned int   y_pels_per_meter;
		unsigned int   clr_used;
		unsigned int   clr_important;

		unsigned int struct_size() const
		{
			return sizeof(size) +
				sizeof(width) +
				sizeof(height) +
				sizeof(planes) +
				sizeof(bit_count) +
				sizeof(compression) +
				sizeof(size_image) +
				sizeof(x_pels_per_meter) +
				sizeof(y_pels_per_meter) +
				sizeof(clr_used) +
				sizeof(clr_important);
		}

		void clear()
		{
			std::memset(this, 0x00, sizeof(bitmap_information_header));
		}
	};

	inline bool big_endian() const
	{
		unsigned int v = 0x01;

		return (1 != reinterpret_cast<char*>(&v)[0]);
	}

	inline unsigned short flip(const unsigned short& v) const
	{
		return ((v >> 8) | (v << 8));
	}

	inline unsigned int flip(const unsigned int& v) const
	{
		return (
			((v & 0xFF000000) >> 0x18) |
			((v & 0x000000FF) << 0x18) |
			((v & 0x00FF0000) >> 0x08) |
			((v & 0x0000FF00) << 0x08)
			);
	}

	template <typename T>
	inline void read_from_stream(std::ifstream& stream, T& t)
	{
		stream.read(reinterpret_cast<char*>(&t), sizeof(T));
	}

	template <typename T>
	inline void write_to_stream(std::ofstream& stream, const T& t) const
	{
		stream.write(reinterpret_cast<const char*>(&t), sizeof(T));
	}

	inline void read_bfh(std::ifstream& stream, bitmap_file_header& bfh)
	{
		read_from_stream(stream, bfh.type);
		read_from_stream(stream, bfh.size);
		read_from_stream(stream, bfh.reserved1);
		read_from_stream(stream, bfh.reserved2);
		read_from_stream(stream, bfh.off_bits);

		if (big_endian())
		{
			bfh.type = flip(bfh.type);
			bfh.size = flip(bfh.size);
			bfh.reserved1 = flip(bfh.reserved1);
			bfh.reserved2 = flip(bfh.reserved2);
			bfh.off_bits = flip(bfh.off_bits);
		}
	}

	inline void write_bfh(std::ofstream& stream, const bitmap_file_header& bfh) const
	{
		if (big_endian())
		{
			write_to_stream(stream, flip(bfh.type));
			write_to_stream(stream, flip(bfh.size));
			write_to_stream(stream, flip(bfh.reserved1));
			write_to_stream(stream, flip(bfh.reserved2));
			write_to_stream(stream, flip(bfh.off_bits));
		}
		else
		{
			write_to_stream(stream, bfh.type);
			write_to_stream(stream, bfh.size);
			write_to_stream(stream, bfh.reserved1);
			write_to_stream(stream, bfh.reserved2);
			write_to_stream(stream, bfh.off_bits);
		}
	}

	inline void read_bih(std::ifstream& stream, bitmap_information_header& bih)
	{
		read_from_stream(stream, bih.size);
		read_from_stream(stream, bih.width);
		read_from_stream(stream, bih.height);
		read_from_stream(stream, bih.planes);
		read_from_stream(stream, bih.bit_count);
		read_from_stream(stream, bih.compression);
		read_from_stream(stream, bih.size_image);
		read_from_stream(stream, bih.x_pels_per_meter);
		read_from_stream(stream, bih.y_pels_per_meter);
		read_from_stream(stream, bih.clr_used);
		read_from_stream(stream, bih.clr_important);

		if (big_endian())
		{
			bih.size = flip(bih.size);
			bih.width = flip(bih.width);
			bih.height = flip(bih.height);
			bih.planes = flip(bih.planes);
			bih.bit_count = flip(bih.bit_count);
			bih.compression = flip(bih.compression);
			bih.size_image = flip(bih.size_image);
			bih.x_pels_per_meter = flip(bih.x_pels_per_meter);
			bih.y_pels_per_meter = flip(bih.y_pels_per_meter);
			bih.clr_used = flip(bih.clr_used);
			bih.clr_important = flip(bih.clr_important);
		}
	}

	inline void write_bih(std::ofstream& stream, const bitmap_information_header& bih) const
	{
		if (big_endian())
		{
			write_to_stream(stream, flip(bih.size));
			write_to_stream(stream, flip(bih.width));
			write_to_stream(stream, flip(bih.height));
			write_to_stream(stream, flip(bih.planes));
			write_to_stream(stream, flip(bih.bit_count));
			write_to_stream(stream, flip(bih.compression));
			write_to_stream(stream, flip(bih.size_image));
			write_to_stream(stream, flip(bih.x_pels_per_meter));
			write_to_stream(stream, flip(bih.y_pels_per_meter));
			write_to_stream(stream, flip(bih.clr_used));
			write_to_stream(stream, flip(bih.clr_important));
		}
		else
		{
			write_to_stream(stream, bih.size);
			write_to_stream(stream, bih.width);
			write_to_stream(stream, bih.height);
			write_to_stream(stream, bih.planes);
			write_to_stream(stream, bih.bit_count);
			write_to_stream(stream, bih.compression);
			write_to_stream(stream, bih.size_image);
			write_to_stream(stream, bih.x_pels_per_meter);
			write_to_stream(stream, bih.y_pels_per_meter);
			write_to_stream(stream, bih.clr_used);
			write_to_stream(stream, bih.clr_important);
		}
	}

	inline std::size_t file_size(const std::string& file_name) const
	{
		std::ifstream file(file_name.c_str(), std::ios::in | std::ios::binary);
		if (!file) return 0;
		file.seekg(0, std::ios::end);
		return static_cast<std::size_t>(file.tellg());
	}

	void create_bitmap()
	{
		row_increment_ = width_ * bytes_per_pixel_;
		data_.resize(height_ * row_increment_);
	}

	void load_bitmap()
	{
		std::ifstream stream(file_name_.c_str(), std::ios::binary);

		if (!stream)
		{
			std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - file " << file_name_ << " not found!" << std::endl;
			return;
		}

		width_ = 0;
		height_ = 0;

		bitmap_file_header bfh;
		bitmap_information_header bih;

		bfh.clear();
		bih.clear();

		read_bfh(stream, bfh);
		read_bih(stream, bih);

		if (bfh.type != 19778)
		{
			bfh.clear();
			bih.clear();

			stream.close();

			std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - Invalid type value " << bfh.type << " expected 19778." << std::endl;
			return;
		}

		if (bih.bit_count != 24)
		{
			bfh.clear();
			bih.clear();

			stream.close();

			std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - Invalid bit depth " << bih.bit_count << " expected 24." << std::endl;

			return;
		}

		if (bih.size != bih.struct_size())
		{
			bfh.clear();
			bih.clear();

			stream.close();

			std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - Invalid BIH size " << bih.size << " expected " << bih.struct_size() << std::endl;

			return;
		}

		width_ = bih.width;
		height_ = bih.height;

		bytes_per_pixel_ = bih.bit_count >> 3;

		unsigned int padding = (4 - ((3 * width_) % 4)) % 4;
		char padding_data[4] = { 0,0,0,0 };

		std::size_t bitmap_file_size = file_size(file_name_);

		std::size_t bitmap_logical_size = (height_ * width_ * bytes_per_pixel_) +
			(height_ * padding) +
			bih.struct_size() +
			bfh.struct_size();

		if (bitmap_file_size != bitmap_logical_size)
		{
			bfh.clear();
			bih.clear();

			stream.close();

			std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - Mismatch between logical and physical sizes of bitmap. " <<
				"Logical: " << bitmap_logical_size << " " <<
				"Physical: " << bitmap_file_size << std::endl;

			return;
		}

		create_bitmap();

		for (unsigned int i = 0; i < height_; ++i)
		{
			unsigned char* data_ptr = row(height_ - i - 1); // read in inverted row order

			stream.read(reinterpret_cast<char*>(data_ptr), sizeof(char) * bytes_per_pixel_ * width_);
			stream.read(padding_data, padding);
		}
	}

	template <typename T>
	inline T clamp(const T& v, const T& lower_range, const T& upper_range) const
	{
		if (v < lower_range)
			return lower_range;
		else if (v > upper_range)
			return upper_range;
		else
			return v;
	}

	std::string  file_name_;
	unsigned int width_;
	unsigned int height_;
	unsigned int row_increment_;
	unsigned int bytes_per_pixel_;
	channel_mode channel_mode_;
	std::vector<unsigned char> data_;
};

typedef bitmap_image::rgb_t rgb_t;

inline bool operator==(const rgb_t& c0, const rgb_t& c1)
{
	return (c0.red == c1.red) &&
		(c0.green == c1.green) &&
		(c0.blue == c1.blue);
}

inline bool operator!=(const rgb_t& c0, const rgb_t& c1)
{
	return (c0.red != c1.red) ||
		(c0.green != c1.green) ||
		(c0.blue != c1.blue);
}

inline std::size_t hamming_distance(const rgb_t& c0, const rgb_t& c1)
{
	std::size_t result = 0;

	if (c0.red != c1.red) ++result;
	if (c0.green != c1.green) ++result;
	if (c0.blue != c1.blue) ++result;

	return result;
}

inline rgb_t make_colour(const unsigned int& red, const unsigned int& green, const unsigned int& blue)
{
	rgb_t result;

	result.red = static_cast<unsigned char>(red);
	result.green = static_cast<unsigned char>(green);
	result.blue = static_cast<unsigned char>(blue);

	return result;
}

template <typename OutputIterator>
inline void generate_colours(const std::size_t& steps, const rgb_t c0, const rgb_t& c1, OutputIterator out)
{
	double dr = ((double)c1.red - (double)c0.red) / steps;
	double dg = ((double)c1.green - (double)c0.green) / steps;
	double db = ((double)c1.blue - (double)c0.blue) / steps;

	for (std::size_t i = 0; i < steps; ++i)
	{
		rgb_t c;

		c.red = static_cast<unsigned char>(c0.red + (i * dr));
		c.green = static_cast<unsigned char>(c0.green + (i * dg));
		c.blue = static_cast<unsigned char>(c0.blue + (i * db));

		*(out++) = c;
	}
}

template <typename ResponseImage, typename Palette>
inline std::size_t convert_rsp_to_image(const ResponseImage& resp_image, const Palette& palette, bitmap_image& image)
{
	if (
		(resp_image.width() > image.width()) ||
		(resp_image.height() > image.height())
		)
		return 0;

	for (std::size_t y = 0; y < resp_image.height(); ++y)
	{
		for (std::size_t x = 0; x < resp_image.width(); ++x)
		{
			const double v = resp_image(x, y);

			unsigned int index = static_cast<unsigned int>((v < 0) ? 0 : v > (palette.size()) ? (palette.size() - 1) : v);

			image.set_pixel(x, y, palette[index]);
		}
	}

	return (resp_image.width() * resp_image.height());
}

inline void rgb_to_ycbcr(const unsigned int& length, double* red, double* green, double* blue,
	double* y, double* cb, double* cr)
{
	unsigned int i = 0;

	while (i < length)
	{
		(*y) = 16.0 + (65.481 * (*red) + 128.553 * (*green) + 24.966 * (*blue));
		(*cb) = 128.0 + (-37.797 * (*red) + -74.203 * (*green) + 112.000 * (*blue));
		(*cr) = 128.0 + (112.000 * (*red) + -93.786 * (*green) - 18.214 * (*blue));

		++i;
		++red; ++green; ++blue;
		++y;   ++cb;    ++cr;
	}
}

inline void ycbcr_to_rgb(const unsigned int& length, double* y, double* cb, double* cr,
	double* red, double* green, double* blue)
{
	unsigned int i = 0;

	while (i < length)
	{
		double y_ = (*y) - 16.0;
		double cb_ = (*cb) - 128.0;
		double cr_ = (*cr) - 128.0;

		(*red) = 0.000456621 * y_ + 0.00625893 * cr_;
		(*green) = 0.000456621 * y_ - 0.00153632 * cb_ - 0.00318811 * cr_;
		(*blue) = 0.000456621 * y_ + 0.00791071 * cb_;

		++i;
		++red; ++green; ++blue;
		++y;   ++cb;    ++cr;
	}
}

inline void subsample(const unsigned int& width,
	const unsigned int& height,
	const double* source,
	unsigned int& w,
	unsigned int& h,
	double*& dest)
{
	/*  Single channel.  */

	w = 0;
	h = 0;

	bool odd_width = false;
	bool odd_height = false;

	if (0 == (width % 2))
		w = width / 2;
	else
	{
		w = 1 + (width / 2);
		odd_width = true;
	}

	if (0 == (height % 2))
		h = height / 2;
	else
	{
		h = 1 + (height / 2);
		odd_height = true;
	}

	unsigned int horizontal_upper = (odd_width) ? w - 1 : w;
	unsigned int vertical_upper = (odd_height) ? h - 1 : h;

	dest = new double[w * h];

	double* s_itr = dest;
	const double* itr1 = source;
	const double* itr2 = source + width;

	for (unsigned int j = 0; j < vertical_upper; ++j)
	{
		for (unsigned int i = 0; i < horizontal_upper; ++i, ++s_itr)
		{
			(*s_itr) = *(itr1++);
			(*s_itr) += *(itr1++);
			(*s_itr) += *(itr2++);
			(*s_itr) += *(itr2++);
			(*s_itr) /= 4.0;
		}

		if (odd_width)
		{
			(*(s_itr++)) = ((*itr1++) + (*itr2++)) / 2.0;
		}

		itr1 += width;

		if (j != (vertical_upper - 1))
		{
			itr2 += width;
		}
	}

	if (odd_height)
	{
		for (unsigned int i = 0; i < horizontal_upper; ++i, ++s_itr)
		{
			(*s_itr) += (*(itr1++));
			(*s_itr) += (*(itr1++));
			(*s_itr) /= 2.0;
		}

		if (odd_width)
		{
			(*(s_itr++)) = (*itr1);
		}
	}
}

inline void upsample(const unsigned int& width,
	const unsigned int& height,
	const double* source,
	unsigned int& w,
	unsigned int& h,
	double*& dest)
{
	/* Single channel. */

	w = 2 * width;
	h = 2 * height;

	dest = new double[w * h];

	const double* s_itr = source;
	double* itr1 = dest;
	double* itr2 = dest + w;

	for (unsigned int j = 0; j < height; ++j)
	{
		for (unsigned int i = 0; i < width; ++i, ++s_itr)
		{
			*(itr1++) = (*s_itr);
			*(itr1++) = (*s_itr);
			*(itr2++) = (*s_itr);
			*(itr2++) = (*s_itr);
		}

		itr1 += w;
		itr2 += w;
	}
}

inline void checkered_pattern(const unsigned int x_width,
	const unsigned int y_width,
	const unsigned char value,
	const bitmap_image::color_plane color,
	bitmap_image& image)
{
	if (
		(x_width >= image.width()) ||
		(y_width >= image.height())
		)
	{
		return;
	}

	bool setter_x = false;
	bool setter_y = true;

	const unsigned int color_plane_offset = image.offset(color);
	const unsigned int height = image.height();
	const unsigned int width = image.width();

	for (unsigned int y = 0; y < height; ++y)
	{
		if (0 == (y % y_width))
		{
			setter_y = !setter_y;
		}

		unsigned char* row = image.row(y) + color_plane_offset;

		for (unsigned int x = 0; x < width; ++x, row += image.bytes_per_pixel())
		{
			if (0 == (x % x_width))
			{
				setter_x = !setter_x;
			}

			if (setter_x ^ setter_y)
			{
				*row = value;
			}
		}
	}
}

inline void checkered_pattern(const unsigned int x_width,
	const unsigned int y_width,
	const unsigned char red,
	const unsigned char green,
	const unsigned char blue,
	bitmap_image& image)
{
	if (
		(x_width >= image.width()) ||
		(y_width >= image.height())
		)
	{
		return;
	}

	bool setter_x = false;
	bool setter_y = true;

	const unsigned int height = image.height();
	const unsigned int width = image.width();

	for (unsigned int y = 0; y < height; ++y)
	{
		if (0 == (y % y_width))
		{
			setter_y = !setter_y;
		}

		unsigned char* row = image.row(y);

		for (unsigned int x = 0; x < width; ++x, row += image.bytes_per_pixel())
		{
			if (0 == (x % x_width))
			{
				setter_x = !setter_x;
			}

			if (setter_x ^ setter_y)
			{
				*(row + 0) = blue;
				*(row + 1) = green;
				*(row + 2) = red;
			}
		}
	}
}

inline void plasma(bitmap_image& image,
	const double& x, const double& y,
	const double& width, const double& height,
	const double& c1, const double& c2,
	const double& c3, const double& c4,
	const double& roughness = 3.0,
	const rgb_t   colormap[] = 0)
{
	// Note: c1,c2,c3,c4 -> [0.0,1.0]

	const double half_width = (width / 2.0);
	const double half_height = (height / 2.0);

	if ((width >= 1.0) || (height >= 1.0))
	{
		const double corner1 = (c1 + c2) / 2.0;
		const double corner2 = (c2 + c3) / 2.0;
		const double corner3 = (c3 + c4) / 2.0;
		const double corner4 = (c4 + c1) / 2.0;
		double center = (c1 + c2 + c3 + c4) / 4.0 +
			((1.0 * ::rand() / (1.0 * RAND_MAX)) - 0.5) * // should use a better rng
			((1.0 * half_width + half_height) / (image.width() + image.height()) * roughness);

		center = std::min<double>(std::max<double>(0.0, center), 1.0);

		plasma(image, x, y, half_width, half_height, c1, corner1, center, corner4, roughness, colormap);
		plasma(image, x + half_width, y, half_width, half_height, corner1, c2, corner2, center, roughness, colormap);
		plasma(image, x + half_width, y + half_height, half_width, half_height, center, corner2, c3, corner3, roughness, colormap);
		plasma(image, x, y + half_height, half_width, half_height, corner4, center, corner3, c4, roughness, colormap);
	}
	else
	{
		rgb_t color = colormap[static_cast<unsigned int>(1000.0 * ((c1 + c2 + c3 + c4) / 4.0)) % 1000];

		image.set_pixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y), color);
	}
}

inline void plasma(bitmap_image& image,
	const double& c1, const double& c2,
	const double& c3, const double& c4,
	const double& roughness = 3.0,
	const rgb_t   colormap[] = 0)
{
	plasma
	(
		image, 0, 0, image.width(), image.height(),
		c1, c2, c3, c4,
		roughness, colormap
	);
}

inline double psnr_region(const unsigned int& x, const unsigned int& y,
	const unsigned int& width, const unsigned int& height,
	const bitmap_image& image1, const bitmap_image& image2)
{
	if (
		(image1.width() != image2.width()) ||
		(image1.height() != image2.height())
		)
	{
		return 0.0;
	}

	if ((x + width) > image1.width()) { return 0.0; }
	if ((y + height) > image1.height()) { return 0.0; }

	double mse = 0.0;

	for (unsigned int r = 0; r < height; ++r)
	{
		const unsigned char* itr1 = image1.row(r + y) + x * image1.bytes_per_pixel();
		const unsigned char* itr1_end = itr1 + (width * image1.bytes_per_pixel());
		const unsigned char* itr2 = image2.row(r + y) + x * image2.bytes_per_pixel();

		while (itr1 != itr1_end)
		{
			double v = (static_cast<double>(*itr1) - static_cast<double>(*itr2));
			mse += v * v;
			++itr1;
			++itr2;
		}
	}

	if (mse <= 0.0000001)
	{
		return 1000000.0;
	}
	else
	{
		mse /= (3.0 * width * height);
		return 20.0 * std::log10(255.0 / std::sqrt(mse));
	}
}

inline void hierarchical_psnr_r(const double& x, const double& y,
	const double& width, const double& height,
	const bitmap_image& image1,
	bitmap_image& image2,
	const double& threshold,
	const rgb_t colormap[])
{
	if ((width <= 4.0) || (height <= 4.0))
	{
		const double psnr = psnr_region
		(
			static_cast<unsigned int>(x),
			static_cast<unsigned int>(y),
			static_cast<unsigned int>(width),
			static_cast<unsigned int>(height),
			image1, image2
		);

		if (psnr < threshold)
		{
			rgb_t c = colormap[static_cast<unsigned int>(1000.0 * (1.0 - (psnr / threshold)))];

			image2.set_region
			(
				static_cast<unsigned int>(x),
				static_cast<unsigned int>(y),
				static_cast<unsigned int>(width + 1),
				static_cast<unsigned int>(height + 1),
				c.red, c.green, c.blue
			);
		}
	}
	else
	{
		const double half_width = (width / 2.0);
		const double half_height = (height / 2.0);

		hierarchical_psnr_r(x, y, half_width, half_height, image1, image2, threshold, colormap);
		hierarchical_psnr_r(x + half_width, y, half_width, half_height, image1, image2, threshold, colormap);
		hierarchical_psnr_r(x + half_width, y + half_height, half_width, half_height, image1, image2, threshold, colormap);
		hierarchical_psnr_r(x, y + half_height, half_width, half_height, image1, image2, threshold, colormap);
	}
}

inline void hierarchical_psnr(bitmap_image& image1, bitmap_image& image2, const double threshold, const rgb_t colormap[])
{
	if (
		(image1.width() != image2.width()) ||
		(image1.height() != image2.height())
		)
	{
		return;
	}

	const double psnr = psnr_region
	(
		0, 0, image1.width(), image1.height(),
		image1, image2
	);

	if (psnr < threshold)
	{
		hierarchical_psnr_r
		(
			0, 0, image1.width(), image1.height(),
			image1, image2,
			threshold,
			colormap
		);
	}
}

class image_drawer
{
public:

	image_drawer(bitmap_image& image)
		: image_(image),
		pen_width_(1),
		pen_color_red_(0),
		pen_color_green_(0),
		pen_color_blue_(0)
	{}

	void rectangle(int x1, int y1, int x2, int y2)
	{
		line_segment(x1, y1, x2, y1);
		line_segment(x2, y1, x2, y2);
		line_segment(x2, y2, x1, y2);
		line_segment(x1, y2, x1, y1);
	}

	void triangle(int x1, int y1, int x2, int y2, int x3, int y3)
	{
		line_segment(x1, y1, x2, y2);
		line_segment(x2, y2, x3, y3);
		line_segment(x3, y3, x1, y1);
	}

	void quadix(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{
		line_segment(x1, y1, x2, y2);
		line_segment(x2, y2, x3, y3);
		line_segment(x3, y3, x4, y4);
		line_segment(x4, y4, x1, y1);
	}

	void line_segment(int x1, int y1, int x2, int y2)
	{
		int steep = 0;
		int sx = ((x2 - x1) > 0) ? 1 : -1;
		int sy = ((y2 - y1) > 0) ? 1 : -1;
		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);

		if (dy > dx)
		{
			std::swap(x1, y1);
			std::swap(dx, dy);
			std::swap(sx, sy);

			steep = 1;
		}

		int e = 2 * dy - dx;

		for (int i = 0; i < dx; ++i)
		{
			if (steep)
				plot_pen_pixel(y1, x1);
			else
				plot_pen_pixel(x1, y1);

			while (e >= 0)
			{
				y1 += sy;
				e -= (dx << 1);
			}

			x1 += sx;
			e += (dy << 1);
		}

		plot_pen_pixel(x2, y2);
	}

	void horiztonal_line_segment(int x1, int x2, int y)
	{
		if (x1 > x2)
		{
			std::swap(x1, x2);
		}

		for (int i = 0; i < (x2 - x1); ++i)
		{
			plot_pen_pixel(x1 + i, y);
		}
	}

	void vertical_line_segment(int y1, int y2, int x)
	{
		if (y1 > y2)
		{
			std::swap(y1, y2);
		}

		for (int i = 0; i < (y2 - y1); ++i)
		{
			plot_pen_pixel(x, y1 + i);
		}
	}

	void ellipse(int centerx, int centery, int a, int b)
	{
		int t1 = a * a;
		int t2 = t1 << 1;
		int t3 = t2 << 1;
		int t4 = b * b;
		int t5 = t4 << 1;
		int t6 = t5 << 1;
		int t7 = a * t5;
		int t8 = t7 << 1;
		int t9 = 0;

		int d1 = t2 - t7 + (t4 >> 1);
		int d2 = (t1 >> 1) - t8 + t5;
		int x = a;
		int y = 0;

		int negative_tx = centerx - x;
		int positive_tx = centerx + x;
		int negative_ty = centery - y;
		int positive_ty = centery + y;

		while (d2 < 0)
		{
			plot_pen_pixel(positive_tx, positive_ty);
			plot_pen_pixel(positive_tx, negative_ty);
			plot_pen_pixel(negative_tx, positive_ty);
			plot_pen_pixel(negative_tx, negative_ty);

			++y;

			t9 = t9 + t3;

			if (d1 < 0)
			{
				d1 = d1 + t9 + t2;
				d2 = d2 + t9;
			}
			else
			{
				x--;
				t8 = t8 - t6;
				d1 = d1 + (t9 + t2 - t8);
				d2 = d2 + (t9 + t5 - t8);
				negative_tx = centerx - x;
				positive_tx = centerx + x;
			}

			negative_ty = centery - y;
			positive_ty = centery + y;
		}

		do
		{
			plot_pen_pixel(positive_tx, positive_ty);
			plot_pen_pixel(positive_tx, negative_ty);
			plot_pen_pixel(negative_tx, positive_ty);
			plot_pen_pixel(negative_tx, negative_ty);

			x--;
			t8 = t8 - t6;

			if (d2 < 0)
			{
				++y;
				t9 = t9 + t3;
				d2 = d2 + (t9 + t5 - t8);
				negative_ty = centery - y;
				positive_ty = centery + y;
			}
			else
				d2 = d2 + (t5 - t8);

			negative_tx = centerx - x;
			positive_tx = centerx + x;
		} while (x >= 0);
	}

	void circle(int centerx, int centery, int radius)
	{
		int x = 0;
		int d = (1 - radius) << 1;

		while (radius >= 0)
		{
			plot_pen_pixel(centerx + x, centery + radius);
			plot_pen_pixel(centerx + x, centery - radius);
			plot_pen_pixel(centerx - x, centery + radius);
			plot_pen_pixel(centerx - x, centery - radius);

			if ((d + radius) > 0)
				d -= ((--radius) << 1) - 1;
			if (x > d)
				d += ((++x) << 1) + 1;
		}
	}

	void plot_pen_pixel(int x, int y)
	{
		switch (pen_width_)
		{
		case 1: plot_pixel(x, y);
			break;

		case 2: {
			plot_pixel(x, y);
			plot_pixel(x + 1, y);
			plot_pixel(x + 1, y + 1);
			plot_pixel(x, y + 1);
		}
				break;

		case  3: {
			plot_pixel(x, y - 1);
			plot_pixel(x - 1, y - 1);
			plot_pixel(x + 1, y - 1);

			plot_pixel(x, y);
			plot_pixel(x - 1, y);
			plot_pixel(x + 1, y);

			plot_pixel(x, y + 1);
			plot_pixel(x - 1, y + 1);
			plot_pixel(x + 1, y + 1);
		}
				 break;

		default: plot_pixel(x, y);
			break;
		}
	}

	void plot_pixel(int x, int y)
	{
		if (
			(x < 0) ||
			(y < 0) ||
			(x >= static_cast<int>(image_.width())) ||
			(y >= static_cast<int>(image_.height()))
			)
			return;

		image_.set_pixel(x, y, pen_color_red_, pen_color_green_, pen_color_blue_);
	}

	void pen_width(const unsigned int& width)
	{
		if ((width > 0) && (width < 4))
		{
			pen_width_ = width;
		}
	}

	void pen_color(const unsigned char& red,
		const unsigned char& green,
		const unsigned char& blue)
	{
		pen_color_red_ = red;
		pen_color_green_ = green;
		pen_color_blue_ = blue;
	}

	template <typename RGB>
	void pen_color(const RGB colour)
	{
		pen_color_red_ = colour.red;
		pen_color_green_ = colour.green;
		pen_color_blue_ = colour.blue;
	}

private:

	image_drawer(const image_drawer& id);
	image_drawer& operator =(const image_drawer& id);

	bitmap_image& image_;
	unsigned int  pen_width_;
	unsigned char pen_color_red_;
	unsigned char pen_color_green_;
	unsigned char pen_color_blue_;
};

class cartesian_canvas
{
public:

	cartesian_canvas(const double x_length, const double y_length)
		: width_div2_(0.0),
		height_div2_(0.0),
		min_x_(0.0),
		min_y_(0.0),
		max_x_(0.0),
		max_y_(0.0),
		draw_(image_)
	{
		setup_canvas(x_length, y_length);
	}

	inline bool operator!()
	{
		return !image_;
	}

	void rectangle(double x1, double y1, double x2, double y2)
	{
		line_segment(x1, y1, x2, y1);
		line_segment(x2, y1, x2, y2);
		line_segment(x2, y2, x1, y2);
		line_segment(x1, y2, x1, y1);
	}

	void triangle(double x1, double y1, double x2, double y2, double x3, double y3)
	{
		line_segment(x1, y1, x2, y2);
		line_segment(x2, y2, x3, y3);
		line_segment(x3, y3, x1, y1);
	}

	void quadix(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
	{
		line_segment(x1, y1, x2, y2);
		line_segment(x2, y2, x3, y3);
		line_segment(x3, y3, x4, y4);
		line_segment(x4, y4, x1, y1);
	}

	void line_segment(double x1, double y1, double x2, double y2)
	{
		if (clip(x1, y1, x2, y2))
		{
			const int sc_x1 = static_cast<int>(cart_to_screen_x(x1));
			const int sc_x2 = static_cast<int>(cart_to_screen_x(x2));
			const int sc_y1 = static_cast<int>(cart_to_screen_y(y1));
			const int sc_y2 = static_cast<int>(cart_to_screen_y(y2));

			draw_.line_segment(sc_x1, sc_y1, sc_x2, sc_y2);
		}
	}

	void horiztonal_line_segment(double x1, double x2, double y)
	{
		x1 = clamp_x(x1);
		x2 = clamp_x(x2);
		y = clamp_y(y);

		const int sc_x1 = static_cast<int>(cart_to_screen_x(x1));
		const int sc_x2 = static_cast<int>(cart_to_screen_x(x2));
		const int sc_y = static_cast<int>(cart_to_screen_y(y));

		draw_.horiztonal_line_segment(sc_x1, sc_x2, sc_y);
	}

	void vertical_line_segment(double y1, double y2, double x)
	{
		y1 = clamp_y(y1);
		y2 = clamp_y(y2);
		x = clamp_x(x);

		const int sc_y1 = static_cast<int>(cart_to_screen_y(y1));
		const int sc_y2 = static_cast<int>(cart_to_screen_y(y2));
		const int sc_x = static_cast<int>(cart_to_screen_x(x));

		draw_.vertical_line_segment(sc_y1, sc_y2, sc_x);
	}

	void ellipse(double centerx, double centery, double a, double b)
	{

		const int sc_cx = static_cast<int>(cart_to_screen_x(centerx));
		const int sc_cy = static_cast<int>(cart_to_screen_y(centery));

		draw_.ellipse(sc_cx, sc_cy, static_cast<int>(a), static_cast<int>(b));
	}

	void circle(double centerx, double centery, double radius)
	{
		const int sc_cx = static_cast<int>(cart_to_screen_x(centerx));
		const int sc_cy = static_cast<int>(cart_to_screen_y(centery));

		draw_.circle(sc_cx, sc_cy, static_cast<int>(radius));
	}

	void fill_rectangle(double x1, double y1, double x2, double y2)
	{
		if (y1 > y2)
			std::swap(y1, y2);

		for (double y = y1; y <= y2; y += 0.5)
		{
			line_segment(x1, y, x2, y);
		}
	}

	void fill_triangle(double x1, double y1, double x2, double y2, double x3, double y3)
	{
		typedef std::pair<double, double> point_t;

		std::vector<point_t> p;

		p.push_back(std::make_pair(x1, y1));
		p.push_back(std::make_pair(x2, y2));
		p.push_back(std::make_pair(x3, y3));

		if (p[0].second > p[1].second)
			std::swap(p[0], p[1]);
		if (p[0].second > p[2].second)
			std::swap(p[0], p[2]);
		if (p[1].second > p[2].second)
			std::swap(p[1], p[2]);

		class draw_modes
		{
		private:

			cartesian_canvas & canvas;

			// Needed for incompetent and broken msvc compiler versions
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4822)
#endif
			draw_modes& operator=(const draw_modes&);
#ifdef _MSC_VER
#pragma warning(pop)
#endif

		public:

			draw_modes(cartesian_canvas& c)
				: canvas(c)
			{}

			void bottom(const point_t& p0, const point_t& p1, const point_t& p2)
			{
				const double m0 = (p1.first - p0.first) / (2.0 * (p1.second - p0.second));
				const double m1 = (p2.first - p0.first) / (2.0 * (p2.second - p0.second));

				double x0 = p0.first;
				double x1 = p0.first;

				for (double y = p0.second; y <= p1.second; y += 0.5)
				{
					canvas.horiztonal_line_segment(x0, x1, y);

					x0 += m0;
					x1 += m1;
				}
			}

			void top(const point_t& p0, const point_t& p1, const point_t& p2)
			{
				const double m0 = (p2.first - p0.first) / (2.0 * (p2.second - p0.second));
				const double m1 = (p2.first - p1.first) / (2.0 * (p2.second - p1.second));

				double x0 = p2.first;
				double x1 = p2.first;

				for (double y = p2.second; y >= p0.second; y -= 0.5)
				{
					canvas.horiztonal_line_segment(x0, x1, y);

					x0 -= m0;
					x1 -= m1;
				}
			}
		};

		draw_modes dm(*this);

		const double eps = 0.00001;

		if (std::abs(p[1].second - p[2].second) < eps)
			dm.bottom(p[0], p[1], p[2]);
		else if (std::abs(p[0].second - p[1].second) < eps)
			dm.top(p[0], p[1], p[2]);
		else
		{
			point_t p3;

			p3.first = (p[0].first + ((p[1].second - p[0].second) / (p[2].second - p[0].second)) * (p[2].first - p[0].first));
			p3.second = p[1].second;

			dm.bottom(p[0], p[1], p3);
			dm.top(p[1], p3, p[2]);
		}
	}

	void fill_quadix(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
	{
		fill_triangle(x1, y1, x2, y2, x3, y3);
		fill_triangle(x1, y1, x3, y3, x4, y4);
	}

	void fill_circle(double cx, double cy, double radius)
	{
		const double delta = 1.0;
		double  x = radius;
		double  y = 0.0;
		double dx = delta - (2.0 * delta * radius);
		double dy = 0.0;
		double dr = 0.0;

		while (x >= y)
		{
			for (double i = cx - x; i <= cx + x; i += delta)
			{
				horiztonal_line_segment(cx - x, cx + x, cy + y);
				horiztonal_line_segment(cx - x, cx + x, cy - y);
			}

			for (double i = cx - y; i <= cx + y; i += delta)
			{
				horiztonal_line_segment(cx - y, cx + y, cy + x);
				horiztonal_line_segment(cx - y, cx + y, cy - x);
			}

			y += delta;

			dr += dy;
			dy += 2.0 * delta;

			if ((2.0 * delta * dr + dx) > 0)
			{
				x -= delta;
				dr += dx;
				dx += 2.0 * delta;
			}
		}
	}

	void plot_pen_pixel(double x, double y)
	{
		if ((x < min_x_) || (x > max_x_)) return;
		if ((y < min_y_) || (y > max_y_)) return;

		const int sc_x = static_cast<int>(cart_to_screen_x(x));
		const int sc_y = static_cast<int>(cart_to_screen_y(y));

		draw_.plot_pen_pixel(sc_x, sc_y);
	}

	void plot_pixel(double x, double y)
	{
		if ((x < min_x_) || (x > max_x_)) return;
		if ((y < min_y_) || (y > max_y_)) return;

		const int sc_x = static_cast<int>(cart_to_screen_x(x));
		const int sc_y = static_cast<int>(cart_to_screen_y(y));

		draw_.plot_pixel(sc_x, sc_y);
	}

	void pen_width(const unsigned int& width)
	{
		draw_.pen_width(width);
	}

	void pen_color(const unsigned char&   red,
		const unsigned char& green,
		const unsigned char&  blue)
	{
		draw_.pen_color(red, green, blue);
	}

	template <typename RGB>
	void pen_color(const RGB colour)
	{
		draw_.pen_color(colour);
	}

	const bitmap_image& image() const
	{
		return image_;
	}

	bitmap_image& image()
	{
		return image_;
	}

	void set_widthheight(const double x_length, const double y_length)
	{
		setup_canvas(x_length, y_length);
	}

	double min_x() const { return min_x_; }
	double min_y() const { return min_y_; }
	double max_x() const { return max_x_; }
	double max_y() const { return max_y_; }

private:

	void setup_canvas(const double x_length, const double y_length)
	{
		if ((x_length < 2.0) || (y_length < 2.0))
			return;

		width_div2_ = x_length / 2.0;
		height_div2_ = y_length / 2.0;

		min_x_ = -width_div2_;
		min_y_ = -height_div2_;
		max_x_ = width_div2_;
		max_y_ = height_div2_;

		image_.setwidth_height(static_cast<unsigned int>(x_length) + 1, static_cast<unsigned int>(y_length) + 1);

		image_.clear(0xFF);
	}

	double clamp_x(const double& x)
	{
		if (x < min_x_)  return min_x_;
		else if (x > max_x_)  return max_x_;
		else                  return x;
	}

	double clamp_y(const double& y)
	{
		if (y < min_y_)  return min_y_;
		else if (y > max_y_)  return max_y_;
		else                  return y;
	}

	double cart_to_screen_x(const double& x)
	{
		return x + width_div2_;
	}

	double cart_to_screen_y(const double& y)
	{
		return height_div2_ - y;
	}

	enum clip_code
	{
		e_clip_bottom = 1,
		e_clip_top = 2,
		e_clip_left = 4,
		e_clip_right = 8
	};

	int out_code(
		const double&  x, const double&  y,
		const double& x1, const double& y1,
		const double& x2, const double& y2
	)
	{
		int result = 0;
		if (y < y1)      result |= e_clip_bottom;
		else if (y > y2) result |= e_clip_top;

		if (x < x1)      result |= e_clip_left;
		else if (x > x2) result |= e_clip_right;

		return result;
	}

	bool clip(double& x1, double& y1, double& x2, double& y2)
	{
		bool   result = false;
		double x = 0.0;
		double y = 0.0;

		int outcode0 = out_code(x1, y1, min_x_, min_y_, max_x_, max_y_);
		int outcode1 = out_code(x2, y2, min_x_, min_y_, max_x_, max_y_);
		int outcodeout = 0;

		while ((outcode0 != 0) || (outcode1 != 0))
		{
			if ((outcode0 & outcode1) != 0)
				return result;
			else
			{
				if (outcode0 != 0)
					outcodeout = outcode0;
				else
					outcodeout = outcode1;

				double dx = (x2 - x1);
				double dy = (y2 - y1);

				if ((outcodeout & e_clip_bottom) == e_clip_bottom)
				{
					x = x1 + dx * (min_y_ - y1) / dy;
					y = min_y_;
				}
				else if ((outcodeout & e_clip_top) == e_clip_top)
				{
					x = x1 + dx * (max_y_ - y1) / dy;
					y = max_y_;
				}
				else if ((outcodeout & e_clip_right) == e_clip_right)
				{
					y = y1 + dy * (max_x_ - x1) / dx;
					x = max_x_;
				}
				else if ((outcodeout & e_clip_left) == e_clip_left)
				{
					y = y1 + dy * (min_x_ - x1) / dx;
					x = min_x_;
				}

				if (outcodeout == outcode0)
				{
					x1 = x;
					y1 = y;
					outcode0 = out_code(x1, y1, min_x_, min_y_, max_x_, max_y_);
				}
				else
				{
					x2 = x;
					y2 = y;
					outcode1 = out_code(x2, y2, min_x_, min_y_, max_x_, max_y_);
				}
			}
		}

		return true;
	}

	cartesian_canvas(const cartesian_canvas&);
	cartesian_canvas operator=(const cartesian_canvas&);

	double width_div2_;
	double height_div2_;
	double min_x_;
	double min_y_;
	double max_x_;
	double max_y_;
	bitmap_image image_;
	image_drawer draw_;
};

inline rgb_t convert_wave_length_nm_to_rgb(const double wave_length_nm)
{
	// Credits: Dan Bruton http://www.physics.sfasu.edu/astro/color.html
	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;

	if ((380.0 <= wave_length_nm) && (wave_length_nm <= 439.0))
	{
		red = -(wave_length_nm - 440.0) / (440.0 - 380.0);
		green = 0.0;
		blue = 1.0;
	}
	else if ((440.0 <= wave_length_nm) && (wave_length_nm <= 489.0))
	{
		red = 0.0;
		green = (wave_length_nm - 440.0) / (490.0 - 440.0);
		blue = 1.0;
	}
	else if ((490.0 <= wave_length_nm) && (wave_length_nm <= 509.0))
	{
		red = 0.0;
		green = 1.0;
		blue = -(wave_length_nm - 510.0) / (510.0 - 490.0);
	}
	else if ((510.0 <= wave_length_nm) && (wave_length_nm <= 579.0))
	{
		red = (wave_length_nm - 510.0) / (580.0 - 510.0);
		green = 1.0;
		blue = 0.0;
	}
	else if ((580.0 <= wave_length_nm) && (wave_length_nm <= 644.0))
	{
		red = 1.0;
		green = -(wave_length_nm - 645.0) / (645.0 - 580.0);
		blue = 0.0;
	}
	else if ((645.0 <= wave_length_nm) && (wave_length_nm <= 780.0))
	{
		red = 1.0;
		green = 0.0;
		blue = 0.0;
	}

	double factor = 0.0;

	if ((380.0 <= wave_length_nm) && (wave_length_nm <= 419.0))
		factor = 0.3 + 0.7 * (wave_length_nm - 380.0) / (420.0 - 380.0);
	else if ((420.0 <= wave_length_nm) && (wave_length_nm <= 700.0))
		factor = 1.0;
	else if ((701.0 <= wave_length_nm) && (wave_length_nm <= 780.0))
		factor = 0.3 + 0.7 * (780.0 - wave_length_nm) / (780.0 - 700.0);
	else
		factor = 0.0;

	rgb_t result;

	const double gamma = 0.8;
	const double intensity_max = 255.0;

#define round(d) std::floor(d + 0.5)

	result.red = static_cast<unsigned char>((red == 0.0) ? red : round(intensity_max * std::pow(red   * factor, gamma)));
	result.green = static_cast<unsigned char>((green == 0.0) ? green : round(intensity_max * std::pow(green * factor, gamma)));
	result.blue = static_cast<unsigned char>((blue == 0.0) ? blue : round(intensity_max * std::pow(blue  * factor, gamma)));

#undef round

	return result;
}

inline double weighted_distance(const unsigned char r0, const unsigned char g0, const unsigned char b0,
	const unsigned char r1, const unsigned char g1, const unsigned char b1)
{
	const double diff_r = /*0.30 */ (r0 - r1);
	const double diff_g = /*0.59 */ (g0 - g1);
	const double diff_b = /*0.11 */ (b0 - b1);

	return std::sqrt((diff_r * diff_r) + (diff_g * diff_g) + (diff_b * diff_b));
}

inline double weighted_distance(const rgb_t c0, const rgb_t c1)
{
	return weighted_distance(c0.red, c0.green, c0.blue,
		c1.red, c1.green, c1.blue);
}

template <typename Iterator>
inline rgb_t find_nearest_color(const rgb_t& c, const Iterator begin, const Iterator end)
{
	if (0 == std::distance(begin, end))
		return c;

	double min_d = std::numeric_limits<double>::max();
	rgb_t result = *begin;

	for (Iterator itr = begin; itr != end; ++itr)
	{
		if (c == (*itr))
		{
			return (*itr);
		}

		double curr_d = weighted_distance(c, *itr);

		if (curr_d < min_d)
		{
			min_d = curr_d;
			result = *itr;
		}
	}

	return result;
}

template <template <typename, typename> class Sequence,
	typename Allocator>
	inline rgb_t find_nearest_color(const rgb_t& c, const Sequence<rgb_t, Allocator>& seq)
{
	return find_nearest_color(c, seq.begin(), seq.end());
}

template <std::size_t N>
inline rgb_t find_nearest_color(const rgb_t& c, const rgb_t(&colors)[N])
{
	return find_nearest_color(c, colors, colors + N);
}

inline double find_nearest_wave_length(const rgb_t& c, const double increment = 0.001)
{
	const double max_wave_length = 800.0; //800nm

	double min_wave_length = 0.0;
	double min_d = std::numeric_limits<double>::max();

	for (double i = 0.0; i < max_wave_length; i += increment)
	{
		const rgb_t  curr_rgb = convert_wave_length_nm_to_rgb(i);

		if (c == curr_rgb)
		{
			return i;
		}

		const double curr_d = weighted_distance(c, curr_rgb);

		if (curr_d <= min_d)
		{
			min_wave_length = i;
			min_d = curr_d;
		}
	}

	return min_wave_length;
}

template <typename T>
class response_image
{
public:

	response_image(const std::size_t& width, const std::size_t& height, const T null = T(0))
		: width_(width),
		height_(height),
		null_(null)
	{
		data_.resize(width_ * height_);
	}

	std::size_t width() const { return  width_; }
	std::size_t height() const { return height_; }

	void set_all(const T& t)
	{
		std::fill_n(data_.begin(), data_.size(), t);
	}

	const T& operator()(const std::size_t& x, const std::size_t& y) const
	{
		if (y >= height_) return null_;
		if (x >= width_) return null_;

		return data_[width_ * y + x];
	}

	T& operator()(const std::size_t& x, const std::size_t& y)
	{
		if (y >= height_) return null_;
		if (x >= width_) return null_;

		return data_[width_ * y + x];
	}

	bool valid(const std::size_t& x, const std::size_t& y)
	{
		return ((x < width_) || (y < height_));
	}

	void inc_all(const T& v)
	{
		for (std::size_t i = 0; i < data_.size(); ++i)
		{
			data_[i] += v;
		}
	}

	void mul_all(const T& v)
	{
		for (std::size_t i = 0; i < data_.size(); ++i)
		{
			data_[i] *= v;
		}
	}

	T* row(const std::size_t& row_index)
	{
		if (row_index < height_)
			return &data_[width_ * row_index];
		else
			return reinterpret_cast<T*>(0);
	}

	const T* row(const std::size_t& row_index) const
	{
		if (row_index < height_)
			return data_[width_ * row_index];
		else
			return reinterpret_cast<T*>(0);
	}

private:

	std::size_t    width_;
	std::size_t    height_;
	std::vector<T> data_;
	T              null_;
};

inline void sobel_operator(const bitmap_image& src_image,
	bitmap_image& dst_image,
	const double threshold = 0.0)
{
	typedef double T;

	response_image<T> im0(src_image.width(), src_image.height(), 0.0);
	response_image<T> im1(src_image.width(), src_image.height(), 0.0);

	src_image.export_gray_scale_response_image(&im0(0, 0));

	for (std::size_t y = 1; y < im0.height() - 1; ++y)
	{
		const T* itr0 = im0.row(y - 1);
		const T* itr1 = im0.row(y);
		const T* itr2 = im0.row(y + 1);
		T* out = im1.row(y) + 1;

		for (std::size_t x = 1; x < im0.width() - 1; ++x)
		{
			const T c0 = *(itr0 + x - 1);   const T c1 = *(itr0 + x);   const T c2 = *(itr0 + x + 1);
			const T c3 = *(itr1 + x - 1); /*const T c4 = *(itr1 + x);*/ const T c5 = *(itr1 + x + 1);
			const T c6 = *(itr2 + x - 1);   const T c7 = *(itr2 + x);   const T c8 = *(itr2 + x + 1);

			const T gx = (2.0 * (c5 - c3)) + (c2 - c0) + (c8 - c6);
			const T gy = (2.0 * (c1 - c7)) + (c0 - c6) + (c2 - c8);

			*(out++) = std::sqrt((gx * gx) + (gy * gy));
		}
	}

	if (threshold > 0.0)
	{
		const T* end = im1.row(0) + (im1.width() * im1.height());

		for (T* itr = im1.row(0); itr != end; ++itr)
		{
			T& v = *itr;
			if (v <= threshold) v = 0;
		}
	}

	dst_image.setwidth_height
	(
		static_cast<unsigned int>(im1.width()),
		static_cast<unsigned int>(im1.height())
	);

	dst_image.import_gray_scale_clamped(&im1(0, 0));
}

enum palette_name
{
	e_red, e_scarlet, e_vermilion, e_tangelo, e_orange,
	e_gamboge, e_amber, e_gold, e_yellow, e_apple_green,
	e_lime_green, e_spring_bud, e_chartreuse_green, e_pistachio, e_harlequin,
	e_sap_green, e_green, e_emerald_green, e_malachite_green, e_sea_green,
	e_spring_green, e_aquamarine, e_turquoise, e_opal, e_cyan,
	e_arctic_blue, e_cerulean, e_cornflower_blue, e_azure, e_cobalt_blue,
	e_sapphire_blue, e_phthalo_blue, e_blue, e_persian_blue, e_indigo,
	e_blue_violet, e_violet, e_purple, e_mulberry, e_heliotrope,
	e_magenta, e_orchid, e_fuchsia, e_cerise, e_rose,
	e_raspberry, e_crimson, e_amaranth, e_white, e_black
};

const rgb_t palette_colormap[] = 
{
   {255,   0,   0}, {255,  31,   0}, {255,  63,   0}, {255,  95,   0}, {255, 127,   0},
   {255, 159,   0}, {255, 191,   0}, {255, 223,   0}, {255, 255,   0}, {223, 255,   0},
   {191, 255,   0}, {159, 255,   0}, {127, 255,   0}, { 95, 255,   0}, { 63, 255,   0},
   { 31, 255,   0}, {  0, 255,   0}, {  0, 255,  31}, {  0, 255,  63}, {  0, 255,  95},
   {  0, 255, 127}, {  0, 255, 159}, {  0, 255, 191}, {  0, 255, 223}, {  0, 255, 255},
   {  0, 223, 255}, {  0, 191, 255}, {  0, 159, 255}, {  0, 127, 255}, {  0,  95, 255},
   {  0,  63, 255}, {  0,  31, 255}, {  0,   0, 255}, { 31,   0, 255}, { 63,   0, 255},
   { 95,   0, 255}, {127,   0, 255}, {159,   0, 255}, {191,   0, 255}, {223,   0, 255},
   {255,   0, 255}, {255,   0, 223}, {255,   0, 191}, {255,   0, 159}, {255,   0, 127},
   {255,   0,  95}, {255,   0,  63}, {255,   0,  31}, {255, 255, 255}, {  0,   0,   0}
};

#endif
