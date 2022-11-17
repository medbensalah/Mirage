-- Mirage Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

--include directories relative to solution directory
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Mirage/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Mirage/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Mirage/vendor/ImGui"
IncludeDir["glm"] = "%{wks.location}/Mirage/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Mirage/vendor/stb_image"
IncludeDir["enTT"] = "%{wks.location}/Mirage/vendor/enTT/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Mirage/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Mirage/vendor/ImGuizmo"
IncludeDir["shaderc"] = "%{wks.location}/Mirage/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Mirage/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Mirage/vendor/VulkanSDK/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
