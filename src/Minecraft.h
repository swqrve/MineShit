#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void windowChangeSizeEvent(GLFWwindow* window, int width, int height);
void scrollEvent(GLFWwindow* window, double xoffset, double yoffset);
void mouseMoveEvent(GLFWwindow* window, double xpos, double ypos);
void mouseClickEvent(GLFWwindow* window, int button, int action, int mods);
