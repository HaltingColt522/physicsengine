#include <vkSetup/vkSetup.h>

#include <vkSetup/physicalDevice.h>
#include <vkSetup/logicalDevice.h>
#include <vkSetup/swapchain.h>
#include <vkSetup/imageView.h>
#include <vkSetup/debug.h>

void Vulkan::winsetup(WINDOW* window, VkInstance instance, HWND handle) {

	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.pNext = NULL;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.hinstance = GetModuleHandle(NULL);
	surfaceCreateInfo.hwnd = handle;

	if (vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, NULL, &window->surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create VkSurface for win32 window");
	}

	pickPhysicalDevice(window, instance);

	createLogicalDevice(window);

	createSwapChain(window, handle);

	createImageViews(window);
}

void Vulkan::cleanup(WINDOW* window, INSTANCE instance) {
	for (auto imageView : window->swapChainImageViews) {
		vkDestroyImageView(window->device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(window->device, window->swapChain, nullptr);
	vkDestroyDevice(window->device, nullptr);

	if (enableValidationLayers) {
		Debug::DestroyDebugUtilsMessengerEXT(instance.instance, instance.debugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(instance.instance, window->surface, nullptr);

}