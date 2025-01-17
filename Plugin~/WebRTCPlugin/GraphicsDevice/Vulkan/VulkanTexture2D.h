#pragma once

#include "GraphicsDevice/ITexture2D.h"
#include "IUnityGraphicsVulkan.h"
#include "VulkanUtility.h"

namespace unity
{
namespace webrtc
{
    class VulkanTexture2D : public ITexture2D
    {
    public:
        VulkanTexture2D(const uint32_t w, const uint32_t h);
        virtual ~VulkanTexture2D() override;

        bool Init(const UnityVulkanInstance* instance, const VkCommandPool commandPool);
        bool InitStaging(
            const UnityVulkanInstance* instance,
            const VkCommandPool commandPool,
            bool writable,
            bool hasHostCachedMemory);
        void Shutdown();

        void* GetNativeTexturePtrV() override { return m_unityVulkanImage.image; }
        const void* GetNativeTexturePtrV() const override { return m_unityVulkanImage.image; };
        void* GetEncodeTexturePtrV() override { return nullptr; }
        const void* GetEncodeTexturePtrV() const override { return nullptr; }

        UnityVulkanImage* GetUnityVulkanImage() { return &m_unityVulkanImage; }
        VkImage GetImage() const { return m_unityVulkanImage.image; }
        VkDeviceMemory GetTextureImageMemory() const { return m_unityVulkanImage.memory.memory; }
        VkDeviceSize GetTextureImageMemorySize() const { return m_unityVulkanImage.memory.size; }
        VkFormat GetTextureFormat() const { return m_textureFormat; }

        size_t GetPitch() const { return m_rowPitch; }

#if VULKAN_USE_CRS
        void ResetFrameNumber() const { currentFrameNumber = 0; }
        mutable unsigned long long currentFrameNumber = 0;
#else
        bool CreateFence();
        VkFence GetFence() const { return m_fence; }
        VkCommandBuffer GetCommandBuffer() const { return m_commandBuffer; }

        VkCommandPool m_commandPool;
        VkCommandBuffer m_commandBuffer;
        VkFence m_fence;
#endif

    private:
        UnityVulkanInstance m_Instance = {};
        VkFormat m_textureFormat;
        size_t m_rowPitch;
        UnityVulkanImage m_unityVulkanImage = {};
        const VkAllocationCallbacks* m_allocator = nullptr;
    };

} // end namespace unity
} // end namespace webrtc
