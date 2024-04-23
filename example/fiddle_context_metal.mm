#include "fiddle_context.hpp"

#include "rive/pls/pls_renderer.hpp"
//#include "rive/pls/gl/pls_render_context_gl_impl.hpp"
//#include "rive/pls/gl/pls_render_target_gl.hpp"
#include "rive/pls/metal/pls_render_context_metal_impl.h"

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

using namespace rive;
using namespace rive::pls;

class FiddleContextMetalPLS : public FiddleContext
{
public:
    FiddleContextMetalPLS(FiddleContextOptions fiddleOptions)
        : m_fiddleOptions(fiddleOptions)
    {
        PLSRenderContextMetalImpl::ContextOptions metalOptions;
        if (m_fiddleOptions.synchronousShaderCompilations)
        {
            // Turn on synchronous shader compilations to ensure deterministic rendering and to make
            // sure we test every unique shader.
            metalOptions.synchronousShaderCompilations = true;
        }

        if (m_fiddleOptions.disableRasterOrdering)
        {
            // Turn on synchronous shader compilations to ensure deterministic rendering and to make
            // sure we test every unique shader.
            metalOptions.disableFramebufferReads = true;
        }

        m_plsContext = PLSRenderContextMetalImpl::MakeContext(m_gpu, metalOptions);
        printf("==== MTLDevice: %s ====\n", m_gpu.name.UTF8String);
    }

    float dpiScale(GLFWwindow* window) const override
    {
        NSWindow* nsWindow = glfwGetCocoaWindow(window);
        return m_fiddleOptions.retinaDisplay ? nsWindow.backingScaleFactor : 1;
    }

    Factory* factory() override { return m_plsContext.get(); }

    rive::pls::PLSRenderContext* plsContextOrNull() override { return m_plsContext.get(); }

    rive::pls::PLSRenderTarget* plsRenderTargetOrNull() override { return m_renderTarget.get(); }

    void onSizeChanged(GLFWwindow* window, int width, int height, uint32_t sampleCount) override
    {
        NSWindow* nsWindow = glfwGetCocoaWindow(window);
        NSView* view = [nsWindow contentView];
        view.wantsLayer = YES;

        m_swapchain = [CAMetalLayer layer];
        m_swapchain.device = m_gpu;
        m_swapchain.opaque = YES;
        m_swapchain.framebufferOnly = !m_fiddleOptions.readableFramebuffer;
        m_swapchain.pixelFormat = MTLPixelFormatBGRA8Unorm;
        m_swapchain.contentsScale = dpiScale(window);
        m_swapchain.displaySyncEnabled = NO;
        view.layer = m_swapchain;

        auto plsContextImpl = m_plsContext->static_impl_cast<PLSRenderContextMetalImpl>();
        m_renderTarget = plsContextImpl->makeRenderTarget(MTLPixelFormatBGRA8Unorm, width, height);
    }

    void toggleZoomWindow() override {}

    std::unique_ptr<Renderer> makeRenderer(int width, int height) override
    {
        return std::make_unique<PLSRenderer>(m_plsContext.get());
    }

    void begin(const PLSRenderContext::FrameDescriptor& frameDescriptor) override
    {
        m_plsContext->beginFrame(frameDescriptor);
    }

    void flushPLSContext() final
    {
        if (m_currentFrameSurface == nil)
        {
            m_currentFrameSurface = [m_swapchain nextDrawable];
            assert(m_currentFrameSurface.texture.width == m_renderTarget->width());
            assert(m_currentFrameSurface.texture.height == m_renderTarget->height());
            m_renderTarget->setTargetTexture(m_currentFrameSurface.texture);
        }

        id<MTLCommandBuffer> flushCommandBuffer = [m_queue commandBuffer];
        m_plsContext->flush({.renderTarget = m_renderTarget.get(),
                             .externalCommandBuffer = (__bridge void*)flushCommandBuffer});
        [flushCommandBuffer commit];
    }

    void end(GLFWwindow* window, std::vector<uint8_t>*) final
    {
        flushPLSContext();

        id<MTLCommandBuffer> presentCommandBuffer = [m_queue commandBuffer];
        [presentCommandBuffer presentDrawable:m_currentFrameSurface];
        [presentCommandBuffer commit];

        m_currentFrameSurface = nil;
        m_renderTarget->setTargetTexture(nil);
    }

private:
    const FiddleContextOptions m_fiddleOptions;
    id<MTLDevice> m_gpu = MTLCreateSystemDefaultDevice();
    id<MTLCommandQueue> m_queue = [m_gpu newCommandQueue];
    std::unique_ptr<PLSRenderContext> m_plsContext;
    CAMetalLayer* m_swapchain;
    rcp<PLSRenderTargetMetal> m_renderTarget;
    id<CAMetalDrawable> m_currentFrameSurface = nil;
};

std::unique_ptr<FiddleContext> FiddleContext::MakeMetalPLS(FiddleContextOptions fiddleOptions)
{
    return std::make_unique<FiddleContextMetalPLS>(fiddleOptions);
}
