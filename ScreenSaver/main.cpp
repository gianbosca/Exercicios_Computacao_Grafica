//To GLEW
#include <GL/glew.h>

//To GLFW
#include <GLFW/glfw3.h>

//To GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//demais libs
#include <stdio.h>
#include <math.h>

int main () {
    GLFWwindow* window = NULL;
    const GLubyte* renderer;
    const GLubyte* version;
    GLuint vao, vao2;
    GLuint vbo, vbo2;
    /* geometry to use. these are 3 xyz points (9 floats total) to make a triangle
    */

    typedef struct Vertex {
        GLfloat coords[3];
    } VecXYZ;

    VecXYZ points[3];

    //top point
    points[0].coords[0] = 0.0f;
    points[0].coords[1] = 0.4f;
    points[0].coords[2] = 0.0f;

    //right point
    points[1].coords[0] = 0.25f;
    points[1].coords[1] = -0.25f;
    points[1].coords[2] = 0.0f;

    //left point
    points[2].coords[0] = -0.25f;
    points[2].coords[1] = -0.25f;
    points[2].coords[2] = 0.0f;

    GLfloat colors[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
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

    window = glfwCreateWindow (
            640, 480, "Screen Saver - Moving Triangle", NULL, NULL
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

    /* a vertex buffer object (VBO) is created here. this stores an array of data
    on the graphics adapter's memory. in our case - the vertex points */
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (GLfloat), points, GL_STATIC_DRAW);


    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0); // habilitado primeiro atributo do vbo bound atual
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // identifica vbo atual

    // associação do vbo atual com primeiro atributo
    // 0 identifica que o primeiro atributo está sendo definido
    // 3, GL_FLOAT identifica que dados são vec3 e estão a cada 3 float.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // é possível associar outros atributos, como normais, mapeamento e cores
    // lembre-se: um por vértice!
    GLuint colorsVBO;
    glGenBuffers (1, &colorsVBO);
    glBindBuffer (GL_ARRAY_BUFFER, colorsVBO);
    glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (GLfloat), colors, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    // note que agora o atributo 1 está definido
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1); // habilitado segundo atributo do vbo bound atual

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

    int matrixLocation = glGetUniformLocation(shader_programme, "matrix");

    glClearColor(0.666f, 0.666f, 0.666f, 1.0f);

    /* this loop clears the drawing surface, then draws the geometry described by
    the VAO onto the drawing surface. we 'poll events' to see if the window was
    closed, etc. finally, we 'swap the buffers' which displays our drawing surface
    onto the view area. we use a double-buffering system which means that we have
    a 'currently displayed' surface, and 'currently being drawn' surface. hence
    the 'swap' idea. in a single-buffering system we would see stuff being drawn
    one-after-the-other */

    float speedX = 0.5f;
    float speedY = 0.5f;
    float nextPositionX = 0.0f;
    float nextPositionY = 0.0f;
    float lastPositionX = 0.0f;
    float lastPositionY = 0.0f;


    bool doPisc = false;

    glUseProgram (shader_programme);
    while (!glfwWindowShouldClose (window)) {

        static double previousSeconds = glfwGetTime();
        double currentSeconds = glfwGetTime();
        double elapsedSeconds = currentSeconds - previousSeconds;
        if(elapsedSeconds > 0) {
            static double previousSeconds = glfwGetTime();
            double currentSeconds = glfwGetTime();
            double elapsedSeconds = currentSeconds - previousSeconds;
            if (elapsedSeconds > 0) {

                previousSeconds = currentSeconds;

                // move to down when collide on top
                if (fabs(points[0].coords[1] + matrix[13]) >= 1.0f) {
                    float normalY = -1.0f;
                    float rY = lastPositionY + normalY;
                    float pEscalarX = rY * normalY;
                    speedY = 2 * normalY * pEscalarX - rY;
                }

                // move to left when collide on right
                if (fabs(points[1].coords[0] + matrix[12]) >= 1.0f) {
                    float normalX = -1.0f;
                    float rX = lastPositionX + normalX;
                    float pEscalarX = rX * normalX;
                    speedX = 2 * normalX * pEscalarX - rX;
                }

                // move to up when collide on down
                if (fabs(points[1].coords[1] + matrix[13]) >= 1.0f || fabs(points[2].coords[1] + matrix[13]) >= 1.0f) {
                    float normalY = 1.0f;
                    float rY = lastPositionY + normalY;
                    float productEscalarX = rY * normalY;
                    speedY = 2 * normalY * productEscalarX - rY;
                }

                // move to rigth when collide on left
                if (fabs(points[2].coords[1] + matrix[12]) >= 1.0f) {
                    float normalX = 1.0f;
                    float rX = lastPositionX + normalX;
                    float pEscalarX = rX * normalX;
                    speedX = 2 * normalX * pEscalarX - rX;
                }

                //store the move of x
                matrix[12] = elapsedSeconds * speedX + lastPositionX;
                lastPositionX = matrix[12];


                //store the move of y
                matrix[13] = elapsedSeconds * speedY + lastPositionY;
                lastPositionY = matrix[13];

                doPisc = !doPisc;
            }
        }

        //pass uniform location to shader
        glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);

        /* wipe the drawing surface clear */
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind of VAO
        glBindVertexArray (vao);

        /* draw points 0-3 from the currently bound VAO with current in-use shader*/
//        if(doPisc){
            glDrawArrays (GL_TRIANGLES, 0, 3);
//        }
//        else{
//            glDrawArrays (GL_LINE_LOOP, 0, 3);
//        }


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