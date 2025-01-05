#include "synchro.h"

#include <stdexcept>

Synchro::Synchro() {}
Synchro::~Synchro() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Synchro::setDvcePtr(const Dvce &dvce) { dvcePtr_ = &dvce; }

void Synchro::init(int maxFramesInFlight) {
  util::log("Initializing syncro class...");
  maxFramesInFlight_ = maxFramesInFlight;
  createSyncObjects();
}

void Synchro::createSyncObjects() {
  util::log("Creating sync objects...");

  if (maxFramesInFlight_ < 1) {
    throw std::runtime_error("Synchro: max frames in flight must be > 0! ");
  }

  imageAvailableSemaphores_.resize(maxFramesInFlight_);
  renderFinishedSemaphores_.resize(maxFramesInFlight_);
  inFlightFences_.resize(maxFramesInFlight_);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < maxFramesInFlight_; i++) {
    if (vkCreateSemaphore(dvcePtr_->getLogical(), &semaphoreInfo, nullptr,
                          &imageAvailableSemaphores_[i]) != VK_SUCCESS ||
        vkCreateSemaphore(dvcePtr_->getLogical(), &semaphoreInfo, nullptr,
                          &renderFinishedSemaphores_[i]) != VK_SUCCESS ||
        vkCreateFence(dvcePtr_->getLogical(), &fenceInfo, nullptr,
                      &inFlightFences_[i]) != VK_SUCCESS) {
      throw std::runtime_error(
          "failed to create synchronization objects for a frame!");
    }
  }
}

/*-----------------------------------------------------------------------------
-----------------------------GETTERS-------------------------------------------
-----------------------------------------------------------------------------*/
const std::vector<VkSemaphore> &Synchro::getImageAvailableSemaphores() const {
  return imageAvailableSemaphores_;
}
const std::vector<VkSemaphore> &Synchro::getRenderFinishedSemaphores() const {
  return renderFinishedSemaphores_;
}
const std::vector<VkFence> &Synchro::getInFlightFences() const {
  return inFlightFences_;
}

/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Synchro::cleanup() {
  util::log("Cleaning up synchro...");

  util::log("Destroying semaphores and fences...");
  for (size_t i = 0; i < maxFramesInFlight_; i++) {
    vkDestroySemaphore(dvcePtr_->getLogical(), renderFinishedSemaphores_[i],
                       nullptr);
    vkDestroySemaphore(dvcePtr_->getLogical(), imageAvailableSemaphores_[i],
                       nullptr);
    vkDestroyFence(dvcePtr_->getLogical(), inFlightFences_[i], nullptr);
  }
}
