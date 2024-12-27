#pragma once

class Camera {
public:
  Camera();
  ~Camera();

private:
  float FOV_;
  // float znear, zfar;

  void updateViewMatrix();
};