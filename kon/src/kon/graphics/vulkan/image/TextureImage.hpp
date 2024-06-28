
#pragma once

#include "kon/core/Core.hpp"
#include "kon/graphics/vulkan/Device.hpp"
#include "kon/graphics/vulkan/commands/CommandPool.hpp"
#include "kon/graphics/vulkan/image/Image.hpp"
#include "kon/graphics/vulkan/image/ImageView.hpp"
#include "vulkan/vulkan_core.h"
namespace kon
{
	struct TextureData
	{
		int width;
		int height;
		VkDeviceSize size;
	};

	class TextureImage
	{
	public:
		TextureImage(Device *device, CommandPool *commandPool, u8 *image, const TextureData &data, bool mipmap=true);
		~TextureImage();

		Image *GetImage() { return m_image; }
		ImageView *GetImageView() { return m_imageView; }

	private:
		void GenerateMipmaps(VkFormat imageFormat);

	private:
		Device *m_device;
		CommandPool *m_commandPool;

		Image *m_image;
		ImageView *m_imageView;
		// TextureImage *m_textureImage;

		int m_width;
		int m_height;
		int m_mipLevels;
	};
}

