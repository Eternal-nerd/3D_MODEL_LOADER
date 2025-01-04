#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
  Camera();
  ~Camera();

  const glm::mat4& getProjection() const { return projectionMatrix; }

  void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
  void setPerspectiveProjection(float fovY, float aspect, float near, float far);


private:
  glm::mat4 projectionMatrix{1.0f};


  //void updateViewMatrix();
};