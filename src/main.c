#include <stdio.h>
#include <GLFW/glfw3.h>

int main(int argc, char** argv) {
	if (!glfwInit()) {
		printf("Init failed...");
		return -1;
	}
	printf("kake!");
	return 0;
}
