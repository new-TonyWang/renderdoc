/******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2019-2021 Baldur Karlsson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#include "vk_common.h"
#include "vk_info.h"
#include "vk_manager.h"
#include "vk_resources.h"

// there's an annoying little difference between VkFoo*Flags* and VkFoo*FlagBits* meaning we can't
// just append 'Bits' onto a flag type to get the bits enum. So we declare typedefs to do that for
// us so the macros are cleaner below.
// we could avoid this but we'd need to do the base + suffix thing in SERIALISE_MEMBER_VKFLAGS()
// which is uglier.

#define DECL_VKFLAG(flagbase) typedef CONCAT(flagbase, FlagBits) CONCAT(flagbase, FlagsBits);
#define DECL_VKFLAG_EXT(flagbase, suffix)            \
  typedef CONCAT(flagbase, CONCAT(FlagBits, suffix)) \
      CONCAT(flagbase, CONCAT(Flags, CONCAT(suffix, Bits)));

// duplicates for enums that don't yet have a FlagBits enum, which we direct to VkFlagWithNoBits
// we declare our own enum of the same name will then compile error if this macro is mistakenly
// used for a flags type with some bits (for example, if a bit is later added to a previously empty
// flags field).
#define DECL_VKFLAG_EMPTY(flagbase) \
  enum CONCAT(flagbase, FlagBits)   \
  {                                 \
  };                                \
  typedef VkFlagWithNoBits CONCAT(flagbase, FlagsBits);
#define DECL_VKFLAG_EMPTY_EXT(flagbase, suffix)   \
  enum CONCAT(flagbase, CONCAT(FlagBits, suffix)) \
  {                                               \
  };                                              \
  typedef VkFlagWithNoBits CONCAT(flagbase, CONCAT(Flags, CONCAT(suffix, Bits)));

DECL_VKFLAG(VkAccess);
DECL_VKFLAG(VkAttachmentDescription);
DECL_VKFLAG(VkBufferCreate);
DECL_VKFLAG(VkBufferUsage);
DECL_VKFLAG_EMPTY(VkBufferViewCreate);
DECL_VKFLAG(VkColorComponent);
DECL_VKFLAG(VkCommandBufferReset);
DECL_VKFLAG(VkCommandBufferUsage);
DECL_VKFLAG(VkCommandPoolCreate);
DECL_VKFLAG(VkCommandPoolReset);
DECL_VKFLAG_EMPTY(VkCommandPoolTrim);
DECL_VKFLAG(VkCullMode);
DECL_VKFLAG(VkDependency);
DECL_VKFLAG(VkDescriptorPoolCreate);
DECL_VKFLAG_EMPTY(VkDescriptorPoolReset);
DECL_VKFLAG(VkDescriptorSetLayoutCreate);
DECL_VKFLAG_EMPTY(VkDescriptorUpdateTemplateCreate);
DECL_VKFLAG_EMPTY(VkDeviceCreate);
DECL_VKFLAG(VkDeviceQueueCreate);
DECL_VKFLAG(VkEventCreate);
DECL_VKFLAG(VkExternalFenceHandleType);
DECL_VKFLAG(VkExternalFenceFeature);
DECL_VKFLAG(VkExternalMemoryHandleType);
DECL_VKFLAG(VkExternalMemoryFeature);
DECL_VKFLAG(VkExternalSemaphoreHandleType);
DECL_VKFLAG(VkExternalSemaphoreFeature);
DECL_VKFLAG(VkFormatFeature);
DECL_VKFLAG(VkImageAspect);
DECL_VKFLAG(VkImageUsage);
DECL_VKFLAG(VkImageCreate);
DECL_VKFLAG(VkImageViewCreate);
DECL_VKFLAG_EMPTY(VkInstanceCreate);
DECL_VKFLAG(VkFenceCreate);
DECL_VKFLAG(VkFenceImport);
DECL_VKFLAG(VkFramebufferCreate);
DECL_VKFLAG(VkMemoryAllocate);
DECL_VKFLAG(VkMemoryHeap);
DECL_VKFLAG_EMPTY(VkMemoryMap);
DECL_VKFLAG(VkMemoryProperty);
DECL_VKFLAG(VkPeerMemoryFeature);
DECL_VKFLAG(VkPipelineCacheCreate);
DECL_VKFLAG_EMPTY(VkPipelineColorBlendStateCreate);
DECL_VKFLAG(VkPipelineCreate);
DECL_VKFLAG_EMPTY(VkPipelineDepthStencilStateCreate);
DECL_VKFLAG_EMPTY(VkPipelineDynamicStateCreate);
DECL_VKFLAG_EMPTY(VkPipelineInputAssemblyStateCreate);
DECL_VKFLAG_EMPTY(VkPipelineLayoutCreate);
DECL_VKFLAG_EMPTY(VkPipelineMultisampleStateCreate);
DECL_VKFLAG_EMPTY(VkPipelineRasterizationStateCreate);
DECL_VKFLAG(VkPipelineShaderStageCreate);
DECL_VKFLAG(VkPipelineStage);
DECL_VKFLAG_EMPTY(VkPipelineTessellationStateCreate);
DECL_VKFLAG_EMPTY(VkPipelineVertexInputStateCreate);
DECL_VKFLAG_EMPTY(VkPipelineViewportStateCreate);
DECL_VKFLAG(VkQueryControl);
DECL_VKFLAG(VkQueryPipelineStatistic);
DECL_VKFLAG_EMPTY(VkQueryPoolCreate);
DECL_VKFLAG(VkQueryResult);
DECL_VKFLAG(VkQueue);
DECL_VKFLAG(VkRenderPassCreate);
DECL_VKFLAG(VkSamplerCreate);
DECL_VKFLAG(VkSampleCount);
DECL_VKFLAG_EMPTY(VkSemaphoreCreate);
DECL_VKFLAG(VkSemaphoreImport);
DECL_VKFLAG(VkShaderModuleCreate);
DECL_VKFLAG(VkShaderStage);
DECL_VKFLAG(VkSparseImageFormat);
DECL_VKFLAG(VkSparseMemoryBind);
DECL_VKFLAG(VkStencilFace);
DECL_VKFLAG(VkSubgroupFeature);
DECL_VKFLAG(VkSubpassDescription);
DECL_VKFLAG(VkDescriptorBinding);
DECL_VKFLAG(VkSemaphoreWait);
DECL_VKFLAG(VkResolveMode);
DECL_VKFLAG_EXT(VkAcquireProfilingLock, KHR);
DECL_VKFLAG_EXT(VkBuildAccelerationStructure, NV);
DECL_VKFLAG_EXT(VkCompositeAlpha, KHR);
DECL_VKFLAG_EXT(VkConditionalRendering, EXT);
DECL_VKFLAG_EXT(VkDebugReport, EXT);
DECL_VKFLAG_EMPTY_EXT(VkDebugUtilsMessengerCallbackData, EXT);
DECL_VKFLAG_EMPTY_EXT(VkDebugUtilsMessengerCreate, EXT);
DECL_VKFLAG_EXT(VkDebugUtilsMessageSeverity, EXT);
DECL_VKFLAG_EXT(VkDebugUtilsMessageType, EXT);
DECL_VKFLAG_EXT(VkDeviceGroupPresentMode, KHR);
DECL_VKFLAG_EMPTY_EXT(VkDisplayModeCreate, KHR);
DECL_VKFLAG_EXT(VkDisplayPlaneAlpha, KHR);
DECL_VKFLAG_EMPTY_EXT(VkDisplaySurfaceCreate, KHR);
DECL_VKFLAG_EXT(VkExternalMemoryHandleType, NV);
DECL_VKFLAG_EXT(VkExternalMemoryFeature, NV);
DECL_VKFLAG_EXT(VkGeometry, NV);
DECL_VKFLAG_EXT(VkGeometryInstance, NV);
DECL_VKFLAG_EXT(VkIndirectCommandsLayoutUsage, NV);
DECL_VKFLAG_EXT(VkPerformanceCounterDescription, KHR);
DECL_VKFLAG_EMPTY_EXT(VkPipelineCoverageModulationStateCreate, NV);
DECL_VKFLAG_EMPTY_EXT(VkPipelineCoverageToColorStateCreate, NV);
DECL_VKFLAG_EMPTY_EXT(VkPipelineDiscardRectangleStateCreate, EXT);
DECL_VKFLAG_EMPTY_EXT(VkPipelineRasterizationConservativeStateCreate, EXT);
DECL_VKFLAG_EMPTY_EXT(VkPipelineRasterizationStateStreamCreate, EXT);
DECL_VKFLAG_EMPTY_EXT(VkPipelineViewportSwizzleStateCreate, NV);
DECL_VKFLAG_EXT(VkPrivateDataSlotCreate, EXT);
DECL_VKFLAG_EXT(VkSurfaceCounter, EXT);
DECL_VKFLAG_EXT(VkSurfaceTransform, KHR);
DECL_VKFLAG_EXT(VkSwapchainCreate, KHR);
DECL_VKFLAG_EMPTY_EXT(VkValidationCacheCreate, EXT);
DECL_VKFLAG_EMPTY_EXT(VkPipelineRasterizationDepthClipStateCreate, EXT);
DECL_VKFLAG_EXT(VkToolPurpose, EXT);
DECL_VKFLAG_EXT(VkSubmit, KHR);
DECL_VKFLAG_EXT(VkPipelineStage, 2KHR);
DECL_VKFLAG_EXT(VkAccess, 2KHR);

// serialise a member as flags - cast to the Bits enum for serialisation so the stringification
// picks up the bitfield and doesn't treat it as uint32_t. Then we rename the type back to the base
// flags type so the structured data is as accurate as possible.
#define SERIALISE_MEMBER_VKFLAGS(flagstype, name)       \
  SERIALISE_MEMBER_TYPED(CONCAT(flagstype, Bits), name) \
      .TypedAs(STRING_LITERAL(STRINGIZE(flagstype)))

#define SERIALISE_MEMBER_ARRAY_VKFLAGS(flagstype, name, count)       \
  SERIALISE_MEMBER_ARRAY_TYPED(CONCAT(flagstype, Bits), name, count) \
      .TypedAs(STRING_LITERAL(STRINGIZE(flagstype)))

// simple way to express "resources referenced from this struct don't have to be present."
// since this is used during read when the processing is single-threaded, we make it a static
// flag.
// If we multi-thread reading, this could be stored in the Serialiser context somehow.
template <typename SerialiserType>
struct OptionalResources
{
private:
  OptionalResources() = default;
};

// does nothing on writing
template <>
struct OptionalResources<Serialiser<SerialiserMode::Writing>>
{
  OptionalResources<Serialiser<SerialiserMode::Writing>>(Serialiser<SerialiserMode::Writing> &ser)
  {
  }
  ~OptionalResources<Serialiser<SerialiserMode::Writing>>() {}
  OptionalResources<Serialiser<SerialiserMode::Writing>>(
      const OptionalResources<Serialiser<SerialiserMode::Writing>> &) = default;
  OptionalResources<Serialiser<SerialiserMode::Writing>> &operator=(
      const OptionalResources<Serialiser<SerialiserMode::Writing>> &) = default;
};

template <>
struct OptionalResources<Serialiser<SerialiserMode::Reading>>
{
  OptionalResources<Serialiser<SerialiserMode::Reading>>(Serialiser<SerialiserMode::Reading> &ser)
  {
    Counter++;
  }
  ~OptionalResources<Serialiser<SerialiserMode::Reading>>() { Counter--; }
  OptionalResources<Serialiser<SerialiserMode::Reading>>(
      const OptionalResources<Serialiser<SerialiserMode::Reading>> &) = default;
  OptionalResources<Serialiser<SerialiserMode::Reading>> &operator=(
      const OptionalResources<Serialiser<SerialiserMode::Reading>> &) = default;
  static int Counter;
};

template <typename SerialiserType>
OptionalResources<SerialiserType> ScopedOptional(SerialiserType &ser)
{
  return OptionalResources<SerialiserType>(ser);
}

int OptionalResources<Serialiser<SerialiserMode::Reading>>::Counter = 0;

bool OptionalResourcesEnabled()
{
  return OptionalResources<Serialiser<SerialiserMode::Reading>>::Counter > 0;
}

// push/pop the optional flag. This doesn't allow non-optional objects in a sub-struct inside a
// struct that had optional objects... but that doesn't come up and seems unlikely.
#define OPTIONAL_RESOURCES() auto opt__LINE__ = ScopedOptional(ser);

// serialisation of object handles via IDs.
template <class SerialiserType, class type>
void DoSerialiseViaResourceId(SerialiserType &ser, type &el)
{
  VulkanResourceManager *rm = (VulkanResourceManager *)ser.GetUserData();

  ResourceId id;

  if(ser.IsWriting() && rm)
    id = GetResID(el);
  if(ser.IsStructurising() && rm)
    id = rm->GetOriginalID(GetResID(el));

  DoSerialise(ser, id);

  if(ser.IsReading() && rm && !IsStructuredExporting(rm->GetState()))
  {
    el = VK_NULL_HANDLE;

    if(id != ResourceId() && rm)
    {
      if(rm->HasLiveResource(id))
      {
        // we leave this wrapped.
        el = rm->GetLiveHandle<type>(id);
      }
      else if(!OptionalResourcesEnabled())
      {
        // It can be OK for a resource to have no live equivalent if the capture decided its not
        // needed, which some APIs do fairly often.
        RDCWARN("Capture may be missing reference to %s resource (%s).", TypeName<type>().c_str(),
                ToStr(id).c_str());
      }
    }
  }
}

#undef SERIALISE_HANDLE
#define SERIALISE_HANDLE(type)                    \
  template <class SerialiserType>                 \
  void DoSerialise(SerialiserType &ser, type &el) \
  {                                               \
    DoSerialiseViaResourceId(ser, el);            \
  }                                               \
  INSTANTIATE_SERIALISE_TYPE(type);

SERIALISE_VK_HANDLES();

#ifdef VK_USE_PLATFORM_GGP

#define HANDLE_PNEXT_OS_GGP() \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PRESENT_FRAME_TOKEN_GGP, VkPresentFrameTokenGGP)

#else

#define HANDLE_PNEXT_OS_GGP() PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PRESENT_FRAME_TOKEN_GGP)

#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR

#define HANDLE_PNEXT_OS_WIN32()                                                                       \
  /* VK_NV_external_memory_win32 */                                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMPORT_MEMORY_WIN32_HANDLE_INFO_NV, VkImportMemoryWin32HandleInfoNV) \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXPORT_MEMORY_WIN32_HANDLE_INFO_NV, VkExportMemoryWin32HandleInfoNV) \
                                                                                                      \
  /* VK_NV_win32_keyed_mutex */                                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_NV,                           \
               VkWin32KeyedMutexAcquireReleaseInfoNV)                                                 \
                                                                                                      \
  /* VK_KHR_win32_keyed_mutex */                                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_KHR,                          \
               VkWin32KeyedMutexAcquireReleaseInfoKHR)                                                \
                                                                                                      \
  /* VK_KHR_external_memory_win32 */                                                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMPORT_MEMORY_WIN32_HANDLE_INFO_KHR,                                 \
               VkImportMemoryWin32HandleInfoKHR)                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXPORT_MEMORY_WIN32_HANDLE_INFO_KHR,                                 \
               VkExportMemoryWin32HandleInfoKHR)                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_WIN32_HANDLE_PROPERTIES_KHR,                                  \
               VkMemoryWin32HandlePropertiesKHR)                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_GET_WIN32_HANDLE_INFO_KHR, VkMemoryGetWin32HandleInfoKHR)     \
                                                                                                      \
  /* VK_KHR_external_semaphore_win32 */                                                               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR,                              \
               VkExportSemaphoreWin32HandleInfoKHR)                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR,                              \
               VkImportSemaphoreWin32HandleInfoKHR)                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_D3D12_FENCE_SUBMIT_INFO_KHR, VkD3D12FenceSubmitInfoKHR)              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SEMAPHORE_GET_WIN32_HANDLE_INFO_KHR,                                 \
               VkSemaphoreGetWin32HandleInfoKHR)                                                      \
                                                                                                      \
  /* VK_KHR_external_fence_win32 */                                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXPORT_FENCE_WIN32_HANDLE_INFO_KHR, VkExportFenceWin32HandleInfoKHR) \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMPORT_FENCE_WIN32_HANDLE_INFO_KHR, VkImportFenceWin32HandleInfoKHR) \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_FENCE_GET_WIN32_HANDLE_INFO_KHR, VkFenceGetWin32HandleInfoKHR)       \
                                                                                                      \
  /* VK_EXT_full_screen_exclusive */                                                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT,                        \
               VkSurfaceFullScreenExclusiveWin32InfoEXT)                                              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_FULL_SCREEN_EXCLUSIVE_EXT,                      \
               VkSurfaceCapabilitiesFullScreenExclusiveEXT)                                           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT,                              \
               VkSurfaceFullScreenExclusiveInfoEXT)

#else

#define HANDLE_PNEXT_OS_WIN32()                                                       \
  /* VK_NV_external_memory_win32 */                                                   \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_IMPORT_MEMORY_WIN32_HANDLE_INFO_NV)             \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_EXPORT_MEMORY_WIN32_HANDLE_INFO_NV)             \
                                                                                      \
  /* VK_NV_win32_keyed_mutex */                                                       \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_NV)      \
                                                                                      \
  /* VK_KHR_win32_keyed_mutex */                                                      \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_KHR)     \
                                                                                      \
  /* VK_KHR_external_memory_win32 */                                                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_IMPORT_MEMORY_WIN32_HANDLE_INFO_KHR)            \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_EXPORT_MEMORY_WIN32_HANDLE_INFO_KHR)            \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_MEMORY_WIN32_HANDLE_PROPERTIES_KHR)             \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_MEMORY_GET_WIN32_HANDLE_INFO_KHR)               \
                                                                                      \
  /* VK_KHR_external_semaphore_win32 */                                               \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR)         \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR)         \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_D3D12_FENCE_SUBMIT_INFO_KHR)                    \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_SEMAPHORE_GET_WIN32_HANDLE_INFO_KHR)            \
                                                                                      \
  /* VK_KHR_external_fence_win32 */                                                   \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_EXPORT_FENCE_WIN32_HANDLE_INFO_KHR)             \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_IMPORT_FENCE_WIN32_HANDLE_INFO_KHR)             \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_FENCE_GET_WIN32_HANDLE_INFO_KHR)                \
                                                                                      \
  /* VK_EXT_full_screen_exclusive */                                                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT)   \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_FULL_SCREEN_EXCLUSIVE_EXT) \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT)

#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR

#define HANDLE_PNEXT_OS_ANDROID()                                                   \
  /* VK_ANDROID_external_memory_android_hardware_buffer */                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_USAGE_ANDROID,             \
               VkAndroidHardwareBufferUsageANDROID)                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_PROPERTIES_ANDROID,        \
               VkAndroidHardwareBufferPropertiesANDROID)                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_FORMAT_PROPERTIES_ANDROID, \
               VkAndroidHardwareBufferFormatPropertiesANDROID)                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMPORT_ANDROID_HARDWARE_BUFFER_INFO_ANDROID,       \
               VkImportAndroidHardwareBufferInfoANDROID)                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_GET_ANDROID_HARDWARE_BUFFER_INFO_ANDROID,   \
               VkMemoryGetAndroidHardwareBufferInfoANDROID)                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXTERNAL_FORMAT_ANDROID, VkExternalFormatANDROID)
#else

#define HANDLE_PNEXT_OS_ANDROID()                                                        \
  /* VK_ANDROID_external_memory_android_hardware_buffer */                               \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_USAGE_ANDROID)             \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_PROPERTIES_ANDROID)        \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_FORMAT_PROPERTIES_ANDROID) \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_IMPORT_ANDROID_HARDWARE_BUFFER_INFO_ANDROID)       \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_MEMORY_GET_ANDROID_HARDWARE_BUFFER_INFO_ANDROID)   \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_EXTERNAL_FORMAT_ANDROID)
#endif

// pNext structure type dispatch
#define HANDLE_PNEXT()                                                                                 \
  /* OS-specific extensions */                                                                         \
  HANDLE_PNEXT_OS_WIN32()                                                                              \
  HANDLE_PNEXT_OS_ANDROID()                                                                            \
  HANDLE_PNEXT_OS_GGP()                                                                                \
                                                                                                       \
  /* Core 1.0 structs. Should never be serialised in a pNext chain */                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_APPLICATION_INFO, VkApplicationInfo)                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, VkInstanceCreateInfo)                           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, VkDeviceQueueCreateInfo)                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, VkDeviceCreateInfo)                               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SUBMIT_INFO, VkSubmitInfo)                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, VkMemoryAllocateInfo)                           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE, VkMappedMemoryRange)                             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BIND_SPARSE_INFO, VkBindSparseInfo)                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, VkFenceCreateInfo)                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, VkSemaphoreCreateInfo)                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EVENT_CREATE_INFO, VkEventCreateInfo)                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO, VkQueryPoolCreateInfo)                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, VkBufferCreateInfo)                               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO, VkBufferViewCreateInfo)                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, VkImageCreateInfo)                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, VkImageViewCreateInfo)                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, VkShaderModuleCreateInfo)                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO, VkPipelineCacheCreateInfo)                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, VkPipelineShaderStageCreateInfo)   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,                              \
               VkPipelineVertexInputStateCreateInfo)                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,                            \
               VkPipelineInputAssemblyStateCreateInfo)                                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,                              \
               VkPipelineTessellationStateCreateInfo)                                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,                                  \
               VkPipelineViewportStateCreateInfo)                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,                             \
               VkPipelineRasterizationStateCreateInfo)                                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,                               \
               VkPipelineMultisampleStateCreateInfo)                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,                             \
               VkPipelineDepthStencilStateCreateInfo)                                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,                               \
               VkPipelineColorBlendStateCreateInfo)                                                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,                                   \
               VkPipelineDynamicStateCreateInfo)                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, VkGraphicsPipelineCreateInfo)          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO, VkComputePipelineCreateInfo)            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, VkPipelineLayoutCreateInfo)              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, VkSamplerCreateInfo)                             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, VkDescriptorSetLayoutCreateInfo)   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, VkDescriptorPoolCreateInfo)              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, VkDescriptorSetAllocateInfo)            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, VkWriteDescriptorSet)                           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET, VkCopyDescriptorSet)                             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, VkFramebufferCreateInfo)                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, VkRenderPassCreateInfo)                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, VkCommandPoolCreateInfo)                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, VkCommandBufferAllocateInfo)            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO, VkCommandBufferInheritanceInfo)      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, VkCommandBufferBeginInfo)                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO, VkRenderPassBeginInfo)                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER, VkBufferMemoryBarrier)                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, VkImageMemoryBarrier)                           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_BARRIER, VkMemoryBarrier)                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO, VkLayerInstanceCreateInfo)               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO, VkLayerDeviceCreateInfo)                   \
                                                                                                       \
  /* Vulkan 1.1 only, no extension - subgroups, protected memory */                                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES,                                  \
               VkPhysicalDeviceSubgroupProperties)                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PROTECTED_SUBMIT_INFO, VkProtectedSubmitInfo)                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2, VkDeviceQueueInfo2)                              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_FEATURES,                            \
               VkPhysicalDeviceProtectedMemoryFeatures)                                                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_PROPERTIES,                          \
               VkPhysicalDeviceProtectedMemoryProperties)                                              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES,                      \
               VkPhysicalDeviceShaderDrawParametersFeatures)                                           \
                                                                                                       \
  /* Vulkan 1.2 only, no extension */                                                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,                                  \
               VkPhysicalDeviceVulkan11Features)                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES,                                \
               VkPhysicalDeviceVulkan11Properties)                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,                                  \
               VkPhysicalDeviceVulkan12Features)                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES,                                \
               VkPhysicalDeviceVulkan12Properties)                                                     \
                                                                                                       \
  /* VK_AMD_device_coherent_memory */                                                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COHERENT_MEMORY_FEATURES_AMD,                         \
               VkPhysicalDeviceCoherentMemoryFeaturesAMD)                                              \
                                                                                                       \
  /* VK_AMD_display_native_hdr */                                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SWAPCHAIN_DISPLAY_NATIVE_HDR_CREATE_INFO_AMD,                         \
               VkSwapchainDisplayNativeHdrCreateInfoAMD)                                               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DISPLAY_NATIVE_HDR_SURFACE_CAPABILITIES_AMD,                          \
               VkDisplayNativeHdrSurfaceCapabilitiesAMD)                                               \
                                                                                                       \
  /* VK_AMD_shader_core_properties */                                                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_AMD,                           \
               VkPhysicalDeviceShaderCorePropertiesAMD)                                                \
                                                                                                       \
  /* VK_AMD_texture_gather_bias_lod */                                                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_TEXTURE_LOD_GATHER_FORMAT_PROPERTIES_AMD,                             \
               VkTextureLODGatherFormatPropertiesAMD)                                                  \
                                                                                                       \
  /* VK_EXT_astc_decode_mode */                                                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_VIEW_ASTC_DECODE_MODE_EXT, VkImageViewASTCDecodeModeEXT)        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ASTC_DECODE_FEATURES_EXT,                             \
               VkPhysicalDeviceASTCDecodeFeaturesEXT)                                                  \
                                                                                                       \
  /* VK_EXT_buffer_device_address */                                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_CREATE_INFO_EXT,                                \
               VkBufferDeviceAddressCreateInfoEXT)                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_EXT,                   \
               VkPhysicalDeviceBufferDeviceAddressFeaturesEXT)                                         \
                                                                                                       \
  /* VK_EXT_calibrated_timestamps */                                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_CALIBRATED_TIMESTAMP_INFO_EXT, VkCalibratedTimestampInfoEXT)          \
                                                                                                       \
  /* VK_EXT_conditional_rendering */                                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_CONDITIONAL_RENDERING_INFO_EXT,            \
               VkCommandBufferInheritanceConditionalRenderingInfoEXT)                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT,                   \
               VkPhysicalDeviceConditionalRenderingFeaturesEXT)                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_CONDITIONAL_RENDERING_BEGIN_INFO_EXT,                                 \
               VkConditionalRenderingBeginInfoEXT)                                                     \
                                                                                                       \
  /* VK_EXT_conservative_rasterization */                                                              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT,            \
               VkPhysicalDeviceConservativeRasterizationPropertiesEXT)                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT,            \
               VkPipelineRasterizationConservativeStateCreateInfoEXT)                                  \
                                                                                                       \
  /* VK_EXT_custom_border_color */                                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SAMPLER_CUSTOM_BORDER_COLOR_CREATE_INFO_EXT,                          \
               VkSamplerCustomBorderColorCreateInfoEXT)                                                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT,                   \
               VkPhysicalDeviceCustomBorderColorPropertiesEXT)                                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT,                     \
               VkPhysicalDeviceCustomBorderColorFeaturesEXT)                                           \
                                                                                                       \
  /* VK_EXT_debug_marker */                                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT, VkDebugMarkerObjectNameInfoEXT)    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT, VkDebugMarkerObjectTagInfoEXT)      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT, VkDebugMarkerMarkerInfoEXT)             \
                                                                                                       \
  /* VK_EXT_debug_report */                                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,                                \
               VkDebugReportCallbackCreateInfoEXT)                                                     \
                                                                                                       \
  /* VK_EXT_debug_utils */                                                                             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, VkDebugUtilsObjectNameInfoEXT)      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT, VkDebugUtilsObjectTagInfoEXT)        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT, VkDebugUtilsLabelEXT)                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT,                              \
               VkDebugUtilsMessengerCallbackDataEXT)                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,                                \
               VkDebugUtilsMessengerCreateInfoEXT)                                                     \
                                                                                                       \
  /* VK_EXT_depth_clip_enable */                                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT,                       \
               VkPhysicalDeviceDepthClipEnableFeaturesEXT)                                             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_DEPTH_CLIP_STATE_CREATE_INFO_EXT,              \
               VkPipelineRasterizationDepthClipStateCreateInfoEXT)                                     \
                                                                                                       \
  /* VK_EXT_descriptor_indexing */                                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,                      \
               VkDescriptorSetLayoutBindingFlagsCreateInfo)                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES,                         \
               VkPhysicalDeviceDescriptorIndexingFeatures)                                             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES,                       \
               VkPhysicalDeviceDescriptorIndexingProperties)                                           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,               \
               VkDescriptorSetVariableDescriptorCountAllocateInfo)                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_LAYOUT_SUPPORT,              \
               VkDescriptorSetVariableDescriptorCountLayoutSupport)                                    \
                                                                                                       \
  /* VK_EXT_discard_rectangles */                                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DISCARD_RECTANGLE_PROPERTIES_EXT,                     \
               VkPhysicalDeviceDiscardRectanglePropertiesEXT)                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_DISCARD_RECTANGLE_STATE_CREATE_INFO_EXT,                     \
               VkPipelineDiscardRectangleStateCreateInfoEXT)                                           \
                                                                                                       \
  /* VK_EXT_display_control */                                                                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DISPLAY_POWER_INFO_EXT, VkDisplayPowerInfoEXT)                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_EVENT_INFO_EXT, VkDeviceEventInfoEXT)                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DISPLAY_EVENT_INFO_EXT, VkDisplayEventInfoEXT)                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SWAPCHAIN_COUNTER_CREATE_INFO_EXT, VkSwapchainCounterCreateInfoEXT)   \
                                                                                                       \
  /* VK_EXT_display_surface_counter */                                                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_EXT, VkSurfaceCapabilities2EXT)                \
                                                                                                       \
  /* VK_EXT_extended_dynamic_state */                                                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,                  \
               VkPhysicalDeviceExtendedDynamicStateFeaturesEXT)                                        \
                                                                                                       \
  /* VK_EXT_filter_cubic */                                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_VIEW_IMAGE_FORMAT_INFO_EXT,                     \
               VkPhysicalDeviceImageViewImageFormatInfoEXT)                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_FILTER_CUBIC_IMAGE_VIEW_IMAGE_FORMAT_PROPERTIES_EXT,                  \
               VkFilterCubicImageViewImageFormatPropertiesEXT)                                         \
                                                                                                       \
  /* VK_EXT_fragment_density_map */                                                                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_FEATURES_EXT,                    \
               VkPhysicalDeviceFragmentDensityMapFeaturesEXT)                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_PROPERTIES_EXT,                  \
               VkPhysicalDeviceFragmentDensityMapPropertiesEXT)                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_RENDER_PASS_FRAGMENT_DENSITY_MAP_CREATE_INFO_EXT,                     \
               VkRenderPassFragmentDensityMapCreateInfoEXT)                                            \
                                                                                                       \
  /* VK_EXT_fragment_shader_interlock */                                                               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_INTERLOCK_FEATURES_EXT,               \
               VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT)                                     \
                                                                                                       \
  /* VK_EXT_global_priority */                                                                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_QUEUE_GLOBAL_PRIORITY_CREATE_INFO_EXT,                         \
               VkDeviceQueueGlobalPriorityCreateInfoEXT)                                               \
                                                                                                       \
  /* VK_EXT_hdr_metadata */                                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_HDR_METADATA_EXT, VkHdrMetadataEXT)                                   \
                                                                                                       \
  /* VK_EXT_host_query_reset */                                                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES,                            \
               VkPhysicalDeviceHostQueryResetFeatures)                                                 \
                                                                                                       \
  /* VK_EXT_image_robustness */                                                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ROBUSTNESS_FEATURES_EXT,                        \
               VkPhysicalDeviceImageRobustnessFeaturesEXT)                                             \
                                                                                                       \
  /* VK_EXT_index_type_uint8 */                                                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT8_FEATURES_EXT,                        \
               VkPhysicalDeviceIndexTypeUint8FeaturesEXT)                                              \
                                                                                                       \
  /* VK_EXT_inline_uniform_block */                                                                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT,                    \
               VkPhysicalDeviceInlineUniformBlockFeaturesEXT)                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT,                  \
               VkPhysicalDeviceInlineUniformBlockPropertiesEXT)                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK_EXT,                        \
               VkWriteDescriptorSetInlineUniformBlockEXT)                                              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_INLINE_UNIFORM_BLOCK_CREATE_INFO_EXT,                 \
               VkDescriptorPoolInlineUniformBlockCreateInfoEXT)                                        \
                                                                                                       \
  /* VK_EXT_line_rasterization */                                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_FEATURES_EXT,                      \
               VkPhysicalDeviceLineRasterizationFeaturesEXT)                                           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_LINE_STATE_CREATE_INFO_EXT,                    \
               VkPipelineRasterizationLineStateCreateInfoEXT)                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_PROPERTIES_EXT,                    \
               VkPhysicalDeviceLineRasterizationPropertiesEXT)                                         \
                                                                                                       \
  /* VK_EXT_memory_budget */                                                                           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT,                         \
               VkPhysicalDeviceMemoryBudgetPropertiesEXT)                                              \
                                                                                                       \
  /* VK_EXT_memory_priority */                                                                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PRIORITY_FEATURES_EXT,                         \
               VkPhysicalDeviceMemoryPriorityFeaturesEXT)                                              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT, VkMemoryPriorityAllocateInfoEXT)   \
                                                                                                       \
  /* VK_EXT_pci_bus_info */                                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PCI_BUS_INFO_PROPERTIES_EXT,                          \
               VkPhysicalDevicePCIBusInfoPropertiesEXT)                                                \
                                                                                                       \
  /* VK_EXT_pipeline_creation_cache_control */                                                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_CREATION_CACHE_CONTROL_FEATURES_EXT,         \
               VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT)                                \
                                                                                                       \
  /* VK_EXT_pipeline_creation_feedback */                                                              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT,                           \
               VkPipelineCreationFeedbackCreateInfoEXT)                                                \
                                                                                                       \
  /* VK_EXT_private_data */                                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIVATE_DATA_FEATURES_EXT,                            \
               VkPhysicalDevicePrivateDataFeaturesEXT)                                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_PRIVATE_DATA_CREATE_INFO_EXT,                                  \
               VkDevicePrivateDataCreateInfoEXT)                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PRIVATE_DATA_SLOT_CREATE_INFO_EXT, VkPrivateDataSlotCreateInfoEXT)    \
                                                                                                       \
  /* VK_EXT_robustness2 */                                                                             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT,                            \
               VkPhysicalDeviceRobustness2FeaturesEXT)                                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT,                          \
               VkPhysicalDeviceRobustness2PropertiesEXT)                                               \
                                                                                                       \
  /* VK_EXT_sampler_filter_minmax */                                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_FILTER_MINMAX_PROPERTIES,                     \
               VkPhysicalDeviceSamplerFilterMinmaxProperties)                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO,                                   \
               VkSamplerReductionModeCreateInfo)                                                       \
                                                                                                       \
  /* VK_EXT_sample_locations */                                                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SAMPLE_LOCATIONS_INFO_EXT, VkSampleLocationsInfoEXT)                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_RENDER_PASS_SAMPLE_LOCATIONS_BEGIN_INFO_EXT,                          \
               VkRenderPassSampleLocationsBeginInfoEXT)                                                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_SAMPLE_LOCATIONS_STATE_CREATE_INFO_EXT,                      \
               VkPipelineSampleLocationsStateCreateInfoEXT)                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLE_LOCATIONS_PROPERTIES_EXT,                      \
               VkPhysicalDeviceSampleLocationsPropertiesEXT)                                           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MULTISAMPLE_PROPERTIES_EXT, VkMultisamplePropertiesEXT)               \
                                                                                                       \
  /* VK_EXT_scalar_block_layout */                                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES,                         \
               VkPhysicalDeviceScalarBlockLayoutFeatures)                                              \
                                                                                                       \
  /* VK_EXT_shader_atomic_float */                                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_FEATURES_EXT,                     \
               VkPhysicalDeviceShaderAtomicFloatFeaturesEXT)                                           \
                                                                                                       \
  /* VK_EXT_shader_demote_to_helper_invocation */                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES_EXT,      \
               VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT)                              \
                                                                                                       \
  /* VK_EXT_shader_image_atomic_int64 */                                                               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_ATOMIC_INT64_FEATURES_EXT,               \
               VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT)                                      \
                                                                                                       \
  /* VK_EXT_separate_stencil_usage */                                                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_STENCIL_USAGE_CREATE_INFO, VkImageStencilUsageCreateInfo)       \
                                                                                                       \
  /* VK_EXT_subgroup_size_control */                                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES_EXT,                   \
               VkPhysicalDeviceSubgroupSizeControlFeaturesEXT)                                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT,                 \
               VkPhysicalDeviceSubgroupSizeControlPropertiesEXT)                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_REQUIRED_SUBGROUP_SIZE_CREATE_INFO_EXT,         \
               VkPipelineShaderStageRequiredSubgroupSizeCreateInfoEXT)                                 \
                                                                                                       \
  /* VK_EXT_texel_buffer_alignment */                                                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT,                  \
               VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT)                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES_EXT,                \
               VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT)                                      \
                                                                                                       \
  /* VK_EXT_tooling_info */                                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TOOL_PROPERTIES_EXT,                                  \
               VkPhysicalDeviceToolPropertiesEXT)                                                      \
                                                                                                       \
  /* VK_EXT_transform_feedback */                                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT,                      \
               VkPhysicalDeviceTransformFeedbackFeaturesEXT)                                           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT,                    \
               VkPhysicalDeviceTransformFeedbackPropertiesEXT)                                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_STREAM_CREATE_INFO_EXT,                  \
               VkPipelineRasterizationStateStreamCreateInfoEXT)                                        \
                                                                                                       \
  /* VK_EXT_validation_cache */                                                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_VALIDATION_CACHE_CREATE_INFO_EXT, VkValidationCacheCreateInfoEXT)     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SHADER_MODULE_VALIDATION_CACHE_CREATE_INFO_EXT,                       \
               VkShaderModuleValidationCacheCreateInfoEXT)                                             \
                                                                                                       \
  /* VK_EXT_validation_features */                                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT, VkValidationFeaturesEXT)                     \
                                                                                                       \
  /* VK_EXT_validation_flags */                                                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT, VkValidationFlagsEXT)                           \
                                                                                                       \
  /* VK_EXT_vertex_attribute_divisor */                                                                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT,              \
               VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT)                                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT,                  \
               VkPipelineVertexInputDivisorStateCreateInfoEXT)                                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT,                \
               VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT)                                      \
                                                                                                       \
  /* VK_EXT_ycbcr_image_arrays */                                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_IMAGE_ARRAYS_FEATURES_EXT,                      \
               VkPhysicalDeviceYcbcrImageArraysFeaturesEXT)                                            \
                                                                                                       \
  /* VK_GOOGLE_display_timing */                                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PRESENT_TIMES_INFO_GOOGLE, VkPresentTimesInfoGOOGLE)                  \
                                                                                                       \
  /* VK_KHR_8bit_storage */                                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES,                                \
               VkPhysicalDevice8BitStorageFeatures)                                                    \
                                                                                                       \
  /* VK_KHR_16bit_storage */                                                                           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES,                               \
               VkPhysicalDevice16BitStorageFeatures)                                                   \
                                                                                                       \
  /* VK_KHR_bind_memory2 */                                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO, VkBindBufferMemoryInfo)                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO, VkBindImageMemoryInfo)                        \
                                                                                                       \
  /* VK_KHR_buffer_device_address */                                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES,                       \
               VkPhysicalDeviceBufferDeviceAddressFeatures)                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO, VkBufferDeviceAddressInfo)                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BUFFER_OPAQUE_CAPTURE_ADDRESS_CREATE_INFO,                            \
               VkBufferOpaqueCaptureAddressCreateInfo)                                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_OPAQUE_CAPTURE_ADDRESS_ALLOCATE_INFO,                          \
               VkMemoryOpaqueCaptureAddressAllocateInfo)                                               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_MEMORY_OPAQUE_CAPTURE_ADDRESS_INFO,                            \
               VkDeviceMemoryOpaqueCaptureAddressInfo)                                                 \
                                                                                                       \
  /* VK_KHR_copy_commands2 */                                                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2_KHR, VkBlitImageInfo2KHR)                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_COPY_IMAGE_INFO_2_KHR, VkBufferCopy2KHR)                              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2_KHR, VkBufferImageCopy2KHR)               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_COPY_IMAGE_TO_BUFFER_INFO_2_KHR, VkCopyBufferInfo2KHR)                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2_KHR, VkCopyBufferToImageInfo2KHR)                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_RESOLVE_IMAGE_INFO_2_KHR, VkCopyImageInfo2KHR)                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BUFFER_COPY_2_KHR, VkCopyImageToBufferInfo2KHR)                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_COPY_2_KHR, VkImageBlit2KHR)                                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_BLIT_2_KHR, VkImageCopy2KHR)                                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2_KHR, VkImageResolve2KHR)                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_RESOLVE_2_KHR, VkResolveImageInfo2KHR)                          \
                                                                                                       \
  /* VK_KHR_create_renderpass2 */                                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2, VkAttachmentDescription2)                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2, VkAttachmentReference2)                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2, VkSubpassDescription2)                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2, VkSubpassDependency2)                           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2, VkRenderPassCreateInfo2)                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO, VkSubpassBeginInfo)                               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SUBPASS_END_INFO, VkSubpassEndInfo)                                   \
                                                                                                       \
  /* VK_KHR_dedicated_allocation */                                                                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS, VkMemoryDedicatedRequirements)         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO, VkMemoryDedicatedAllocateInfo)        \
                                                                                                       \
  /* VK_KHR_depth_stencil_resolve */                                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES,                     \
               VkPhysicalDeviceDepthStencilResolveProperties)                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_DEPTH_STENCIL_RESOLVE,                            \
               VkSubpassDescriptionDepthStencilResolve)                                                \
                                                                                                       \
  /* VK_KHR_descriptor_update_template */                                                              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_CREATE_INFO,                               \
               VkDescriptorUpdateTemplateCreateInfo)                                                   \
                                                                                                       \
  /* VK_KHR_device_group_creation */                                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GROUP_PROPERTIES, VkPhysicalDeviceGroupProperties)    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO, VkDeviceGroupDeviceCreateInfo)       \
                                                                                                       \
  /* VK_KHR_device_group */                                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_SWAPCHAIN_CREATE_INFO_KHR, VkImageSwapchainCreateInfoKHR)       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_DEVICE_GROUP_INFO,                                  \
               VkBindImageMemoryDeviceGroupInfo)                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_DEVICE_GROUP_INFO,                                 \
               VkBindBufferMemoryDeviceGroupInfo)                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_GROUP_BIND_SPARSE_INFO, VkDeviceGroupBindSparseInfo)           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_GROUP_SUBMIT_INFO, VkDeviceGroupSubmitInfo)                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_GROUP_COMMAND_BUFFER_BEGIN_INFO,                               \
               VkDeviceGroupCommandBufferBeginInfo)                                                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO,                                  \
               VkDeviceGroupRenderPassBeginInfo)                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO, VkMemoryAllocateFlagsInfo)                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_GROUP_SWAPCHAIN_CREATE_INFO_KHR,                               \
               VkDeviceGroupSwapchainCreateInfoKHR)                                                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_SWAPCHAIN_INFO_KHR,                                 \
               VkBindImageMemorySwapchainInfoKHR)                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_CAPABILITIES_KHR,                                \
               VkDeviceGroupPresentCapabilitiesKHR)                                                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_INFO_KHR, VkDeviceGroupPresentInfoKHR)           \
                                                                                                       \
  /* VK_KHR_display_swapchain */                                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR, VkDisplayPresentInfoKHR)                    \
                                                                                                       \
  /* VK_KHR_driver_properties */                                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES, VkPhysicalDeviceDriverProperties)  \
                                                                                                       \
  /* VK_KHR_external_fence_capabilities */                                                             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_FENCE_INFO,                                  \
               VkPhysicalDeviceExternalFenceInfo)                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXTERNAL_FENCE_PROPERTIES, VkExternalFenceProperties)                 \
                                                                                                       \
  /* VK_KHR_external_fence / ..._fd */                                                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXPORT_FENCE_CREATE_INFO, VkExportFenceCreateInfo)                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMPORT_FENCE_FD_INFO_KHR, VkImportFenceFdInfoKHR)                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_FENCE_GET_FD_INFO_KHR, VkFenceGetFdInfoKHR)                           \
                                                                                                       \
  /* VK_KHR_external_memory_capabilities */                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_IMAGE_FORMAT_INFO,                           \
               VkPhysicalDeviceExternalImageFormatInfo)                                                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXTERNAL_IMAGE_FORMAT_PROPERTIES, VkExternalImageFormatProperties)    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_BUFFER_INFO,                                 \
               VkPhysicalDeviceExternalBufferInfo)                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXTERNAL_BUFFER_PROPERTIES, VkExternalBufferProperties)               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES, VkPhysicalDeviceIDProperties)          \
                                                                                                       \
  /* VK_KHR_external_memory / ..._fd */                                                                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO, VkExportMemoryAllocateInfo)              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO, VkExternalMemoryImageCreateInfo)   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO,                                   \
               VkExternalMemoryBufferCreateInfo)                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMPORT_MEMORY_FD_INFO_KHR, VkImportMemoryFdInfoKHR)                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_FD_PROPERTIES_KHR, VkMemoryFdPropertiesKHR)                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_GET_FD_INFO_KHR, VkMemoryGetFdInfoKHR)                         \
                                                                                                       \
  /* VK_KHR_external_semaphore_capabilities */                                                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SEMAPHORE_INFO,                              \
               VkPhysicalDeviceExternalSemaphoreInfo)                                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXTERNAL_SEMAPHORE_PROPERTIES, VkExternalSemaphoreProperties)         \
                                                                                                       \
  /* VK_KHR_external_semaphore / ..._fd */                                                             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO, VkExportSemaphoreCreateInfo)            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_FD_INFO_KHR, VkImportSemaphoreFdInfoKHR)             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SEMAPHORE_GET_FD_INFO_KHR, VkSemaphoreGetFdInfoKHR)                   \
                                                                                                       \
  /* VK_KHR_get_display_properties2 */                                                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DISPLAY_PROPERTIES_2_KHR, VkDisplayProperties2KHR)                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DISPLAY_PLANE_PROPERTIES_2_KHR, VkDisplayPlaneProperties2KHR)         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DISPLAY_MODE_PROPERTIES_2_KHR, VkDisplayModeProperties2KHR)           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DISPLAY_PLANE_INFO_2_KHR, VkDisplayPlaneInfo2KHR)                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DISPLAY_PLANE_CAPABILITIES_2_KHR, VkDisplayPlaneCapabilities2KHR)     \
                                                                                                       \
  /* VK_KHR_get_memory_requirements2 */                                                                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2, VkBufferMemoryRequirementsInfo2)   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2, VkImageMemoryRequirementsInfo2)     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_SPARSE_MEMORY_REQUIREMENTS_INFO_2,                              \
               VkImageSparseMemoryRequirementsInfo2)                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2, VkMemoryRequirements2)                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SPARSE_IMAGE_MEMORY_REQUIREMENTS_2,                                   \
               VkSparseImageMemoryRequirements2)                                                       \
                                                                                                       \
  /* VK_KHR_get_physical_device_properties2 */                                                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, VkPhysicalDeviceFeatures2)                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, VkPhysicalDeviceProperties2)            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2, VkFormatProperties2)                             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_FORMAT_PROPERTIES_2, VkImageFormatProperties2)                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_FORMAT_INFO_2,                                  \
               VkPhysicalDeviceImageFormatInfo2)                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2, VkQueueFamilyProperties2)                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2,                                  \
               VkPhysicalDeviceMemoryProperties2)                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SPARSE_IMAGE_FORMAT_PROPERTIES_2, VkSparseImageFormatProperties2)     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SPARSE_IMAGE_FORMAT_INFO_2,                           \
               VkPhysicalDeviceSparseImageFormatInfo2)                                                 \
                                                                                                       \
  /* VK_KHR_get_surface_capabilities2 */                                                               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR, VkPhysicalDeviceSurfaceInfo2KHR)  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR, VkSurfaceCapabilities2KHR)                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR, VkSurfaceFormat2KHR)                            \
                                                                                                       \
  /* VK_KHR_image_format_list */                                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_FORMAT_LIST_CREATE_INFO, VkImageFormatListCreateInfo)           \
                                                                                                       \
  /* VK_KHR_imageless_framebuffer */                                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGELESS_FRAMEBUFFER_FEATURES,                       \
               VkPhysicalDeviceImagelessFramebufferFeatures)                                           \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO,                                  \
               VkFramebufferAttachmentsCreateInfo)                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO, VkFramebufferAttachmentImageInfo)  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO, VkRenderPassAttachmentBeginInfo)   \
                                                                                                       \
  /* VK_KHR_incremental_present */                                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PRESENT_REGIONS_KHR, VkPresentRegionsKHR)                             \
                                                                                                       \
  /* VK_KHR_maintenance2 */                                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_POINT_CLIPPING_PROPERTIES,                            \
               VkPhysicalDevicePointClippingProperties)                                                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_RENDER_PASS_INPUT_ATTACHMENT_ASPECT_CREATE_INFO,                      \
               VkRenderPassInputAttachmentAspectCreateInfo)                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO, VkImageViewUsageCreateInfo)             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_DOMAIN_ORIGIN_STATE_CREATE_INFO,                \
               VkPipelineTessellationDomainOriginStateCreateInfo)                                      \
                                                                                                       \
  /* VK_KHR_maintenance3 */                                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES,                             \
               VkPhysicalDeviceMaintenance3Properties)                                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_SUPPORT, VkDescriptorSetLayoutSupport)          \
                                                                                                       \
  /* VK_KHR_multiview */                                                                               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO, VkRenderPassMultiviewCreateInfo)   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES,                                   \
               VkPhysicalDeviceMultiviewFeatures)                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES,                                 \
               VkPhysicalDeviceMultiviewProperties)                                                    \
                                                                                                       \
  /* VK_KHR_performance_query */                                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_FEATURES_KHR,                       \
               VkPhysicalDevicePerformanceQueryFeaturesKHR)                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_PROPERTIES_KHR,                     \
               VkPhysicalDevicePerformanceQueryPropertiesKHR)                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_QUERY_POOL_PERFORMANCE_CREATE_INFO_KHR,                               \
               VkQueryPoolPerformanceCreateInfoKHR)                                                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PERFORMANCE_QUERY_SUBMIT_INFO_KHR, VkPerformanceQuerySubmitInfoKHR)   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_ACQUIRE_PROFILING_LOCK_INFO_KHR, VkAcquireProfilingLockInfoKHR)       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PERFORMANCE_COUNTER_KHR, VkPerformanceCounterKHR)                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PERFORMANCE_COUNTER_DESCRIPTION_KHR,                                  \
               VkPerformanceCounterDescriptionKHR)                                                     \
                                                                                                       \
  /* VK_KHR_pipeline_executable_properties */                                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_EXECUTABLE_PROPERTIES_FEATURES_KHR,          \
               VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR)                                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_INFO_KHR, VkPipelineInfoKHR)                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_PROPERTIES_KHR,                                   \
               VkPipelineExecutablePropertiesKHR)                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INFO_KHR, VkPipelineExecutableInfoKHR)            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_STATISTIC_KHR, VkPipelineExecutableStatisticKHR)  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INTERNAL_REPRESENTATION_KHR,                      \
               VkPipelineExecutableInternalRepresentationKHR)                                          \
                                                                                                       \
  /* VK_KHR_push_descriptor */                                                                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR,                       \
               VkPhysicalDevicePushDescriptorPropertiesKHR)                                            \
                                                                                                       \
  /* VK_KHR_sampler_ycbcr_conversion */                                                                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_CREATE_INFO,                                 \
               VkSamplerYcbcrConversionCreateInfo)                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_INFO, VkSamplerYcbcrConversionInfo)          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BIND_IMAGE_PLANE_MEMORY_INFO, VkBindImagePlaneMemoryInfo)             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_PLANE_MEMORY_REQUIREMENTS_INFO,                                 \
               VkImagePlaneMemoryRequirementsInfo)                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES,                    \
               VkPhysicalDeviceSamplerYcbcrConversionFeatures)                                         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_IMAGE_FORMAT_PROPERTIES,                     \
               VkSamplerYcbcrConversionImageFormatProperties)                                          \
                                                                                                       \
  /* VK_KHR_separate_depth_stencil_layouts */                                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES,              \
               VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures)                                    \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_STENCIL_LAYOUT,                                  \
               VkAttachmentReferenceStencilLayout)                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_STENCIL_LAYOUT,                                \
               VkAttachmentDescriptionStencilLayout)                                                   \
                                                                                                       \
  /* VK_KHR_shader_atomic_int64 */                                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES,                         \
               VkPhysicalDeviceShaderAtomicInt64Features)                                              \
                                                                                                       \
  /* VK_KHR_shader_clock */                                                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR,                            \
               VkPhysicalDeviceShaderClockFeaturesKHR)                                                 \
                                                                                                       \
  /* VK_KHR_shader_float16_int8 */                                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES,                         \
               VkPhysicalDeviceShaderFloat16Int8Features)                                              \
                                                                                                       \
  /* VK_KHR_shader_float_controls */                                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES,                            \
               VkPhysicalDeviceFloatControlsProperties)                                                \
                                                                                                       \
  /* VK_KHR_shader_subgroup_extended_types */                                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_EXTENDED_TYPES_FEATURES,              \
               VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures)                                    \
                                                                                                       \
  /* VK_KHR_shader_terminate_invocation */                                                             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TERMINATE_INVOCATION_FEATURES_KHR,             \
               VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR)                                   \
                                                                                                       \
  /* VK_KHR_shared_presentable_image */                                                                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SHARED_PRESENT_SURFACE_CAPABILITIES_KHR,                              \
               VkSharedPresentSurfaceCapabilitiesKHR)                                                  \
                                                                                                       \
  /* VK_KHR_surface_protected_capabilities */                                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SURFACE_PROTECTED_CAPABILITIES_KHR,                                   \
               VkSurfaceProtectedCapabilitiesKHR)                                                      \
                                                                                                       \
  /* VK_KHR_swapchain */                                                                               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, VkSwapchainCreateInfoKHR)                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PRESENT_INFO_KHR, VkPresentInfoKHR)                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR, VkAcquireNextImageInfoKHR)               \
                                                                                                       \
  /* VK_KHR_synchronization2 */                                                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_MEMORY_BARRIER_2_KHR, VkMemoryBarrier2KHR)                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2_KHR, VkBufferMemoryBarrier2KHR)               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR, VkImageMemoryBarrier2KHR)                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR, VkDependencyInfoKHR)                             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR, VkSubmitInfo2KHR)                                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR, VkSemaphoreSubmitInfoKHR)                  \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR, VkCommandBufferSubmitInfoKHR)         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR,                       \
               VkPhysicalDeviceSynchronization2FeaturesKHR)                                            \
                                                                                                       \
  /* VK_KHR_timeline_semaphore */                                                                      \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES,                          \
               VkPhysicalDeviceTimelineSemaphoreFeatures)                                              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES,                        \
               VkPhysicalDeviceTimelineSemaphoreProperties)                                            \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO, VkSemaphoreTypeCreateInfo)                \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO, VkTimelineSemaphoreSubmitInfo)        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO, VkSemaphoreWaitInfo)                             \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO, VkSemaphoreSignalInfo)                         \
                                                                                                       \
  /* VK_KHR_uniform_buffer_standard_layout */                                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES,              \
               VkPhysicalDeviceUniformBufferStandardLayoutFeatures)                                    \
                                                                                                       \
  /* VK_KHR_workgroup_memory_explicit_layout */                                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_FEATURES_KHR,        \
               VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR)                               \
                                                                                                       \
  /* VK_KHR_variable_pointers */                                                                       \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES,                           \
               VkPhysicalDeviceVariablePointerFeatures)                                                \
                                                                                                       \
  /* VK_KHR_vulkan_memory_model */                                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES,                         \
               VkPhysicalDeviceVulkanMemoryModelFeatures)                                              \
                                                                                                       \
  /* VK_KHR_zero_initialize_workgroup_memory */                                                        \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ZERO_INITIALIZE_WORKGROUP_MEMORY_FEATURES_KHR,        \
               VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR)                               \
                                                                                                       \
  /* VK_NV_compute_shader_derivatives */                                                               \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_FEATURES_NV,               \
               VkPhysicalDeviceComputeShaderDerivativesFeaturesNV)                                     \
                                                                                                       \
  /* VK_NV_dedicated_allocation */                                                                     \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_MEMORY_ALLOCATE_INFO_NV,                         \
               VkDedicatedAllocationMemoryAllocateInfoNV)                                              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_IMAGE_CREATE_INFO_NV,                            \
               VkDedicatedAllocationImageCreateInfoNV)                                                 \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_BUFFER_CREATE_INFO_NV,                           \
               VkDedicatedAllocationBufferCreateInfoNV)                                                \
                                                                                                       \
  /* VK_NV_fragment_shader_barycentric */                                                              \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_FEATURES_NV,              \
               VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV)                                    \
                                                                                                       \
  /* VK_NV_external_memory */                                                                          \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO_NV, VkExportMemoryAllocateInfoNV)         \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO_NV,                                 \
               VkExternalMemoryImageCreateInfoNV)                                                      \
                                                                                                       \
  /* VK_NV_shader_image_footprint */                                                                   \
  PNEXT_STRUCT(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_FOOTPRINT_FEATURES_NV,                   \
               VkPhysicalDeviceShaderImageFootprintFeaturesNV)                                         \
                                                                                                       \
  /* Surface creation structs. These would pull in dependencies on OS-specific includes. */            \
  /* So treat them as unsupported. */                                                                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR)                                 \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_DIRECTFB_SURFACE_CREATE_INFO_EXT)                                \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_DISPLAY_MODE_CREATE_INFO_KHR)                                    \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR)                                 \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_IMAGEPIPE_SURFACE_CREATE_INFO_FUCHSIA)                           \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK)                                     \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK)                                   \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT)                                   \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_STREAM_DESCRIPTOR_SURFACE_CREATE_INFO_GGP)                       \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_VI_SURFACE_CREATE_INFO_NN)                                       \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR)                                 \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR)                                   \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR)                                     \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR)                                    \
                                                                                                       \
  /* VK_AMD_memory_overallocation_behavior */                                                          \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_DEVICE_MEMORY_OVERALLOCATION_CREATE_INFO_AMD)                    \
                                                                                                       \
  /* VK_AMD_pipeline_compiler_control */                                                               \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_COMPILER_CONTROL_CREATE_INFO_AMD)                       \
                                                                                                       \
  /* VK_AMD_rasterization_order */                                                                     \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_RASTERIZATION_ORDER_AMD)            \
                                                                                                       \
  /* VK_AMD_shader_core_properties2 */                                                                 \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_2_AMD)                    \
                                                                                                       \
  /* VK_EXT_4444_formats */                                                                            \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_4444_FORMATS_FEATURES_EXT)                       \
                                                                                                       \
  /* VK_EXT_blend_operation_advanced */                                                                \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_FEATURES_EXT)           \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_PROPERTIES_EXT)         \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_ADVANCED_STATE_CREATE_INFO_EXT)             \
                                                                                                       \
  /* VK_EXT_device_memory_report */                                                                    \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_MEMORY_REPORT_FEATURES_EXT)               \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_DEVICE_DEVICE_MEMORY_REPORT_CREATE_INFO_EXT)                     \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_DEVICE_MEMORY_REPORT_CALLBACK_DATA_EXT)                          \
                                                                                                       \
  /* VK_EXT_external_memory_host */                                                                    \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_IMPORT_MEMORY_HOST_POINTER_INFO_EXT)                             \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_MEMORY_HOST_POINTER_PROPERTIES_EXT)                              \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_HOST_PROPERTIES_EXT)             \
                                                                                                       \
  /* VK_EXT_fragment_density_map_2 */                                                                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_2_FEATURES_EXT)             \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_2_PROPERTIES_EXT)           \
                                                                                                       \
  /* VK_EXT_headless_surface */                                                                        \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_HEADLESS_SURFACE_CREATE_INFO_EXT)                                \
                                                                                                       \
  /* VK_EXT_image_drm_format_modifier */                                                               \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_DRM_FORMAT_MODIFIER_PROPERTIES_LIST_EXT)                         \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_DRM_FORMAT_MODIFIER_INFO_EXT)              \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_IMAGE_DRM_FORMAT_MODIFIER_LIST_CREATE_INFO_EXT)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_IMAGE_DRM_FORMAT_MODIFIER_EXPLICIT_CREATE_INFO_EXT)              \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_IMAGE_DRM_FORMAT_MODIFIER_PROPERTIES_EXT)                        \
                                                                                                       \
  /* VK_EXT_texture_compression_astc_hdr */                                                            \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXTURE_COMPRESSION_ASTC_HDR_FEATURES_EXT)       \
                                                                                                       \
  /* VK_INTEL_performance_query */                                                                     \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_QUERY_POOL_PERFORMANCE_QUERY_CREATE_INFO_INTEL)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_INITIALIZE_PERFORMANCE_API_INFO_INTEL)                           \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PERFORMANCE_MARKER_INFO_INTEL)                                   \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PERFORMANCE_STREAM_MARKER_INFO_INTEL)                            \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PERFORMANCE_OVERRIDE_INFO_INTEL)                                 \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PERFORMANCE_CONFIGURATION_ACQUIRE_INFO_INTEL)                    \
                                                                                                       \
  /* VK_INTEL_shader_integer_functions2 */                                                             \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_FUNCTIONS_2_FEATURES_INTEL)       \
                                                                                                       \
  /* VK_KHR_acceleration_structure */                                                                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR)                     \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR)                          \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR)              \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR)                             \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR)              \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_VERSION_INFO_KHR)                         \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR)                            \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_INFO_KHR)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_INFO_KHR)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR)             \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR)           \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR)                 \
                                                                                                       \
  /* VK_KHR_fragment_shading_rate */                                                                   \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_FRAGMENT_SHADING_RATE_ATTACHMENT_INFO_KHR)                       \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_FRAGMENT_SHADING_RATE_STATE_CREATE_INFO_KHR)            \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_PROPERTIES_KHR)            \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR)              \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_KHR)                       \
                                                                                                       \
  /* VK_KHR_pipeline_library */                                                                        \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR)                                \
                                                                                                       \
  /* VK_KHR_portability_subset */                                                                      \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_KHR)                 \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_PROPERTIES_KHR)               \
                                                                                                       \
  /* VK_KHR_ray_tracing_pipeline */                                                                    \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR)                            \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_INTERFACE_CREATE_INFO_KHR)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR)                        \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR)               \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR)             \
                                                                                                       \
  /* VK_KHR_ray_query */                                                                               \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR)                          \
                                                                                                       \
  /* VK_NV_clip_space_w_scaling */                                                                     \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_W_SCALING_STATE_CREATE_INFO_NV)                \
                                                                                                       \
  /* VK_NV_cooperative_matrix */                                                                       \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_FEATURES_NV)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_COOPERATIVE_MATRIX_PROPERTIES_NV)                                \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_PROPERTIES_NV)                \
                                                                                                       \
  /* VK_NV_corner_sampled_image */                                                                     \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CORNER_SAMPLED_IMAGE_FEATURES_NV)                \
                                                                                                       \
  /* VK_NV_coverage_reduction_mode */                                                                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COVERAGE_REDUCTION_MODE_FEATURES_NV)             \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_COVERAGE_REDUCTION_STATE_CREATE_INFO_NV)                \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_FRAMEBUFFER_MIXED_SAMPLES_COMBINATION_NV)                        \
                                                                                                       \
  /* VK_NV_dedicated_allocation_image_aliasing */                                                      \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEDICATED_ALLOCATION_IMAGE_ALIASING_FEATURES_NV) \
                                                                                                       \
  /* VK_NV_device_diagnostic_checkpoints */                                                            \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_CHECKPOINT_DATA_NV)                                              \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_QUEUE_FAMILY_CHECKPOINT_PROPERTIES_NV)                           \
  /* VK_KHR_synchronization2 interactions */                                                           \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_QUEUE_FAMILY_CHECKPOINT_PROPERTIES_2_NV)                         \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_CHECKPOINT_DATA_2_NV)                                            \
                                                                                                       \
  /* VK_NV_device_diagnostics_config */                                                                \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DIAGNOSTICS_CONFIG_FEATURES_NV)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_DEVICE_DIAGNOSTICS_CONFIG_CREATE_INFO_NV)                        \
                                                                                                       \
  /* VK_NV_fragment_coverage_to_color */                                                               \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_COVERAGE_TO_COLOR_STATE_CREATE_INFO_NV)                 \
                                                                                                       \
  /* VK_NV_framebuffer_mixed_samples */                                                                \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_COVERAGE_MODULATION_STATE_CREATE_INFO_NV)               \
                                                                                                       \
  /* VK_NVX_image_view_handle */                                                                       \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_IMAGE_VIEW_HANDLE_INFO_NVX)                                      \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_IMAGE_VIEW_ADDRESS_PROPERTIES_NVX)                               \
                                                                                                       \
  /* VK_NV_mesh_shader */                                                                              \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV)                         \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_NV)                       \
                                                                                                       \
  /* VK_NV_ray_tracing */                                                                              \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NV)                           \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_INFO_NV)                                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_NV)              \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_BIND_ACCELERATION_STRUCTURE_MEMORY_INFO_NV)                      \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_GEOMETRY_AABB_NV)                                                \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_GEOMETRY_NV)                                                     \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_GEOMETRY_TRIANGLES_NV)                                           \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV)                       \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_NV)                             \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV)                         \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV)                  \
                                                                                                       \
  /* VK_NV_representative_fragment_test */                                                             \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_REPRESENTATIVE_FRAGMENT_TEST_FEATURES_NV)        \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_REPRESENTATIVE_FRAGMENT_TEST_STATE_CREATE_INFO_NV)      \
                                                                                                       \
  /* VK_NV_scissor_exclusive */                                                                        \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_EXCLUSIVE_SCISSOR_STATE_CREATE_INFO_NV)        \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXCLUSIVE_SCISSOR_FEATURES_NV)                   \
                                                                                                       \
  /* VK_NV_shader_sm_builtins  */                                                                      \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SM_BUILTINS_FEATURES_NV)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SM_BUILTINS_PROPERTIES_NV)                \
                                                                                                       \
  /* VK_NV_fragment_shading_rate_enums */                                                              \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_ENUMS_FEATURES_NV)         \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_ENUMS_PROPERTIES_NV)       \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_FRAGMENT_SHADING_RATE_ENUM_STATE_CREATE_INFO_NV)        \
                                                                                                       \
  /* VK_NV_shading_rate_image */                                                                       \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_SHADING_RATE_IMAGE_STATE_CREATE_INFO_NV)       \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_FEATURES_NV)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_PROPERTIES_NV)                \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_COARSE_SAMPLE_ORDER_STATE_CREATE_INFO_NV)      \
                                                                                                       \
  /* VK_NV_viewport_swizzle */                                                                         \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_SWIZZLE_STATE_CREATE_INFO_NV)                  \
                                                                                                       \
  /* VK_NVX_device_generated_commands */                                                               \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_PROPERTIES_NV)         \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_GRAPHICS_SHADER_GROUP_CREATE_INFO_NV)                            \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_SHADER_GROUPS_CREATE_INFO_NV)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV)                               \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_CREATE_INFO_NV)                         \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_GENERATED_COMMANDS_INFO_NV)                                      \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_GENERATED_COMMANDS_MEMORY_REQUIREMENTS_INFO_NV)                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_FEATURES_NV)           \
                                                                                                       \
  /* VK_NVX_multiview_per_view_attributes */                                                           \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PER_VIEW_ATTRIBUTES_PROPERTIES_NVX)    \
                                                                                                       \
  /* VK_QCOM_render_pass_transform */                                                                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_RENDER_PASS_TRANSFORM_INFO_QCOM)      \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_RENDER_PASS_TRANSFORM_BEGIN_INFO_QCOM)                           \
                                                                                                       \
  /* VK_QCOM_rotated_copy_commands */                                                                  \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_COPY_COMMAND_TRANSFORM_INFO_QCOM)                                \
                                                                                                       \
  /* VK_VALVE_mutable_descriptor_type */                                                               \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_VALVE)          \
  PNEXT_UNSUPPORTED(VK_STRUCTURE_TYPE_MUTABLE_DESCRIPTOR_TYPE_CREATE_INFO_VALVE)

template <typename SerialiserType>
static void SerialiseNext(SerialiserType &ser, VkStructureType &sType, const void *&pNext)
{
  // this is the parent sType, serialised here for convenience
  ser.Serialise("sType"_lit, sType).Unimportant();

  if(ser.IsReading() && !ser.IsStructurising())
  {
    // default to a NULL pNext
    pNext = NULL;

    // serialise a nullable next type, to tell us the type of object that's serialised. This is
    // hidden as it doesn't correspond to the actual pNext, it's just metadata to help us launch the
    // serialisation.
    VkStructureType *nextType = NULL;
    ser.SerialiseNullable("pNext"_lit, nextType);

    // most common case - no pNext serialised. Bail immediately
    if(nextType == NULL)
    {
      // fixup the structured data, set the type to void*
      ser.TypedAs("void *"_lit);
      return;
    }

    // hide and rename the pNextType if we got something - we'll want the pNext below to be the only
    // thing user-facing.
    ser.Named("pNextType"_lit).Hidden();

// if we encounter an unsupported struct on read we *cannot* continue since we don't know the
// members that got serialised after it
#define PNEXT_UNSUPPORTED(StructType)                                    \
  case StructType:                                                       \
  {                                                                      \
    RDCERR("No support for " #StructType " is available in this build"); \
    pNext = NULL;                                                        \
    ser.SetErrored();                                                    \
    return;                                                              \
  }

// if we come across a struct we should process, then serialise a pointer to it.
#define PNEXT_STRUCT(StructType, StructName)        \
  case StructType:                                  \
  {                                                 \
    StructName *nextStruct = NULL;                  \
    ser.SerialiseNullable("pNext"_lit, nextStruct); \
    pNext = nextStruct;                             \
    handled = true;                                 \
    break;                                          \
  }

    // we don't want a default case to ensure we get a compile error if we forget to implement a
    // structure type, but we also want to error if the input is invalid, so have this flag here.
    bool handled = false;

    // this serialises the pNext with the right type, as nullable. We already know from above that
    // there IS something here, so the nullable is redundant but convenient
    switch(*nextType)
    {
      HANDLE_PNEXT();
      case VK_STRUCTURE_TYPE_MAX_ENUM: break;
    }

    if(!handled)
      RDCERR("Invalid next structure sType: %u", *nextType);

    // delete the type itself. Any pNext we serialised is saved in the pNext pointer and will be
    // deleted in DeserialiseNext()
    delete nextType;

    // note, we don't have to serialise more of the chain - this is recursive, if there was more of
    // the pNext chain it would be done recursively above
  }
  else    // ser.IsWriting()
  {
// if we hit an unsupported struct, error and then just skip it entirely from the chain and move
// onto the next one in the list
#undef PNEXT_UNSUPPORTED
#define PNEXT_UNSUPPORTED(StructType)                                    \
  case StructType:                                                       \
  {                                                                      \
    RDCERR("No support for " #StructType " is available in this build"); \
    handled = true;                                                      \
    break;                                                               \
  }

// if we come across a struct we should process, then serialise a pointer to its type (to tell the
// reading serialisation what struct is coming up), then a pointer to it.
// We don't have to go any further, the act of serialising this struct will walk the chain further,
// so we can return immediately.
#undef PNEXT_STRUCT
#define PNEXT_STRUCT(StructType, StructName)          \
  case StructType:                                    \
  {                                                   \
    VkStructureType *nextType = &next->sType;         \
    ser.SerialiseNullable("pNextType"_lit, nextType); \
    StructName *actual = (StructName *)next;          \
    ser.SerialiseNullable("pNext"_lit, actual);       \
    handled = true;                                   \
    return;                                           \
  }

    // we don't want a default case to ensure we get a compile error if we forget to implement a
    // structure type, but we also want to error if the input is invalid, so have this flag here.
    bool handled = false;

    // walk the pNext chain, skipping any structs we don't care about serialising.
    VkBaseInStructure *next = (VkBaseInStructure *)pNext;

    while(next)
    {
      switch(next->sType)
      {
        HANDLE_PNEXT();
        case VK_STRUCTURE_TYPE_MAX_ENUM: break;
      }

      if(!handled)
        RDCERR("Invalid pNext structure sType: %u", next->sType);

      // walk to the next item if we didn't serialise the current one
      next = (VkBaseInStructure *)next->pNext;
    }

    // if we got here, either pNext was NULL (common) or we skipped the whole chain. Serialise a
    // NULL structure type to indicate that.
    VkStructureType *dummy = NULL;
    ser.SerialiseNullable("pNext"_lit, dummy);
  }
}

template <typename SerialiserType>
static void SerialiseNext(SerialiserType &ser, VkStructureType &sType, void *&pNext)
{
  const void *tmpNext = pNext;
  SerialiseNext(ser, sType, tmpNext);
  if(ser.IsReading())
    pNext = (void *)tmpNext;
}

static inline void DeserialiseNext(const void *pNext)
{
  if(pNext == NULL)
    return;

#undef PNEXT_UNSUPPORTED
#define PNEXT_UNSUPPORTED(StructType)                                    \
  case StructType:                                                       \
  {                                                                      \
    RDCERR("No support for " #StructType " is available in this build"); \
    return;                                                              \
  }

#undef PNEXT_STRUCT
#define PNEXT_STRUCT(StructType, StructName)            \
  case StructType:                                      \
  {                                                     \
    VkStructureType *nextType = (VkStructureType *)gen; \
    Deserialise(nextType);                              \
    return;                                             \
  }

  // walk the chain, deserialising from the tail back
  const VkBaseInStructure *gen = (const VkBaseInStructure *)pNext;
  switch(gen->sType)
  {
    HANDLE_PNEXT();
    case VK_STRUCTURE_TYPE_MAX_ENUM: DeserialiseNext(gen); break;
  }
  delete gen;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkApplicationInfo &el)
{
  RDCERR("Serialising VkApplicationInfo - this should always be a NULL optional element");
  RDCEraseEl(el);
  el.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkInstanceCreateInfo &el)
{
  RDCERR("Serialising VkInstanceCreateInfo - this should always be a NULL optional element");
  RDCEraseEl(el);
  el.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkLayerInstanceCreateInfo &el)
{
  RDCERR("Serialising VkLayerInstanceCreateInfo - this should always be a NULL optional element");
  RDCEraseEl(el);
  el.sType = VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkLayerDeviceCreateInfo &el)
{
  RDCERR("Serialising VkLayerDeviceCreateInfo - this should always be a NULL optional element");
  RDCEraseEl(el);
  el.sType = VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkAllocationCallbacks &el)
{
  RDCERR("Serialising VkAllocationCallbacks - this should always be a NULL optional element");
  RDCEraseEl(el);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDebugMarkerObjectNameInfoEXT &el)
{
  RDCERR("Serialising VkDebugMarkerObjectNameInfoEXT - this should be handled specially");
  // can't handle it here without duplicating objectType logic
  RDCEraseEl(el);
  el.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDebugMarkerObjectTagInfoEXT &el)
{
  RDCERR("Serialising VkDebugMarkerObjectTagInfoEXT - this should be handled specially");
  // can't handle it here without duplicating objectType logic
  RDCEraseEl(el);
  el.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDebugUtilsObjectNameInfoEXT &el)
{
  RDCERR("Serialising VkDebugUtilsObjectNameInfoEXT - this should be handled specially");
  // can't handle it here without duplicating objectType logic
  RDCEraseEl(el);
  el.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
}

template <>
void Deserialise(const VkDebugUtilsObjectNameInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDebugUtilsObjectTagInfoEXT &el)
{
  RDCERR("Serialising VkDebugUtilsObjectTagInfoEXT - this should be handled specially");
  // can't handle it here without duplicating objectType logic
  RDCEraseEl(el);
  el.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceQueueCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkDeviceQueueCreateFlags, flags);
  SERIALISE_MEMBER(queueFamilyIndex);
  SERIALISE_MEMBER(queueCount);
  SERIALISE_MEMBER_ARRAY(pQueuePriorities, queueCount);
}

template <>
void Deserialise(const VkDeviceQueueCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pQueuePriorities;
}

// technically this doesn't need a serialise function as it's POD,
// but we give it one just for ease of printing etc.
template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceFeatures &el)
{
  SERIALISE_MEMBER(robustBufferAccess);
  SERIALISE_MEMBER(fullDrawIndexUint32);
  SERIALISE_MEMBER(imageCubeArray);
  SERIALISE_MEMBER(independentBlend);
  SERIALISE_MEMBER(geometryShader);
  SERIALISE_MEMBER(tessellationShader);
  SERIALISE_MEMBER(sampleRateShading);
  SERIALISE_MEMBER(dualSrcBlend);
  SERIALISE_MEMBER(logicOp);
  SERIALISE_MEMBER(multiDrawIndirect);
  SERIALISE_MEMBER(drawIndirectFirstInstance);
  SERIALISE_MEMBER(depthClamp);
  SERIALISE_MEMBER(depthBiasClamp);
  SERIALISE_MEMBER(fillModeNonSolid);
  SERIALISE_MEMBER(depthBounds);
  SERIALISE_MEMBER(wideLines);
  SERIALISE_MEMBER(largePoints);
  SERIALISE_MEMBER(alphaToOne);
  SERIALISE_MEMBER(multiViewport);
  SERIALISE_MEMBER(samplerAnisotropy);
  SERIALISE_MEMBER(textureCompressionETC2);
  SERIALISE_MEMBER(textureCompressionASTC_LDR);
  SERIALISE_MEMBER(textureCompressionBC);
  SERIALISE_MEMBER(occlusionQueryPrecise);
  SERIALISE_MEMBER(pipelineStatisticsQuery);
  SERIALISE_MEMBER(vertexPipelineStoresAndAtomics);
  SERIALISE_MEMBER(fragmentStoresAndAtomics);
  SERIALISE_MEMBER(shaderTessellationAndGeometryPointSize);
  SERIALISE_MEMBER(shaderImageGatherExtended);
  SERIALISE_MEMBER(shaderStorageImageExtendedFormats);
  SERIALISE_MEMBER(shaderStorageImageMultisample);
  SERIALISE_MEMBER(shaderStorageImageReadWithoutFormat);
  SERIALISE_MEMBER(shaderStorageImageWriteWithoutFormat);
  SERIALISE_MEMBER(shaderUniformBufferArrayDynamicIndexing);
  SERIALISE_MEMBER(shaderSampledImageArrayDynamicIndexing);
  SERIALISE_MEMBER(shaderStorageBufferArrayDynamicIndexing);
  SERIALISE_MEMBER(shaderStorageImageArrayDynamicIndexing);
  SERIALISE_MEMBER(shaderClipDistance);
  SERIALISE_MEMBER(shaderCullDistance);
  SERIALISE_MEMBER(shaderFloat64);
  SERIALISE_MEMBER(shaderInt64);
  SERIALISE_MEMBER(shaderInt16);
  SERIALISE_MEMBER(shaderResourceResidency);
  SERIALISE_MEMBER(shaderResourceMinLod);
  SERIALISE_MEMBER(sparseBinding);
  SERIALISE_MEMBER(sparseResidencyBuffer);
  SERIALISE_MEMBER(sparseResidencyImage2D);
  SERIALISE_MEMBER(sparseResidencyImage3D);
  SERIALISE_MEMBER(sparseResidency2Samples);
  SERIALISE_MEMBER(sparseResidency4Samples);
  SERIALISE_MEMBER(sparseResidency8Samples);
  SERIALISE_MEMBER(sparseResidency16Samples);
  SERIALISE_MEMBER(sparseResidencyAliased);
  SERIALISE_MEMBER(variableMultisampleRate);
  SERIALISE_MEMBER(inheritedQueries);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryHeap &el)
{
  SERIALISE_MEMBER(size);
  SERIALISE_MEMBER_VKFLAGS(VkMemoryHeapFlags, flags);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryType &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkMemoryPropertyFlags, propertyFlags);
  SERIALISE_MEMBER(heapIndex);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceMemoryProperties &el)
{
  SERIALISE_MEMBER(memoryTypeCount);
  SERIALISE_MEMBER(memoryTypes);
  SERIALISE_MEMBER(memoryHeapCount);
  SERIALISE_MEMBER(memoryHeaps);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceLimits &el)
{
  SERIALISE_MEMBER(maxImageDimension1D);
  SERIALISE_MEMBER(maxImageDimension2D);
  SERIALISE_MEMBER(maxImageDimension3D);
  SERIALISE_MEMBER(maxImageDimensionCube);
  SERIALISE_MEMBER(maxImageArrayLayers);
  SERIALISE_MEMBER(maxTexelBufferElements);
  SERIALISE_MEMBER(maxUniformBufferRange);
  SERIALISE_MEMBER(maxStorageBufferRange);
  SERIALISE_MEMBER(maxPushConstantsSize);
  SERIALISE_MEMBER(maxMemoryAllocationCount);
  SERIALISE_MEMBER(maxSamplerAllocationCount);
  SERIALISE_MEMBER(bufferImageGranularity);
  SERIALISE_MEMBER(sparseAddressSpaceSize);
  SERIALISE_MEMBER(maxBoundDescriptorSets);
  SERIALISE_MEMBER(maxPerStageDescriptorSamplers);
  SERIALISE_MEMBER(maxPerStageDescriptorUniformBuffers);
  SERIALISE_MEMBER(maxPerStageDescriptorStorageBuffers);
  SERIALISE_MEMBER(maxPerStageDescriptorSampledImages);
  SERIALISE_MEMBER(maxPerStageDescriptorStorageImages);
  SERIALISE_MEMBER(maxPerStageDescriptorInputAttachments);
  SERIALISE_MEMBER(maxPerStageResources);
  SERIALISE_MEMBER(maxDescriptorSetSamplers);
  SERIALISE_MEMBER(maxDescriptorSetUniformBuffers);
  SERIALISE_MEMBER(maxDescriptorSetUniformBuffersDynamic);
  SERIALISE_MEMBER(maxDescriptorSetStorageBuffers);
  SERIALISE_MEMBER(maxDescriptorSetStorageBuffersDynamic);
  SERIALISE_MEMBER(maxDescriptorSetSampledImages);
  SERIALISE_MEMBER(maxDescriptorSetStorageImages);
  SERIALISE_MEMBER(maxDescriptorSetInputAttachments);
  SERIALISE_MEMBER(maxVertexInputAttributes);
  SERIALISE_MEMBER(maxVertexInputBindings);
  SERIALISE_MEMBER(maxVertexInputAttributeOffset);
  SERIALISE_MEMBER(maxVertexInputBindingStride);
  SERIALISE_MEMBER(maxVertexOutputComponents);
  SERIALISE_MEMBER(maxTessellationGenerationLevel);
  SERIALISE_MEMBER(maxTessellationPatchSize);
  SERIALISE_MEMBER(maxTessellationControlPerVertexInputComponents);
  SERIALISE_MEMBER(maxTessellationControlPerVertexOutputComponents);
  SERIALISE_MEMBER(maxTessellationControlPerPatchOutputComponents);
  SERIALISE_MEMBER(maxTessellationControlTotalOutputComponents);
  SERIALISE_MEMBER(maxTessellationEvaluationInputComponents);
  SERIALISE_MEMBER(maxTessellationEvaluationOutputComponents);
  SERIALISE_MEMBER(maxGeometryShaderInvocations);
  SERIALISE_MEMBER(maxGeometryInputComponents);
  SERIALISE_MEMBER(maxGeometryOutputComponents);
  SERIALISE_MEMBER(maxGeometryOutputVertices);
  SERIALISE_MEMBER(maxGeometryTotalOutputComponents);
  SERIALISE_MEMBER(maxFragmentInputComponents);
  SERIALISE_MEMBER(maxFragmentOutputAttachments);
  SERIALISE_MEMBER(maxFragmentDualSrcAttachments);
  SERIALISE_MEMBER(maxFragmentCombinedOutputResources);
  SERIALISE_MEMBER(maxComputeSharedMemorySize);
  SERIALISE_MEMBER(maxComputeWorkGroupCount);
  SERIALISE_MEMBER(maxComputeWorkGroupInvocations);
  SERIALISE_MEMBER(maxComputeWorkGroupSize);
  SERIALISE_MEMBER(subPixelPrecisionBits);
  SERIALISE_MEMBER(subTexelPrecisionBits);
  SERIALISE_MEMBER(mipmapPrecisionBits);
  SERIALISE_MEMBER(maxDrawIndexedIndexValue);
  SERIALISE_MEMBER(maxDrawIndirectCount);
  SERIALISE_MEMBER(maxSamplerLodBias);
  SERIALISE_MEMBER(maxSamplerAnisotropy);
  SERIALISE_MEMBER(maxViewports);
  SERIALISE_MEMBER(maxViewportDimensions);
  SERIALISE_MEMBER(viewportBoundsRange);
  SERIALISE_MEMBER(viewportSubPixelBits);

  // don't serialise size_t, otherwise capture/replay between different bit-ness won't work
  {
    uint64_t minMemoryMapAlignment = (uint64_t)el.minMemoryMapAlignment;
    ser.Serialise("minMemoryMapAlignment"_lit, minMemoryMapAlignment);
    if(ser.IsReading())
      el.minMemoryMapAlignment = (size_t)minMemoryMapAlignment;
  }

  SERIALISE_MEMBER(minTexelBufferOffsetAlignment);
  SERIALISE_MEMBER(minUniformBufferOffsetAlignment);
  SERIALISE_MEMBER(minStorageBufferOffsetAlignment);
  SERIALISE_MEMBER(minTexelOffset);
  SERIALISE_MEMBER(maxTexelOffset);
  SERIALISE_MEMBER(minTexelGatherOffset);
  SERIALISE_MEMBER(maxTexelGatherOffset);
  SERIALISE_MEMBER(minInterpolationOffset);
  SERIALISE_MEMBER(maxInterpolationOffset);
  SERIALISE_MEMBER(subPixelInterpolationOffsetBits);
  SERIALISE_MEMBER(maxFramebufferWidth);
  SERIALISE_MEMBER(maxFramebufferHeight);
  SERIALISE_MEMBER(maxFramebufferLayers);
  SERIALISE_MEMBER_VKFLAGS(VkSampleCountFlags, framebufferColorSampleCounts);
  SERIALISE_MEMBER_VKFLAGS(VkSampleCountFlags, framebufferDepthSampleCounts);
  SERIALISE_MEMBER_VKFLAGS(VkSampleCountFlags, framebufferStencilSampleCounts);
  SERIALISE_MEMBER_VKFLAGS(VkSampleCountFlags, framebufferNoAttachmentsSampleCounts);
  SERIALISE_MEMBER(maxColorAttachments);
  SERIALISE_MEMBER_VKFLAGS(VkSampleCountFlags, sampledImageColorSampleCounts);
  SERIALISE_MEMBER_VKFLAGS(VkSampleCountFlags, sampledImageIntegerSampleCounts);
  SERIALISE_MEMBER_VKFLAGS(VkSampleCountFlags, sampledImageDepthSampleCounts);
  SERIALISE_MEMBER_VKFLAGS(VkSampleCountFlags, sampledImageStencilSampleCounts);
  SERIALISE_MEMBER_VKFLAGS(VkSampleCountFlags, storageImageSampleCounts);
  SERIALISE_MEMBER(maxSampleMaskWords);
  SERIALISE_MEMBER(timestampComputeAndGraphics);
  SERIALISE_MEMBER(timestampPeriod);
  SERIALISE_MEMBER(maxClipDistances);
  SERIALISE_MEMBER(maxCullDistances);
  SERIALISE_MEMBER(maxCombinedClipAndCullDistances);
  SERIALISE_MEMBER(discreteQueuePriorities);
  SERIALISE_MEMBER(pointSizeRange);
  SERIALISE_MEMBER(lineWidthRange);
  SERIALISE_MEMBER(pointSizeGranularity);
  SERIALISE_MEMBER(lineWidthGranularity);
  SERIALISE_MEMBER(strictLines);
  SERIALISE_MEMBER(standardSampleLocations);
  SERIALISE_MEMBER(optimalBufferCopyOffsetAlignment);
  SERIALISE_MEMBER(optimalBufferCopyRowPitchAlignment);
  SERIALISE_MEMBER(nonCoherentAtomSize);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceSparseProperties &el)
{
  SERIALISE_MEMBER(residencyStandard2DBlockShape);
  SERIALISE_MEMBER(residencyStandard2DMultisampleBlockShape);
  SERIALISE_MEMBER(residencyStandard3DBlockShape);
  SERIALISE_MEMBER(residencyAlignedMipSize);
  SERIALISE_MEMBER(residencyNonResidentStrict);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkQueueFamilyProperties &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkQueueFlags, queueFlags);
  SERIALISE_MEMBER(queueCount);
  SERIALISE_MEMBER(timestampValidBits);
  SERIALISE_MEMBER(minImageTransferGranularity);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceProperties &el)
{
  // serialise apiVersion as packed version so we get a nice display.
  VkPackedVersion packedVer = el.apiVersion;
  ser.Serialise("apiVersion"_lit, packedVer).TypedAs("uint32_t"_lit);
  el.apiVersion = packedVer;

  // driver version is *not* necessarily via VK_MAKE_VERSION
  SERIALISE_MEMBER(driverVersion);
  SERIALISE_MEMBER(vendorID);
  SERIALISE_MEMBER(deviceID);
  SERIALISE_MEMBER(deviceType);
  SERIALISE_MEMBER(deviceName);
  SERIALISE_MEMBER(pipelineCacheUUID);
  SERIALISE_MEMBER(limits);
  SERIALISE_MEMBER(sparseProperties);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkDeviceCreateFlags, flags);
  SERIALISE_MEMBER(queueCreateInfoCount);
  SERIALISE_MEMBER_ARRAY(pQueueCreateInfos, queueCreateInfoCount);
  SERIALISE_MEMBER(enabledLayerCount);
  SERIALISE_MEMBER_ARRAY(ppEnabledLayerNames, enabledLayerCount);
  SERIALISE_MEMBER(enabledExtensionCount);
  SERIALISE_MEMBER_ARRAY(ppEnabledExtensionNames, enabledExtensionCount).Important();
  SERIALISE_MEMBER_OPT(pEnabledFeatures);
}

template <>
void Deserialise(const VkDeviceCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  for(uint32_t i = 0; el.pQueueCreateInfos && i < el.queueCreateInfoCount; i++)
    Deserialise(el.pQueueCreateInfos[i]);
  delete[] el.pQueueCreateInfos;
  delete[] el.ppEnabledExtensionNames;
  delete[] el.ppEnabledLayerNames;
  delete el.pEnabledFeatures;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBufferCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkBufferCreateFlags, flags);
  SERIALISE_MEMBER(size).Important();
  SERIALISE_MEMBER_VKFLAGS(VkBufferUsageFlags, usage);
  SERIALISE_MEMBER(sharingMode);

  // pQueueFamilyIndices should *only* be read if the sharing mode is concurrent
  if(el.sharingMode == VK_SHARING_MODE_CONCURRENT)
  {
    SERIALISE_MEMBER(queueFamilyIndexCount);
    SERIALISE_MEMBER_ARRAY(pQueueFamilyIndices, queueFamilyIndexCount);
  }
  else
  {
    // otherwise do a dummy serialise so the struct is the same either way
    SERIALISE_MEMBER_EMPTY(queueFamilyIndexCount);
    SERIALISE_MEMBER_OPT_EMPTY(pQueueFamilyIndices);
  }
}

template <>
void Deserialise(const VkBufferCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pQueueFamilyIndices;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBufferViewCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkBufferViewCreateFlags, flags);
  SERIALISE_MEMBER(buffer).Important();
  SERIALISE_MEMBER(format).Important();
  SERIALISE_MEMBER(offset);
  SERIALISE_MEMBER(range);
}

template <>
void Deserialise(const VkBufferViewCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkImageCreateFlags, flags);
  SERIALISE_MEMBER(imageType);
  SERIALISE_MEMBER(format).Important();
  SERIALISE_MEMBER(extent).Important();
  SERIALISE_MEMBER(mipLevels);
  SERIALISE_MEMBER(arrayLayers);
  SERIALISE_MEMBER(samples);
  SERIALISE_MEMBER(tiling);
  SERIALISE_MEMBER_VKFLAGS(VkImageUsageFlags, usage);
  SERIALISE_MEMBER(sharingMode);

  // pQueueFamilyIndices should *only* be read if the sharing mode is concurrent
  if(el.sharingMode == VK_SHARING_MODE_CONCURRENT)
  {
    SERIALISE_MEMBER(queueFamilyIndexCount);
    SERIALISE_MEMBER_ARRAY(pQueueFamilyIndices, queueFamilyIndexCount);
  }
  else
  {
    // otherwise do a dummy serialise so the struct is the same either way
    SERIALISE_MEMBER_EMPTY(queueFamilyIndexCount);
    SERIALISE_MEMBER_ARRAY_EMPTY(pQueueFamilyIndices);
  }

  SERIALISE_MEMBER(initialLayout);
}

template <>
void Deserialise(const VkImageCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pQueueFamilyIndices;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryRequirements &el)
{
  SERIALISE_MEMBER(size);
  SERIALISE_MEMBER(alignment);
  SERIALISE_MEMBER(memoryTypeBits);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageViewCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkImageViewCreateFlags, flags);
  SERIALISE_MEMBER(image).Important();
  SERIALISE_MEMBER(viewType);
  SERIALISE_MEMBER(format).Important();
  SERIALISE_MEMBER(components);
  SERIALISE_MEMBER(subresourceRange);
}

template <>
void Deserialise(const VkImageViewCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSparseMemoryBind &el)
{
  SERIALISE_MEMBER(resourceOffset);
  SERIALISE_MEMBER(size);
  SERIALISE_MEMBER(memory);
  SERIALISE_MEMBER(memoryOffset);
  SERIALISE_MEMBER_VKFLAGS(VkSparseMemoryBindFlags, flags);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSparseBufferMemoryBindInfo &el)
{
  SERIALISE_MEMBER(buffer);
  SERIALISE_MEMBER(bindCount);
  SERIALISE_MEMBER_ARRAY(pBinds, bindCount);
}

template <>
void Deserialise(const VkSparseBufferMemoryBindInfo &el)
{
  delete[] el.pBinds;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSparseImageOpaqueMemoryBindInfo &el)
{
  SERIALISE_MEMBER(image);
  SERIALISE_MEMBER(bindCount);
  SERIALISE_MEMBER_ARRAY(pBinds, bindCount);
}

template <>
void Deserialise(const VkSparseImageOpaqueMemoryBindInfo &el)
{
  delete[] el.pBinds;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSparseImageMemoryBind &el)
{
  SERIALISE_MEMBER(subresource);
  SERIALISE_MEMBER(offset);
  SERIALISE_MEMBER(extent);
  SERIALISE_MEMBER(memory);
  SERIALISE_MEMBER(memoryOffset);
  SERIALISE_MEMBER_VKFLAGS(VkSparseMemoryBindFlags, flags);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSparseImageMemoryBindInfo &el)
{
  SERIALISE_MEMBER(image);
  SERIALISE_MEMBER(bindCount);
  SERIALISE_MEMBER_ARRAY(pBinds, bindCount);
}

template <>
void Deserialise(const VkSparseImageMemoryBindInfo &el)
{
  delete[] el.pBinds;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBindSparseInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BIND_SPARSE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(waitSemaphoreCount);
  SERIALISE_MEMBER_ARRAY(pWaitSemaphores, waitSemaphoreCount);

  SERIALISE_MEMBER(bufferBindCount);
  SERIALISE_MEMBER_ARRAY(pBufferBinds, bufferBindCount);
  SERIALISE_MEMBER(imageOpaqueBindCount);
  SERIALISE_MEMBER_ARRAY(pImageOpaqueBinds, imageOpaqueBindCount);
  SERIALISE_MEMBER(imageBindCount);
  SERIALISE_MEMBER_ARRAY(pImageBinds, imageBindCount);

  SERIALISE_MEMBER(signalSemaphoreCount);
  SERIALISE_MEMBER_ARRAY(pSignalSemaphores, signalSemaphoreCount);
}

template <>
void Deserialise(const VkBindSparseInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pWaitSemaphores;
  for(uint32_t i = 0; el.pBufferBinds && i < el.bufferBindCount; i++)
    Deserialise(el.pBufferBinds[i]);
  delete[] el.pBufferBinds;
  for(uint32_t i = 0; el.pImageOpaqueBinds && i < el.imageOpaqueBindCount; i++)
    Deserialise(el.pImageOpaqueBinds[i]);
  delete[] el.pImageOpaqueBinds;
  for(uint32_t i = 0; el.pImageBinds && i < el.imageBindCount; i++)
    Deserialise(el.pImageBinds[i]);
  delete[] el.pImageBinds;
  delete[] el.pSignalSemaphores;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSubmitInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SUBMIT_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(waitSemaphoreCount);
  SERIALISE_MEMBER_ARRAY(pWaitSemaphores, waitSemaphoreCount);

  // bit of a hack, we alias the ptr here to the bits type so we serialise with better type info
  union
  {
    const VkPipelineStageFlagBits **typed;
    const VkPipelineStageFlags **orig;
  } u;
  u.orig = &el.pWaitDstStageMask;

  ser.Serialise("pWaitDstStageMask"_lit, *u.typed, el.waitSemaphoreCount,
                SerialiserFlags::AllocateMemory)
      .TypedAs("VkPipelineStageFlags"_lit);

  SERIALISE_MEMBER(commandBufferCount);
  SERIALISE_MEMBER_ARRAY(pCommandBuffers, commandBufferCount).Important();
  SERIALISE_MEMBER(signalSemaphoreCount);
  SERIALISE_MEMBER_ARRAY(pSignalSemaphores, signalSemaphoreCount);
}

template <>
void Deserialise(const VkSubmitInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pWaitSemaphores;
  delete[] el.pWaitDstStageMask;
  delete[] el.pCommandBuffers;
  delete[] el.pSignalSemaphores;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkFramebufferCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkFramebufferCreateFlags, flags);
  SERIALISE_MEMBER(renderPass).Important();
  SERIALISE_MEMBER(attachmentCount);
  if((el.flags & VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT) == 0)
  {
    SERIALISE_MEMBER_ARRAY(pAttachments, attachmentCount).Important();
  }
  else
  {
    // for imageless, mark the attachment count as important
    ser.Important();
    SERIALISE_MEMBER_ARRAY_EMPTY(pAttachments);
  }
  SERIALISE_MEMBER(width);
  SERIALISE_MEMBER(height);
  SERIALISE_MEMBER(layers);
}

template <>
void Deserialise(const VkFramebufferCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pAttachments;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkAttachmentDescription &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkAttachmentDescriptionFlags, flags);
  SERIALISE_MEMBER(format).Important();
  SERIALISE_MEMBER(samples);
  SERIALISE_MEMBER(loadOp);
  SERIALISE_MEMBER(storeOp);
  SERIALISE_MEMBER(stencilLoadOp);
  SERIALISE_MEMBER(stencilStoreOp);
  SERIALISE_MEMBER(initialLayout);
  SERIALISE_MEMBER(finalLayout);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSubpassDescription &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkSubpassDescriptionFlags, flags);
  SERIALISE_MEMBER(pipelineBindPoint);

  SERIALISE_MEMBER(inputAttachmentCount);
  SERIALISE_MEMBER_ARRAY(pInputAttachments, inputAttachmentCount);

  SERIALISE_MEMBER(colorAttachmentCount);
  SERIALISE_MEMBER_ARRAY(pColorAttachments, colorAttachmentCount).Important();
  SERIALISE_MEMBER_ARRAY(pResolveAttachments, colorAttachmentCount);

  SERIALISE_MEMBER_OPT(pDepthStencilAttachment).Important();

  SERIALISE_MEMBER(preserveAttachmentCount);
  SERIALISE_MEMBER_ARRAY(pPreserveAttachments, preserveAttachmentCount);
}

template <>
void Deserialise(const VkSubpassDescription &el)
{
  delete[] el.pInputAttachments;
  delete[] el.pColorAttachments;
  delete[] el.pResolveAttachments;
  delete el.pDepthStencilAttachment;
  delete[] el.pPreserveAttachments;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSubpassDependency &el)
{
  SERIALISE_MEMBER(srcSubpass);
  SERIALISE_MEMBER(dstSubpass);
  SERIALISE_MEMBER_VKFLAGS(VkPipelineStageFlags, srcStageMask);
  SERIALISE_MEMBER_VKFLAGS(VkPipelineStageFlags, dstStageMask);
  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags, srcAccessMask);
  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags, dstAccessMask);
  SERIALISE_MEMBER_VKFLAGS(VkDependencyFlags, dependencyFlags);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkAttachmentReference &el)
{
  SERIALISE_MEMBER(attachment).Important();
  SERIALISE_MEMBER(layout);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkRenderPassCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkRenderPassCreateFlags, flags);
  SERIALISE_MEMBER(attachmentCount);
  SERIALISE_MEMBER_ARRAY(pAttachments, attachmentCount).Important();
  SERIALISE_MEMBER(subpassCount);
  SERIALISE_MEMBER_ARRAY(pSubpasses, subpassCount).Important();
  SERIALISE_MEMBER(dependencyCount);
  SERIALISE_MEMBER_ARRAY(pDependencies, dependencyCount);
}

template <>
void Deserialise(const VkRenderPassCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pAttachments;
  for(uint32_t i = 0; el.pSubpasses && i < el.subpassCount; i++)
    Deserialise(el.pSubpasses[i]);
  delete[] el.pSubpasses;
  delete[] el.pDependencies;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkRenderPassBeginInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(renderPass).Important();
  SERIALISE_MEMBER(framebuffer).Important();
  SERIALISE_MEMBER(renderArea);
  SERIALISE_MEMBER(clearValueCount);
  SERIALISE_MEMBER_ARRAY(pClearValues, clearValueCount);
}

template <>
void Deserialise(const VkRenderPassBeginInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pClearValues;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkVertexInputBindingDescription &el)
{
  SERIALISE_MEMBER(binding);
  SERIALISE_MEMBER(stride);
  SERIALISE_MEMBER(inputRate);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkVertexInputAttributeDescription &el)
{
  SERIALISE_MEMBER(location);
  SERIALISE_MEMBER(binding);
  SERIALISE_MEMBER(format);
  SERIALISE_MEMBER(offset);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineVertexInputStateCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineVertexInputStateCreateFlags, flags);
  SERIALISE_MEMBER(vertexBindingDescriptionCount);
  SERIALISE_MEMBER_ARRAY(pVertexBindingDescriptions, vertexBindingDescriptionCount);
  SERIALISE_MEMBER(vertexAttributeDescriptionCount);
  SERIALISE_MEMBER_ARRAY(pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
}

template <>
void Deserialise(const VkPipelineVertexInputStateCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pVertexBindingDescriptions;
  delete[] el.pVertexAttributeDescriptions;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineInputAssemblyStateCreateInfo &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineInputAssemblyStateCreateFlags, flags);
  SERIALISE_MEMBER(topology);
  SERIALISE_MEMBER(primitiveRestartEnable);
}

template <>
void Deserialise(const VkPipelineInputAssemblyStateCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineTessellationStateCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineTessellationStateCreateFlags, flags);
  SERIALISE_MEMBER(patchControlPoints);
}

template <>
void Deserialise(const VkPipelineTessellationStateCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineViewportStateCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineViewportStateCreateFlags, flags);

  SERIALISE_MEMBER(viewportCount);
  SERIALISE_MEMBER_ARRAY(pViewports, viewportCount);
  SERIALISE_MEMBER(scissorCount);
  SERIALISE_MEMBER_ARRAY(pScissors, scissorCount);
}

template <>
void Deserialise(const VkPipelineViewportStateCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pViewports;
  delete[] el.pScissors;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineRasterizationStateCreateInfo &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineRasterizationStateCreateFlags, flags);
  SERIALISE_MEMBER(depthClampEnable);
  SERIALISE_MEMBER(rasterizerDiscardEnable);
  SERIALISE_MEMBER(polygonMode);
  SERIALISE_MEMBER_VKFLAGS(VkCullModeFlags, cullMode);
  SERIALISE_MEMBER(frontFace);
  SERIALISE_MEMBER(depthBiasEnable);
  SERIALISE_MEMBER(depthBiasConstantFactor);
  SERIALISE_MEMBER(depthBiasClamp);
  SERIALISE_MEMBER(depthBiasSlopeFactor);
  SERIALISE_MEMBER(lineWidth);
}

template <>
void Deserialise(const VkPipelineRasterizationStateCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineMultisampleStateCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineMultisampleStateCreateFlags, flags);
  SERIALISE_MEMBER(rasterizationSamples);
  RDCASSERT(el.rasterizationSamples <= VK_SAMPLE_COUNT_32_BIT);
  SERIALISE_MEMBER(sampleShadingEnable);
  SERIALISE_MEMBER(minSampleShading);
  SERIALISE_MEMBER_OPT(pSampleMask);
  SERIALISE_MEMBER(alphaToCoverageEnable);
  SERIALISE_MEMBER(alphaToOneEnable);
}

template <>
void Deserialise(const VkPipelineMultisampleStateCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete el.pSampleMask;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineColorBlendAttachmentState &el)
{
  SERIALISE_MEMBER(blendEnable);
  SERIALISE_MEMBER(srcColorBlendFactor);
  SERIALISE_MEMBER(dstColorBlendFactor);
  SERIALISE_MEMBER(colorBlendOp);
  SERIALISE_MEMBER(srcAlphaBlendFactor);
  SERIALISE_MEMBER(dstAlphaBlendFactor);
  SERIALISE_MEMBER(alphaBlendOp);
  SERIALISE_MEMBER_VKFLAGS(VkColorComponentFlags, colorWriteMask);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineColorBlendStateCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineColorBlendStateCreateFlags, flags);
  SERIALISE_MEMBER(logicOpEnable);
  SERIALISE_MEMBER(logicOp);
  SERIALISE_MEMBER(attachmentCount);
  SERIALISE_MEMBER_ARRAY(pAttachments, attachmentCount);
  SERIALISE_MEMBER(blendConstants);
}

template <>
void Deserialise(const VkPipelineColorBlendStateCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pAttachments;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineDepthStencilStateCreateInfo &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineDepthStencilStateCreateFlags, flags);
  SERIALISE_MEMBER(depthTestEnable);
  SERIALISE_MEMBER(depthWriteEnable);
  SERIALISE_MEMBER(depthCompareOp);
  SERIALISE_MEMBER(depthBoundsTestEnable);
  SERIALISE_MEMBER(stencilTestEnable);
  SERIALISE_MEMBER(front);
  SERIALISE_MEMBER(back);
  SERIALISE_MEMBER(minDepthBounds);
  SERIALISE_MEMBER(maxDepthBounds);
}

template <>
void Deserialise(const VkPipelineDepthStencilStateCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineDynamicStateCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineDynamicStateCreateFlags, flags);
  SERIALISE_MEMBER(dynamicStateCount);
  SERIALISE_MEMBER_ARRAY(pDynamicStates, dynamicStateCount);
}

template <>
void Deserialise(const VkPipelineDynamicStateCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pDynamicStates;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkCommandPoolCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkCommandPoolCreateFlags, flags).Important();
  SERIALISE_MEMBER(queueFamilyIndex);
}

template <>
void Deserialise(const VkCommandPoolCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkCommandBufferAllocateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(commandPool).Important();
  SERIALISE_MEMBER(level).Important();
  SERIALISE_MEMBER(commandBufferCount);
}

template <>
void Deserialise(const VkCommandBufferAllocateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkCommandBufferInheritanceInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(renderPass).Important();
  SERIALISE_MEMBER(subpass).Important();
  SERIALISE_MEMBER(framebuffer).Important();
  SERIALISE_MEMBER(occlusionQueryEnable);
  SERIALISE_MEMBER_VKFLAGS(VkQueryControlFlags, queryFlags);
  SERIALISE_MEMBER_VKFLAGS(VkQueryPipelineStatisticFlags, pipelineStatistics);
}

template <>
void Deserialise(const VkCommandBufferInheritanceInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkCommandBufferBeginInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkCommandBufferUsageFlags, flags);
  SERIALISE_MEMBER_OPT(pInheritanceInfo).Important();
}

template <>
void Deserialise(const VkCommandBufferBeginInfo &el)
{
  DeserialiseNext(el.pNext);
  if(el.pInheritanceInfo)
    Deserialise(*el.pInheritanceInfo);
  delete el.pInheritanceInfo;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkStencilOpState &el)
{
  SERIALISE_MEMBER(failOp);
  SERIALISE_MEMBER(passOp);
  SERIALISE_MEMBER(depthFailOp);
  SERIALISE_MEMBER(compareOp);
  SERIALISE_MEMBER(compareMask);
  SERIALISE_MEMBER(writeMask);
  SERIALISE_MEMBER(reference);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkQueryPoolCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkQueryPoolCreateFlags, flags);
  SERIALISE_MEMBER(queryType).Important();
  SERIALISE_MEMBER(queryCount).Important();
  SERIALISE_MEMBER_VKFLAGS(VkQueryPipelineStatisticFlags, pipelineStatistics);
}

template <>
void Deserialise(const VkQueryPoolCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSemaphoreCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkSemaphoreCreateFlags, flags).Important();
}

template <>
void Deserialise(const VkSemaphoreCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkEventCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EVENT_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkEventCreateFlags, flags).Important();
}

template <>
void Deserialise(const VkEventCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkFenceCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkFenceCreateFlags, flags).Important();
}

template <>
void Deserialise(const VkFenceCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSamplerCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkSamplerCreateFlags, flags);
  SERIALISE_MEMBER(magFilter).Important();
  SERIALISE_MEMBER(minFilter).Important();
  SERIALISE_MEMBER(mipmapMode);
  SERIALISE_MEMBER(addressModeU);
  SERIALISE_MEMBER(addressModeV);
  SERIALISE_MEMBER(addressModeW);
  SERIALISE_MEMBER(mipLodBias);
  SERIALISE_MEMBER(anisotropyEnable);
  SERIALISE_MEMBER(maxAnisotropy);
  SERIALISE_MEMBER(compareEnable);
  SERIALISE_MEMBER(compareOp);
  SERIALISE_MEMBER(minLod);
  SERIALISE_MEMBER(maxLod);
  SERIALISE_MEMBER(borderColor);
  SERIALISE_MEMBER(unnormalizedCoordinates);
}

template <>
void Deserialise(const VkSamplerCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineShaderStageCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineShaderStageCreateFlags, flags);
  SERIALISE_MEMBER(stage);
  SERIALISE_MEMBER(module).Important();
  SERIALISE_MEMBER(pName).Important();
  SERIALISE_MEMBER_OPT(pSpecializationInfo);
}

template <>
void Deserialise(const VkPipelineShaderStageCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  if(el.pSpecializationInfo)
  {
    Deserialise(*el.pSpecializationInfo);
    delete el.pSpecializationInfo;
  }
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSpecializationMapEntry &el)
{
  SERIALISE_MEMBER(constantID);
  SERIALISE_MEMBER(offset);
  // this was accidentally duplicated - hide it from the UI
  SERIALISE_MEMBER(constantID).Hidden();

  // don't serialise size_t, otherwise capture/replay between different bit-ness won't work
  {
    uint64_t size = el.size;
    ser.Serialise("size"_lit, size);
    if(ser.IsReading())
      el.size = (size_t)size;
  }
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSpecializationInfo &el)
{
  SERIALISE_MEMBER(mapEntryCount);
  SERIALISE_MEMBER_ARRAY(pMapEntries, mapEntryCount);

  // don't serialise size_t, otherwise capture/replay between different bit-ness won't work
  {
    uint64_t dataSize = el.dataSize;
    ser.Serialise("dataSize"_lit, dataSize);
    if(ser.IsReading())
      el.dataSize = (size_t)dataSize;
  }

  SERIALISE_MEMBER_ARRAY(pData, dataSize);
}

template <>
void Deserialise(const VkSpecializationInfo &el)
{
  FreeAlignedBuffer((byte *)el.pData);
  delete[] el.pMapEntries;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineCacheCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineCacheCreateFlags, flags);

  // don't serialise size_t, otherwise capture/replay between different bit-ness won't work
  {
    uint64_t initialDataSize = el.initialDataSize;
    ser.Serialise("initialDataSize"_lit, initialDataSize);
    if(ser.IsReading())
      el.initialDataSize = (size_t)initialDataSize;
  }

  SERIALISE_MEMBER_ARRAY(pInitialData, initialDataSize).Important();
}

template <>
void Deserialise(const VkPipelineCacheCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  FreeAlignedBuffer((byte *)el.pInitialData);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineLayoutCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineLayoutCreateFlags, flags);
  SERIALISE_MEMBER(setLayoutCount);
  SERIALISE_MEMBER_ARRAY(pSetLayouts, setLayoutCount).Important();
  SERIALISE_MEMBER(pushConstantRangeCount).Important();
  SERIALISE_MEMBER_ARRAY(pPushConstantRanges, pushConstantRangeCount);
}

template <>
void Deserialise(const VkPipelineLayoutCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pSetLayouts;
  delete[] el.pPushConstantRanges;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkShaderModuleCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkShaderModuleCreateFlags, flags);

  // don't serialise size_t, otherwise capture/replay between different bit-ness won't work
  {
    uint64_t codeSize = el.codeSize;
    ser.Serialise("codeSize"_lit, codeSize);
    if(ser.IsReading())
      el.codeSize = (size_t)codeSize;
  }

  // serialise as void* so it goes through as a buffer, not an actual array of integers.
  {
    const void *pCode = el.pCode;
    ser.Serialise("pCode"_lit, pCode, el.codeSize, SerialiserFlags::AllocateMemory).Important();
    if(ser.IsReading())
      el.pCode = (uint32_t *)pCode;
  }
}

template <>
void Deserialise(const VkShaderModuleCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  FreeAlignedBuffer((byte *)el.pCode);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageSubresourceRange &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkImageAspectFlags, aspectMask);
  SERIALISE_MEMBER(baseMipLevel);
  SERIALISE_MEMBER(levelCount);
  SERIALISE_MEMBER(baseArrayLayer);
  SERIALISE_MEMBER(layerCount);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageSubresourceLayers &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkImageAspectFlags, aspectMask);
  SERIALISE_MEMBER(mipLevel);
  SERIALISE_MEMBER(baseArrayLayer);
  SERIALISE_MEMBER(layerCount);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageSubresource &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkImageAspectFlags, aspectMask);
  SERIALISE_MEMBER(mipLevel);
  SERIALISE_MEMBER(arrayLayer);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryAllocateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(allocationSize).Important();
  SERIALISE_MEMBER(memoryTypeIndex).Important();
}

template <>
void Deserialise(const VkMemoryAllocateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryBarrier &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MEMORY_BARRIER);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags, srcAccessMask);
  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags, dstAccessMask);
}

template <>
void Deserialise(const VkMemoryBarrier &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBufferMemoryBarrier &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER);
  SerialiseNext(ser, el.sType, el.pNext);

  // Resources in this struct are optional, because if we decided a resource wasn't used - we
  // might still have recorded some barriers on it
  OPTIONAL_RESOURCES();

  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags, srcAccessMask);
  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags, dstAccessMask);
  // serialise as signed because then QUEUE_FAMILY_IGNORED is -1 and queue
  // family index won't be legitimately larger than 2 billion
  SERIALISE_MEMBER_TYPED(int32_t, srcQueueFamilyIndex);
  SERIALISE_MEMBER_TYPED(int32_t, dstQueueFamilyIndex);
  SERIALISE_MEMBER(buffer).Important();
  SERIALISE_MEMBER(offset);
  SERIALISE_MEMBER(size);
}

template <>
void Deserialise(const VkBufferMemoryBarrier &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageMemoryBarrier &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER);
  SerialiseNext(ser, el.sType, el.pNext);

  // Resources in this struct are optional, because if we decided a resource wasn't used - we
  // might still have recorded some barriers on it
  OPTIONAL_RESOURCES();

  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags, srcAccessMask);
  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags, dstAccessMask);
  SERIALISE_MEMBER(oldLayout);
  SERIALISE_MEMBER(newLayout);
  // serialise as signed because then QUEUE_FAMILY_IGNORED is -1 and queue
  // family index won't be legitimately larger than 2 billion
  SERIALISE_MEMBER_TYPED(int32_t, srcQueueFamilyIndex);
  SERIALISE_MEMBER_TYPED(int32_t, dstQueueFamilyIndex);
  SERIALISE_MEMBER(image).Important();
  SERIALISE_MEMBER(subresourceRange);
}

template <>
void Deserialise(const VkImageMemoryBarrier &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkGraphicsPipelineCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineCreateFlags, flags);
  SERIALISE_MEMBER(stageCount);
  SERIALISE_MEMBER_ARRAY(pStages, stageCount).Important();

  bool hasTess = false;
  for(uint32_t i = 0; i < el.stageCount; i++)
    hasTess |= (el.pStages[i].stage & (VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT |
                                       VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT)) != 0;

  SERIALISE_MEMBER_OPT(pVertexInputState);
  SERIALISE_MEMBER_OPT(pInputAssemblyState);

  // if we don't have tessellation shaders, pTessellationState is ignored and may be garbage
  if(hasTess)
  {
    SERIALISE_MEMBER_OPT(pTessellationState);
  }
  else
  {
    SERIALISE_MEMBER_OPT_EMPTY(pTessellationState);
  }

  // this gets messy. We need to ignore pViewportState, pMultisampleState, pDepthStencilState, and
  // pColorBlendState if rasterization is disabled. Unfortunately... pViewportState is BEFORE the
  // pRasterization state so we can't check it on read.
  // Instead we rely on the fact that while writing we can check it and serialise an explicit NULL
  // indicating that it's not present. Then on read we can just read it as if it's either NULL or
  // present.

  // this bool just means "we can use SERIALISE_MEMBER_OPT" - i.e. the struct is present, or NULL.
  const bool hasValidRasterization =
      ser.IsReading() || (ser.IsWriting() && el.pRasterizationState &&
                          el.pRasterizationState->rasterizerDiscardEnable == VK_FALSE);

  if(hasValidRasterization)
  {
    // we have a similar problem with needing pDynamicState to determine if the pViewports/pScissors
    // members are valid or not.
    if(ser.IsReading() || el.pViewportState == NULL)
    {
      // all the hard work is done while writing. On reading, just serialise (optionally) directly.
      SERIALISE_MEMBER_OPT(pViewportState);
    }
    else
    {
      bool dynamicViewport = false, dynamicScissor = false;

      for(uint32_t i = 0; el.pDynamicState && i < el.pDynamicState->dynamicStateCount; i++)
      {
        if(el.pDynamicState->pDynamicStates[i] == VK_DYNAMIC_STATE_VIEWPORT)
          dynamicViewport = true;
        else if(el.pDynamicState->pDynamicStates[i] == VK_DYNAMIC_STATE_SCISSOR)
          dynamicScissor = true;
      }

      VkPipelineViewportStateCreateInfo viewportState = *el.pViewportState;
      VkPipelineViewportStateCreateInfo *pViewportState = &viewportState;

      if(dynamicScissor)
        viewportState.pScissors = NULL;
      if(dynamicViewport)
        viewportState.pViewports = NULL;

      SERIALISE_ELEMENT_OPT(pViewportState);
    }
  }
  else
  {
    SERIALISE_MEMBER_OPT_EMPTY(pViewportState);
  }

  SERIALISE_MEMBER_OPT(pRasterizationState);

  if(hasValidRasterization)
  {
    SERIALISE_MEMBER_OPT(pMultisampleState);
    SERIALISE_MEMBER_OPT(pDepthStencilState);
    SERIALISE_MEMBER_OPT(pColorBlendState);
  }
  else
  {
    SERIALISE_MEMBER_OPT_EMPTY(pMultisampleState);
    SERIALISE_MEMBER_OPT_EMPTY(pDepthStencilState);
    SERIALISE_MEMBER_OPT_EMPTY(pColorBlendState);
  }

  SERIALISE_MEMBER_OPT(pDynamicState);

  SERIALISE_MEMBER(layout);
  SERIALISE_MEMBER(renderPass);
  SERIALISE_MEMBER(subpass);

  // handle must be explicitly ignored if the flag isn't set, since it could be garbage
  if(el.flags & VK_PIPELINE_CREATE_DERIVATIVE_BIT)
  {
    SERIALISE_MEMBER(basePipelineHandle);
  }
  else
  {
    SERIALISE_MEMBER_EMPTY(basePipelineHandle);
  }

  SERIALISE_MEMBER(basePipelineIndex);
}

template <>
void Deserialise(const VkGraphicsPipelineCreateInfo &el)
{
  DeserialiseNext(el.pNext);

  if(el.pVertexInputState)
  {
    Deserialise(*el.pVertexInputState);
    delete el.pVertexInputState;
  }

  if(el.pInputAssemblyState)
  {
    Deserialise(*el.pInputAssemblyState);
    delete el.pInputAssemblyState;
  }

  if(el.pTessellationState)
  {
    Deserialise(*el.pTessellationState);
    delete el.pTessellationState;
  }
  if(el.pViewportState)
  {
    Deserialise(*el.pViewportState);
    delete el.pViewportState;
  }
  if(el.pRasterizationState)
  {
    Deserialise(*el.pRasterizationState);
    delete el.pRasterizationState;
  }
  if(el.pMultisampleState)
  {
    Deserialise(*el.pMultisampleState);
    delete el.pMultisampleState;
  }
  if(el.pDepthStencilState)
  {
    Deserialise(*el.pDepthStencilState);
    delete el.pDepthStencilState;
  }
  if(el.pColorBlendState)
  {
    Deserialise(*el.pColorBlendState);
    delete el.pColorBlendState;
  }
  if(el.pDynamicState)
  {
    Deserialise(*el.pDynamicState);
    delete el.pDynamicState;
  }
  for(uint32_t i = 0; el.pStages && i < el.stageCount; i++)
  {
    Deserialise(el.pStages[i]);
  }
  delete[] el.pStages;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkComputePipelineCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineCreateFlags, flags);
  SERIALISE_MEMBER(stage).Important();
  SERIALISE_MEMBER(layout);

  if(el.flags & VK_PIPELINE_CREATE_DERIVATIVE_BIT)
    SERIALISE_MEMBER(basePipelineHandle);
  else
    SERIALISE_MEMBER_EMPTY(basePipelineHandle);

  SERIALISE_MEMBER(basePipelineIndex);
}

template <>
void Deserialise(const VkComputePipelineCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  DeserialiseNext(el.stage.pNext);
  if(el.stage.pSpecializationInfo)
  {
    Deserialise(*el.stage.pSpecializationInfo);
    delete el.stage.pSpecializationInfo;
  }
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorPoolSize &el)
{
  SERIALISE_MEMBER(type);
  SERIALISE_MEMBER(descriptorCount);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorPoolCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkDescriptorPoolCreateFlags, flags);
  SERIALISE_MEMBER(maxSets).Important();
  SERIALISE_MEMBER(poolSizeCount);
  SERIALISE_MEMBER_ARRAY(pPoolSizes, poolSizeCount).Important();
}

template <>
void Deserialise(const VkDescriptorPoolCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pPoolSizes;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorSetAllocateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(descriptorPool).Important();
  SERIALISE_MEMBER(descriptorSetCount);
  SERIALISE_MEMBER_ARRAY(pSetLayouts, descriptorSetCount).Important();
}

template <>
void Deserialise(const VkDescriptorSetAllocateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pSetLayouts;
}

enum class VkDescriptorImageInfoValidity
{
  Neither = 0x0,
  Sampler = 0x1,
  ImageView = 0x100,
};

BITMASK_OPERATORS(VkDescriptorImageInfoValidity);

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorImageInfo &el)
{
  // Resources in this struct are optional, because if we decided a descriptor wasn't used - we
  // might still have recorded some updates to it
  OPTIONAL_RESOURCES();

  VkDescriptorImageInfoValidity validity = (VkDescriptorImageInfoValidity)ser.GetStructArg();

  RDCASSERT(validity != VkDescriptorImageInfoValidity::Neither, (uint64_t)validity);

  if(validity & VkDescriptorImageInfoValidity::Sampler)
    SERIALISE_MEMBER(sampler);
  else
    SERIALISE_MEMBER_EMPTY(sampler);

  if(validity & VkDescriptorImageInfoValidity::ImageView)
    SERIALISE_MEMBER(imageView);
  else
    SERIALISE_MEMBER_EMPTY(imageView);

  SERIALISE_MEMBER(imageLayout);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorBufferInfo &el)
{
  // Resources in this struct are optional, because if we decided a descriptor wasn't used - we
  // might still have recorded some updates to it
  OPTIONAL_RESOURCES();

  SERIALISE_MEMBER(buffer);
  SERIALISE_MEMBER(offset);
  SERIALISE_MEMBER(range);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkWriteDescriptorSet &el)
{
  // Resources in this struct are optional, because if we decided a descriptor wasn't used - we
  // might still have recorded some updates to it
  OPTIONAL_RESOURCES();

  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(dstSet).Important();
  SERIALISE_MEMBER(dstBinding).Important();
  SERIALISE_MEMBER(dstArrayElement);
  SERIALISE_MEMBER(descriptorCount);
  SERIALISE_MEMBER(descriptorType).Important();

  // only serialise the array type used, the others are ignored
  if(el.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER ||
     el.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
     el.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE ||
     el.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ||
     el.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
  {
    VkDescriptorImageInfoValidity validity = VkDescriptorImageInfoValidity::Neither;

    if(el.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER ||
       el.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
    {
      validity = validity | VkDescriptorImageInfoValidity::Sampler;

      // on writing check if this is an immutable samplers binding. If it is we can't treat the
      // sampler as valid. On replay we don't have to do this because invalid samplers got
      // serialised as NULL safely.
      if(ser.IsWriting() && el.dstSet != VK_NULL_HANDLE)
      {
        if(GetRecord(el.dstSet)->descInfo->layout->bindings[el.dstBinding].immutableSampler != NULL)
          validity = validity & ~VkDescriptorImageInfoValidity::Sampler;
      }
    }

    if(el.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
       el.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE ||
       el.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ||
       el.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
      validity = validity | VkDescriptorImageInfoValidity::ImageView;

    // set the validity flags so the serialisation of VkDescriptorImageInfo knows which members are
    // safe to read. We pass this as just flags so the comparisons happen here once, not per-element
    // in this array
    ser.SetStructArg((uint64_t)validity);

    SERIALISE_MEMBER_ARRAY(pImageInfo, descriptorCount);
  }
  else
  {
    SERIALISE_MEMBER_ARRAY_EMPTY(pImageInfo);
  }

  if(el.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ||
     el.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER ||
     el.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC ||
     el.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC)
  {
    SERIALISE_MEMBER_ARRAY(pBufferInfo, descriptorCount);
  }
  else
  {
    SERIALISE_MEMBER_ARRAY_EMPTY(pBufferInfo);
  }

  if(el.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER ||
     el.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER)
  {
    SERIALISE_MEMBER_ARRAY(pTexelBufferView, descriptorCount);
  }
  else
  {
    SERIALISE_MEMBER_ARRAY_EMPTY(pTexelBufferView);
  }
}

template <>
void Deserialise(const VkWriteDescriptorSet &el)
{
  DeserialiseNext(el.pNext);
  if(el.pImageInfo)
    delete[] el.pImageInfo;
  if(el.pBufferInfo)
    delete[] el.pBufferInfo;
  if(el.pTexelBufferView)
    delete[] el.pTexelBufferView;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkCopyDescriptorSet &el)
{
  // Resources in this struct are optional, because if we decided a descriptor wasn't used - we
  // might still have recorded some copies to or from it
  OPTIONAL_RESOURCES();

  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(srcSet).Important();
  SERIALISE_MEMBER(srcBinding).Important();
  SERIALISE_MEMBER(srcArrayElement);
  SERIALISE_MEMBER(dstSet).Important();
  SERIALISE_MEMBER(dstBinding).Important();
  SERIALISE_MEMBER(dstArrayElement);
  SERIALISE_MEMBER(descriptorCount);
}

template <>
void Deserialise(const VkCopyDescriptorSet &el)
{
  DeserialiseNext(el.pNext);
};

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPushConstantRange &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkShaderStageFlags, stageFlags);
  SERIALISE_MEMBER(offset);
  SERIALISE_MEMBER(size);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorSetLayoutBinding &el)
{
  SERIALISE_MEMBER(binding);
  SERIALISE_MEMBER(descriptorType);
  SERIALISE_MEMBER(descriptorCount);
  SERIALISE_MEMBER_VKFLAGS(VkShaderStageFlags, stageFlags);

  if(el.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER ||
     el.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
  {
    SERIALISE_MEMBER_ARRAY(pImmutableSamplers, descriptorCount);
  }
  else
  {
    SERIALISE_MEMBER_ARRAY_EMPTY(pImmutableSamplers);
  }
}

template <>
void Deserialise(const VkDescriptorSetLayoutBinding &el)
{
  delete[] el.pImmutableSamplers;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorSetLayoutCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkDescriptorSetLayoutCreateFlags, flags);
  SERIALISE_MEMBER(bindingCount).Important();
  SERIALISE_MEMBER_ARRAY(pBindings, bindingCount);
}

template <>
void Deserialise(const VkDescriptorSetLayoutCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  for(uint32_t i = 0; i < el.bindingCount; i++)
    if(el.pBindings[i].pImmutableSamplers)
      delete[] el.pBindings[i].pImmutableSamplers;
  delete[] el.pBindings;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkComponentMapping &el)
{
  SERIALISE_MEMBER(r);
  SERIALISE_MEMBER(g);
  SERIALISE_MEMBER(b);
  SERIALISE_MEMBER(a);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMappedMemoryRange &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(memory).Important();
  SERIALISE_MEMBER(offset);
  SERIALISE_MEMBER(size);
}

template <>
void Deserialise(const VkMappedMemoryRange &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBufferImageCopy &el)
{
  SERIALISE_MEMBER(bufferOffset);
  SERIALISE_MEMBER(bufferRowLength);
  SERIALISE_MEMBER(bufferImageHeight);
  SERIALISE_MEMBER(imageSubresource);
  SERIALISE_MEMBER(imageOffset);
  SERIALISE_MEMBER(imageExtent);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBufferCopy &el)
{
  SERIALISE_MEMBER(srcOffset);
  SERIALISE_MEMBER(dstOffset);
  SERIALISE_MEMBER(size);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageCopy &el)
{
  SERIALISE_MEMBER(srcSubresource);
  SERIALISE_MEMBER(srcOffset);
  SERIALISE_MEMBER(dstSubresource);
  SERIALISE_MEMBER(dstOffset);
  SERIALISE_MEMBER(extent);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageBlit &el)
{
  SERIALISE_MEMBER(srcSubresource);
  SERIALISE_MEMBER(srcOffsets);
  SERIALISE_MEMBER(dstSubresource);
  SERIALISE_MEMBER(dstOffsets);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageResolve &el)
{
  SERIALISE_MEMBER(srcSubresource);
  SERIALISE_MEMBER(srcOffset);
  SERIALISE_MEMBER(dstSubresource);
  SERIALISE_MEMBER(dstOffset);
  SERIALISE_MEMBER(extent);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkClearColorValue &el)
{
  SERIALISE_MEMBER(float32).Important();
  SERIALISE_MEMBER(int32);
  SERIALISE_MEMBER(uint32);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkClearDepthStencilValue &el)
{
  SERIALISE_MEMBER(depth);
  SERIALISE_MEMBER(stencil);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkClearValue &el)
{
  SERIALISE_MEMBER(color);
  SERIALISE_MEMBER(depthStencil);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkClearRect &el)
{
  SERIALISE_MEMBER(rect);
  SERIALISE_MEMBER(baseArrayLayer);
  SERIALISE_MEMBER(layerCount);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkClearAttachment &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkImageAspectFlags, aspectMask);
  SERIALISE_MEMBER(colorAttachment).Important();
  SERIALISE_MEMBER(clearValue);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkRect2D &el)
{
  SERIALISE_MEMBER(offset);
  SERIALISE_MEMBER(extent);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkOffset2D &el)
{
  SERIALISE_MEMBER(x);
  SERIALISE_MEMBER(y);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkOffset3D &el)
{
  SERIALISE_MEMBER(x);
  SERIALISE_MEMBER(y);
  SERIALISE_MEMBER(z);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExtent2D &el)
{
  SERIALISE_MEMBER(width);
  SERIALISE_MEMBER(height);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExtent3D &el)
{
  SERIALISE_MEMBER(width);
  SERIALISE_MEMBER(height);
  SERIALISE_MEMBER(depth);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkViewport &el)
{
  SERIALISE_MEMBER(x).Important();
  SERIALISE_MEMBER(y).Important();
  SERIALISE_MEMBER(width).Important();
  SERIALISE_MEMBER(height).Important();
  SERIALISE_MEMBER(minDepth);
  SERIALISE_MEMBER(maxDepth);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSharedPresentSurfaceCapabilitiesKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SHARED_PRESENT_SURFACE_CAPABILITIES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkImageUsageFlags, sharedPresentSupportedUsageFlags);
}

template <>
void Deserialise(const VkSharedPresentSurfaceCapabilitiesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSwapchainCreateInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkSwapchainCreateFlagsKHR, flags);

  // don't need the surface
  SERIALISE_MEMBER_EMPTY(surface);

  SERIALISE_MEMBER(minImageCount).Important();
  SERIALISE_MEMBER(imageFormat).Important();
  SERIALISE_MEMBER(imageColorSpace);
  SERIALISE_MEMBER(imageExtent);
  SERIALISE_MEMBER(imageArrayLayers);
  SERIALISE_MEMBER_VKFLAGS(VkImageUsageFlags, imageUsage);
  SERIALISE_MEMBER(imageSharingMode);

  // pQueueFamilyIndices should *only* be read if the sharing mode is concurrent, and if the capture
  // is new (old captures always ignored these fields)
  if(ser.VersionAtLeast(0xD) && el.imageSharingMode == VK_SHARING_MODE_CONCURRENT)
  {
    SERIALISE_MEMBER(queueFamilyIndexCount);
    SERIALISE_MEMBER_ARRAY(pQueueFamilyIndices, queueFamilyIndexCount);
  }
  else
  {
    // otherwise do a dummy serialise so the struct is the same either way
    SERIALISE_MEMBER_EMPTY(queueFamilyIndexCount);
    SERIALISE_MEMBER_ARRAY_EMPTY(pQueueFamilyIndices);
  }

  SERIALISE_MEMBER(preTransform);
  SERIALISE_MEMBER(compositeAlpha);
  SERIALISE_MEMBER(presentMode);
  SERIALISE_MEMBER(clipped);

  // don't need the old swap chain
  SERIALISE_MEMBER_EMPTY(oldSwapchain);
}

template <>
void Deserialise(const VkSwapchainCreateInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSwapchainDisplayNativeHdrCreateInfoAMD &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_SWAPCHAIN_DISPLAY_NATIVE_HDR_CREATE_INFO_AMD);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(localDimmingEnable);
}

template <>
void Deserialise(const VkSwapchainDisplayNativeHdrCreateInfoAMD &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPresentInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PRESENT_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(waitSemaphoreCount);
  SERIALISE_MEMBER_ARRAY(pWaitSemaphores, waitSemaphoreCount);

  SERIALISE_MEMBER(swapchainCount);
  SERIALISE_MEMBER_ARRAY_EMPTY(pSwapchains);
  SERIALISE_MEMBER_ARRAY(pImageIndices, swapchainCount).Important();
  SERIALISE_MEMBER_ARRAY(pResults, swapchainCount);
}

template <>
void Deserialise(const VkPresentInfoKHR &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pWaitSemaphores;
  delete[] el.pSwapchains;
  delete[] el.pImageIndices;
  delete[] el.pResults;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkAcquireNextImageInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  // don't need the swap chain itself
  SERIALISE_MEMBER_EMPTY(swapchain);
  SERIALISE_MEMBER(timeout);
  SERIALISE_MEMBER(semaphore);
  SERIALISE_MEMBER(fence);
  SERIALISE_MEMBER(deviceMask);
}

template <>
void Deserialise(const VkAcquireNextImageInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceVariablePointerFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(variablePointersStorageBuffer);
  SERIALISE_MEMBER(variablePointers);
}

template <>
void Deserialise(const VkPhysicalDeviceVariablePointerFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceVulkanMemoryModelFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(vulkanMemoryModel);
  SERIALISE_MEMBER(vulkanMemoryModelDeviceScope);

  // this field was added in Vulkan 1.1.99
  if(ser.VersionAtLeast(0xF))
  {
    SERIALISE_MEMBER(vulkanMemoryModelAvailabilityVisibilityChains);
  }
  else if(ser.IsReading())
  {
    // default to FALSE conservatively
    el.vulkanMemoryModelAvailabilityVisibilityChains = VK_FALSE;
  }
}

template <>
void Deserialise(const VkPhysicalDeviceVulkanMemoryModelFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType ==
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ZERO_INITIALIZE_WORKGROUP_MEMORY_FEATURES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(shaderZeroInitializeWorkgroupMemory);
}

template <>
void Deserialise(const VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType ==
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_FEATURES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(workgroupMemoryExplicitLayout);
  SERIALISE_MEMBER(workgroupMemoryExplicitLayoutScalarBlockLayout);
  SERIALISE_MEMBER(workgroupMemoryExplicitLayout8BitAccess);
  SERIALISE_MEMBER(workgroupMemoryExplicitLayout16BitAccess);
}

template <>
void Deserialise(const VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceVulkan11Features &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(storageBuffer16BitAccess);
  SERIALISE_MEMBER(uniformAndStorageBuffer16BitAccess);
  SERIALISE_MEMBER(storagePushConstant16);
  SERIALISE_MEMBER(storageInputOutput16);
  SERIALISE_MEMBER(multiview);
  SERIALISE_MEMBER(multiviewGeometryShader);
  SERIALISE_MEMBER(multiviewTessellationShader);
  SERIALISE_MEMBER(variablePointersStorageBuffer);
  SERIALISE_MEMBER(variablePointers);
  SERIALISE_MEMBER(protectedMemory);
  SERIALISE_MEMBER(samplerYcbcrConversion);
  SERIALISE_MEMBER(shaderDrawParameters);
}

template <>
void Deserialise(const VkPhysicalDeviceVulkan11Features &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceVulkan11Properties &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(deviceUUID);
  SERIALISE_MEMBER(driverUUID);
  SERIALISE_MEMBER(deviceLUID);
  SERIALISE_MEMBER(deviceNodeMask);
  SERIALISE_MEMBER(deviceLUIDValid);
  SERIALISE_MEMBER(subgroupSize);
  SERIALISE_MEMBER_VKFLAGS(VkShaderStageFlags, subgroupSupportedStages);
  SERIALISE_MEMBER_VKFLAGS(VkSubgroupFeatureFlags, subgroupSupportedOperations);
  SERIALISE_MEMBER(subgroupQuadOperationsInAllStages);
  SERIALISE_MEMBER(pointClippingBehavior);
  SERIALISE_MEMBER(maxMultiviewViewCount);
  SERIALISE_MEMBER(maxMultiviewInstanceIndex);
  SERIALISE_MEMBER(protectedNoFault);
  SERIALISE_MEMBER(maxPerSetDescriptors);
  SERIALISE_MEMBER(maxMemoryAllocationSize);
}

template <>
void Deserialise(const VkPhysicalDeviceVulkan11Properties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceVulkan12Features &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(samplerMirrorClampToEdge);
  SERIALISE_MEMBER(drawIndirectCount);
  SERIALISE_MEMBER(storageBuffer8BitAccess);
  SERIALISE_MEMBER(uniformAndStorageBuffer8BitAccess);
  SERIALISE_MEMBER(storagePushConstant8);
  SERIALISE_MEMBER(shaderBufferInt64Atomics);
  SERIALISE_MEMBER(shaderSharedInt64Atomics);
  SERIALISE_MEMBER(shaderFloat16);
  SERIALISE_MEMBER(shaderInt8);
  SERIALISE_MEMBER(descriptorIndexing);
  SERIALISE_MEMBER(shaderInputAttachmentArrayDynamicIndexing);
  SERIALISE_MEMBER(shaderUniformTexelBufferArrayDynamicIndexing);
  SERIALISE_MEMBER(shaderStorageTexelBufferArrayDynamicIndexing);
  SERIALISE_MEMBER(shaderUniformBufferArrayNonUniformIndexing);
  SERIALISE_MEMBER(shaderSampledImageArrayNonUniformIndexing);
  SERIALISE_MEMBER(shaderStorageBufferArrayNonUniformIndexing);
  SERIALISE_MEMBER(shaderStorageImageArrayNonUniformIndexing);
  SERIALISE_MEMBER(shaderInputAttachmentArrayNonUniformIndexing);
  SERIALISE_MEMBER(shaderUniformTexelBufferArrayNonUniformIndexing);
  SERIALISE_MEMBER(shaderStorageTexelBufferArrayNonUniformIndexing);
  SERIALISE_MEMBER(descriptorBindingUniformBufferUpdateAfterBind);
  SERIALISE_MEMBER(descriptorBindingSampledImageUpdateAfterBind);
  SERIALISE_MEMBER(descriptorBindingStorageImageUpdateAfterBind);
  SERIALISE_MEMBER(descriptorBindingStorageBufferUpdateAfterBind);
  SERIALISE_MEMBER(descriptorBindingUniformTexelBufferUpdateAfterBind);
  SERIALISE_MEMBER(descriptorBindingStorageTexelBufferUpdateAfterBind);
  SERIALISE_MEMBER(descriptorBindingUpdateUnusedWhilePending);
  SERIALISE_MEMBER(descriptorBindingPartiallyBound);
  SERIALISE_MEMBER(descriptorBindingVariableDescriptorCount);
  SERIALISE_MEMBER(runtimeDescriptorArray);
  SERIALISE_MEMBER(samplerFilterMinmax);
  SERIALISE_MEMBER(scalarBlockLayout);
  SERIALISE_MEMBER(imagelessFramebuffer);
  SERIALISE_MEMBER(uniformBufferStandardLayout);
  SERIALISE_MEMBER(shaderSubgroupExtendedTypes);
  SERIALISE_MEMBER(separateDepthStencilLayouts);
  SERIALISE_MEMBER(hostQueryReset);
  SERIALISE_MEMBER(timelineSemaphore);
  SERIALISE_MEMBER(bufferDeviceAddress);
  SERIALISE_MEMBER(bufferDeviceAddressCaptureReplay);
  SERIALISE_MEMBER(bufferDeviceAddressMultiDevice);
  SERIALISE_MEMBER(vulkanMemoryModel);
  SERIALISE_MEMBER(vulkanMemoryModelDeviceScope);
  SERIALISE_MEMBER(vulkanMemoryModelAvailabilityVisibilityChains);
  SERIALISE_MEMBER(shaderOutputViewportIndex);
  SERIALISE_MEMBER(shaderOutputLayer);
  SERIALISE_MEMBER(subgroupBroadcastDynamicId);
}

template <>
void Deserialise(const VkPhysicalDeviceVulkan12Features &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceVulkan12Properties &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(driverID);
  SERIALISE_MEMBER(driverName);
  SERIALISE_MEMBER(driverInfo);
  SERIALISE_MEMBER(conformanceVersion);
  SERIALISE_MEMBER(denormBehaviorIndependence);
  SERIALISE_MEMBER(roundingModeIndependence);
  SERIALISE_MEMBER(shaderSignedZeroInfNanPreserveFloat16);
  SERIALISE_MEMBER(shaderSignedZeroInfNanPreserveFloat32);
  SERIALISE_MEMBER(shaderSignedZeroInfNanPreserveFloat64);
  SERIALISE_MEMBER(shaderDenormPreserveFloat16);
  SERIALISE_MEMBER(shaderDenormPreserveFloat32);
  SERIALISE_MEMBER(shaderDenormPreserveFloat64);
  SERIALISE_MEMBER(shaderDenormFlushToZeroFloat16);
  SERIALISE_MEMBER(shaderDenormFlushToZeroFloat32);
  SERIALISE_MEMBER(shaderDenormFlushToZeroFloat64);
  SERIALISE_MEMBER(shaderRoundingModeRTEFloat16);
  SERIALISE_MEMBER(shaderRoundingModeRTEFloat32);
  SERIALISE_MEMBER(shaderRoundingModeRTEFloat64);
  SERIALISE_MEMBER(shaderRoundingModeRTZFloat16);
  SERIALISE_MEMBER(shaderRoundingModeRTZFloat32);
  SERIALISE_MEMBER(shaderRoundingModeRTZFloat64);
  SERIALISE_MEMBER(maxUpdateAfterBindDescriptorsInAllPools);
  SERIALISE_MEMBER(shaderUniformBufferArrayNonUniformIndexingNative);
  SERIALISE_MEMBER(shaderSampledImageArrayNonUniformIndexingNative);
  SERIALISE_MEMBER(shaderStorageBufferArrayNonUniformIndexingNative);
  SERIALISE_MEMBER(shaderStorageImageArrayNonUniformIndexingNative);
  SERIALISE_MEMBER(shaderInputAttachmentArrayNonUniformIndexingNative);
  SERIALISE_MEMBER(robustBufferAccessUpdateAfterBind);
  SERIALISE_MEMBER(quadDivergentImplicitLod);
  SERIALISE_MEMBER(maxPerStageDescriptorUpdateAfterBindSamplers);
  SERIALISE_MEMBER(maxPerStageDescriptorUpdateAfterBindUniformBuffers);
  SERIALISE_MEMBER(maxPerStageDescriptorUpdateAfterBindStorageBuffers);
  SERIALISE_MEMBER(maxPerStageDescriptorUpdateAfterBindSampledImages);
  SERIALISE_MEMBER(maxPerStageDescriptorUpdateAfterBindStorageImages);
  SERIALISE_MEMBER(maxPerStageDescriptorUpdateAfterBindInputAttachments);
  SERIALISE_MEMBER(maxPerStageUpdateAfterBindResources);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindSamplers);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindUniformBuffers);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindUniformBuffersDynamic);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindStorageBuffers);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindStorageBuffersDynamic);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindSampledImages);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindStorageImages);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindInputAttachments);
  SERIALISE_MEMBER_VKFLAGS(VkResolveModeFlags, supportedDepthResolveModes);
  SERIALISE_MEMBER_VKFLAGS(VkResolveModeFlags, supportedStencilResolveModes);
  SERIALISE_MEMBER(independentResolveNone);
  SERIALISE_MEMBER(independentResolve);
  SERIALISE_MEMBER(filterMinmaxSingleComponentFormats);
  SERIALISE_MEMBER(filterMinmaxImageComponentMapping);
  SERIALISE_MEMBER(maxTimelineSemaphoreValueDifference);
  SERIALISE_MEMBER_VKFLAGS(VkSampleCountFlags, framebufferIntegerColorSampleCounts);
}

template <>
void Deserialise(const VkPhysicalDeviceVulkan12Properties &el)
{
  DeserialiseNext(el.pNext);
}
template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceUniformBufferStandardLayoutFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(uniformBufferStandardLayout);
}

template <>
void Deserialise(const VkPhysicalDeviceUniformBufferStandardLayoutFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceYcbcrImageArraysFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_IMAGE_ARRAYS_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(ycbcrImageArrays);
}

template <>
void Deserialise(const VkPhysicalDeviceYcbcrImageArraysFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceTimelineSemaphoreFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(timelineSemaphore);
}

template <>
void Deserialise(const VkPhysicalDeviceTimelineSemaphoreFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceTimelineSemaphoreProperties &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(maxTimelineSemaphoreValueDifference);
}

template <>
void Deserialise(const VkPhysicalDeviceTimelineSemaphoreProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSemaphoreTypeCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(semaphoreType);
  SERIALISE_MEMBER(initialValue);
}

template <>
void Deserialise(const VkSemaphoreTypeCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkTimelineSemaphoreSubmitInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(waitSemaphoreValueCount);
  SERIALISE_MEMBER_ARRAY(pWaitSemaphoreValues, waitSemaphoreValueCount);
  SERIALISE_MEMBER(signalSemaphoreValueCount);
  SERIALISE_MEMBER_ARRAY(pSignalSemaphoreValues, signalSemaphoreValueCount);
}

template <>
void Deserialise(const VkTimelineSemaphoreSubmitInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pWaitSemaphoreValues;
  delete[] el.pSignalSemaphoreValues;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSemaphoreWaitInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(semaphoreCount);
  SERIALISE_MEMBER_ARRAY(pSemaphores, semaphoreCount).Important();
  SERIALISE_MEMBER_ARRAY(pValues, semaphoreCount).Important();
}

template <>
void Deserialise(const VkSemaphoreWaitInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pSemaphores;
  delete[] el.pValues;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSemaphoreSignalInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(semaphore).Important();
  SERIALISE_MEMBER(value).Important();
}

template <>
void Deserialise(const VkSemaphoreSignalInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDebugMarkerMarkerInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(pMarkerName).Important();
  SERIALISE_MEMBER(color);
}

template <>
void Deserialise(const VkDebugMarkerMarkerInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDebugReportCallbackCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkDebugReportFlagsEXT, flags);

  // serialise pointers as uint64 to ensure portability. We can't do much with the values apart from
  // display them
  uint64_t pfnCallback = (uint64_t)el.pfnCallback;
  uint64_t pUserData = (uint64_t)el.pUserData;
  ser.Serialise("pfnCallback"_lit, pfnCallback);
  ser.Serialise("pUserData"_lit, pUserData);
}

template <>
void Deserialise(const VkDebugReportCallbackCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDebugUtilsLabelEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(pLabelName).Important();
  SERIALISE_MEMBER(color);
}

template <>
void Deserialise(const VkDebugUtilsLabelEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDebugUtilsMessengerCallbackDataEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkDebugUtilsMessengerCallbackDataFlagsEXT, flags);
  SERIALISE_MEMBER(pMessageIdName);
  SERIALISE_MEMBER(messageIdNumber);
  SERIALISE_MEMBER(pMessage);
  SERIALISE_MEMBER(queueLabelCount);
  SERIALISE_MEMBER_ARRAY(pQueueLabels, queueLabelCount);
  SERIALISE_MEMBER(cmdBufLabelCount);
  SERIALISE_MEMBER_ARRAY(pCmdBufLabels, cmdBufLabelCount);
  SERIALISE_MEMBER(objectCount);
  SERIALISE_MEMBER_ARRAY(pObjects, objectCount);
}

template <>
void Deserialise(const VkDebugUtilsMessengerCallbackDataEXT &el)
{
  DeserialiseNext(el.pNext);

  for(uint32_t i = 0; el.pQueueLabels && i < el.queueLabelCount; i++)
    Deserialise(el.pQueueLabels[i]);
  delete[] el.pQueueLabels;
  for(uint32_t i = 0; el.pCmdBufLabels && i < el.cmdBufLabelCount; i++)
    Deserialise(el.pCmdBufLabels[i]);
  delete[] el.pCmdBufLabels;
  for(uint32_t i = 0; el.pObjects && i < el.objectCount; i++)
    Deserialise(el.pObjects[i]);
  delete[] el.pObjects;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDebugUtilsMessengerCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkDebugUtilsMessengerCreateFlagsEXT, flags);
  SERIALISE_MEMBER_VKFLAGS(VkDebugUtilsMessageSeverityFlagsEXT, messageSeverity);
  SERIALISE_MEMBER_VKFLAGS(VkDebugUtilsMessageTypeFlagsEXT, messageType);

  // serialise pointers as uint64 to ensure portability. We can't do much with the values apart from
  // display them
  uint64_t pfnUserCallback = (uint64_t)el.pfnUserCallback;
  uint64_t pUserData = (uint64_t)el.pUserData;
  ser.Serialise("pfnUserCallback"_lit, pfnUserCallback);
  ser.Serialise("pUserData"_lit, pUserData);
}

template <>
void Deserialise(const VkDebugUtilsMessengerCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

// this isn't a real vulkan type, it's our own "anything that could be in a descriptor"
// structure that
template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, DescriptorSetSlotImageInfo &el)
{
  SERIALISE_MEMBER(sampler).TypedAs("VkSampler"_lit);
  SERIALISE_MEMBER(imageView).TypedAs("VkImageView"_lit);
  SERIALISE_MEMBER(imageLayout);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, DescriptorSetSlotBufferInfo &el)
{
  SERIALISE_MEMBER(buffer).TypedAs("VkBuffer"_lit);
  SERIALISE_MEMBER(offset);
  SERIALISE_MEMBER(range);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, DescriptorSetSlot &el)
{
  // Resources in this struct are optional, because if we decided a descriptor wasn't used - we
  // might still have recorded the contents of it
  OPTIONAL_RESOURCES();

  // all members are valid because it's either NULL or pointing at an existing element, it won't
  // point to garbage.
  ser.SetStructArg(
      uint64_t(VkDescriptorImageInfoValidity::Sampler | VkDescriptorImageInfoValidity::ImageView));

  SERIALISE_MEMBER(bufferInfo).TypedAs("VkDescriptorBufferInfo"_lit);
  SERIALISE_MEMBER(imageInfo).TypedAs("VkDescriptorImageInfo"_lit);
  SERIALISE_MEMBER(texelBufferView).TypedAs("VkBufferView"_lit);

  if(ser.VersionAtLeast(0x12))
  {
    SERIALISE_MEMBER(inlineOffset).Named("InlineDataOffset"_lit);
  }
  else if(ser.IsReading())
  {
    el.inlineOffset = 0;
  }
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, ImageRegionState &el)
{
  if(ser.VersionAtLeast(0xD))
  {
    // added in 0xD
    SERIALISE_MEMBER(dstQueueFamilyIndex);
  }
  SERIALISE_MEMBER(subresourceRange);
  SERIALISE_MEMBER(oldLayout);
  SERIALISE_MEMBER(newLayout);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, ImageLayouts &el)
{
  if(ser.VersionAtLeast(0xD))
  {
    // added in 0xD
    SERIALISE_MEMBER(queueFamilyIndex);
  }
  SERIALISE_MEMBER(subresourceStates);
  SERIALISE_MEMBER(imageInfo);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, ImageSubresourceRange &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkImageAspectFlags, aspectMask);
  SERIALISE_MEMBER(baseMipLevel);
  SERIALISE_MEMBER(levelCount);
  SERIALISE_MEMBER(baseArrayLayer);
  SERIALISE_MEMBER(layerCount);
  SERIALISE_MEMBER(baseDepthSlice);
  SERIALISE_MEMBER(sliceCount);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, ImageSubresourceState &el)
{
  SERIALISE_MEMBER(oldQueueFamilyIndex);
  SERIALISE_MEMBER(newQueueFamilyIndex);
  SERIALISE_MEMBER(oldLayout);
  SERIALISE_MEMBER(newLayout);
  SERIALISE_MEMBER(refType);
}

template <class SerialiserType>
void DoSerialise(SerialiserType &ser, ImageSubresourceStateForRange &el)
{
  SERIALISE_MEMBER(range);
  SERIALISE_MEMBER(state);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, ImageState &el)
{
  SERIALISE_ELEMENT_LOCAL(imageInfo, el.GetImageInfo());

  rdcarray<ImageSubresourceStateForRange> subresourceStates;
  if(ser.IsWriting() || ser.IsStructurising())
  {
    el.subresourceStates.ToArray(subresourceStates);
  }
  SERIALISE_ELEMENT(subresourceStates);

  if(ser.IsReading())
  {
    FrameRefType maxRefType = eFrameRef_None;
    for(auto it = subresourceStates.begin(); it != subresourceStates.end(); ++it)
    {
      maxRefType = ComposeFrameRefsDisjoint(maxRefType, it->state.refType);
    }
    el = ImageState(VK_NULL_HANDLE, imageInfo, maxRefType);
    el.subresourceStates.FromArray(subresourceStates);
  }
  SERIALISE_MEMBER(oldQueueFamilyTransfers);
  SERIALISE_MEMBER(newQueueFamilyTransfers);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, ImageInfo &el)
{
  SERIALISE_MEMBER(layerCount);
  // serialise these as full 32-bit integers for backwards compatibility
  {
    uint32_t levelCount = el.levelCount;
    uint32_t sampleCount = el.sampleCount;
    SERIALISE_ELEMENT(levelCount);
    SERIALISE_ELEMENT(sampleCount);
    if(ser.IsReading())
    {
      el.levelCount = (uint16_t)levelCount;
      el.sampleCount = (uint16_t)sampleCount;
    }
  }
  SERIALISE_MEMBER(extent);
  SERIALISE_MEMBER(format);
  if(ser.VersionAtLeast(0x11))
  {
    SERIALISE_MEMBER(imageType);
    SERIALISE_MEMBER(initialLayout);
    SERIALISE_MEMBER(sharingMode);
  }

  if(ser.IsReading())
  {
    el.aspects = FormatImageAspects(el.format);
  }
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorUpdateTemplateEntry &el)
{
  SERIALISE_MEMBER(dstBinding);
  SERIALISE_MEMBER(dstArrayElement);
  SERIALISE_MEMBER(descriptorCount);
  SERIALISE_MEMBER(descriptorType);

// these fields are size_t and should not be serialised as-is. They're not used so we can just
// serialise them as uint64_t. Unfortunately this wasn't correct initially and they were
// serialised as-is making a 32-bit/64-bit incompatibility, so for older versions all we can do is
// continue to serialise them as size_t as it's impossible to know which one was used.
//
// On mac we can't compile a size_t serialise, which is good in general but makes this backwards
// compatibility a bit more annoying. We just assume a 64-bit capture.

#if DISABLED(RDOC_APPLE)
  if(ser.VersionAtLeast(0xE))
#endif
  {
    uint64_t offset = 0;
    uint64_t stride = 0;
    if(ser.IsWriting() || ser.IsStructurising())
    {
      offset = el.offset;
      stride = el.stride;
    }
    ser.Serialise("offset"_lit, offset);
    ser.Serialise("stride"_lit, stride);
    if(ser.IsReading())
    {
      el.offset = (size_t)offset;
      el.stride = (size_t)stride;
    }
  }
#if DISABLED(RDOC_APPLE)
  else
  {
    SERIALISE_MEMBER(offset);
    SERIALISE_MEMBER(stride);
  }
#endif
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorUpdateTemplateCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkDescriptorUpdateTemplateCreateFlags, flags);
  SERIALISE_MEMBER(descriptorUpdateEntryCount).Important();
  SERIALISE_MEMBER_ARRAY(pDescriptorUpdateEntries, descriptorUpdateEntryCount);
  SERIALISE_MEMBER(templateType).Important();

  if(el.templateType == VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_DESCRIPTOR_SET)
  {
    SERIALISE_MEMBER(descriptorSetLayout);
  }
  else
  {
    SERIALISE_MEMBER_EMPTY(descriptorSetLayout);
  }

  if(el.templateType == VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_PUSH_DESCRIPTORS_KHR)
  {
    SERIALISE_MEMBER(pipelineBindPoint);
    SERIALISE_MEMBER(pipelineLayout);
    SERIALISE_MEMBER(set);
  }
  else
  {
    SERIALISE_MEMBER_EMPTY(pipelineBindPoint);
    SERIALISE_MEMBER_EMPTY(pipelineLayout);
    SERIALISE_MEMBER_EMPTY(set);
  }
}

template <>
void Deserialise(const VkDescriptorUpdateTemplateCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pDescriptorUpdateEntries;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBindBufferMemoryInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(buffer).Important();
  SERIALISE_MEMBER(memory).Important();
  SERIALISE_MEMBER(memoryOffset);
}

template <>
void Deserialise(const VkBindBufferMemoryInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBindImageMemoryInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(image).Important();
  SERIALISE_MEMBER(memory).Important();
  SERIALISE_MEMBER(memoryOffset);
}

template <>
void Deserialise(const VkBindImageMemoryInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceConservativeRasterizationPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(primitiveOverestimationSize);
  SERIALISE_MEMBER(maxExtraPrimitiveOverestimationSize);
  SERIALISE_MEMBER(extraPrimitiveOverestimationSizeGranularity);
  SERIALISE_MEMBER(primitiveUnderestimation);
  SERIALISE_MEMBER(conservativePointAndLineRasterization);
  SERIALISE_MEMBER(degenerateTrianglesRasterized);
  SERIALISE_MEMBER(degenerateLinesRasterized);
  SERIALISE_MEMBER(fullyCoveredFragmentShaderInputVariable);
  SERIALISE_MEMBER(conservativeRasterizationPostDepthCoverage);
}

template <>
void Deserialise(const VkPhysicalDeviceConservativeRasterizationPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceDepthStencilResolveProperties &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkResolveModeFlags, supportedDepthResolveModes);
  SERIALISE_MEMBER_VKFLAGS(VkResolveModeFlags, supportedStencilResolveModes);
  SERIALISE_MEMBER(independentResolveNone);
  SERIALISE_MEMBER(independentResolve);
}

template <>
void Deserialise(const VkPhysicalDeviceDepthStencilResolveProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSubpassDescriptionDepthStencilResolve &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_DEPTH_STENCIL_RESOLVE);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(depthResolveMode);
  SERIALISE_MEMBER(stencilResolveMode);
  SERIALISE_MEMBER_OPT(pDepthStencilResolveAttachment);
}

template <>
void Deserialise(const VkSubpassDescriptionDepthStencilResolve &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineRasterizationConservativeStateCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineRasterizationConservativeStateCreateFlagsEXT, flags);
  SERIALISE_MEMBER(conservativeRasterizationMode);
  SERIALISE_MEMBER(extraPrimitiveOverestimationSize);
}

template <>
void Deserialise(const VkPipelineRasterizationConservativeStateCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSamplerCustomBorderColorCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_SAMPLER_CUSTOM_BORDER_COLOR_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(customBorderColor);
  SERIALISE_MEMBER(format);
}

template <>
void Deserialise(const VkSamplerCustomBorderColorCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceCustomBorderColorPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(maxCustomBorderColorSamplers);
}

template <>
void Deserialise(const VkPhysicalDeviceCustomBorderColorPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceCustomBorderColorFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(customBorderColors);
  SERIALISE_MEMBER(customBorderColorWithoutFormat);
}

template <>
void Deserialise(const VkPhysicalDeviceCustomBorderColorFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineTessellationDomainOriginStateCreateInfo &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_DOMAIN_ORIGIN_STATE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(domainOrigin);
}

template <>
void Deserialise(const VkPipelineTessellationDomainOriginStateCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageViewUsageCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkImageUsageFlags, usage);
}

template <>
void Deserialise(const VkImageViewUsageCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkInputAttachmentAspectReference &el)
{
  SERIALISE_MEMBER(subpass);
  SERIALISE_MEMBER(inputAttachmentIndex);
  SERIALISE_MEMBER_VKFLAGS(VkImageAspectFlags, aspectMask);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDevicePointClippingProperties &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_POINT_CLIPPING_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(pointClippingBehavior);
}

template <>
void Deserialise(const VkPhysicalDevicePointClippingProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkRenderPassInputAttachmentAspectCreateInfo &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_RENDER_PASS_INPUT_ATTACHMENT_ASPECT_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(aspectReferenceCount);
  SERIALISE_MEMBER_ARRAY(pAspectReferences, aspectReferenceCount);
}

template <>
void Deserialise(const VkRenderPassInputAttachmentAspectCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pAspectReferences;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkVertexInputBindingDivisorDescriptionEXT &el)
{
  SERIALISE_MEMBER(binding);
  SERIALISE_MEMBER(divisor);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(maxVertexAttribDivisor);
}

template <>
void Deserialise(const VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineVertexInputDivisorStateCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(vertexBindingDivisorCount);
  SERIALISE_MEMBER_ARRAY(pVertexBindingDivisors, vertexBindingDivisorCount);
}

template <>
void Deserialise(const VkPipelineVertexInputDivisorStateCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pVertexBindingDivisors;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(vertexAttributeInstanceRateDivisor);
  SERIALISE_MEMBER(vertexAttributeInstanceRateZeroDivisor);
}

template <>
void Deserialise(const VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDevice8BitStorageFeatures &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(storageBuffer8BitAccess);
  SERIALISE_MEMBER(uniformAndStorageBuffer8BitAccess);
  SERIALISE_MEMBER(storagePushConstant8);
}

template <>
void Deserialise(const VkPhysicalDevice8BitStorageFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDevice16BitStorageFeatures &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(storageBuffer16BitAccess);
  SERIALISE_MEMBER(uniformAndStorageBuffer16BitAccess);
  SERIALISE_MEMBER(storagePushConstant16);
  SERIALISE_MEMBER(storageInputOutput16);
}

template <>
void Deserialise(const VkPhysicalDevice16BitStorageFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceSamplerFilterMinmaxProperties &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_FILTER_MINMAX_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(filterMinmaxSingleComponentFormats);
  SERIALISE_MEMBER(filterMinmaxImageComponentMapping);
}

template <>
void Deserialise(const VkPhysicalDeviceSamplerFilterMinmaxProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSamplerReductionModeCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(reductionMode);
}

template <>
void Deserialise(const VkSamplerReductionModeCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSamplerYcbcrConversionCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(format).Important();
  SERIALISE_MEMBER(ycbcrModel).Important();
  SERIALISE_MEMBER(ycbcrRange);
  SERIALISE_MEMBER(components);
  SERIALISE_MEMBER(xChromaOffset);
  SERIALISE_MEMBER(yChromaOffset);
  SERIALISE_MEMBER(chromaFilter);
  SERIALISE_MEMBER(forceExplicitReconstruction);
}

template <>
void Deserialise(const VkSamplerYcbcrConversionCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceMaintenance3Properties &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(maxPerSetDescriptors);
  SERIALISE_MEMBER(maxMemoryAllocationSize);
}

template <>
void Deserialise(const VkPhysicalDeviceMaintenance3Properties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorSetLayoutSupport &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_SUPPORT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(supported);
}

template <>
void Deserialise(const VkDescriptorSetLayoutSupport &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkRenderPassMultiviewCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(subpassCount);
  SERIALISE_MEMBER_ARRAY(pViewMasks, subpassCount);
  SERIALISE_MEMBER(dependencyCount);
  SERIALISE_MEMBER_ARRAY(pViewOffsets, dependencyCount);
  SERIALISE_MEMBER(correlationMaskCount);
  SERIALISE_MEMBER_ARRAY(pCorrelationMasks, correlationMaskCount);
}

template <>
void Deserialise(const VkRenderPassMultiviewCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pViewMasks;
  delete[] el.pViewOffsets;
  delete[] el.pCorrelationMasks;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkRenderPassFragmentDensityMapCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_RENDER_PASS_FRAGMENT_DENSITY_MAP_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(fragmentDensityMapAttachment);
}

template <>
void Deserialise(const VkRenderPassFragmentDensityMapCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSampleLocationEXT &el)
{
  SERIALISE_MEMBER(x);
  SERIALISE_MEMBER(y);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSampleLocationsInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SAMPLE_LOCATIONS_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(sampleLocationsPerPixel);
  SERIALISE_MEMBER(sampleLocationGridSize);
  SERIALISE_MEMBER(sampleLocationsCount);
  SERIALISE_MEMBER_ARRAY(pSampleLocations, sampleLocationsCount).Important();
}

template <>
void Deserialise(const VkSampleLocationsInfoEXT &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pSampleLocations;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSubpassSampleLocationsEXT &el)
{
  SERIALISE_MEMBER(subpassIndex);
  SERIALISE_MEMBER(sampleLocationsInfo);
}

template <>
void Deserialise(const VkSubpassSampleLocationsEXT &el)
{
  Deserialise(el.sampleLocationsInfo);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkAttachmentSampleLocationsEXT &el)
{
  SERIALISE_MEMBER(attachmentIndex);
  SERIALISE_MEMBER(sampleLocationsInfo);
}

template <>
void Deserialise(const VkAttachmentSampleLocationsEXT &el)
{
  Deserialise(el.sampleLocationsInfo);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkRenderPassSampleLocationsBeginInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_RENDER_PASS_SAMPLE_LOCATIONS_BEGIN_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(attachmentInitialSampleLocationsCount);
  SERIALISE_MEMBER_ARRAY(pAttachmentInitialSampleLocations, attachmentInitialSampleLocationsCount);
  SERIALISE_MEMBER(postSubpassSampleLocationsCount);
  SERIALISE_MEMBER_ARRAY(pPostSubpassSampleLocations, postSubpassSampleLocationsCount);
}

template <>
void Deserialise(const VkRenderPassSampleLocationsBeginInfoEXT &el)
{
  DeserialiseNext(el.pNext);
  for(uint32_t j = 0;
      el.pAttachmentInitialSampleLocations && j < el.attachmentInitialSampleLocationsCount; j++)
    Deserialise(el.pAttachmentInitialSampleLocations[j]);
  delete[] el.pAttachmentInitialSampleLocations;
  for(uint32_t j = 0; el.pPostSubpassSampleLocations && j < el.postSubpassSampleLocationsCount; j++)
    Deserialise(el.pPostSubpassSampleLocations[j]);
  delete[] el.pPostSubpassSampleLocations;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceImageViewImageFormatInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_VIEW_IMAGE_FORMAT_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(imageViewType);
}

template <>
void Deserialise(const VkPhysicalDeviceImageViewImageFormatInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkFilterCubicImageViewImageFormatPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_FILTER_CUBIC_IMAGE_VIEW_IMAGE_FORMAT_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(filterCubic);
  SERIALISE_MEMBER(filterCubicMinmax);
}

template <>
void Deserialise(const VkFilterCubicImageViewImageFormatPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceExtendedDynamicStateFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(extendedDynamicState);
}

template <>
void Deserialise(const VkPhysicalDeviceExtendedDynamicStateFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceFragmentDensityMapFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(fragmentDensityMap);
  SERIALISE_MEMBER(fragmentDensityMapDynamic);
  SERIALISE_MEMBER(fragmentDensityMapNonSubsampledImages);
}

template <>
void Deserialise(const VkPhysicalDeviceFragmentDensityMapFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceHostQueryResetFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(hostQueryReset);
}

template <>
void Deserialise(const VkPhysicalDeviceHostQueryResetFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceFragmentDensityMapPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(minFragmentDensityTexelSize);
  SERIALISE_MEMBER(maxFragmentDensityTexelSize);
  SERIALISE_MEMBER(fragmentDensityInvocations);
}

template <>
void Deserialise(const VkPhysicalDeviceFragmentDensityMapPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_INTERLOCK_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(fragmentShaderSampleInterlock);
  SERIALISE_MEMBER(fragmentShaderPixelInterlock);
  SERIALISE_MEMBER(fragmentShaderShadingRateInterlock);
}

template <>
void Deserialise(const VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceMultiviewFeatures &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(multiview);
  SERIALISE_MEMBER(multiviewGeometryShader);
  SERIALISE_MEMBER(multiviewTessellationShader);
}

template <>
void Deserialise(const VkPhysicalDeviceMultiviewFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceMultiviewProperties &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(maxMultiviewViewCount);
  SERIALISE_MEMBER(maxMultiviewInstanceIndex);
}

template <>
void Deserialise(const VkPhysicalDeviceMultiviewProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType ==
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_CREATION_CACHE_CONTROL_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(pipelineCreationCacheControl);
}

template <>
void Deserialise(const VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_EXECUTABLE_PROPERTIES_FEATURES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(pipelineExecutableInfo);
}

template <>
void Deserialise(const VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PIPELINE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(pipeline);
}

template <>
void Deserialise(const VkPipelineInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineExecutablePropertiesKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_PROPERTIES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkShaderStageFlags, stages);
  SERIALISE_MEMBER(name);
  SERIALISE_MEMBER(description);
  SERIALISE_MEMBER(subgroupSize);
}

template <>
void Deserialise(const VkPipelineExecutablePropertiesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineExecutableInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(pipeline);
  SERIALISE_MEMBER(executableIndex);
}

template <>
void Deserialise(const VkPipelineExecutableInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineExecutableStatisticValueKHR &el)
{
  SERIALISE_MEMBER(b32);
  SERIALISE_MEMBER(i64);
  SERIALISE_MEMBER(u64);
  SERIALISE_MEMBER(f64);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineExecutableStatisticKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_STATISTIC_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(name);
  SERIALISE_MEMBER(description);
  SERIALISE_MEMBER(format);
  SERIALISE_MEMBER(value);
}

template <>
void Deserialise(const VkPipelineExecutableStatisticKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineExecutableInternalRepresentationKHR &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INTERNAL_REPRESENTATION_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(name);
  SERIALISE_MEMBER(description);
  SERIALISE_MEMBER(isText);

  // don't serialise size_t, otherwise capture/replay between different bit-ness won't work
  {
    uint64_t dataSize = el.dataSize;
    ser.Serialise("dataSize"_lit, dataSize);
    if(ser.IsReading())
      el.dataSize = (size_t)dataSize;
  }

  SERIALISE_MEMBER_ARRAY(pData, dataSize);
}

template <>
void Deserialise(const VkPipelineExecutableInternalRepresentationKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDevicePushDescriptorPropertiesKHR &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(maxPushDescriptors);
}

template <>
void Deserialise(const VkPhysicalDevicePushDescriptorPropertiesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceSurfaceInfo2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  // don't need the surface
  SERIALISE_MEMBER_EMPTY(surface);
}

template <>
void Deserialise(const VkPhysicalDeviceSurfaceInfo2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSurfaceCapabilitiesKHR &el)
{
  SERIALISE_MEMBER(minImageCount);
  SERIALISE_MEMBER(maxImageCount);
  SERIALISE_MEMBER(currentExtent);
  SERIALISE_MEMBER(minImageExtent);
  SERIALISE_MEMBER(maxImageExtent);
  SERIALISE_MEMBER(maxImageArrayLayers);
  SERIALISE_MEMBER_VKFLAGS(VkSurfaceTransformFlagsKHR, supportedTransforms);
  SERIALISE_MEMBER(currentTransform);
  SERIALISE_MEMBER_VKFLAGS(VkCompositeAlphaFlagsKHR, supportedCompositeAlpha);
  SERIALISE_MEMBER_VKFLAGS(VkImageUsageFlags, supportedUsageFlags);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSurfaceCapabilities2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(surfaceCapabilities);
}

template <>
void Deserialise(const VkSurfaceCapabilities2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSurfaceFormatKHR &el)
{
  SERIALISE_MEMBER(format);
  SERIALISE_MEMBER(colorSpace);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSurfaceFormat2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(surfaceFormat);
}

template <>
void Deserialise(const VkSurfaceFormat2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSurfaceProtectedCapabilitiesKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SURFACE_PROTECTED_CAPABILITIES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(supportsProtected);
}

template <>
void Deserialise(const VkSurfaceProtectedCapabilitiesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayNativeHdrSurfaceCapabilitiesAMD &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_DISPLAY_NATIVE_HDR_SURFACE_CAPABILITIES_AMD);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(localDimmingSupport);
}

template <>
void Deserialise(const VkDisplayNativeHdrSurfaceCapabilitiesAMD &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageFormatListCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_FORMAT_LIST_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(viewFormatCount);
  SERIALISE_MEMBER_ARRAY(pViewFormats, viewFormatCount);
}

template <>
void Deserialise(const VkImageFormatListCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pViewFormats;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceImageRobustnessFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ROBUSTNESS_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(robustImageAccess);
}

template <>
void Deserialise(const VkPhysicalDeviceImageRobustnessFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceImagelessFramebufferFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGELESS_FRAMEBUFFER_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(imagelessFramebuffer);
}

template <>
void Deserialise(const VkPhysicalDeviceImagelessFramebufferFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkFramebufferAttachmentsCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(attachmentImageInfoCount);
  SERIALISE_MEMBER_ARRAY(pAttachmentImageInfos, attachmentImageInfoCount);
}

template <>
void Deserialise(const VkFramebufferAttachmentsCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pAttachmentImageInfos;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkFramebufferAttachmentImageInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkImageCreateFlags, flags);
  SERIALISE_MEMBER_VKFLAGS(VkImageUsageFlags, usage);
  SERIALISE_MEMBER(width);
  SERIALISE_MEMBER(height);
  SERIALISE_MEMBER(layerCount);
  SERIALISE_MEMBER(viewFormatCount);
  SERIALISE_MEMBER_ARRAY(pViewFormats, viewFormatCount);
}

template <>
void Deserialise(const VkFramebufferAttachmentImageInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pViewFormats;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkRenderPassAttachmentBeginInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(attachmentCount);
  SERIALISE_MEMBER_ARRAY(pAttachments, attachmentCount);
}

template <>
void Deserialise(const VkRenderPassAttachmentBeginInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pAttachments;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkRectLayerKHR &el)
{
  SERIALISE_MEMBER(offset);
  SERIALISE_MEMBER(extent);
  SERIALISE_MEMBER(layer);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPresentRegionKHR &el)
{
  SERIALISE_MEMBER(rectangleCount);
  SERIALISE_MEMBER_ARRAY(pRectangles, rectangleCount);
}

template <>
void Deserialise(const VkPresentRegionKHR &el)
{
  delete[] el.pRectangles;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPresentRegionsKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PRESENT_REGIONS_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(swapchainCount);
  SERIALISE_MEMBER_ARRAY(pRegions, swapchainCount);
}

template <>
void Deserialise(const VkPresentRegionsKHR &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pRegions;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPastPresentationTimingGOOGLE &el)
{
  SERIALISE_MEMBER(presentID);
  SERIALISE_MEMBER(desiredPresentTime);
  SERIALISE_MEMBER(actualPresentTime);
  SERIALISE_MEMBER(earliestPresentTime);
  SERIALISE_MEMBER(presentMargin);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPresentTimesInfoGOOGLE &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PRESENT_TIMES_INFO_GOOGLE);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(swapchainCount);
  SERIALISE_MEMBER_ARRAY(pTimes, swapchainCount);
}

template <>
void Deserialise(const VkPresentTimesInfoGOOGLE &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pTimes;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPresentTimeGOOGLE &el)
{
  SERIALISE_MEMBER(presentID);
  SERIALISE_MEMBER(desiredPresentTime);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkRefreshCycleDurationGOOGLE &el)
{
  SERIALISE_MEMBER(refreshDuration);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceShaderCorePropertiesAMD &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_AMD);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(shaderEngineCount);
  SERIALISE_MEMBER(shaderArraysPerEngineCount);
  SERIALISE_MEMBER(computeUnitsPerShaderArray);
  SERIALISE_MEMBER(simdPerComputeUnit);
  SERIALISE_MEMBER(wavefrontsPerSimd);
  SERIALISE_MEMBER(wavefrontSize);
  SERIALISE_MEMBER(sgprsPerSimd);
  SERIALISE_MEMBER(minSgprAllocation);
  SERIALISE_MEMBER(maxSgprAllocation);
  SERIALISE_MEMBER(sgprAllocationGranularity);
  SERIALISE_MEMBER(vgprsPerSimd);
  SERIALISE_MEMBER(minVgprAllocation);
  SERIALISE_MEMBER(maxVgprAllocation);
  SERIALISE_MEMBER(vgprAllocationGranularity);
}

template <>
void Deserialise(const VkPhysicalDeviceShaderCorePropertiesAMD &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkTextureLODGatherFormatPropertiesAMD &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_TEXTURE_LOD_GATHER_FORMAT_PROPERTIES_AMD);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(supportsTextureGatherLODBiasAMD);
}

template <>
void Deserialise(const VkTextureLODGatherFormatPropertiesAMD &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageViewASTCDecodeModeEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_VIEW_ASTC_DECODE_MODE_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(decodeMode);
}

template <>
void Deserialise(const VkImageViewASTCDecodeModeEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceASTCDecodeFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ASTC_DECODE_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(decodeModeSharedExponent);
}

template <>
void Deserialise(const VkPhysicalDeviceASTCDecodeFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceBufferDeviceAddressFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(bufferDeviceAddress);
  SERIALISE_MEMBER(bufferDeviceAddressCaptureReplay);
  SERIALISE_MEMBER(bufferDeviceAddressMultiDevice);
}

template <>
void Deserialise(const VkPhysicalDeviceBufferDeviceAddressFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBufferDeviceAddressInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(buffer);
}

template <>
void Deserialise(const VkBufferDeviceAddressInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBufferOpaqueCaptureAddressCreateInfo &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_BUFFER_OPAQUE_CAPTURE_ADDRESS_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(opaqueCaptureAddress);
}

template <>
void Deserialise(const VkBufferOpaqueCaptureAddressCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryOpaqueCaptureAddressAllocateInfo &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_MEMORY_OPAQUE_CAPTURE_ADDRESS_ALLOCATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(opaqueCaptureAddress);
}

template <>
void Deserialise(const VkMemoryOpaqueCaptureAddressAllocateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceMemoryOpaqueCaptureAddressInfo &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_DEVICE_MEMORY_OPAQUE_CAPTURE_ADDRESS_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(memory);
}

template <>
void Deserialise(const VkDeviceMemoryOpaqueCaptureAddressInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBufferDeviceAddressCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(deviceAddress);
}

template <>
void Deserialise(const VkBufferDeviceAddressCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceBufferDeviceAddressFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(bufferDeviceAddress);
  SERIALISE_MEMBER(bufferDeviceAddressCaptureReplay);
  SERIALISE_MEMBER(bufferDeviceAddressMultiDevice);
}

template <>
void Deserialise(const VkPhysicalDeviceBufferDeviceAddressFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkValidationCacheCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_VALIDATION_CACHE_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkValidationCacheCreateFlagsEXT, flags);

  // don't serialise size_t, otherwise capture/replay between different bit-ness won't work
  {
    uint64_t initialDataSize = (uint64_t)el.initialDataSize;
    ser.Serialise("initialDataSize"_lit, initialDataSize);
    if(ser.IsReading())
      el.initialDataSize = (size_t)initialDataSize;
  }

  // don't serialise the data
  // SERIALISE_MEMBER_ARRAY(pInitialData, el.initialDataSize);
}

template <>
void Deserialise(const VkValidationCacheCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
  // FreeAlignedBuffer((byte *)el.pInitialData);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkShaderModuleValidationCacheCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_SHADER_MODULE_VALIDATION_CACHE_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  // we skip this, because it's unwrapped and has no ResourceId. The presence of this struct is
  // enough
  // SERIALISE_MEMBER(validationCache);
}

template <>
void Deserialise(const VkShaderModuleValidationCacheCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkValidationFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(enabledValidationFeatureCount);
  SERIALISE_MEMBER_ARRAY(pEnabledValidationFeatures, enabledValidationFeatureCount);
  SERIALISE_MEMBER(disabledValidationFeatureCount);
  SERIALISE_MEMBER_ARRAY(pDisabledValidationFeatures, disabledValidationFeatureCount);
}

template <>
void Deserialise(const VkValidationFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pEnabledValidationFeatures;
  delete[] el.pDisabledValidationFeatures;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkValidationFlagsEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(disabledValidationCheckCount);
  SERIALISE_MEMBER_ARRAY(pDisabledValidationChecks, disabledValidationCheckCount);
}

template <>
void Deserialise(const VkValidationFlagsEXT &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pDisabledValidationChecks;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceSampleLocationsPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLE_LOCATIONS_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(sampleLocationSampleCounts);
  SERIALISE_MEMBER(maxSampleLocationGridSize);
  SERIALISE_MEMBER(sampleLocationCoordinateRange);
  SERIALISE_MEMBER(sampleLocationSubPixelBits);
  SERIALISE_MEMBER(variableSampleLocations);
}

template <>
void Deserialise(const VkPhysicalDeviceSampleLocationsPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMultisamplePropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MULTISAMPLE_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(maxSampleLocationGridSize);
}

template <>
void Deserialise(const VkMultisamplePropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageStencilUsageCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_STENCIL_USAGE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(stencilUsage);
}

template <>
void Deserialise(const VkImageStencilUsageCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceSubgroupSizeControlFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(subgroupSizeControl);
  SERIALISE_MEMBER(computeFullSubgroups);
}

template <>
void Deserialise(const VkPhysicalDeviceSubgroupSizeControlFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceSubgroupSizeControlPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(minSubgroupSize);
  SERIALISE_MEMBER(maxSubgroupSize);
  SERIALISE_MEMBER(maxComputeWorkgroupSubgroups);
  SERIALISE_MEMBER_VKFLAGS(VkShaderStageFlags, requiredSubgroupSizeStages);
}

template <>
void Deserialise(const VkPhysicalDeviceSubgroupSizeControlPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineShaderStageRequiredSubgroupSizeCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType ==
                VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_REQUIRED_SUBGROUP_SIZE_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(requiredSubgroupSize);
}

template <>
void Deserialise(const VkPipelineShaderStageRequiredSubgroupSizeCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDevicePrivateDataFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIVATE_DATA_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(privateData);
}

template <>
void Deserialise(const VkPhysicalDevicePrivateDataFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDevicePrivateDataCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEVICE_PRIVATE_DATA_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(privateDataSlotRequestCount);
}

template <>
void Deserialise(const VkDevicePrivateDataCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPrivateDataSlotCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PRIVATE_DATA_SLOT_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPrivateDataSlotCreateFlagsEXT, flags);
}

template <>
void Deserialise(const VkPrivateDataSlotCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceRobustness2FeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(robustBufferAccess2);
  SERIALISE_MEMBER(robustImageAccess2);
  SERIALISE_MEMBER(nullDescriptor);
}

template <>
void Deserialise(const VkPhysicalDeviceRobustness2FeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceRobustness2PropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(robustStorageBufferAccessSizeAlignment);
  SERIALISE_MEMBER(robustUniformBufferAccessSizeAlignment);
}

template <>
void Deserialise(const VkPhysicalDeviceRobustness2PropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceTransformFeedbackFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(transformFeedback);
  SERIALISE_MEMBER(geometryStreams);
}

template <>
void Deserialise(const VkPhysicalDeviceTransformFeedbackFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceTransformFeedbackPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(maxTransformFeedbackStreams);
  SERIALISE_MEMBER(maxTransformFeedbackBuffers);
  SERIALISE_MEMBER(maxTransformFeedbackBufferSize);
  SERIALISE_MEMBER(maxTransformFeedbackStreamDataSize);
  SERIALISE_MEMBER(maxTransformFeedbackBufferDataSize);
  SERIALISE_MEMBER(maxTransformFeedbackBufferDataStride);
  SERIALISE_MEMBER(transformFeedbackQueries);
  SERIALISE_MEMBER(transformFeedbackStreamsLinesTriangles);
  SERIALISE_MEMBER(transformFeedbackRasterizationStreamSelect);
  SERIALISE_MEMBER(transformFeedbackDraw);
}

template <>
void Deserialise(const VkPhysicalDeviceTransformFeedbackPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType ==
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(shaderDemoteToHelperInvocation);
}

template <>
void Deserialise(const VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(texelBufferAlignment);
}

template <>
void Deserialise(const VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(storageTexelBufferOffsetAlignmentBytes);
  SERIALISE_MEMBER(storageTexelBufferOffsetSingleTexelAlignment);
  SERIALISE_MEMBER(uniformTexelBufferOffsetAlignmentBytes);
  SERIALISE_MEMBER(uniformTexelBufferOffsetSingleTexelAlignment);
}

template <>
void Deserialise(const VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceToolPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TOOL_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(name);
  SERIALISE_MEMBER(version);
  SERIALISE_MEMBER_VKFLAGS(VkToolPurposeFlagsEXT, purposes);
  SERIALISE_MEMBER(description);
  SERIALISE_MEMBER(layer);
}

template <>
void Deserialise(const VkPhysicalDeviceToolPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineSampleLocationsStateCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PIPELINE_SAMPLE_LOCATIONS_STATE_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(sampleLocationsEnable);
  SERIALISE_MEMBER(sampleLocationsInfo);
}

template <>
void Deserialise(const VkPipelineSampleLocationsStateCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineRasterizationStateStreamCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_STREAM_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineRasterizationStateStreamCreateFlagsEXT, flags);
  SERIALISE_MEMBER(rasterizationStream);
}

template <>
void Deserialise(const VkPipelineRasterizationStateStreamCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBufferCopy2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BUFFER_COPY_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(srcOffset);
  SERIALISE_MEMBER(dstOffset);
  SERIALISE_MEMBER(size);
}

template <>
void Deserialise(const VkBufferCopy2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkCopyBufferInfo2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(srcBuffer).Important();
  SERIALISE_MEMBER(dstBuffer).Important();
  SERIALISE_MEMBER(regionCount);
  SERIALISE_MEMBER_ARRAY(pRegions, regionCount);
}

template <>
void Deserialise(const VkCopyBufferInfo2KHR &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pRegions;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageCopy2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_COPY_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(srcSubresource);
  SERIALISE_MEMBER(srcOffset);
  SERIALISE_MEMBER(dstSubresource);
  SERIALISE_MEMBER(dstOffset);
  SERIALISE_MEMBER(extent);
}

template <>
void Deserialise(const VkImageCopy2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkCopyImageInfo2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_COPY_IMAGE_INFO_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(srcImage).Important();
  SERIALISE_MEMBER(srcImageLayout);
  SERIALISE_MEMBER(dstImage).Important();
  SERIALISE_MEMBER(dstImageLayout);
  SERIALISE_MEMBER(regionCount);
  SERIALISE_MEMBER_ARRAY(pRegions, regionCount);
}

template <>
void Deserialise(const VkCopyImageInfo2KHR &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pRegions;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBufferImageCopy2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(bufferOffset);
  SERIALISE_MEMBER(bufferRowLength);
  SERIALISE_MEMBER(bufferImageHeight);
  SERIALISE_MEMBER(imageSubresource);
  SERIALISE_MEMBER(imageOffset);
  SERIALISE_MEMBER(imageExtent);
}

template <>
void Deserialise(const VkBufferImageCopy2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkCopyBufferToImageInfo2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(srcBuffer).Important();
  SERIALISE_MEMBER(dstImage).Important();
  SERIALISE_MEMBER(dstImageLayout);
  SERIALISE_MEMBER(regionCount);
  SERIALISE_MEMBER_ARRAY(pRegions, regionCount);
}

template <>
void Deserialise(const VkCopyBufferToImageInfo2KHR &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pRegions;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkCopyImageToBufferInfo2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_COPY_IMAGE_TO_BUFFER_INFO_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(srcImage);
  SERIALISE_MEMBER(srcImageLayout);
  SERIALISE_MEMBER(dstBuffer);
  SERIALISE_MEMBER(regionCount);
  SERIALISE_MEMBER_ARRAY(pRegions, regionCount);
}

template <>
void Deserialise(const VkCopyImageToBufferInfo2KHR &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pRegions;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageBlit2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_BLIT_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(srcSubresource);
  SERIALISE_MEMBER(srcOffsets);
  SERIALISE_MEMBER(dstSubresource);
  SERIALISE_MEMBER(dstOffsets);
}

template <>
void Deserialise(const VkImageBlit2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBlitImageInfo2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(srcImage).Important();
  SERIALISE_MEMBER(srcImageLayout);
  SERIALISE_MEMBER(dstImage).Important();
  SERIALISE_MEMBER(dstImageLayout);
  SERIALISE_MEMBER(regionCount);
  SERIALISE_MEMBER_ARRAY(pRegions, regionCount);
  SERIALISE_MEMBER(filter);
}

template <>
void Deserialise(const VkBlitImageInfo2KHR &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pRegions;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageResolve2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_RESOLVE_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(srcSubresource);
  SERIALISE_MEMBER(srcOffset);
  SERIALISE_MEMBER(dstSubresource);
  SERIALISE_MEMBER(dstOffset);
  SERIALISE_MEMBER(extent);
}

template <>
void Deserialise(const VkImageResolve2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkResolveImageInfo2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_RESOLVE_IMAGE_INFO_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(srcImage).Important();
  SERIALISE_MEMBER(srcImageLayout);
  SERIALISE_MEMBER(dstImage).Important();
  SERIALISE_MEMBER(dstImageLayout);
  SERIALISE_MEMBER(regionCount);
  SERIALISE_MEMBER_ARRAY(pRegions, regionCount);
}

template <>
void Deserialise(const VkResolveImageInfo2KHR &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pRegions;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkAttachmentDescription2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkAttachmentDescriptionFlags, flags);
  SERIALISE_MEMBER(format);
  SERIALISE_MEMBER(samples);
  SERIALISE_MEMBER(loadOp);
  SERIALISE_MEMBER(storeOp);
  SERIALISE_MEMBER(stencilLoadOp);
  SERIALISE_MEMBER(stencilStoreOp);
  SERIALISE_MEMBER(initialLayout);
  SERIALISE_MEMBER(finalLayout);
}

template <>
void Deserialise(const VkAttachmentDescription2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkAttachmentReference2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(attachment).Important();
  SERIALISE_MEMBER(layout);
  SERIALISE_MEMBER_VKFLAGS(VkImageAspectFlags, aspectMask);
}

template <>
void Deserialise(const VkAttachmentReference2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSubpassDescription2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkSubpassDescriptionFlags, flags);
  SERIALISE_MEMBER(pipelineBindPoint);
  SERIALISE_MEMBER(viewMask);

  SERIALISE_MEMBER(inputAttachmentCount);
  SERIALISE_MEMBER_ARRAY(pInputAttachments, inputAttachmentCount);

  SERIALISE_MEMBER(colorAttachmentCount);
  SERIALISE_MEMBER_ARRAY(pColorAttachments, colorAttachmentCount).Important();
  SERIALISE_MEMBER_ARRAY(pResolveAttachments, colorAttachmentCount);

  SERIALISE_MEMBER_OPT(pDepthStencilAttachment).Important();

  SERIALISE_MEMBER(preserveAttachmentCount);
  SERIALISE_MEMBER_ARRAY(pPreserveAttachments, preserveAttachmentCount);
}

template <>
void Deserialise(const VkSubpassDescription2 &el)
{
  DeserialiseNext(el.pNext);

  if(el.pDepthStencilAttachment)
    Deserialise(*el.pDepthStencilAttachment);
  delete el.pDepthStencilAttachment;

  for(uint32_t j = 0; el.pColorAttachments && j < el.colorAttachmentCount; j++)
  {
    Deserialise(el.pColorAttachments[j]);
    if(el.pResolveAttachments)
      Deserialise(el.pResolveAttachments[j]);
  }
  delete[] el.pColorAttachments;
  delete[] el.pResolveAttachments;

  for(uint32_t j = 0; el.pInputAttachments && j < el.inputAttachmentCount; j++)
    Deserialise(el.pInputAttachments[j]);

  delete[] el.pInputAttachments;
  delete[] el.pPreserveAttachments;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSubpassDependency2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(srcSubpass);
  SERIALISE_MEMBER(dstSubpass);
  SERIALISE_MEMBER_VKFLAGS(VkPipelineStageFlags, srcStageMask);
  SERIALISE_MEMBER_VKFLAGS(VkPipelineStageFlags, dstStageMask);
  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags, srcAccessMask);
  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags, dstAccessMask);
  SERIALISE_MEMBER_VKFLAGS(VkDependencyFlags, dependencyFlags);
  SERIALISE_MEMBER(viewOffset);
}

template <>
void Deserialise(const VkSubpassDependency2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkRenderPassCreateInfo2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkRenderPassCreateFlags, flags);
  SERIALISE_MEMBER(attachmentCount);
  SERIALISE_MEMBER_ARRAY(pAttachments, attachmentCount).Important();
  SERIALISE_MEMBER(subpassCount);
  SERIALISE_MEMBER_ARRAY(pSubpasses, subpassCount).Important();
  SERIALISE_MEMBER(dependencyCount);
  SERIALISE_MEMBER_ARRAY(pDependencies, dependencyCount);
  SERIALISE_MEMBER(correlatedViewMaskCount);
  SERIALISE_MEMBER_ARRAY(pCorrelatedViewMasks, correlatedViewMaskCount);
}

template <>
void Deserialise(const VkRenderPassCreateInfo2 &el)
{
  DeserialiseNext(el.pNext);

  for(uint32_t i = 0; el.pAttachments && i < el.attachmentCount; i++)
    Deserialise(el.pAttachments[i]);
  delete[] el.pAttachments;

  for(uint32_t i = 0; el.pSubpasses && i < el.subpassCount; i++)
    Deserialise(el.pSubpasses[i]);
  delete[] el.pSubpasses;

  for(uint32_t i = 0; el.pDependencies && i < el.dependencyCount; i++)
    Deserialise(el.pDependencies[i]);
  delete[] el.pDependencies;

  delete[] el.pCorrelatedViewMasks;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSubpassBeginInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(contents);
}

template <>
void Deserialise(const VkSubpassBeginInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSubpassEndInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SUBPASS_END_INFO);
  SerialiseNext(ser, el.sType, el.pNext);
}

template <>
void Deserialise(const VkSubpassEndInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDispatchIndirectCommand &el)
{
  SERIALISE_MEMBER(x);
  SERIALISE_MEMBER(y);
  SERIALISE_MEMBER(z);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDrawIndirectCommand &el)
{
  SERIALISE_MEMBER(vertexCount).Important();
  SERIALISE_MEMBER(instanceCount).Important();
  SERIALISE_MEMBER(firstVertex);
  SERIALISE_MEMBER(firstInstance);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDrawIndexedIndirectCommand &el)
{
  SERIALISE_MEMBER(indexCount).Important();
  SERIALISE_MEMBER(instanceCount).Important();
  SERIALISE_MEMBER(firstIndex);
  SERIALISE_MEMBER(vertexOffset);
  SERIALISE_MEMBER(firstInstance);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceQueueInfo2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkDeviceQueueCreateFlags, flags);
  SERIALISE_MEMBER(queueFamilyIndex).Important();
  SERIALISE_MEMBER(queueIndex).Important();
}

template <>
void Deserialise(const VkDeviceQueueInfo2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceProtectedMemoryFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(protectedMemory);
}

template <>
void Deserialise(const VkPhysicalDeviceProtectedMemoryFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceProtectedMemoryProperties &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(protectedNoFault);
}

template <>
void Deserialise(const VkPhysicalDeviceProtectedMemoryProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceShaderDrawParametersFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(shaderDrawParameters);
}

template <>
void Deserialise(const VkPhysicalDeviceShaderDrawParametersFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExportMemoryAllocateInfoNV &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO_NV);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkExternalMemoryHandleTypeFlagsNV, handleTypes);
}

template <>
void Deserialise(const VkExportMemoryAllocateInfoNV &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExternalMemoryImageCreateInfoNV &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO_NV);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkExternalMemoryHandleTypeFlagsNV, handleTypes);
}

template <>
void Deserialise(const VkExternalMemoryImageCreateInfoNV &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceShaderImageFootprintFeaturesNV &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_FOOTPRINT_FEATURES_NV);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(imageFootprint);
}

template <>
void Deserialise(const VkPhysicalDeviceShaderImageFootprintFeaturesNV &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_FEATURES_NV);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(fragmentShaderBarycentric);
}

template <>
void Deserialise(const VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExternalMemoryImageCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkExternalMemoryHandleTypeFlags, handleTypes);
}

template <>
void Deserialise(const VkExternalMemoryImageCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceExternalImageFormatInfo &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_IMAGE_FORMAT_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(handleType);
}

template <>
void Deserialise(const VkPhysicalDeviceExternalImageFormatInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExternalMemoryProperties &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkExternalMemoryFeatureFlags, externalMemoryFeatures);
  SERIALISE_MEMBER_VKFLAGS(VkExternalMemoryHandleTypeFlags, exportFromImportedHandleTypes);
  SERIALISE_MEMBER_VKFLAGS(VkExternalMemoryHandleTypeFlags, compatibleHandleTypes);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExternalImageFormatProperties &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXTERNAL_IMAGE_FORMAT_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(externalMemoryProperties);
}

template <>
void Deserialise(const VkExternalImageFormatProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceExternalBufferInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_BUFFER_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkBufferCreateFlags, flags);
  SERIALISE_MEMBER_VKFLAGS(VkBufferUsageFlags, usage);
  SERIALISE_MEMBER(handleType);
}

template <>
void Deserialise(const VkPhysicalDeviceExternalBufferInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExternalBufferProperties &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXTERNAL_BUFFER_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(externalMemoryProperties);
}

template <>
void Deserialise(const VkExternalBufferProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceIDProperties &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(deviceUUID);
  SERIALISE_MEMBER(driverUUID);
  SERIALISE_MEMBER(deviceLUID);
  SERIALISE_MEMBER(deviceNodeMask);
  SERIALISE_MEMBER(deviceLUIDValid);
}

template <>
void Deserialise(const VkPhysicalDeviceIDProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExportMemoryAllocateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkExternalMemoryHandleTypeFlags, handleTypes);
}

template <>
void Deserialise(const VkExportMemoryAllocateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExternalMemoryBufferCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkExternalMemoryHandleTypeFlags, handleTypes);
}

template <>
void Deserialise(const VkExternalMemoryBufferCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImportMemoryFdInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMPORT_MEMORY_FD_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(handleType);
  SERIALISE_MEMBER_TYPED(int32_t, fd);
}

template <>
void Deserialise(const VkImportMemoryFdInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryFdPropertiesKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MEMORY_FD_PROPERTIES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(memoryTypeBits);
}

template <>
void Deserialise(const VkMemoryFdPropertiesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryGetFdInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MEMORY_GET_FD_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(memory);
  SERIALISE_MEMBER(handleType);
}

template <>
void Deserialise(const VkMemoryGetFdInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceExternalSemaphoreInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SEMAPHORE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(handleType);
}

template <>
void Deserialise(const VkPhysicalDeviceExternalSemaphoreInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExternalSemaphoreProperties &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXTERNAL_SEMAPHORE_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkExternalSemaphoreHandleTypeFlags, exportFromImportedHandleTypes);
  SERIALISE_MEMBER_VKFLAGS(VkExternalSemaphoreHandleTypeFlags, compatibleHandleTypes);
  SERIALISE_MEMBER_VKFLAGS(VkExternalSemaphoreFeatureFlags, externalSemaphoreFeatures);
}

template <>
void Deserialise(const VkExternalSemaphoreProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExportSemaphoreCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkExternalSemaphoreHandleTypeFlags, handleTypes);
}

template <>
void Deserialise(const VkExportSemaphoreCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImportSemaphoreFdInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_FD_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(semaphore);
  SERIALISE_MEMBER_VKFLAGS(VkSemaphoreImportFlags, flags);
  SERIALISE_MEMBER_VKFLAGS(VkExternalSemaphoreHandleTypeFlags, handleType);
  SERIALISE_MEMBER_TYPED(int32_t, fd);
}

template <>
void Deserialise(const VkImportSemaphoreFdInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSemaphoreGetFdInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SEMAPHORE_GET_FD_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(semaphore);
  SERIALISE_MEMBER_VKFLAGS(VkExternalSemaphoreHandleTypeFlags, handleType);
}

template <>
void Deserialise(const VkSemaphoreGetFdInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayPresentInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(srcRect);
  SERIALISE_MEMBER(dstRect);
  SERIALISE_MEMBER(persistent);
}

template <>
void Deserialise(const VkDisplayPresentInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkConformanceVersion &el)
{
  SERIALISE_MEMBER(major);
  SERIALISE_MEMBER(minor);
  SERIALISE_MEMBER(subminor);
  SERIALISE_MEMBER(patch);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceDriverProperties &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(driverID);
  SERIALISE_MEMBER(driverName);
  SERIALISE_MEMBER(driverInfo);
  SERIALISE_MEMBER(conformanceVersion);
}

template <>
void Deserialise(const VkPhysicalDeviceDriverProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceExternalFenceInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_FENCE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(handleType);
}

template <>
void Deserialise(const VkPhysicalDeviceExternalFenceInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExternalFenceProperties &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXTERNAL_FENCE_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkExternalFenceHandleTypeFlags, exportFromImportedHandleTypes);
  SERIALISE_MEMBER_VKFLAGS(VkExternalFenceHandleTypeFlags, compatibleHandleTypes);
  SERIALISE_MEMBER_VKFLAGS(VkExternalFenceFeatureFlags, externalFenceFeatures);
}

template <>
void Deserialise(const VkExternalFenceProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExportFenceCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXPORT_FENCE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkExternalFenceHandleTypeFlags, handleTypes);
}

template <>
void Deserialise(const VkExportFenceCreateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImportFenceFdInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMPORT_FENCE_FD_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(fence);
  SERIALISE_MEMBER_VKFLAGS(VkFenceImportFlags, flags);
  SERIALISE_MEMBER_VKFLAGS(VkExternalFenceHandleTypeFlags, handleType);
  SERIALISE_MEMBER_TYPED(int32_t, fd);
}

template <>
void Deserialise(const VkImportFenceFdInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkFenceGetFdInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_FENCE_GET_FD_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(handleType);
}

template <>
void Deserialise(const VkFenceGetFdInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorSetLayoutBindingFlagsCreateInfo &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(bindingCount);
  SERIALISE_MEMBER_ARRAY_VKFLAGS(VkDescriptorBindingFlags, pBindingFlags, bindingCount);
}

template <>
void Deserialise(const VkDescriptorSetLayoutBindingFlagsCreateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pBindingFlags;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceDescriptorIndexingFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(shaderInputAttachmentArrayDynamicIndexing);
  SERIALISE_MEMBER(shaderUniformTexelBufferArrayDynamicIndexing);
  SERIALISE_MEMBER(shaderStorageTexelBufferArrayDynamicIndexing);
  SERIALISE_MEMBER(shaderUniformBufferArrayNonUniformIndexing);
  SERIALISE_MEMBER(shaderSampledImageArrayNonUniformIndexing);
  SERIALISE_MEMBER(shaderStorageBufferArrayNonUniformIndexing);
  SERIALISE_MEMBER(shaderStorageImageArrayNonUniformIndexing);
  SERIALISE_MEMBER(shaderInputAttachmentArrayNonUniformIndexing);
  SERIALISE_MEMBER(shaderUniformTexelBufferArrayNonUniformIndexing);
  SERIALISE_MEMBER(shaderStorageTexelBufferArrayNonUniformIndexing);
  SERIALISE_MEMBER(descriptorBindingUniformBufferUpdateAfterBind);
  SERIALISE_MEMBER(descriptorBindingSampledImageUpdateAfterBind);
  SERIALISE_MEMBER(descriptorBindingStorageImageUpdateAfterBind);
  SERIALISE_MEMBER(descriptorBindingStorageBufferUpdateAfterBind);
  SERIALISE_MEMBER(descriptorBindingUniformTexelBufferUpdateAfterBind);
  SERIALISE_MEMBER(descriptorBindingStorageTexelBufferUpdateAfterBind);
  SERIALISE_MEMBER(descriptorBindingUpdateUnusedWhilePending);
  SERIALISE_MEMBER(descriptorBindingPartiallyBound);
  SERIALISE_MEMBER(descriptorBindingVariableDescriptorCount);
  SERIALISE_MEMBER(runtimeDescriptorArray);
}

template <>
void Deserialise(const VkPhysicalDeviceDescriptorIndexingFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceDescriptorIndexingProperties &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(maxUpdateAfterBindDescriptorsInAllPools);
  SERIALISE_MEMBER(shaderUniformBufferArrayNonUniformIndexingNative);
  SERIALISE_MEMBER(shaderSampledImageArrayNonUniformIndexingNative);
  SERIALISE_MEMBER(shaderStorageBufferArrayNonUniformIndexingNative);
  SERIALISE_MEMBER(shaderStorageImageArrayNonUniformIndexingNative);
  SERIALISE_MEMBER(shaderInputAttachmentArrayNonUniformIndexingNative);
  SERIALISE_MEMBER(robustBufferAccessUpdateAfterBind);
  SERIALISE_MEMBER(quadDivergentImplicitLod);
  SERIALISE_MEMBER(maxPerStageDescriptorUpdateAfterBindSamplers);
  SERIALISE_MEMBER(maxPerStageDescriptorUpdateAfterBindUniformBuffers);
  SERIALISE_MEMBER(maxPerStageDescriptorUpdateAfterBindStorageBuffers);
  SERIALISE_MEMBER(maxPerStageDescriptorUpdateAfterBindSampledImages);
  SERIALISE_MEMBER(maxPerStageDescriptorUpdateAfterBindStorageImages);
  SERIALISE_MEMBER(maxPerStageDescriptorUpdateAfterBindInputAttachments);
  SERIALISE_MEMBER(maxPerStageUpdateAfterBindResources);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindSamplers);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindUniformBuffers);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindUniformBuffersDynamic);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindStorageBuffers);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindStorageBuffersDynamic);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindSampledImages);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindStorageImages);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindInputAttachments);
}

template <>
void Deserialise(const VkPhysicalDeviceDescriptorIndexingProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorSetVariableDescriptorCountAllocateInfo &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(descriptorSetCount);
  SERIALISE_MEMBER_ARRAY(pDescriptorCounts, descriptorSetCount);
}

template <>
void Deserialise(const VkDescriptorSetVariableDescriptorCountAllocateInfo &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pDescriptorCounts;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorSetVariableDescriptorCountLayoutSupport &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_LAYOUT_SUPPORT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(maxVariableDescriptorCount);
}

template <>
void Deserialise(const VkDescriptorSetVariableDescriptorCountLayoutSupport &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceDiscardRectanglePropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DISCARD_RECTANGLE_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(maxDiscardRectangles);
}

template <>
void Deserialise(const VkPhysicalDeviceDiscardRectanglePropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineDiscardRectangleStateCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PIPELINE_DISCARD_RECTANGLE_STATE_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineDiscardRectangleStateCreateFlagsEXT, flags);

  SERIALISE_MEMBER(discardRectangleMode);
  SERIALISE_MEMBER(discardRectangleCount);
  SERIALISE_MEMBER_ARRAY(pDiscardRectangles, discardRectangleCount);
}

template <>
void Deserialise(const VkPipelineDiscardRectangleStateCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pDiscardRectangles;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceDepthClipEnableFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(depthClipEnable);
}

template <>
void Deserialise(const VkPhysicalDeviceDepthClipEnableFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineRasterizationDepthClipStateCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_DEPTH_CLIP_STATE_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineRasterizationDepthClipStateCreateFlagsEXT, flags);

  SERIALISE_MEMBER(depthClipEnable);
}

template <>
void Deserialise(const VkPipelineRasterizationDepthClipStateCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineCreationFeedbackEXT &el)
{
  SERIALISE_MEMBER(flags);
  SERIALISE_MEMBER(duration);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineCreationFeedbackCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_OPT(pPipelineCreationFeedback);
  SERIALISE_MEMBER(pipelineStageCreationFeedbackCount);
  SERIALISE_MEMBER_ARRAY(pPipelineStageCreationFeedbacks, pipelineStageCreationFeedbackCount);
}

template <>
void Deserialise(const VkPipelineCreationFeedbackCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
  delete el.pPipelineCreationFeedback;
  delete[] el.pPipelineStageCreationFeedbacks;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayPowerInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DISPLAY_POWER_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(powerState);
}

template <>
void Deserialise(const VkDisplayPowerInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceEventInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEVICE_EVENT_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(deviceEvent);
}

template <>
void Deserialise(const VkDeviceEventInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayEventInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DISPLAY_EVENT_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(displayEvent);
}

template <>
void Deserialise(const VkDisplayEventInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkCalibratedTimestampInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_CALIBRATED_TIMESTAMP_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(timeDomain);
}

template <>
void Deserialise(const VkCalibratedTimestampInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSwapchainCounterCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SWAPCHAIN_COUNTER_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkSurfaceCounterFlagsEXT, surfaceCounters);
}

template <>
void Deserialise(const VkSwapchainCounterCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSurfaceCapabilities2EXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(minImageCount);
  SERIALISE_MEMBER(maxImageCount);
  SERIALISE_MEMBER(currentExtent);
  SERIALISE_MEMBER(minImageExtent);
  SERIALISE_MEMBER(maxImageExtent);
  SERIALISE_MEMBER(maxImageArrayLayers);
  SERIALISE_MEMBER_VKFLAGS(VkSurfaceTransformFlagsKHR, supportedTransforms);
  SERIALISE_MEMBER(currentTransform);
  SERIALISE_MEMBER_VKFLAGS(VkCompositeAlphaFlagsKHR, supportedCompositeAlpha);
  SERIALISE_MEMBER_VKFLAGS(VkImageUsageFlags, supportedUsageFlags);
  SERIALISE_MEMBER_VKFLAGS(VkSurfaceCounterFlagsEXT, supportedSurfaceCounters);
}

template <>
void Deserialise(const VkSurfaceCapabilities2EXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDedicatedAllocationMemoryAllocateInfoNV &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_MEMORY_ALLOCATE_INFO_NV);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(image);
  SERIALISE_MEMBER(buffer);
}

template <>
void Deserialise(const VkDedicatedAllocationMemoryAllocateInfoNV &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDedicatedAllocationImageCreateInfoNV &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_IMAGE_CREATE_INFO_NV);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(dedicatedAllocation);
}

template <>
void Deserialise(const VkDedicatedAllocationImageCreateInfoNV &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDedicatedAllocationBufferCreateInfoNV &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_BUFFER_CREATE_INFO_NV);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(dedicatedAllocation);
}

template <>
void Deserialise(const VkDedicatedAllocationBufferCreateInfoNV &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryDedicatedRequirements &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(prefersDedicatedAllocation);
  SERIALISE_MEMBER(requiresDedicatedAllocation);
}

template <>
void Deserialise(const VkMemoryDedicatedRequirements &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryDedicatedAllocateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(image);
  SERIALISE_MEMBER(buffer);
}

template <>
void Deserialise(const VkMemoryDedicatedAllocateInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceQueueGlobalPriorityCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_DEVICE_QUEUE_GLOBAL_PRIORITY_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(globalPriority);
}

template <>
void Deserialise(const VkDeviceQueueGlobalPriorityCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceMemoryBudgetPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(heapBudget);
  SERIALISE_MEMBER(heapUsage);
}

template <>
void Deserialise(const VkPhysicalDeviceMemoryBudgetPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceIndexTypeUint8FeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT8_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(indexTypeUint8);
}

template <>
void Deserialise(const VkPhysicalDeviceIndexTypeUint8FeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceInlineUniformBlockFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(inlineUniformBlock);
  SERIALISE_MEMBER(descriptorBindingInlineUniformBlockUpdateAfterBind);
}

template <>
void Deserialise(const VkPhysicalDeviceInlineUniformBlockFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceInlineUniformBlockPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(maxInlineUniformBlockSize);
  SERIALISE_MEMBER(maxPerStageDescriptorInlineUniformBlocks);
  SERIALISE_MEMBER(maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks);
  SERIALISE_MEMBER(maxDescriptorSetInlineUniformBlocks);
  SERIALISE_MEMBER(maxDescriptorSetUpdateAfterBindInlineUniformBlocks);
}

template <>
void Deserialise(const VkPhysicalDeviceInlineUniformBlockPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkWriteDescriptorSetInlineUniformBlockEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(dataSize);
  SERIALISE_MEMBER_ARRAY(pData, dataSize);
}

template <>
void Deserialise(const VkWriteDescriptorSetInlineUniformBlockEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDescriptorPoolInlineUniformBlockCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_INLINE_UNIFORM_BLOCK_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(maxInlineUniformBlockBindings);
}

template <>
void Deserialise(const VkDescriptorPoolInlineUniformBlockCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceLineRasterizationFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(rectangularLines);
  SERIALISE_MEMBER(bresenhamLines);
  SERIALISE_MEMBER(smoothLines);
  SERIALISE_MEMBER(stippledRectangularLines);
  SERIALISE_MEMBER(stippledBresenhamLines);
  SERIALISE_MEMBER(stippledSmoothLines);
}

template <>
void Deserialise(const VkPhysicalDeviceLineRasterizationFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPipelineRasterizationLineStateCreateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_LINE_STATE_CREATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(lineRasterizationMode);
  SERIALISE_MEMBER(stippledLineEnable);
  SERIALISE_MEMBER(lineStippleFactor);
  SERIALISE_MEMBER(lineStipplePattern);
}

template <>
void Deserialise(const VkPipelineRasterizationLineStateCreateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceLineRasterizationPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(lineSubPixelPrecisionBits);
}

template <>
void Deserialise(const VkPhysicalDeviceLineRasterizationPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TERMINATE_INVOCATION_FEATURES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(shaderTerminateInvocation);
}

template <>
void Deserialise(const VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_EXTENDED_TYPES_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(shaderSubgroupExtendedTypes);
}

template <>
void Deserialise(const VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceMemoryPriorityFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PRIORITY_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(memoryPriority);
}

template <>
void Deserialise(const VkPhysicalDeviceMemoryPriorityFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryPriorityAllocateInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(priority);
}

template <>
void Deserialise(const VkMemoryPriorityAllocateInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDevicePCIBusInfoPropertiesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PCI_BUS_INFO_PROPERTIES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(pciDomain);
  SERIALISE_MEMBER(pciBus);
  SERIALISE_MEMBER(pciDevice);
  SERIALISE_MEMBER(pciFunction);
}

template <>
void Deserialise(const VkPhysicalDevicePCIBusInfoPropertiesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBindImagePlaneMemoryInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BIND_IMAGE_PLANE_MEMORY_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(planeAspect);
}

template <>
void Deserialise(const VkBindImagePlaneMemoryInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImagePlaneMemoryRequirementsInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_PLANE_MEMORY_REQUIREMENTS_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(planeAspect);
}

template <>
void Deserialise(const VkImagePlaneMemoryRequirementsInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceSamplerYcbcrConversionFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(samplerYcbcrConversion);
}

template <>
void Deserialise(const VkPhysicalDeviceSamplerYcbcrConversionFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSamplerYcbcrConversionImageFormatProperties &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_IMAGE_FORMAT_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(combinedImageSamplerDescriptorCount);
}

template <>
void Deserialise(const VkSamplerYcbcrConversionImageFormatProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceShaderAtomicFloatFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(shaderBufferFloat32Atomics);
  SERIALISE_MEMBER(shaderBufferFloat32AtomicAdd);
  SERIALISE_MEMBER(shaderBufferFloat64Atomics);
  SERIALISE_MEMBER(shaderBufferFloat64AtomicAdd);
  SERIALISE_MEMBER(shaderSharedFloat32Atomics);
  SERIALISE_MEMBER(shaderSharedFloat32AtomicAdd);
  SERIALISE_MEMBER(shaderSharedFloat64Atomics);
  SERIALISE_MEMBER(shaderSharedFloat64AtomicAdd);
  SERIALISE_MEMBER(shaderImageFloat32Atomics);
  SERIALISE_MEMBER(shaderImageFloat32AtomicAdd);
  SERIALISE_MEMBER(sparseImageFloat32Atomics);
  SERIALISE_MEMBER(sparseImageFloat32AtomicAdd);
}

template <>
void Deserialise(const VkPhysicalDeviceShaderAtomicFloatFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_ATOMIC_INT64_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(shaderImageInt64Atomics);
  SERIALISE_MEMBER(sparseImageInt64Atomics);
}

template <>
void Deserialise(const VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceShaderAtomicInt64Features &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(shaderBufferInt64Atomics);
  SERIALISE_MEMBER(shaderSharedInt64Atomics);
}

template <>
void Deserialise(const VkPhysicalDeviceShaderAtomicInt64Features &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceScalarBlockLayoutFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(scalarBlockLayout);
}

template <>
void Deserialise(const VkPhysicalDeviceScalarBlockLayoutFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceShaderClockFeaturesKHR &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(shaderSubgroupClock);
  SERIALISE_MEMBER(shaderDeviceClock);
}

template <>
void Deserialise(const VkPhysicalDeviceShaderClockFeaturesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceShaderFloat16Int8Features &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(shaderFloat16);
  SERIALISE_MEMBER(shaderInt8);
}

template <>
void Deserialise(const VkPhysicalDeviceShaderFloat16Int8Features &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceFloatControlsProperties &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(denormBehaviorIndependence);
  SERIALISE_MEMBER(roundingModeIndependence);
  SERIALISE_MEMBER(shaderSignedZeroInfNanPreserveFloat16);
  SERIALISE_MEMBER(shaderSignedZeroInfNanPreserveFloat32);
  SERIALISE_MEMBER(shaderSignedZeroInfNanPreserveFloat64);
  SERIALISE_MEMBER(shaderDenormPreserveFloat16);
  SERIALISE_MEMBER(shaderDenormPreserveFloat32);
  SERIALISE_MEMBER(shaderDenormPreserveFloat64);
  SERIALISE_MEMBER(shaderDenormFlushToZeroFloat16);
  SERIALISE_MEMBER(shaderDenormFlushToZeroFloat32);
  SERIALISE_MEMBER(shaderDenormFlushToZeroFloat64);
  SERIALISE_MEMBER(shaderRoundingModeRTEFloat16);
  SERIALISE_MEMBER(shaderRoundingModeRTEFloat32);
  SERIALISE_MEMBER(shaderRoundingModeRTEFloat64);
  SERIALISE_MEMBER(shaderRoundingModeRTZFloat16);
  SERIALISE_MEMBER(shaderRoundingModeRTZFloat32);
  SERIALISE_MEMBER(shaderRoundingModeRTZFloat64);
}

template <>
void Deserialise(const VkPhysicalDeviceFloatControlsProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSamplerYcbcrConversionInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(conversion);
}

template <>
void Deserialise(const VkSamplerYcbcrConversionInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceGroupSwapchainCreateInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEVICE_GROUP_SWAPCHAIN_CREATE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkDeviceGroupPresentModeFlagsKHR, modes);
}

template <>
void Deserialise(const VkDeviceGroupSwapchainCreateInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBindImageMemorySwapchainInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_SWAPCHAIN_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(swapchain);
  SERIALISE_MEMBER(imageIndex);
}

template <>
void Deserialise(const VkBindImageMemorySwapchainInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceGroupPresentCapabilitiesKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_CAPABILITIES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(presentMask);
  SERIALISE_MEMBER_VKFLAGS(VkDeviceGroupPresentModeFlagsKHR, modes);
}

template <>
void Deserialise(const VkDeviceGroupPresentCapabilitiesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceGroupPresentInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(swapchainCount);
  SERIALISE_MEMBER_ARRAY(pDeviceMasks, swapchainCount);
  SERIALISE_MEMBER_VKFLAGS(VkDeviceGroupPresentModeFlagsKHR, mode);
}

template <>
void Deserialise(const VkDeviceGroupPresentInfoKHR &el)
{
  DeserialiseNext(el.pNext);

  delete[] el.pDeviceMasks;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceGroupProperties &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GROUP_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  if(ser.IsReading())
  {
    for(uint32_t i = 0; i < VK_MAX_DEVICE_GROUP_SIZE; i++)
      el.physicalDevices[i] = VK_NULL_HANDLE;
  }

  SERIALISE_MEMBER(physicalDeviceCount);
  // manual call to Serialise to ensure we only serialise the number of devices, but also don't
  // allocate memory
  VkPhysicalDevice *devs = el.physicalDevices;
  ser.Serialise("physicalDevices"_lit, devs, (uint64_t)el.physicalDeviceCount,
                SerialiserFlags::NoFlags);
  SERIALISE_MEMBER(subsetAllocation);
}

template <>
void Deserialise(const VkPhysicalDeviceGroupProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceGroupDeviceCreateInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(physicalDeviceCount);
  SERIALISE_MEMBER_ARRAY(pPhysicalDevices, physicalDeviceCount);
}

template <>
void Deserialise(const VkDeviceGroupDeviceCreateInfo &el)
{
  DeserialiseNext(el.pNext);

  delete[] el.pPhysicalDevices;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageSwapchainCreateInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_SWAPCHAIN_CREATE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(swapchain);
}

template <>
void Deserialise(const VkImageSwapchainCreateInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBindImageMemoryDeviceGroupInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_DEVICE_GROUP_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(deviceIndexCount);
  SERIALISE_MEMBER_ARRAY(pDeviceIndices, deviceIndexCount);
  SERIALISE_MEMBER(splitInstanceBindRegionCount);
  SERIALISE_MEMBER_ARRAY(pSplitInstanceBindRegions, splitInstanceBindRegionCount);
}

template <>
void Deserialise(const VkBindImageMemoryDeviceGroupInfo &el)
{
  DeserialiseNext(el.pNext);

  delete[] el.pDeviceIndices;
  delete[] el.pSplitInstanceBindRegions;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBindBufferMemoryDeviceGroupInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_DEVICE_GROUP_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(deviceIndexCount);
  SERIALISE_MEMBER_ARRAY(pDeviceIndices, deviceIndexCount);
}

template <>
void Deserialise(const VkBindBufferMemoryDeviceGroupInfo &el)
{
  DeserialiseNext(el.pNext);

  delete[] el.pDeviceIndices;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceGroupBindSparseInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEVICE_GROUP_BIND_SPARSE_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(resourceDeviceIndex);
  SERIALISE_MEMBER(memoryDeviceIndex);
}

template <>
void Deserialise(const VkDeviceGroupBindSparseInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceGroupSubmitInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEVICE_GROUP_SUBMIT_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(waitSemaphoreCount);
  SERIALISE_MEMBER_ARRAY(pWaitSemaphoreDeviceIndices, waitSemaphoreCount);
  SERIALISE_MEMBER(commandBufferCount);
  SERIALISE_MEMBER_ARRAY(pCommandBufferDeviceMasks, commandBufferCount);
  SERIALISE_MEMBER(signalSemaphoreCount);
  SERIALISE_MEMBER_ARRAY(pSignalSemaphoreDeviceIndices, signalSemaphoreCount);
}

template <>
void Deserialise(const VkDeviceGroupSubmitInfo &el)
{
  DeserialiseNext(el.pNext);

  delete[] el.pWaitSemaphoreDeviceIndices;
  delete[] el.pCommandBufferDeviceMasks;
  delete[] el.pSignalSemaphoreDeviceIndices;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceGroupCommandBufferBeginInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEVICE_GROUP_COMMAND_BUFFER_BEGIN_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(deviceMask);
}

template <>
void Deserialise(const VkDeviceGroupCommandBufferBeginInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDeviceGroupRenderPassBeginInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(deviceMask);
  SERIALISE_MEMBER(deviceRenderAreaCount);
  SERIALISE_MEMBER_ARRAY(pDeviceRenderAreas, deviceRenderAreaCount);
}

template <>
void Deserialise(const VkDeviceGroupRenderPassBeginInfo &el)
{
  DeserialiseNext(el.pNext);

  delete[] el.pDeviceRenderAreas;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryBarrier2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MEMORY_BARRIER_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineStageFlags2KHR, srcStageMask);
  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags2KHR, srcAccessMask);
  SERIALISE_MEMBER_VKFLAGS(VkPipelineStageFlags2KHR, dstStageMask);
  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags2KHR, dstAccessMask);
}

template <>
void Deserialise(const VkMemoryBarrier2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBufferMemoryBarrier2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineStageFlags2KHR, srcStageMask);
  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags2KHR, srcAccessMask);
  SERIALISE_MEMBER_VKFLAGS(VkPipelineStageFlags2KHR, dstStageMask);
  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags2KHR, dstAccessMask);
  // serialise as signed because then QUEUE_FAMILY_IGNORED is -1 and queue
  // family index won't be legitimately larger than 2 billion
  SERIALISE_MEMBER_TYPED(int32_t, srcQueueFamilyIndex);
  SERIALISE_MEMBER_TYPED(int32_t, dstQueueFamilyIndex);
  SERIALISE_MEMBER(buffer).Important();
  SERIALISE_MEMBER(offset);
  SERIALISE_MEMBER(size);
}

template <>
void Deserialise(const VkBufferMemoryBarrier2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageMemoryBarrier2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPipelineStageFlags2KHR, srcStageMask);
  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags2KHR, srcAccessMask);
  SERIALISE_MEMBER_VKFLAGS(VkPipelineStageFlags2KHR, dstStageMask);
  SERIALISE_MEMBER_VKFLAGS(VkAccessFlags2KHR, dstAccessMask);
  SERIALISE_MEMBER(oldLayout);
  SERIALISE_MEMBER(newLayout);
  // serialise as signed because then QUEUE_FAMILY_IGNORED is -1 and queue
  // family index won't be legitimately larger than 2 billion
  SERIALISE_MEMBER_TYPED(int32_t, srcQueueFamilyIndex);
  SERIALISE_MEMBER_TYPED(int32_t, dstQueueFamilyIndex);
  SERIALISE_MEMBER(image).Important();
  SERIALISE_MEMBER(subresourceRange);
}

template <>
void Deserialise(const VkImageMemoryBarrier2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceSynchronization2FeaturesKHR &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(synchronization2);
}

template <>
void Deserialise(const VkPhysicalDeviceSynchronization2FeaturesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSemaphoreSubmitInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(semaphore);
  SERIALISE_MEMBER(value);
  SERIALISE_MEMBER_VKFLAGS(VkPipelineStageFlags2KHR, stageMask);
  SERIALISE_MEMBER(deviceIndex);
}

template <>
void Deserialise(const VkSemaphoreSubmitInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkCommandBufferSubmitInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(commandBuffer).Important();
  SERIALISE_MEMBER(deviceMask);
}

template <>
void Deserialise(const VkCommandBufferSubmitInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSubmitInfo2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkSubmitFlagsKHR, flags);
  SERIALISE_MEMBER(waitSemaphoreInfoCount);
  SERIALISE_MEMBER_ARRAY(pWaitSemaphoreInfos, waitSemaphoreInfoCount);
  SERIALISE_MEMBER(commandBufferInfoCount);
  SERIALISE_MEMBER_ARRAY(pCommandBufferInfos, commandBufferInfoCount).Important();
  SERIALISE_MEMBER(signalSemaphoreInfoCount);
  SERIALISE_MEMBER_ARRAY(pSignalSemaphoreInfos, signalSemaphoreInfoCount);
}

template <>
void Deserialise(const VkSubmitInfo2KHR &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pWaitSemaphoreInfos;
  delete[] el.pCommandBufferInfos;
  delete[] el.pSignalSemaphoreInfos;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDependencyInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  // mark this as unimportant so even if somehow there are no barriers at all, we won't in-line all
  // the struct overhead
  SERIALISE_MEMBER_VKFLAGS(VkDependencyFlags, dependencyFlags).Unimportant();
  SERIALISE_MEMBER(memoryBarrierCount);
  // memory barriers don't have anything important, just list the number of global memory barriers
  if(el.memoryBarrierCount > 0)
    ser.Important();
  SERIALISE_MEMBER_ARRAY(pMemoryBarriers, memoryBarrierCount);
  SERIALISE_MEMBER(bufferMemoryBarrierCount);
  SERIALISE_MEMBER_ARRAY(pBufferMemoryBarriers, bufferMemoryBarrierCount);
  if(el.bufferMemoryBarrierCount > 0)
    ser.Important();
  SERIALISE_MEMBER(imageMemoryBarrierCount);
  SERIALISE_MEMBER_ARRAY(pImageMemoryBarriers, imageMemoryBarrierCount);
  if(el.imageMemoryBarrierCount > 0)
    ser.Important();
}

template <>
void Deserialise(const VkDependencyInfoKHR &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pMemoryBarriers;
  delete[] el.pBufferMemoryBarriers;
  delete[] el.pImageMemoryBarriers;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkHdrMetadataEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_HDR_METADATA_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(displayPrimaryRed);
  SERIALISE_MEMBER(displayPrimaryGreen);
  SERIALISE_MEMBER(displayPrimaryBlue);
  SERIALISE_MEMBER(whitePoint);
  SERIALISE_MEMBER(maxLuminance);
  SERIALISE_MEMBER(minLuminance);
  SERIALISE_MEMBER(maxContentLightLevel);
  SERIALISE_MEMBER(maxFrameAverageLightLevel);
}

template <>
void Deserialise(const VkHdrMetadataEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkXYColorEXT &el)
{
  SERIALISE_MEMBER(x);
  SERIALISE_MEMBER(y);
}

template <>
void Deserialise(const VkXYColorEXT &el)
{
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryAllocateFlagsInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkMemoryAllocateFlags, flags);
  SERIALISE_MEMBER(deviceMask);
}

template <>
void Deserialise(const VkMemoryAllocateFlagsInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceSubgroupProperties &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(subgroupSize);
  SERIALISE_MEMBER_VKFLAGS(VkShaderStageFlags, supportedStages);
  SERIALISE_MEMBER_VKFLAGS(VkSubgroupFeatureFlags, supportedOperations);
  SERIALISE_MEMBER(quadOperationsInAllStages);
}

template <>
void Deserialise(const VkPhysicalDeviceSubgroupProperties &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkProtectedSubmitInfo &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PROTECTED_SUBMIT_INFO);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(protectedSubmit);
}

template <>
void Deserialise(const VkProtectedSubmitInfo &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayPlaneInfo2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DISPLAY_PLANE_INFO_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  // don't serialise unwrapped VkDisplayModeKHR
  // SERIALISE_MEMBER(mode);
  SERIALISE_MEMBER(planeIndex);
}

template <>
void Deserialise(const VkDisplayPlaneInfo2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayPlaneCapabilitiesKHR &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkDisplayPlaneAlphaFlagsKHR, supportedAlpha);
  SERIALISE_MEMBER(minSrcPosition);
  SERIALISE_MEMBER(maxSrcPosition);
  SERIALISE_MEMBER(minSrcExtent);
  SERIALISE_MEMBER(maxSrcExtent);
  SERIALISE_MEMBER(minDstPosition);
  SERIALISE_MEMBER(maxDstPosition);
  SERIALISE_MEMBER(minDstExtent);
  SERIALISE_MEMBER(maxDstExtent);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayPlaneCapabilities2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DISPLAY_PLANE_CAPABILITIES_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(capabilities);
}

template <>
void Deserialise(const VkDisplayPlaneCapabilities2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayPropertiesKHR &el)
{
  // don't serialise unwrapped VkDisplayKHR
  // SERIALISE_MEMBER(display);
  SERIALISE_MEMBER(displayName);
  SERIALISE_MEMBER(physicalDimensions);
  SERIALISE_MEMBER(physicalResolution);
  SERIALISE_MEMBER_VKFLAGS(VkSurfaceTransformFlagsKHR, supportedTransforms);
  SERIALISE_MEMBER(planeReorderPossible);
  SERIALISE_MEMBER(persistentContent);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayProperties2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DISPLAY_PROPERTIES_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(displayProperties);
}

template <>
void Deserialise(const VkDisplayProperties2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayPlanePropertiesKHR &el)
{
  // don't serialise unwrapped VkDisplayKHR
  // SERIALISE_MEMBER(currentDisplay);
  SERIALISE_MEMBER(currentStackIndex);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayPlaneProperties2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DISPLAY_PLANE_PROPERTIES_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(displayPlaneProperties);
}

template <>
void Deserialise(const VkDisplayPlaneProperties2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayModeParametersKHR &el)
{
  SERIALISE_MEMBER(visibleRegion);
  SERIALISE_MEMBER(refreshRate);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayModePropertiesKHR &el)
{
  // don't serialise unwrapped VkDisplayModeKHR
  // SERIALISE_MEMBER(displayMode);
  SERIALISE_MEMBER(parameters);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkDisplayModeProperties2KHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_DISPLAY_MODE_PROPERTIES_2_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(displayModeProperties);
}

template <>
void Deserialise(const VkDisplayModeProperties2KHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkBufferMemoryRequirementsInfo2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(buffer);
}

template <>
void Deserialise(const VkBufferMemoryRequirementsInfo2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageMemoryRequirementsInfo2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(image);
}

template <>
void Deserialise(const VkImageMemoryRequirementsInfo2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageSparseMemoryRequirementsInfo2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_SPARSE_MEMORY_REQUIREMENTS_INFO_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(image);
}

template <>
void Deserialise(const VkImageSparseMemoryRequirementsInfo2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryRequirements2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(memoryRequirements);
}

template <>
void Deserialise(const VkMemoryRequirements2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSparseImageMemoryRequirements &el)
{
  SERIALISE_MEMBER(formatProperties);
  SERIALISE_MEMBER(imageMipTailFirstLod);
  SERIALISE_MEMBER(imageMipTailSize);
  SERIALISE_MEMBER(imageMipTailOffset);
  SERIALISE_MEMBER(imageMipTailStride);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSparseImageMemoryRequirements2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SPARSE_IMAGE_MEMORY_REQUIREMENTS_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(memoryRequirements);
}

template <>
void Deserialise(const VkSparseImageMemoryRequirements2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceFeatures2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(features);
}

template <>
void Deserialise(const VkPhysicalDeviceFeatures2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceProperties2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(properties);
}

template <>
void Deserialise(const VkPhysicalDeviceProperties2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkFormatProperties &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkFormatFeatureFlags, linearTilingFeatures);
  SERIALISE_MEMBER_VKFLAGS(VkFormatFeatureFlags, optimalTilingFeatures);
  SERIALISE_MEMBER_VKFLAGS(VkFormatFeatureFlags, bufferFeatures);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkFormatProperties2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(formatProperties);
}

template <>
void Deserialise(const VkFormatProperties2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageFormatProperties &el)
{
  SERIALISE_MEMBER(maxExtent);
  SERIALISE_MEMBER(maxMipLevels);
  SERIALISE_MEMBER(maxArrayLayers);
  SERIALISE_MEMBER_VKFLAGS(VkSampleCountFlags, sampleCounts);
  SERIALISE_MEMBER(maxResourceSize);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImageFormatProperties2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMAGE_FORMAT_PROPERTIES_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(imageFormatProperties);
}

template <>
void Deserialise(const VkImageFormatProperties2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceImageFormatInfo2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_FORMAT_INFO_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(format);
  SERIALISE_MEMBER(type);
  SERIALISE_MEMBER(tiling);
  SERIALISE_MEMBER_VKFLAGS(VkImageUsageFlags, usage);
  SERIALISE_MEMBER_VKFLAGS(VkImageCreateFlags, flags);
}

template <>
void Deserialise(const VkPhysicalDeviceImageFormatInfo2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkQueueFamilyProperties2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(queueFamilyProperties);
}

template <>
void Deserialise(const VkQueueFamilyProperties2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceMemoryProperties2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(memoryProperties);
}

template <>
void Deserialise(const VkPhysicalDeviceMemoryProperties2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSparseImageFormatProperties &el)
{
  SERIALISE_MEMBER_VKFLAGS(VkImageAspectFlags, aspectMask);
  SERIALISE_MEMBER(imageGranularity);
  SERIALISE_MEMBER_VKFLAGS(VkSparseImageFormatFlags, flags);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSparseImageFormatProperties2 &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SPARSE_IMAGE_FORMAT_PROPERTIES_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(properties);
}

template <>
void Deserialise(const VkSparseImageFormatProperties2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceSparseImageFormatInfo2 &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SPARSE_IMAGE_FORMAT_INFO_2);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(format);
  SERIALISE_MEMBER(type);
  SERIALISE_MEMBER(samples);
  SERIALISE_MEMBER_VKFLAGS(VkImageUsageFlags, usage);
  SERIALISE_MEMBER(tiling);
}

template <>
void Deserialise(const VkPhysicalDeviceSparseImageFormatInfo2 &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkCommandBufferInheritanceConditionalRenderingInfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_CONDITIONAL_RENDERING_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(conditionalRenderingEnable);
}

template <>
void Deserialise(const VkCommandBufferInheritanceConditionalRenderingInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceConditionalRenderingFeaturesEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(conditionalRendering);
  SERIALISE_MEMBER(inheritedConditionalRendering);
}

template <>
void Deserialise(const VkPhysicalDeviceConditionalRenderingFeaturesEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkConditionalRenderingBeginInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_CONDITIONAL_RENDERING_BEGIN_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(buffer).Important();
  SERIALISE_MEMBER(offset);
  SERIALISE_MEMBER_VKFLAGS(VkConditionalRenderingFlagsEXT, flags);
}

template <>
void Deserialise(const VkConditionalRenderingBeginInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceCoherentMemoryFeaturesAMD &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COHERENT_MEMORY_FEATURES_AMD);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(deviceCoherentMemory);
}

template <>
void Deserialise(const VkPhysicalDeviceCoherentMemoryFeaturesAMD &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceComputeShaderDerivativesFeaturesNV &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_FEATURES_NV);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(computeDerivativeGroupQuads);
  SERIALISE_MEMBER(computeDerivativeGroupLinear);
}

template <>
void Deserialise(const VkPhysicalDeviceComputeShaderDerivativesFeaturesNV &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(separateDepthStencilLayouts);
}

template <>
void Deserialise(const VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkAttachmentDescriptionStencilLayout &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_STENCIL_LAYOUT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(stencilInitialLayout);
  SERIALISE_MEMBER(stencilFinalLayout);
}

template <>
void Deserialise(const VkAttachmentDescriptionStencilLayout &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkAttachmentReferenceStencilLayout &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_STENCIL_LAYOUT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(stencilLayout);
}

template <>
void Deserialise(const VkAttachmentReferenceStencilLayout &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPerformanceCounterKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PERFORMANCE_COUNTER_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(unit);
  SERIALISE_MEMBER(scope);
  SERIALISE_MEMBER(storage);
  SERIALISE_MEMBER(uuid);
}

template <>
void Deserialise(const VkPerformanceCounterKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPerformanceCounterDescriptionKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PERFORMANCE_COUNTER_DESCRIPTION_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkPerformanceCounterDescriptionFlagsKHR, flags);
  SERIALISE_MEMBER(name);
  SERIALISE_MEMBER(category);
  SERIALISE_MEMBER(description);
}

template <>
void Deserialise(const VkPerformanceCounterDescriptionKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDevicePerformanceQueryFeaturesKHR &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_FEATURES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(performanceCounterQueryPools);
  SERIALISE_MEMBER(performanceCounterMultipleQueryPools);
}

template <>
void Deserialise(const VkPhysicalDevicePerformanceQueryFeaturesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPhysicalDevicePerformanceQueryPropertiesKHR &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_PROPERTIES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(allowCommandBufferQueryCopies);
}

template <>
void Deserialise(const VkPhysicalDevicePerformanceQueryPropertiesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkQueryPoolPerformanceCreateInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_QUERY_POOL_PERFORMANCE_CREATE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(queueFamilyIndex);
  SERIALISE_MEMBER(counterIndexCount);
  SERIALISE_MEMBER_ARRAY(pCounterIndices, counterIndexCount);
}

template <>
void Deserialise(const VkQueryPoolPerformanceCreateInfoKHR &el)
{
  delete[] el.pCounterIndices;
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkAcquireProfilingLockInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_ACQUIRE_PROFILING_LOCK_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkAcquireProfilingLockFlagsKHR, flags);
  SERIALISE_MEMBER(timeout);
}

template <>
void Deserialise(const VkAcquireProfilingLockInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPerformanceQuerySubmitInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PERFORMANCE_QUERY_SUBMIT_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(counterPassIndex);
}

template <>
void Deserialise(const VkPerformanceQuerySubmitInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

// pNext structs - always have deserialise for the next chain
INSTANTIATE_SERIALISE_TYPE(VkAcquireNextImageInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkAcquireProfilingLockInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkApplicationInfo);
INSTANTIATE_SERIALISE_TYPE(VkAttachmentDescription2);
INSTANTIATE_SERIALISE_TYPE(VkAttachmentDescriptionStencilLayoutKHR);
INSTANTIATE_SERIALISE_TYPE(VkAttachmentReference2);
INSTANTIATE_SERIALISE_TYPE(VkAttachmentReferenceStencilLayoutKHR);
INSTANTIATE_SERIALISE_TYPE(VkAttachmentSampleLocationsEXT);
INSTANTIATE_SERIALISE_TYPE(VkBindBufferMemoryDeviceGroupInfo);
INSTANTIATE_SERIALISE_TYPE(VkBindBufferMemoryInfo);
INSTANTIATE_SERIALISE_TYPE(VkBindImageMemoryDeviceGroupInfo);
INSTANTIATE_SERIALISE_TYPE(VkBindImageMemoryInfo);
INSTANTIATE_SERIALISE_TYPE(VkBindImageMemorySwapchainInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkBindImagePlaneMemoryInfo);
INSTANTIATE_SERIALISE_TYPE(VkBindSparseInfo);
INSTANTIATE_SERIALISE_TYPE(VkBlitImageInfo2KHR);
INSTANTIATE_SERIALISE_TYPE(VkBufferCopy2KHR);
INSTANTIATE_SERIALISE_TYPE(VkBufferCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkBufferDeviceAddressCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkBufferDeviceAddressInfo);
INSTANTIATE_SERIALISE_TYPE(VkBufferImageCopy2KHR);
INSTANTIATE_SERIALISE_TYPE(VkBufferMemoryBarrier);
INSTANTIATE_SERIALISE_TYPE(VkBufferMemoryBarrier2KHR);
INSTANTIATE_SERIALISE_TYPE(VkBufferMemoryRequirementsInfo2);
INSTANTIATE_SERIALISE_TYPE(VkBufferOpaqueCaptureAddressCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkBufferViewCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkCalibratedTimestampInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkCommandBufferAllocateInfo);
INSTANTIATE_SERIALISE_TYPE(VkCommandBufferBeginInfo);
INSTANTIATE_SERIALISE_TYPE(VkCommandBufferInheritanceConditionalRenderingInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkCommandBufferInheritanceInfo);
INSTANTIATE_SERIALISE_TYPE(VkCommandBufferSubmitInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkCommandPoolCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkComputePipelineCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkConditionalRenderingBeginInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkCopyBufferInfo2KHR);
INSTANTIATE_SERIALISE_TYPE(VkCopyBufferToImageInfo2KHR);
INSTANTIATE_SERIALISE_TYPE(VkCopyDescriptorSet);
INSTANTIATE_SERIALISE_TYPE(VkCopyImageInfo2KHR);
INSTANTIATE_SERIALISE_TYPE(VkCopyImageToBufferInfo2KHR);
INSTANTIATE_SERIALISE_TYPE(VkDebugMarkerMarkerInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkDebugMarkerObjectNameInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkDebugMarkerObjectTagInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkDebugReportCallbackCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkDebugUtilsLabelEXT);
INSTANTIATE_SERIALISE_TYPE(VkDebugUtilsMessengerCallbackDataEXT);
INSTANTIATE_SERIALISE_TYPE(VkDebugUtilsMessengerCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkDebugUtilsObjectNameInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkDebugUtilsObjectTagInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkDedicatedAllocationBufferCreateInfoNV);
INSTANTIATE_SERIALISE_TYPE(VkDedicatedAllocationImageCreateInfoNV);
INSTANTIATE_SERIALISE_TYPE(VkDedicatedAllocationMemoryAllocateInfoNV);
INSTANTIATE_SERIALISE_TYPE(VkDependencyInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkDescriptorPoolCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkDescriptorSetAllocateInfo);
INSTANTIATE_SERIALISE_TYPE(VkDescriptorSetLayoutBindingFlagsCreateInfo)
INSTANTIATE_SERIALISE_TYPE(VkDescriptorSetLayoutCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkDescriptorSetLayoutSupport);
INSTANTIATE_SERIALISE_TYPE(VkDescriptorSetVariableDescriptorCountAllocateInfo)
INSTANTIATE_SERIALISE_TYPE(VkDescriptorSetVariableDescriptorCountLayoutSupport)
INSTANTIATE_SERIALISE_TYPE(VkDescriptorUpdateTemplateCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkDeviceCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkDeviceEventInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkDeviceGroupBindSparseInfo);
INSTANTIATE_SERIALISE_TYPE(VkDeviceGroupCommandBufferBeginInfo);
INSTANTIATE_SERIALISE_TYPE(VkDeviceGroupDeviceCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkDeviceGroupPresentCapabilitiesKHR);
INSTANTIATE_SERIALISE_TYPE(VkDeviceGroupPresentInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkDeviceGroupRenderPassBeginInfo);
INSTANTIATE_SERIALISE_TYPE(VkDeviceGroupSubmitInfo);
INSTANTIATE_SERIALISE_TYPE(VkDeviceGroupSwapchainCreateInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkDeviceMemoryOpaqueCaptureAddressInfo);
INSTANTIATE_SERIALISE_TYPE(VkDevicePrivateDataCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkDeviceQueueCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkDeviceQueueGlobalPriorityCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkDeviceQueueInfo2);
INSTANTIATE_SERIALISE_TYPE(VkDisplayEventInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkDisplayModeProperties2KHR);
INSTANTIATE_SERIALISE_TYPE(VkDisplayNativeHdrSurfaceCapabilitiesAMD);
INSTANTIATE_SERIALISE_TYPE(VkDisplayPlaneCapabilities2KHR);
INSTANTIATE_SERIALISE_TYPE(VkDisplayPlaneInfo2KHR);
INSTANTIATE_SERIALISE_TYPE(VkDisplayPlaneProperties2KHR);
INSTANTIATE_SERIALISE_TYPE(VkDisplayPowerInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkDisplayPresentInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkDisplayProperties2KHR);
INSTANTIATE_SERIALISE_TYPE(VkEventCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkExportFenceCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkExportMemoryAllocateInfo);
INSTANTIATE_SERIALISE_TYPE(VkExportMemoryAllocateInfoNV);
INSTANTIATE_SERIALISE_TYPE(VkExportSemaphoreCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkExternalBufferProperties);
INSTANTIATE_SERIALISE_TYPE(VkExternalFenceProperties);
INSTANTIATE_SERIALISE_TYPE(VkExternalImageFormatProperties);
INSTANTIATE_SERIALISE_TYPE(VkExternalMemoryBufferCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkExternalMemoryImageCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkExternalMemoryImageCreateInfoNV);
INSTANTIATE_SERIALISE_TYPE(VkExternalSemaphoreProperties);
INSTANTIATE_SERIALISE_TYPE(VkFenceCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkFenceGetFdInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkFilterCubicImageViewImageFormatPropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkFormatProperties2);
INSTANTIATE_SERIALISE_TYPE(VkFramebufferAttachmentImageInfo);
INSTANTIATE_SERIALISE_TYPE(VkFramebufferAttachmentsCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkFramebufferCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkGraphicsPipelineCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkHdrMetadataEXT);
INSTANTIATE_SERIALISE_TYPE(VkImageBlit2KHR);
INSTANTIATE_SERIALISE_TYPE(VkImageCopy2KHR);
INSTANTIATE_SERIALISE_TYPE(VkImageCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkImageFormatListCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkImageFormatProperties2);
INSTANTIATE_SERIALISE_TYPE(VkImageMemoryBarrier);
INSTANTIATE_SERIALISE_TYPE(VkImageMemoryBarrier2KHR);
INSTANTIATE_SERIALISE_TYPE(VkImageMemoryRequirementsInfo2);
INSTANTIATE_SERIALISE_TYPE(VkImagePlaneMemoryRequirementsInfo);
INSTANTIATE_SERIALISE_TYPE(VkImageResolve2KHR);
INSTANTIATE_SERIALISE_TYPE(VkImageSparseMemoryRequirementsInfo2);
INSTANTIATE_SERIALISE_TYPE(VkImageStencilUsageCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkImageSwapchainCreateInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkImageViewASTCDecodeModeEXT);
INSTANTIATE_SERIALISE_TYPE(VkImageViewCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkImageViewUsageCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkImportFenceFdInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkImportMemoryFdInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkImportSemaphoreFdInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkInstanceCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkLayerDeviceCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkLayerInstanceCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkMappedMemoryRange);
INSTANTIATE_SERIALISE_TYPE(VkMemoryAllocateFlagsInfo);
INSTANTIATE_SERIALISE_TYPE(VkMemoryAllocateInfo);
INSTANTIATE_SERIALISE_TYPE(VkMemoryBarrier);
INSTANTIATE_SERIALISE_TYPE(VkMemoryBarrier2KHR);
INSTANTIATE_SERIALISE_TYPE(VkMemoryDedicatedAllocateInfo);
INSTANTIATE_SERIALISE_TYPE(VkMemoryDedicatedRequirements);
INSTANTIATE_SERIALISE_TYPE(VkMemoryFdPropertiesKHR);
INSTANTIATE_SERIALISE_TYPE(VkMemoryGetFdInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkMemoryOpaqueCaptureAddressAllocateInfo);
INSTANTIATE_SERIALISE_TYPE(VkMemoryPriorityAllocateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkMemoryRequirements2);
INSTANTIATE_SERIALISE_TYPE(VkMultisamplePropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPastPresentationTimingGOOGLE);
INSTANTIATE_SERIALISE_TYPE(VkPerformanceCounterDescriptionKHR);
INSTANTIATE_SERIALISE_TYPE(VkPerformanceCounterKHR);
INSTANTIATE_SERIALISE_TYPE(VkPerformanceQuerySubmitInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDevice16BitStorageFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDevice8BitStorageFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceASTCDecodeFeaturesEXT)
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceBufferDeviceAddressFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceBufferDeviceAddressFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceCoherentMemoryFeaturesAMD);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceComputeShaderDerivativesFeaturesNV);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceConditionalRenderingFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceConservativeRasterizationPropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceCustomBorderColorFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceCustomBorderColorPropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceDepthClipEnableFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceDepthStencilResolveProperties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceDescriptorIndexingFeatures)
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceDescriptorIndexingProperties)
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceDiscardRectanglePropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceDriverProperties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceExtendedDynamicStateFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceExternalBufferInfo);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceExternalFenceInfo);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceExternalImageFormatInfo);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceExternalSemaphoreInfo);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceFeatures2);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceFloatControlsProperties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceFragmentDensityMapFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceFragmentDensityMapPropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceGroupProperties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceHostQueryResetFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceIDProperties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceImageFormatInfo2);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceImagelessFramebufferFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceImageRobustnessFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceImageViewImageFormatInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceIndexTypeUint8FeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceLineRasterizationFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceLineRasterizationPropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceMaintenance3Properties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceMemoryBudgetPropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceMemoryPriorityFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceMemoryProperties2);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceMultiviewFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceMultiviewProperties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDevicePCIBusInfoPropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDevicePerformanceQueryFeaturesKHR);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDevicePerformanceQueryPropertiesKHR);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDevicePointClippingProperties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDevicePrivateDataFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceProperties2);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceProtectedMemoryFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceProtectedMemoryProperties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDevicePushDescriptorPropertiesKHR);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceRobustness2FeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceRobustness2PropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceSampleLocationsPropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceSamplerFilterMinmaxProperties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceSamplerYcbcrConversionFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceScalarBlockLayoutFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceSeparateDepthStencilLayoutsFeaturesKHR);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceShaderAtomicFloatFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceShaderAtomicInt64Features);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceShaderClockFeaturesKHR);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceShaderCorePropertiesAMD);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceShaderDrawParametersFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceShaderFloat16Int8Features);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceShaderImageFootprintFeaturesNV);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceSparseImageFormatInfo2);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceSubgroupProperties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceSubgroupSizeControlFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceSubgroupSizeControlPropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceSurfaceInfo2KHR);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceSynchronization2FeaturesKHR);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceTimelineSemaphoreFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceTimelineSemaphoreProperties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceToolPropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceTransformFeedbackFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceTransformFeedbackPropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceUniformBufferStandardLayoutFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceVariablePointerFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceVulkan11Features);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceVulkan11Properties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceVulkan12Features);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceVulkan12Properties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceVulkanMemoryModelFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceYcbcrImageArraysFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR);
INSTANTIATE_SERIALISE_TYPE(VkPipelineCacheCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkPipelineColorBlendStateCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkPipelineCreationFeedbackCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkPipelineDepthStencilStateCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkPipelineDiscardRectangleStateCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkPipelineDynamicStateCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkPipelineExecutableInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkPipelineExecutableInternalRepresentationKHR);
INSTANTIATE_SERIALISE_TYPE(VkPipelineExecutablePropertiesKHR);
INSTANTIATE_SERIALISE_TYPE(VkPipelineExecutableStatisticKHR);
INSTANTIATE_SERIALISE_TYPE(VkPipelineInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkPipelineInputAssemblyStateCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkPipelineLayoutCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkPipelineMultisampleStateCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkPipelineRasterizationConservativeStateCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkPipelineRasterizationDepthClipStateCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkPipelineRasterizationLineStateCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkPipelineRasterizationStateCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkPipelineRasterizationStateStreamCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkPipelineSampleLocationsStateCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkPipelineShaderStageCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkPipelineShaderStageRequiredSubgroupSizeCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkPipelineTessellationDomainOriginStateCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkPipelineTessellationStateCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkPipelineVertexInputDivisorStateCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkPipelineVertexInputStateCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkPipelineViewportStateCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkPresentInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkPresentRegionsKHR);
INSTANTIATE_SERIALISE_TYPE(VkPresentTimeGOOGLE);
INSTANTIATE_SERIALISE_TYPE(VkPresentTimesInfoGOOGLE);
INSTANTIATE_SERIALISE_TYPE(VkPrivateDataSlotCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkProtectedSubmitInfo);
INSTANTIATE_SERIALISE_TYPE(VkQueryPoolCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkQueryPoolPerformanceCreateInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkQueueFamilyProperties2);
INSTANTIATE_SERIALISE_TYPE(VkRefreshCycleDurationGOOGLE);
INSTANTIATE_SERIALISE_TYPE(VkRenderPassAttachmentBeginInfo);
INSTANTIATE_SERIALISE_TYPE(VkRenderPassBeginInfo);
INSTANTIATE_SERIALISE_TYPE(VkRenderPassCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkRenderPassCreateInfo2);
INSTANTIATE_SERIALISE_TYPE(VkRenderPassFragmentDensityMapCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkRenderPassInputAttachmentAspectCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkRenderPassMultiviewCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkRenderPassSampleLocationsBeginInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkResolveImageInfo2KHR);
INSTANTIATE_SERIALISE_TYPE(VkSampleLocationsInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkSamplerCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkSamplerCustomBorderColorCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkSamplerReductionModeCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkSamplerYcbcrConversionCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkSamplerYcbcrConversionImageFormatProperties);
INSTANTIATE_SERIALISE_TYPE(VkSamplerYcbcrConversionInfo);
INSTANTIATE_SERIALISE_TYPE(VkSemaphoreCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkSemaphoreGetFdInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkSemaphoreSignalInfo);
INSTANTIATE_SERIALISE_TYPE(VkSemaphoreSubmitInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkSemaphoreTypeCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkSemaphoreWaitInfo);
INSTANTIATE_SERIALISE_TYPE(VkShaderModuleCreateInfo);
INSTANTIATE_SERIALISE_TYPE(VkShaderModuleValidationCacheCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkSharedPresentSurfaceCapabilitiesKHR);
INSTANTIATE_SERIALISE_TYPE(VkSparseImageFormatProperties2);
INSTANTIATE_SERIALISE_TYPE(VkSparseImageMemoryRequirements2);
INSTANTIATE_SERIALISE_TYPE(VkSubmitInfo);
INSTANTIATE_SERIALISE_TYPE(VkSubmitInfo2KHR);
INSTANTIATE_SERIALISE_TYPE(VkSubpassBeginInfo);
INSTANTIATE_SERIALISE_TYPE(VkSubpassDependency2);
INSTANTIATE_SERIALISE_TYPE(VkSubpassDescription2);
INSTANTIATE_SERIALISE_TYPE(VkSubpassDescriptionDepthStencilResolve);
INSTANTIATE_SERIALISE_TYPE(VkSubpassEndInfo);
INSTANTIATE_SERIALISE_TYPE(VkSubpassSampleLocationsEXT);
INSTANTIATE_SERIALISE_TYPE(VkSurfaceCapabilities2EXT);
INSTANTIATE_SERIALISE_TYPE(VkSurfaceCapabilities2KHR);
INSTANTIATE_SERIALISE_TYPE(VkSurfaceFormat2KHR);
INSTANTIATE_SERIALISE_TYPE(VkSurfaceProtectedCapabilitiesKHR);
INSTANTIATE_SERIALISE_TYPE(VkSwapchainCounterCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkSwapchainCreateInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkSwapchainDisplayNativeHdrCreateInfoAMD);
INSTANTIATE_SERIALISE_TYPE(VkTextureLODGatherFormatPropertiesAMD);
INSTANTIATE_SERIALISE_TYPE(VkTimelineSemaphoreSubmitInfo);
INSTANTIATE_SERIALISE_TYPE(VkValidationCacheCreateInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkValidationFeaturesEXT);
INSTANTIATE_SERIALISE_TYPE(VkValidationFlagsEXT);
INSTANTIATE_SERIALISE_TYPE(VkWriteDescriptorSet);

// plain structs with no next chain
INSTANTIATE_SERIALISE_TYPE(VkAllocationCallbacks);
INSTANTIATE_SERIALISE_TYPE(VkAttachmentDescription);
INSTANTIATE_SERIALISE_TYPE(VkAttachmentReference);
INSTANTIATE_SERIALISE_TYPE(VkBufferCopy);
INSTANTIATE_SERIALISE_TYPE(VkBufferImageCopy);
INSTANTIATE_SERIALISE_TYPE(VkClearAttachment);
INSTANTIATE_SERIALISE_TYPE(VkClearColorValue);
INSTANTIATE_SERIALISE_TYPE(VkClearDepthStencilValue);
INSTANTIATE_SERIALISE_TYPE(VkClearRect);
INSTANTIATE_SERIALISE_TYPE(VkClearValue);
INSTANTIATE_SERIALISE_TYPE(VkComponentMapping);
INSTANTIATE_SERIALISE_TYPE(VkConformanceVersion);
INSTANTIATE_SERIALISE_TYPE(VkDescriptorBufferInfo);
INSTANTIATE_SERIALISE_TYPE(VkDescriptorImageInfo);
INSTANTIATE_SERIALISE_TYPE(VkDescriptorPoolSize);
INSTANTIATE_SERIALISE_TYPE(VkDescriptorSetLayoutBinding);
INSTANTIATE_SERIALISE_TYPE(VkDescriptorUpdateTemplateEntry);
INSTANTIATE_SERIALISE_TYPE(VkDispatchIndirectCommand);
INSTANTIATE_SERIALISE_TYPE(VkDisplayModeParametersKHR);
INSTANTIATE_SERIALISE_TYPE(VkDisplayModePropertiesKHR);
INSTANTIATE_SERIALISE_TYPE(VkDisplayPlaneCapabilitiesKHR);
INSTANTIATE_SERIALISE_TYPE(VkDisplayPlanePropertiesKHR);
INSTANTIATE_SERIALISE_TYPE(VkDisplayPropertiesKHR);
INSTANTIATE_SERIALISE_TYPE(VkDrawIndexedIndirectCommand);
INSTANTIATE_SERIALISE_TYPE(VkDrawIndirectCommand);
INSTANTIATE_SERIALISE_TYPE(VkExtent2D);
INSTANTIATE_SERIALISE_TYPE(VkExtent3D);
INSTANTIATE_SERIALISE_TYPE(VkExternalMemoryProperties);
INSTANTIATE_SERIALISE_TYPE(VkFormatProperties);
INSTANTIATE_SERIALISE_TYPE(VkImageBlit);
INSTANTIATE_SERIALISE_TYPE(VkImageCopy);
INSTANTIATE_SERIALISE_TYPE(VkImageFormatProperties);
INSTANTIATE_SERIALISE_TYPE(VkImageResolve);
INSTANTIATE_SERIALISE_TYPE(VkImageSubresource);
INSTANTIATE_SERIALISE_TYPE(VkImageSubresourceLayers);
INSTANTIATE_SERIALISE_TYPE(VkImageSubresourceRange);
INSTANTIATE_SERIALISE_TYPE(VkInputAttachmentAspectReference);
INSTANTIATE_SERIALISE_TYPE(VkMemoryHeap);
INSTANTIATE_SERIALISE_TYPE(VkMemoryRequirements);
INSTANTIATE_SERIALISE_TYPE(VkMemoryType);
INSTANTIATE_SERIALISE_TYPE(VkOffset2D);
INSTANTIATE_SERIALISE_TYPE(VkOffset3D);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceFeatures);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceLimits);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceMemoryProperties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceProperties);
INSTANTIATE_SERIALISE_TYPE(VkPhysicalDeviceSparseProperties);
INSTANTIATE_SERIALISE_TYPE(VkPipelineColorBlendAttachmentState);
INSTANTIATE_SERIALISE_TYPE(VkPipelineCreationFeedbackEXT);
INSTANTIATE_SERIALISE_TYPE(VkPipelineExecutableStatisticValueKHR);
INSTANTIATE_SERIALISE_TYPE(VkPresentRegionKHR);
INSTANTIATE_SERIALISE_TYPE(VkPushConstantRange);
INSTANTIATE_SERIALISE_TYPE(VkQueueFamilyProperties);
INSTANTIATE_SERIALISE_TYPE(VkRect2D);
INSTANTIATE_SERIALISE_TYPE(VkRectLayerKHR);
INSTANTIATE_SERIALISE_TYPE(VkSampleLocationEXT);
INSTANTIATE_SERIALISE_TYPE(VkSparseBufferMemoryBindInfo);
INSTANTIATE_SERIALISE_TYPE(VkSparseImageFormatProperties);
INSTANTIATE_SERIALISE_TYPE(VkSparseImageMemoryBind);
INSTANTIATE_SERIALISE_TYPE(VkSparseImageMemoryBindInfo);
INSTANTIATE_SERIALISE_TYPE(VkSparseImageMemoryRequirements);
INSTANTIATE_SERIALISE_TYPE(VkSparseImageOpaqueMemoryBindInfo);
INSTANTIATE_SERIALISE_TYPE(VkSparseMemoryBind);
INSTANTIATE_SERIALISE_TYPE(VkSpecializationInfo);
INSTANTIATE_SERIALISE_TYPE(VkSpecializationMapEntry);
INSTANTIATE_SERIALISE_TYPE(VkStencilOpState);
INSTANTIATE_SERIALISE_TYPE(VkSubpassDependency);
INSTANTIATE_SERIALISE_TYPE(VkSubpassDescription);
INSTANTIATE_SERIALISE_TYPE(VkSurfaceCapabilitiesKHR);
INSTANTIATE_SERIALISE_TYPE(VkSurfaceFormatKHR);
INSTANTIATE_SERIALISE_TYPE(VkVertexInputAttributeDescription);
INSTANTIATE_SERIALISE_TYPE(VkVertexInputBindingDescription);
INSTANTIATE_SERIALISE_TYPE(VkVertexInputBindingDivisorDescriptionEXT);
INSTANTIATE_SERIALISE_TYPE(VkViewport);
INSTANTIATE_SERIALISE_TYPE(VkXYColorEXT);

INSTANTIATE_SERIALISE_TYPE(DescriptorSetSlot);
INSTANTIATE_SERIALISE_TYPE(ImageRegionState);
INSTANTIATE_SERIALISE_TYPE(ImageLayouts);
INSTANTIATE_SERIALISE_TYPE(ImageInfo);
INSTANTIATE_SERIALISE_TYPE(ImageSubresourceRange);
INSTANTIATE_SERIALISE_TYPE(ImageSubresourceStateForRange);
INSTANTIATE_SERIALISE_TYPE(ImageState);

#ifdef VK_USE_PLATFORM_GGP

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkPresentFrameTokenGGP &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_PRESENT_FRAME_TOKEN_GGP);
  SerialiseNext(ser, el.sType, el.pNext);
}

template <>
void Deserialise(const VkPresentFrameTokenGGP &el)
{
  DeserialiseNext(el.pNext);
}

#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImportMemoryWin32HandleInfoNV &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMPORT_MEMORY_WIN32_HANDLE_INFO_NV);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER_VKFLAGS(VkExternalMemoryHandleTypeFlagsNV, handleType);

  {
    uint64_t handle = (uint64_t)el.handle;
    ser.Serialise("handle"_lit, handle);

    // won't be valid on read, though we won't try to replay this anyway
    if(ser.IsReading())
      el.handle = NULL;
  }
}

template <>
void Deserialise(const VkImportMemoryWin32HandleInfoNV &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExportMemoryWin32HandleInfoNV &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXPORT_MEMORY_WIN32_HANDLE_INFO_NV);
  SerialiseNext(ser, el.sType, el.pNext);

  {
    // serialise pointer as plain integer, rather than recursing and serialising struct
    uint64_t pAttributes = (uint64_t)el.pAttributes;
    ser.Serialise("pAttributes"_lit, pAttributes).TypedAs("SECURITY_ATTRIBUTES*"_lit);

    // won't be valid on read, though we won't try to replay this anyway
    if(ser.IsReading())
      el.pAttributes = NULL;
  }

  SERIALISE_MEMBER_TYPED(uint32_t, dwAccess);
}

template <>
void Deserialise(const VkExportMemoryWin32HandleInfoNV &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImportMemoryWin32HandleInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXPORT_MEMORY_WIN32_HANDLE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(handleType);

  {
    uint64_t handle = (uint64_t)el.handle;
    ser.Serialise("handle"_lit, handle);

    // won't be valid on read, though we won't try to replay this anyway
    if(ser.IsReading())
      el.handle = NULL;
  }

  {
    rdcstr name;

    if(ser.IsWriting() || ser.IsStructurising())
      name = el.name ? StringFormat::Wide2UTF8(el.name) : "";

    ser.Serialise("name"_lit, name);

    // we don't expose UTF82Wide on all platforms, but as above this struct won't be valid anyway
    if(ser.IsReading())
      el.name = NULL;
  }
}

template <>
void Deserialise(const VkImportMemoryWin32HandleInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExportMemoryWin32HandleInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXPORT_MEMORY_WIN32_HANDLE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  {
    // serialise pointer as plain integer, rather than recursing and serialising struct
    uint64_t pAttributes = (uint64_t)el.pAttributes;
    ser.Serialise("pAttributes"_lit, pAttributes).TypedAs("SECURITY_ATTRIBUTES*"_lit);

    // won't be valid on read, though we won't try to replay this anyway
    if(ser.IsReading())
      el.pAttributes = NULL;
  }

  SERIALISE_MEMBER_TYPED(uint32_t, dwAccess);

  {
    rdcstr name;

    if(ser.IsWriting() || ser.IsStructurising())
      name = el.name ? StringFormat::Wide2UTF8(el.name) : "";

    ser.Serialise("name"_lit, name);

    // we don't expose UTF82Wide on all platforms, but as above this struct won't be valid anyway
    if(ser.IsReading())
      el.name = NULL;
  }
}

template <>
void Deserialise(const VkExportMemoryWin32HandleInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryWin32HandlePropertiesKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MEMORY_WIN32_HANDLE_PROPERTIES_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(memoryTypeBits);
}

template <>
void Deserialise(const VkMemoryWin32HandlePropertiesKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryGetWin32HandleInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_MEMORY_GET_WIN32_HANDLE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(memory);
  SERIALISE_MEMBER(handleType);
}

template <>
void Deserialise(const VkMemoryGetWin32HandleInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExportFenceWin32HandleInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXPORT_FENCE_WIN32_HANDLE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  {
    // serialise pointer as plain integer, rather than recursing and serialising struct
    uint64_t pAttributes = (uint64_t)el.pAttributes;
    ser.Serialise("pAttributes"_lit, pAttributes).TypedAs("SECURITY_ATTRIBUTES*"_lit);

    // won't be valid on read, though we won't try to replay this anyway
    if(ser.IsReading())
      el.pAttributes = NULL;
  }

  SERIALISE_MEMBER_TYPED(uint32_t, dwAccess);

  {
    rdcstr name;

    if(ser.IsWriting() || ser.IsStructurising())
      name = el.name ? StringFormat::Wide2UTF8(el.name) : "";

    ser.Serialise("name"_lit, name);

    // we don't expose UTF82Wide on all platforms, but as above this struct won't be valid anyway
    if(ser.IsReading())
      el.name = L"???";
  }
}

template <>
void Deserialise(const VkExportFenceWin32HandleInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImportFenceWin32HandleInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMPORT_FENCE_WIN32_HANDLE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(fence);
  SERIALISE_MEMBER_VKFLAGS(VkFenceImportFlags, flags);
  SERIALISE_MEMBER(handleType);

  {
    uint64_t handle = (uint64_t)el.handle;
    ser.Serialise("handle"_lit, handle);

    // won't be valid on read, though we won't try to replay this anyway
    if(ser.IsReading())
      el.handle = NULL;
  }

  {
    rdcstr name;

    if(ser.IsWriting() || ser.IsStructurising())
      name = el.name ? StringFormat::Wide2UTF8(el.name) : "";

    ser.Serialise("name"_lit, name);

    // we don't expose UTF82Wide on all platforms, but as above this struct won't be valid anyway
    if(ser.IsReading())
      el.name = L"???";
  }
}

template <>
void Deserialise(const VkImportFenceWin32HandleInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkFenceGetWin32HandleInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_FENCE_GET_WIN32_HANDLE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(fence);
  SERIALISE_MEMBER(handleType);
}

template <>
void Deserialise(const VkFenceGetWin32HandleInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExportSemaphoreWin32HandleInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  {
    // serialise pointer as plain integer, rather than recursing and serialising struct
    uint64_t pAttributes = (uint64_t)el.pAttributes;
    ser.Serialise("pAttributes"_lit, pAttributes).TypedAs("SECURITY_ATTRIBUTES*"_lit);

    // won't be valid on read, though we won't try to replay this anyway
    if(ser.IsReading())
      el.pAttributes = NULL;
  }

  SERIALISE_MEMBER_TYPED(uint32_t, dwAccess);

  {
    rdcstr name;

    if(ser.IsWriting() || ser.IsStructurising())
      name = el.name ? StringFormat::Wide2UTF8(el.name) : "";

    ser.Serialise("name"_lit, name);

    // we don't expose UTF82Wide on all platforms, but as above this struct won't be valid anyway
    if(ser.IsReading())
      el.name = L"???";
  }
}

template <>
void Deserialise(const VkExportSemaphoreWin32HandleInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImportSemaphoreWin32HandleInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(semaphore);
  SERIALISE_MEMBER_VKFLAGS(VkSemaphoreImportFlags, flags);
  SERIALISE_MEMBER(handleType);

  {
    uint64_t handle = (uint64_t)el.handle;
    ser.Serialise("handle"_lit, handle);

    // won't be valid on read, though we won't try to replay this anyway
    if(ser.IsReading())
      el.handle = NULL;
  }

  {
    rdcstr name;

    if(ser.IsWriting() || ser.IsStructurising())
      name = el.name ? StringFormat::Wide2UTF8(el.name) : "";

    ser.Serialise("name"_lit, name);

    // we don't expose UTF82Wide on all platforms, but as above this struct won't be valid anyway
    if(ser.IsReading())
      el.name = L"???";
  }
}

template <>
void Deserialise(const VkImportSemaphoreWin32HandleInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkD3D12FenceSubmitInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_D3D12_FENCE_SUBMIT_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(waitSemaphoreValuesCount);
  SERIALISE_MEMBER_ARRAY(pWaitSemaphoreValues, waitSemaphoreValuesCount);
  SERIALISE_MEMBER(signalSemaphoreValuesCount);
  SERIALISE_MEMBER_ARRAY(pSignalSemaphoreValues, signalSemaphoreValuesCount);
}

template <>
void Deserialise(const VkD3D12FenceSubmitInfoKHR &el)
{
  DeserialiseNext(el.pNext);
  delete[] el.pWaitSemaphoreValues;
  delete[] el.pSignalSemaphoreValues;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSemaphoreGetWin32HandleInfoKHR &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SEMAPHORE_GET_WIN32_HANDLE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(semaphore);
  SERIALISE_MEMBER(handleType);
}

template <>
void Deserialise(const VkSemaphoreGetWin32HandleInfoKHR &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkWin32KeyedMutexAcquireReleaseInfoNV &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_NV);
  SerialiseNext(ser, el.sType, el.pNext);

  // resources here are optional because we don't take references on the memory as we don't replay
  // the win32 mutexes
  OPTIONAL_RESOURCES();

  SERIALISE_MEMBER(acquireCount);
  SERIALISE_MEMBER_ARRAY(pAcquireSyncs, acquireCount);
  SERIALISE_MEMBER_ARRAY(pAcquireKeys, acquireCount);
  SERIALISE_MEMBER_ARRAY(pAcquireTimeoutMilliseconds, acquireCount);
  SERIALISE_MEMBER(releaseCount);
  SERIALISE_MEMBER_ARRAY(pReleaseSyncs, releaseCount);
  SERIALISE_MEMBER_ARRAY(pReleaseKeys, releaseCount);
}

template <>
void Deserialise(const VkWin32KeyedMutexAcquireReleaseInfoNV &el)
{
  DeserialiseNext(el.pNext);

  delete[] el.pAcquireSyncs;
  delete[] el.pAcquireKeys;
  delete[] el.pAcquireTimeoutMilliseconds;
  delete[] el.pReleaseSyncs;
  delete[] el.pReleaseKeys;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkWin32KeyedMutexAcquireReleaseInfoKHR &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_KHR);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(acquireCount);
  SERIALISE_MEMBER_ARRAY(pAcquireSyncs, acquireCount);
  SERIALISE_MEMBER_ARRAY(pAcquireKeys, acquireCount);
  SERIALISE_MEMBER_ARRAY(pAcquireTimeouts, acquireCount);
  SERIALISE_MEMBER(releaseCount);
  SERIALISE_MEMBER_ARRAY(pReleaseSyncs, releaseCount);
  SERIALISE_MEMBER_ARRAY(pReleaseKeys, releaseCount);
}

template <>
void Deserialise(const VkWin32KeyedMutexAcquireReleaseInfoKHR &el)
{
  DeserialiseNext(el.pNext);

  delete[] el.pAcquireSyncs;
  delete[] el.pAcquireKeys;
  delete[] el.pAcquireTimeouts;
  delete[] el.pReleaseSyncs;
  delete[] el.pReleaseKeys;
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSurfaceFullScreenExclusiveInfoEXT &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(fullScreenExclusive);
}

template <>
void Deserialise(const VkSurfaceFullScreenExclusiveInfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSurfaceCapabilitiesFullScreenExclusiveEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_FULL_SCREEN_EXCLUSIVE_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(fullScreenExclusiveSupported);
}

template <>
void Deserialise(const VkSurfaceCapabilitiesFullScreenExclusiveEXT &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkSurfaceFullScreenExclusiveWin32InfoEXT &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT);
  SerialiseNext(ser, el.sType, el.pNext);

  uint64_t hmonitor = (uint64_t)el.hmonitor;
  ser.Serialise("hmonitor"_lit, hmonitor);
}

template <>
void Deserialise(const VkSurfaceFullScreenExclusiveWin32InfoEXT &el)
{
  DeserialiseNext(el.pNext);
}

INSTANTIATE_SERIALISE_TYPE(VkImportMemoryWin32HandleInfoNV);
INSTANTIATE_SERIALISE_TYPE(VkExportMemoryWin32HandleInfoNV);
INSTANTIATE_SERIALISE_TYPE(VkWin32KeyedMutexAcquireReleaseInfoNV);
INSTANTIATE_SERIALISE_TYPE(VkWin32KeyedMutexAcquireReleaseInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkImportMemoryWin32HandleInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkExportMemoryWin32HandleInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkMemoryWin32HandlePropertiesKHR);
INSTANTIATE_SERIALISE_TYPE(VkMemoryGetWin32HandleInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkExportSemaphoreWin32HandleInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkImportSemaphoreWin32HandleInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkD3D12FenceSubmitInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkSemaphoreGetWin32HandleInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkExportFenceWin32HandleInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkImportFenceWin32HandleInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkFenceGetWin32HandleInfoKHR);
INSTANTIATE_SERIALISE_TYPE(VkSurfaceCapabilitiesFullScreenExclusiveEXT);
INSTANTIATE_SERIALISE_TYPE(VkSurfaceFullScreenExclusiveInfoEXT);
INSTANTIATE_SERIALISE_TYPE(VkSurfaceFullScreenExclusiveWin32InfoEXT);
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkAndroidHardwareBufferPropertiesANDROID &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_PROPERTIES_ANDROID);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(allocationSize);
  SERIALISE_MEMBER(memoryTypeBits);
}

template <>
void Deserialise(const VkAndroidHardwareBufferPropertiesANDROID &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkMemoryGetAndroidHardwareBufferInfoANDROID &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_MEMORY_GET_ANDROID_HARDWARE_BUFFER_INFO_ANDROID);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(memory);
}

template <>
void Deserialise(const VkMemoryGetAndroidHardwareBufferInfoANDROID &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkAndroidHardwareBufferFormatPropertiesANDROID &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_FORMAT_PROPERTIES_ANDROID);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(format);
  SERIALISE_MEMBER(externalFormat);
  SERIALISE_MEMBER_VKFLAGS(VkFormatFeatureFlags, formatFeatures);
  SERIALISE_MEMBER(samplerYcbcrConversionComponents);
  SERIALISE_MEMBER(suggestedYcbcrModel);
  SERIALISE_MEMBER(suggestedYcbcrRange);
  SERIALISE_MEMBER(suggestedXChromaOffset);
  SERIALISE_MEMBER(suggestedYChromaOffset);
}

template <>
void Deserialise(const VkAndroidHardwareBufferFormatPropertiesANDROID &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkExternalFormatANDROID &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_EXTERNAL_FORMAT_ANDROID);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(externalFormat);
}

template <>
void Deserialise(const VkExternalFormatANDROID &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkAndroidHardwareBufferUsageANDROID &el)
{
  RDCASSERT(ser.IsReading() || el.sType == VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_USAGE_ANDROID);
  SerialiseNext(ser, el.sType, el.pNext);

  SERIALISE_MEMBER(androidHardwareBufferUsage);
}

template <>
void Deserialise(const VkAndroidHardwareBufferUsageANDROID &el)
{
  DeserialiseNext(el.pNext);
}

template <typename SerialiserType>
void DoSerialise(SerialiserType &ser, VkImportAndroidHardwareBufferInfoANDROID &el)
{
  RDCASSERT(ser.IsReading() ||
            el.sType == VK_STRUCTURE_TYPE_IMPORT_ANDROID_HARDWARE_BUFFER_INFO_ANDROID);
  SerialiseNext(ser, el.sType, el.pNext);

  {
    uint64_t buffer = (uint64_t)el.buffer;
    ser.Serialise("buffer"_lit, buffer);

    // won't be valid on read, though we won't try to replay this anyway
    if(ser.IsReading())
      el.buffer = NULL;
  }
}

template <>
void Deserialise(const VkImportAndroidHardwareBufferInfoANDROID &el)
{
  DeserialiseNext(el.pNext);
}

INSTANTIATE_SERIALISE_TYPE(VkAndroidHardwareBufferUsageANDROID);
INSTANTIATE_SERIALISE_TYPE(VkAndroidHardwareBufferPropertiesANDROID);
INSTANTIATE_SERIALISE_TYPE(VkAndroidHardwareBufferFormatPropertiesANDROID);
INSTANTIATE_SERIALISE_TYPE(VkImportAndroidHardwareBufferInfoANDROID);
INSTANTIATE_SERIALISE_TYPE(VkMemoryGetAndroidHardwareBufferInfoANDROID);
INSTANTIATE_SERIALISE_TYPE(VkExternalFormatANDROID);
#endif
