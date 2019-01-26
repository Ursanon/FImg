#ifndef RAW_IMAGE_HPP
#define RAW_IMAGE_HPP

namespace bk
{
	//template <typename T>
	//struct Vector2
	//{
	//public:
	//	Vector2();
	//	Vector2(T x, T y);

	//public:
	//	T x_;
	//	T y_;

	//public:
	//	static Vector2 zero()
	//};

	struct Color
	{
	};

	struct GreyscaleColor : Color
	{
	public:
		void combine(const GreyscaleColor& rhs)
		{
			this->greyscale = (this->greyscale + rhs.greyscale) >> 1;
		}

		static GreyscaleColor combine(const GreyscaleColor& lhs, const GreyscaleColor& rhs)
		{
			GreyscaleColor result;
			result.greyscale = (lhs.greyscale + rhs.greyscale) >> 1;

			return result;
		}

	public:
		uint8_t greyscale;
	};

	template <typename TColor>
	class RawImage
	{
	public:
		~RawImage();

		virtual void copy_pixels_from(const RawImage& source);
		virtual void copy_pixels_from(const RawImage& source, const size_t index, const size_t size);

		virtual void set_pixel(const uint32_t x, const uint32_t y, const TColor& color);

		virtual const TColor get_pixel(const uint32_t x, const uint32_t y) const;
		virtual const TColor get_pixel(const uint32_t index) const;
		virtual const size_t get_size() const;
		virtual const int get_height() const;
		virtual const int get_width() const;

		virtual void save_to_file(const char * path) = 0;
		virtual void load_from_file(const char * path, const int& width, const int& height) = 0;
	
	protected:
		RawImage();
		RawImage(const int& width, const int& height);
	
	protected:
		int width_;
		int height_;
		size_t size_;
		TColor* image_ = nullptr;
	};

	template <typename TColor>
	inline void RawImage<TColor>::set_pixel(const uint32_t x, const uint32_t y, const TColor& color)
	{
		*(image_ + (y * width_ + x)) = color;
	}

	template <typename TColor>
	inline const TColor RawImage<TColor>::get_pixel(const uint32_t x, const uint32_t y) const
	{
		return *(image_ + (y * width_ + x));
	}

	template<typename TColor>
	inline const TColor RawImage<TColor>::get_pixel(const uint32_t index) const
	{
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
	RawImage<TColor>::RawImage() { }

	template<typename TColor>
	RawImage<TColor>::RawImage(const int & width, const int & height)
		: width_(width), height_(height), size_(width * height)
	{
		image_ = new GreyscaleColor[size_]{};
	}

	template<typename TColor>
	RawImage<TColor>::~RawImage()
	{
		delete[] image_;
		image_ = nullptr;
	}

	template<typename TColor>
	inline void RawImage<TColor>::copy_pixels_from(const RawImage<TColor>& source)
	{
		for (size_t i = 0; i < size_; ++i)
		{
			this->image_[i] = source.image_[i];
		}

		//memset(this->image_, source.image_, sizeof(TColor) * size_);
	}

	template<typename TColor>
	inline void RawImage<TColor>::copy_pixels_from(const RawImage<TColor>& source, size_t index, size_t size)
	{
		for (size_t i = index; i < size; ++i)
		{
			this->image_[i] = source.image_[i];
		}

		//memset((this->image_ + index), (source.image_ + index), sizeof(*source.image_) * size);
	}

} // bk

#endif