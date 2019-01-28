#ifndef RAW_IMAGE_HPP
#define RAW_IMAGE_HPP

#include <cstdint>
#include <stdexcept>

#include "Color.hpp"

namespace bk
{
	template <typename TColor>
	class RawImage
	{
	public:
		RawImage();
		RawImage(const int& width, const int& height);
		~RawImage();

		void copy_pixels_from(const RawImage& source);
		void copy_pixels_from(const RawImage& source, const size_t index, const size_t size);

		void set_pixel(const uint32_t x, const uint32_t y, const TColor& color);
		void set_pixel(const uint32_t index, const TColor& color);

		const TColor get_pixel(const uint32_t x, const uint32_t y) const;
		const TColor get_pixel(const uint32_t index) const;
		const size_t get_size() const;
		const int get_height() const;
		const int get_width() const;

		bool save_to_file(const char * path);
		bool load_from_file(const char * path, const int& width, const int& height);

	private:
		bool validate_index(const uint32_t index) const;

	private:
		const char* read_mode_ = "rb";
		const char* write_mode_ = "wb";

		int width_ = 0;
		int height_ = 0;
		size_t size_ = 0;
		TColor* image_ = nullptr;
	};

	//typedef RawImage<GreyscaleColor> GreyscaleRawImage;

	template<typename TColor>
	RawImage<TColor>::RawImage() { }

	template<typename TColor>
	RawImage<TColor>::RawImage(const int & width, const int & height)
		: width_(width), height_(height), size_(width * height)
	{
		image_ = new TColor[size_]{};
	}

	template<typename TColor>
	RawImage<TColor>::~RawImage()
	{
		delete[] image_;
		image_ = nullptr;
	}

	template <typename TColor>
	inline void RawImage<TColor>::set_pixel(const uint32_t x, const uint32_t y, const TColor& color)
	{
		set_pixel(y * width_ + x, color);
	}

	template<typename TColor>
	inline void RawImage<TColor>::set_pixel(const uint32_t index, const TColor & color)
	{
		if (!validate_index(index))
		{
			throw std::out_of_range("Set pixel index was outside of image");
		}

		*(image_ + index) = color;
	}

	template <typename TColor>
	inline const TColor RawImage<TColor>::get_pixel(const uint32_t x, const uint32_t y) const
	{
		return get_pixel(y * width_ + x);
	}

	template<typename TColor>
	inline const TColor RawImage<TColor>::get_pixel(const uint32_t index) const
	{
		if (!validate_index(index))
		{
			throw std::out_of_range("Get pixel index was outside of image");
		}

		return *(image_ + index);
	}

	template<typename TColor>
	inline const size_t RawImage<TColor>::get_size() const
	{
		return size_;
	}

	template<typename TColor>
	inline const int RawImage<TColor>::get_height() const
	{
		return height_;
	}

	template<typename TColor>
	inline const int RawImage<TColor>::get_width() const
	{
		return width_;
	}

	template<typename TColor>
	bool RawImage<TColor>::save_to_file(const char * path)
	{
		FILE * file = fopen(path, write_mode_);
		if (file == NULL)
		{
			return false;
		}

		fwrite(image_, sizeof(TColor), size_, file);
		fclose(file);

		return true;
	}

	template<typename TColor>
	bool RawImage<TColor>::load_from_file(const char * path, const int & width, const int & height)
	{
		width_ = width;
		height_ = height;

		size_t newSize = width * height;
		if (size_ != newSize)
		{
			size_ = newSize;

			if (image_ != nullptr)
			{
				delete[] image_;
			}
			image_ = new TColor[size_];
		}

		FILE * file = fopen(path, read_mode_);
		if (file == NULL)
		{
			return false;
		}

		fread(image_, sizeof(TColor), size_, file);

		fclose(file);

		return true;
	}

	template<typename TColor>
	inline bool RawImage<TColor>::validate_index(const uint32_t index) const
	{
		return index < size_;
	}

	template<typename TColor>
	inline void RawImage<TColor>::copy_pixels_from(const RawImage<TColor>& source)
	{
		std::copy(source.image_, source.image_ + size_, this->image_);
	}

	template<typename TColor>
	inline void RawImage<TColor>::copy_pixels_from(const RawImage<TColor>& source, size_t index, size_t size)
	{
		std::copy(source.image_ + index, source.image_ + index + size, this->image_ + index);
	}

} // bk

#endif