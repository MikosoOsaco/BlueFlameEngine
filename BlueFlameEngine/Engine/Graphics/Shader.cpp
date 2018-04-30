#include "Shader.h"

Shader::Shader(std::string vertexPath_, std::string fragmentPath_) {

	//Declare variables that will be required to load the 
	//code in the GLSL files for the fragment and vertex shaders
	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	const char* vertexCode;
	const char* fragmentCode;

	//Declare the streams that will be used to load and
	//read the shader code
	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	//Allow for error checking when loading the files
	//Exceptions is used to in the try catch to track errors during loading
	//Badbit is a flag we are looking for, indicating that the reading of
	//the stream has been disrupted
	vertexFile.exceptions(std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::badbit);

	try {
		//Open the shader code files from the link passed to the constructor
		vertexFile.open(vertexPath_);
		fragmentFile.open(fragmentPath_);

		//Create 2 temporary string streams to read the data
		//Copy the data from the buffer of the opened files above into the stream
		std::stringstream tmpVertexStream;
		std::stringstream tmpFragmentStream;
		tmpVertexStream << vertexFile.rdbuf();
		tmpFragmentStream << fragmentFile.rdbuf();

		//Now that we have stored all the data in our variables 
		//we can close the files
		vertexFile.close();
		fragmentFile.close();

		//Save the code from the stream into the strings we created
		vertexShaderCode = tmpVertexStream.str();
		fragmentShaderCode = tmpFragmentStream.str();
	}
	//Using the exception flags we declared above (BadBit) an exception 
	//will be thrown if the error occurs due to the stream being interrupted
	catch (std::ifstream::failure error_) {
		std::cout << "Error with the shader not being read" << std::endl;
		//Stop the function if an error occurs because it will not be able
		//to complete the rest of the initialization process
		return;
	}

	//Assign the string of shader code to const char* in order to pass to the GPU
	//Create a constant variable that holds the size of the code
	vertexCode = vertexShaderCode.c_str();
	fragmentCode = fragmentShaderCode.c_str();
	const int vertSize = vertexShaderCode.size();
	const int fragSize = fragmentShaderCode.size();

	//---------------Compiling the shader-----------------
	GLuint vertex, fragment; //Hold the address to the shader programs on the GPU
	GLint success; //Check to verify binding to the GPU was a success
	GLchar infoLog[512]; //Stores errors in case something goes wrong

	//Specify the type of shader you are creating & get the address
	vertex = glCreateShader(GL_VERTEX_SHADER);
	//Add the source data to the shader
	//(shader address, number created, the const char* of code, the size of the code)
	glShaderSource(vertex, 1, &vertexCode, &vertSize);
	//Compile the shader on the GPU, just like you would with this project to run it
	glCompileShader(vertex);

	//Ask the GPU what the status of the compilation was
	//The result will be stored in the GLint we created above
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	//Check to make sure that the compilation was a success
	if (!success) {
		//If an error occurred, ask the GPU what the error message is
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "Error vertex compilation failed\n" << infoLog << std::endl;
		//Exit the function because the rest of the initialization is not possible
		return;
	}

	//Do the exact same thing for the fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, &fragSize);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "Error vertex compilation failed\n" << infoLog << std::endl;
		return;
	}

	//Create a new shader program on the GPU
	//Get the address to its home on the GPU
	shaderProgram = glCreateProgram();
	//Attach the vertex shader we compiled above to the shader program
	glAttachShader(shaderProgram, vertex);
	//Attach the fragment shader we compiled above to the shader program
	glAttachShader(shaderProgram, fragment);
	//Link any added shaders within the shader program we created
	glLinkProgram(shaderProgram);

	//Check for errorsbefore continuing 
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error shader linking failed\n" << infoLog << std::endl;
		return;
	}

	//Delete the shaders as they are now linked in our program
	//There is no need to keep them in memory at this point
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader() {
	//Remove the shader from the GPU freeing up memory
	glDeleteProgram(shaderProgram);
}

void Shader::Use() {

	//Use the program that you just created
	//You would call this before every batch of objects that need to be rendered
	//with the same shader
	glUseProgram(shaderProgram);
}

GLuint Shader::GetShaderProgram() {
	return shaderProgram;
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}
