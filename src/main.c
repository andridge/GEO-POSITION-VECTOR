#include "vendors/GLFW/glfw3.h"
#include <stdio.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "vendors/SERIAL/serial_port.h"
#include <math.h>
#define BUFFER_SIZE 256
// Function to render text on the window
void renderText(float x, float y, const char* buffer) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1000, 0, 600, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    // Render the string using the built-in bitmap font
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)buffer);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
int main() {
    GLFWwindow* window;

    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create a window and a context
    window = glfwCreateWindow(1000, 600, "My Window", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);


  
    //
    SerialPort port;
    const char* port_name = "/dev/tty.usbserial-110";  // Change this to the correct port name
    speed_t baudrate = 9600;
    SerialPort_init(&port, port_name, baudrate);

    // Check if the object was created successfully
    if (SerialPort_get_fd(&port) == -1) {
      return 1;
    }
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while (1) {
        // Read data from the serial port
        bytes_read = SerialPort_read(&port, buffer, BUFFER_SIZE);

        if (bytes_read > 0) {
            // Print the data received
           // printf("Data received: %s\n", buffer);
            renderText(10, 10, buffer);
            // Set the position for displaying the text
          // glWindowPos2f(10, 10);

            // Render the text
       //   glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)buffer);
        }

        // Wait for 5 seconds
        sleep(5);
    }

    SerialPort_destroy(&port);


  
     // use the serial_port object to read and write data
   /* char buffer[256];
    int bytes_read = SerialPort_read(&serial_port, buffer, sizeof(buffer));
    printf("Read %d bytes: %s\n", bytes_read, buffer);

    const char* message = "Hello, world!";
    int bytes_written = SerialPort_write(&serial_port, message, strlen(message));
    printf("Wrote %d bytes\n", bytes_written);

    SerialPort_destroy(&serial_port);*/


 // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Set viewport to the entire window
    int window_width, window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);
    glViewport(0, 0, window_width, window_height);

    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)window_width / (float)window_height, 0.1f, 1000.0f);

    // Set up modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   gluLookAt(50.0f, 50.0f, 50.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glScalef(1.5f, 1.5f, 1.5f);
    glRotatef(glfwGetTime() * 50.0f, 0.0f, 1.0f, 0.0f);
    // Draw the grid lines
    float size = 20.0f;
    float step = 1.0f;

      // Draw the origin point
    glPointSize(10.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();

    // Draw the XY plane
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    for (float i = -size; i <= size; i += step) {
        glVertex3f(i, -size, 0.0f);
        glVertex3f(i, size, 0.0f);
        glVertex3f(-size, i, 0.0f);
        glVertex3f(size, i, 0.0f);
    }
    glEnd();

    // Draw the XZ plane
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    for (float i = -size; i <= size; i += step) {
        glVertex3f(i, 0.0f, -size);
        glVertex3f(i, 0.0f, size);
        glVertex3f(0.0f, -size, i);
        glVertex3f(0.0f, size, i);
    }
    glEnd();

    // Draw the YZ plane
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINES);
    for (float i = -size; i <= size; i += step) {
        glVertex3f(0.0f, i, -size);
        glVertex3f(0.0f, i, size);
        glVertex3f(-size, 0.0f, i);
        glVertex3f(size, 0.0f, i);
    }
    glEnd();
    
    // Swap buffers
    glfwSwapBuffers(window);

    // Poll for events
    glfwPollEvents();
}





    // Clean up
    glfwTerminate();
    return 0;
}
