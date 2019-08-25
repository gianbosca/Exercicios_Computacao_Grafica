//To GLEW
#include <GL/glew.h>

//To GLFW
#include <GLFW/glfw3.h>

//To GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//demais libs
#include <math.h>

#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <string>


int counterFPS = 0;

char* charNameWindow = "CG - Triangle with white border - FPS: ";

double previous_seconds;

// função útil para computar e atualizar FPS. Deve ser
// chamada antes das instruções de desenho
void updateFPS (GLFWwindow* window) {
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;
    if ( elapsed_seconds >= 1.0 ){
        char tmp[ 128];
        double fps = (double) counterFPS / elapsed_seconds;
        sprintf (tmp, "CG - Tringle with border - @ fps: %d", counterFPS);
        glfwSetWindowTitle (window, tmp);
        previous_seconds = current_seconds;
        counterFPS=0;
    }
    counterFPS++;


}

int main () {
    GLFWwindow* window = NULL;
    const GLubyte* renderer;
    const GLubyte* version;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;




    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
            // positions         // colors
            0.0f, 0.4f, 0.0f,   1.0f, 0.0f, 0.0f, // top
            0.3f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, //bottom middle
            0.5f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f, // bottom right
            -0.7f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f // bottom left
    };

    unsigned int indices[] = {
            // note that we start from 0!
            0, 1, 3, // first triangle
            0, 1, 2 // second triangle
    };

    float matrix[] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.1f, 0.1f, 0.0f, 1.0f
    };


    /* these are the strings of code for the shaders
    the vertex shader positions each vertex point */
    const char* vertex_shader =
            "#version 410\n"
            "layout(location=0) in vec3 vp;"
            "layout(location=1) in vec3 vc;"
            "uniform mat4 matrix;"
            "out vec3 color;"
            "void main () {"
            "   color = vc;"
            "	gl_Position = matrix * vec4 (vp, 1.0);"
            "}";

    /* the fragment shader colours each fragment (pixel-sized area of the
    triangle) */
    const char* fragment_shader =
            "#version 410\n"
            "in vec3 color;"
            "out vec4 frag_color;"
            "void main () {"
            "	frag_color = vec4 (color, 1.0);"
            "}";

    /* the fragment shader colours each fragment (pixel-sized area of the
   triangle) */
    const char* fragment_shader2 =
            "#version 410\n"
            "out vec4 frag_color;"
            "void main () {"
            "	frag_color = vec4 (1.0,1.0,1.0,1.0);"
            "}";

    /* GL shader objects for vertex and fragment shader [components] */
    GLuint vs, fs, fs2;
    /* GL shader programme object [combined, to link] */
    GLuint shader_programme, shader_programme2;

    /* start GL context and O/S window using the GLFW helper library */
    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    /* change to 3.2 if on Apple OS X */
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //cria a janela
    window = glfwCreateWindow (
            640, 480, charNameWindow, NULL, NULL
    );
    if (!window) {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent (window);
    /* start GLEW extension handler */
    glewExperimental = GL_TRUE;
    glewInit ();

    /* get version info */
    renderer = glGetString (GL_RENDERER); /* get renderer string */
    version = glGetString (GL_VERSION); /* version as a string */
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);

    /* tell GL to only draw onto a pixel if the shape is closer to the viewer */
    glEnable (GL_DEPTH_TEST); /* enable depth-testing */
    glDepthFunc (GL_LESS);/*depth-testing interprets a smaller value as "closer"*/

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // identifica VBO atual
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    // 0 identifica que o primeiro atributo está sendo definido
    // 3, GL_FLOAT identifica que dados são vec3 e estão a cada 3 float.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // habilitado primeiro atributo do VBO bound atual

    // color attribute
    // 3 identifica que o primeiro atributo está sendo definido
    // 3, GL_FLOAT identifica que dados são vec3 e estão a cada 3 float.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // habilitado segundo atributo do VBO bound atual


    /* here we copy the shader strings into GL shaders, and compile them. we then
    create an executable shader 'program' and attach both of the compiled shaders.
    we link this, which matches the outputs of the vertex shader to the inputs of
    the fragment shader, etc. and it is then ready to use */
    vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &vertex_shader, NULL);
    glCompileShader (vs);

    fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs, 1, &fragment_shader, NULL);
    glCompileShader (fs);

    shader_programme = glCreateProgram ();
    glAttachShader (shader_programme, fs);
    glAttachShader (shader_programme, vs);
    glLinkProgram (shader_programme);

    fs2 = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs2, 1, &fragment_shader2, NULL);
    glCompileShader (fs2);

    shader_programme2 = glCreateProgram ();
    glAttachShader (shader_programme2, fs2);
    glAttachShader (shader_programme2, vs);
    glLinkProgram (shader_programme2);

    int matrixLocation = glGetUniformLocation(shader_programme, "matrix");
    int matrixLocation2 = glGetUniformLocation(shader_programme2, "matrix");


    //define fundo padrao
    glClearColor(0.666f, 0.666f, 0.666f, 1.0f);

    //define tempo inicial
    previous_seconds = glfwGetTime();

    while (!glfwWindowShouldClose (window)) {

//        static double previousSeconds = glfwGetTime();
//        double currentSeconds = glfwGetTime();
//        double elapsedSeconds = currentSeconds - previousSeconds;
//        if(elapsedSeconds > 0) {
//            static double previousSeconds = glfwGetTime();
//            double currentSeconds = glfwGetTime();
//            double elapsedSeconds = currentSeconds - previousSeconds;
//            if (elapsedSeconds > 0) {
//                previousSeconds = currentSeconds;
//            }
//        }

        //atualiza FPS da Janela
        updateFPS(window);

        /* wipe the drawing surface clear */
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        glUseProgram (shader_programme2);

        glUniformMatrix4fv(matrixLocation2, 1, GL_FALSE, matrix);

        // bind of VAO
        glBindVertexArray (VAO);

        /* draw points*/
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);


        glUseProgram (shader_programme);

        //pass uniform location to shader
        glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);

        // bind of VAO
        glBindVertexArray (VAO);

        /* draw points*/
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);




        /* update other events like input handling */
        glfwPollEvents ();

        /* put the stuff we've been drawing onto the display */
        glfwSwapBuffers (window);

        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)){
            glfwSetWindowShouldClose(window, 1);
        }
    }

    /* close GL context and any other GLFW resources */
    glfwTerminate();
    return 0;
}