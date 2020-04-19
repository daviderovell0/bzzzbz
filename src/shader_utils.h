/** @file  shader_utils.h
 *
 *  @brief Header for functions to read and create shaders and program object.
 *
 *  @author Peter Nagy (deetrone)
 */

    /**
     * Read a shader file into a string. 
     *
     * @arg in: shader filename
     * @returns  string containing shader source code
     *
     */
char* file_read(const char* filename);

    /**
     * Error handling for create_shader and create_program.
     *
     * @arg in: shader or program object
     * @returns prints error message
     */

void print_log(GLuint object);

     /**
     * Create shader object.
     *
     * @arg in: shader filename , type (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
     * @returns shader object
     *
     * @calls file_read , print_log
     */
GLuint create_shader(const char* filename, GLenum type);


    /**
     * Create program object and attach shaders.
     *
     * @arg in: empty program , vertex shader , fragment shader objects
     * @returns program object with specified shaders attached
     *
     */

GLuint create_program(GLuint program, GLuint vert, GLuint frag);

