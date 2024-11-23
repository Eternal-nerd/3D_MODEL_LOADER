#include "vk_params.h"

vk_params::vk_params() {
	// appInfo   
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "3D_MODEL_LOADER";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;
	
	// instanceCreateInfo
	// instanceCreateInfo.


}

vk_params::~vk_params() {}
