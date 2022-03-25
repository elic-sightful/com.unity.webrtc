#include "pch.h"

#include "MetalDevice.h"

#import <Metal/Metal.h>

namespace unity
{
namespace webrtc
{
    class FakeMetalDeviceImpl : public MetalDevice
    {
    public:
        FakeMetalDeviceImpl()
        : device_(MTLCreateSystemDefaultDevice())
        {
        }
        
        ~FakeMetalDeviceImpl() = default;
        
        id<MTLDevice> Device() override
        {
            return device_;
        }
        id<MTLCommandBuffer> CurrentCommandEncoder() override
        {
            return nullptr;
        }
        void EndCurrentCommandEncoder() override
        {}
    private:
        id<MTLDevice> device_;      
    };

    class UnityMetalDeviceImpl : public MetalDevice
    {
    public:
        UnityMetalDeviceImpl(IUnityGraphicsMetal* graphics)
        : graphics_(graphics)
        {
        }
        
        ~UnityMetalDeviceImpl() = default;
        
        id<MTLDevice> Device() override
        {
            return graphics_->MetalDevice();
        }
        id<MTLCommandBuffer> CurrentCommandEncoder() override
        {
            return graphics_->CurrentCommandBuffer();
        }
        void EndCurrentCommandEncoder() override
        {
            graphics_->EndCurrentCommandEncoder();            
        }
    private:
        IUnityGraphicsMetal* graphics_;
    };

    std::unique_ptr<MetalDevice> MetalDevice::Create(IUnityGraphicsMetal* device)
    {
        return std::make_unique<UnityMetalDeviceImpl>(device);
    }
    std::unique_ptr<MetalDevice> MetalDevice::CreateForTest()
    {
        return std::make_unique<FakeMetalDeviceImpl>();
    }

}
}
