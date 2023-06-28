#pragma once
#include <cstdint>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <vector>
#include <optional>
#include <array>
#include <string>

namespace testengine {

    class TestEngine {
        public:

            struct Vertex {
                glm::vec3 pos;
                glm::vec3 color;
                glm::vec2 texCoord;

                bool operator==(const Vertex& other) const {
                    return pos == other.pos && color == other.color && texCoord == other.texCoord;
                }
            };

            void run();

        private:

            const uint32_t WIDTH = 800;
            const uint32_t HEIGHT = 600;

            const std::string MODEL_PATH = "models/viking_room.obj";
            const std::string TEXTURE_PATH = "textures/viking_room.obj";

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

            struct UniformBufferObject {
                alignas(16) glm::mat4 model;
                alignas(16) glm::mat4 view;
                alignas(16) glm::mat4 projection;
            };

            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            VkBuffer indexBuffer;
            VkDeviceMemory indexBufferMemory;

            std::vector<VkBuffer> uniformBuffers;
            std::vector<VkDeviceMemory> uniformBuffersMemory;
            std::vector<void*> uniformBuffersMapped;

            VkDescriptorPool descriptorPool;
            std::vector<VkDescriptorSet> descriptorSets;

            VkImage textureImage;
            VkDeviceMemory textureImageMemory;
            VkImageView textureImageView;
            VkSampler textureSampler;
            uint32_t mipLevels;

            VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

            VkImage colorImage;
            VkDeviceMemory colorImageMemory;
            VkImageView colorImageView;

            VkImage depthImage;
            VkDeviceMemory depthImageMemory;
            VkImageView depthImageView;


            void initWindow();
            void initVulkan();
            void mainLoop();
            void cleanup();

            //init functions
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
            void createColorResources();
            void createDepthResources();
            void createTextureImage();
            void createTextureImageView();
            void createTextureSampler();
            void loadModel();
            void createVertexBuffer();
            void createIndexBuffer();
            void createUniformBuffers();
            void createDescriptorPool();
            void createDescriptorSets();
            void createCommandBuffers();
            void createSyncObjects();

            void updateUniformBuffer(uint32_t currentImage);
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
            std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();

            uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
            void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
            void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

            void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
                             VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

            VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

            void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

            VkCommandBuffer beginSingleTimeCommands();
            void endSingleTimeCommands(VkCommandBuffer commandBuffer);

            void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

            VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

            VkFormat findDepthFormat();

            bool hasStencilComponent(VkFormat format);

            void generateMipMaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

            VkSampleCountFlagBits getMaxUsableSampleCount();


    };
}

namespace std {
    template<> struct hash<testengine::TestEngine::Vertex> {
        size_t operator()(testengine::TestEngine::Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                    (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                    (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}