#ifndef RENDERINCLUDE_H
#define RENDERINCLUDE_H

#ifdef VULKAN_BACKEND

#include "VulkanEngine/include/VKRenderer.h" 

#else

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#endif

#endif
