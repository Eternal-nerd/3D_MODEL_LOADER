#pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include "dvce.h"
#include "util.h"

class Synchro {
public:
  Synchro();
  ~Synchro();

  // to give sync objs access to device
  void setDvcePtr(const Dvce &dvce);

  void init(int framesInFlight);

  // GETTERS
  const std::vector<VkSemaphore> &getImageAvailableSemaphores() const;
  const std::vector<VkSemaphore> &getRenderFinishedSemaphores() const;
  const std::vector<VkFence> &getInFlightFences() const;

  void cleanup();

private:
  // reference to device:
  const Dvce *dvcePtr_ = nullptr;

  int maxFramesInFlight_ = -1;

  std::vector<VkSemaphore> imageAvailableSemaphores_;
  std::vector<VkSemaphore> renderFinishedSemaphores_;
  std::vector<VkFence> inFlightFences_;

  void createSyncObjects();
};