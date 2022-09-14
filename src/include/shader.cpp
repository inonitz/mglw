#include "shader.hpp"
#include "file.hpp"


Shader::Shader(manvec<srcMetadata> const& shaderSources)
{
	ifcrashdbg(shaderSources.len() == 0);
	const GLenum shaderTypes[3] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER };
	i32 		 srcLen 		= 0;

	debug(shaders.copy(shaderSources));
	shaderID.increaseTo(shaderSources.len());

	for(size_t i = 0; i < shaderSources.len(); ++i) {
		shaderID[i] = {
			glCreateShader(shaderTypes[(u32)shaderSources[i].type]),
			shaderSources[i].handle	
		};
		srcLen = (i32)shaderSources[i].length;

		glShaderSource(
			shaderID[i].first, 
			1,
			&shaderID[i].second,
			&srcLen
		);
	}
	return;
}


void Shader::destroy() 
{ 
	glDeleteProgram(progID); 
	return; 
}


void Shader::compile()
{
	debugnobr(static char errLog[512]);
	i32 success;
	

	progID = glCreateProgram();

	/* compile every shader in shaderID, check for errors and link to program. */
	for(auto& shader : shaderID) 
	{
		glCompileShader(shader.first);
		glGetShaderiv(shader.first, GL_COMPILE_STATUS, &success);


		ifcrashdo(!success, { /* Safety Check. More info in debug mode. */
			glGetShaderInfoLog(shader.first, sizeof(errLog), NULL, errLog);
			printf("%s\n", &errLog[0]);
		});


		glAttachShader(progID, shader.first);
	}


	/* Link shader program and check for errors. Delete shaders when finished. */
	glLinkProgram(progID);
	glGetProgramiv(progID, GL_LINK_STATUS, &success);
	ifcrashdo(!success, {
		glGetProgramInfoLog(progID, sizeof(errLog), NULL, errLog);
		printf("%s\n", &errLog[0]);
	});


	for(auto& shader : shaderID) {
		glDeleteShader(shader.first);
		free(shader.second);
		
		shader.second = nullptr;
		shader.first  = 0xF00DBABE;
	}
	return;
}


void Shader::setUniform(
	const char* name, 
	UniformType dtype, 
	i32  		count,
	void* 		data
) const {
	using glUniformFuncPtr    = void (*)(GLuint, GLint, GLsizei, GLvoid*);
	using glUniformMatFuncPtr = void (*)(GLuint, GLint, GLsizei, GLboolean, GLvoid*);
	const void* desiredFunc;

	const void* lookup[21] = {
		// Functions that get Values (Instead of Pointers) as args
		// are harder to work with, so we'll just use pointers.
		// (void (*)())glProgramUniform1f 
		// (void (*)())glProgramUniform2f 
		// (void (*)())glProgramUniform3f 
		// (void (*)())glProgramUniform4f 
		// (void (*)())glProgramUniform1i 
		// (void (*)())glProgramUniform2i 
		// (void (*)())glProgramUniform3i 
		// (void (*)())glProgramUniform4i 
		// (void (*)())glProgramUniform1ui
		// (void (*)())glProgramUniform2ui
		// (void (*)())glProgramUniform3ui
		// (void (*)())glProgramUniform4ui
		(void*)glProgramUniform1fv        ,
		(void*)glProgramUniform2fv        ,
		(void*)glProgramUniform3fv        ,
		(void*)glProgramUniform4fv        ,
		(void*)glProgramUniform1iv        ,
		(void*)glProgramUniform2iv        ,
		(void*)glProgramUniform3iv        ,
		(void*)glProgramUniform4iv        ,
		(void*)glProgramUniform1uiv       ,
		(void*)glProgramUniform2uiv       ,
		(void*)glProgramUniform3uiv       ,
		(void*)glProgramUniform4uiv       ,
		(void*)glProgramUniformMatrix2fv  ,
		(void*)glProgramUniformMatrix3fv  ,
		(void*)glProgramUniformMatrix4fv  ,
		(void*)glProgramUniformMatrix2x3fv, 
		(void*)glProgramUniformMatrix3x2fv, 
		(void*)glProgramUniformMatrix2x4fv, 
		(void*)glProgramUniformMatrix4x2fv, 
		(void*)glProgramUniformMatrix3x4fv, 
		(void*)glProgramUniformMatrix4x3fv
	};


	desiredFunc = lookup[(int)dtype - (int)UniformType::f32p];
	if((i32)dtype > (i32)UniformType::mat22) /* if type is a matrix */ 
	{
		((glUniformMatFuncPtr)desiredFunc)(
			progID,
			glGetUniformLocation(progID, name),
			count, 
			false, /* Don't transpose the matrix. */
			data
		);
	} 
	else {
		((glUniformFuncPtr)desiredFunc)(
			progID,
			glGetUniformLocation(progID, name),
			count,
			data
		);
	}

	return;
}


Shader::srcMetadata loadShader(std::string_view const& path, ShaderType stype) {
	Shader::srcMetadata mdata{ stype, nullptr, 0 };

	loadFile(path.cbegin(), (size_t*)&mdata.length, mdata.handle); /* Get Necessary Data in mdata */
	mdata.handle = amalloc_t(char, mdata.length, CACHE_LINE_BYTES);	
	
	ifcrash(!boolean(mdata.handle));
	loadFile(path.cbegin(), (size_t*)&mdata.length, mdata.handle);

	return mdata;
}