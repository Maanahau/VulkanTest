#pragma once
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <optional>
#include <array>

namespace testengine {

    class TestEngine {
        public:

            void run();

        private:
    
            const uint32_t WIDTH = 800;
            const uint32_t HEIGHT = 600;

            const int MAX_FRAMES_IN_FLIGHT = 2;

            const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
            };

            #ifdef NDEBUG
                const bool enableValidationLayers = false;
            #else
                const bool enableValidationLayers = true;
            #endif

            const std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };

            struct QueueFamilyIndices {
                std::optional<uint32_t> graphicsFamily;
                std::optional<uint32_t> presentFamily;

                bool isComplete() {
                    return graphicsFamily.has_value() && presentFamily.has_value();
                }
            };

            struct SwapChainSupportDetails {
                VkSurfaceCapabilitiesKHR capabilities;
                std::vector<VkSurfaceFormatKHR> formats;
                std::vector<VkPresentModeKHR> presentModes;
            };

            GLFWwindow* window;
            VkInstance instance;
            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
            VkDevice device;
            VkQueue graphicsQueue;
            VkQueue presentQueue;
            VkSurfaceKHR surface;

            VkSwapchainKHR swapChain;
            std::vector<VkImage> swapChainImages;
            VkFormat swapChainImageFormat;
            VkExtent2D swapChainExtent;
            std::vector<VkImageView> swapChainImageViews;
            std::vector<VkFramebuffer> swapChainFramebuffers;

            std::vector<VkDynamicState> dynamicStates = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR
            };
            VkRenderPass renderPass;
            VkDescriptorSetLayout descriptorSetLayout;
            VkPipelineLayout pipelineLayout;
            VkPipeline graphicsPipeline;

            VkCommandPool commandPool;
            std::vector<VkCommandBuffer> commandBuffers;

            std::vector<VkSemaphore> imageAvailableSemaphores;
            std::vector<VkSemaphore> renderFinishedSemaphores;
            std::vector<VkFence> inFlightFences;

            uint32_t currentFrame = 0;
            bool framebufferResized = false;

            struct Vertex {
                    glm::vec2 pos;
                    glm::vec3 color;
                };

            struct UniformBufferObject {
                glm::mat4 model;
                glm::mat4 view;
                glm::mat4 projection;
            };

            const std::vector<Vertex> vertices = {
                {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
            };
            const std::vector<uint16_t> indices = {
                0, 1, 2, 2, 3, 0
            };

            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            VkBuffer indexBuffer;
            VkDeviceMemory indexBufferMemory;
            
            std::vector<VkBuffer> uniformBuffers;
            std::vector<VkDeviceMemory> uniformBuffersMemory;
            std::vector<void*> uniformBuffersMapped;

            void initWindow();
            void initVulkan();
            void mainLoop();
            void cleanup();

            void createInstance();
            void createSurface();
            void pickPhysicalDevice();
            void createLogicalDevice();
            void createSwapChain();
            void createImageViews();
            void createRenderPass();
            void createDescriptorSetLayout();
            void createGraphicsPipeline();
            void createFramebuffers();
            void createCommandPool();
            void createVertexBuffer();
            void createIndexBuffer();
            void createUniformBuffers();
            void createCommandBuffers();
            void createSyncObjects();

            void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
            void drawFrame();

            void cleanupSwapChain();
            void recreateSwapChain();

            bool checkValidationLayerSupport();
            bool checkDeviceExtensionSupport(VkPhysicalDevice device);
            bool isDeviceSuitable(VkPhysicalDevice device);

            QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
            SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

            VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
            VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
            VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

            VkShaderModule createShaderModule(const std::vector<char>& code);

            static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

            static VkVertexInputBindingDescription getBindingDescription();
            std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();

            uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
            void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
            void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    };
}