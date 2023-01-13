#pragma once
#include "manvec.hpp"
#include <string_view>
#include <utility>
#include <array>
#include "glad/glad.h"




enum class ShaderType {
	Vertex,
	Fragment,
	Compute
};


__force_inline const char* ShaderTypeToStr(ShaderType type) 
{
	constexpr std::array<const char*, 3> convert = {
		"GL_VERTEX_SHADER"  ,
		"GL_FRAGMENT_SHADER",
		"GL_COMPUTE_SHADER" ,
	};
	return convert[(u32)type];
}




enum class pack UniformType {
	f32,
	vec2f,
	vec3f,
	vec4f,
	i32,
	vec2i,
	vec3i,
	vec4i,
	u32,
	vec2u,
	vec3u,
	vec4u,
	f32p,
	vec2fp,
	vec3fp,
	vec4fp,
	i32p,
	vec2ip,
	vec3ip,
	vec4ip,
	u32p,
	vec2up,
	vec3up,
	vec4up,
	mat22,
	mat33,
	mat44,
	mat23,
	mat32,
	mat24,
	mat42,
	mat34,
	mat43
};



struct Shader {
private:
	manvec<std::pair<u32, char*>> shaderID;
	u32 						  progID;

public:
	typedef char* srcHandle;
	
	struct srcMetadata {
		ShaderType type;
		srcHandle  handle;
		i64        length;
	};
	#ifdef _DEBUG
		manvec<srcMetadata> shaders;
	#endif


public:
	Shader() : 
		shaderID{}, 
		progID{DEFAULT32}
	{}

	
	void create(manvec<srcMetadata> const& shaderSources);
	void destroy();

	
	void compile();
	void setUniform(
		const char* name,
		UniformType dtype, 
		i32  		count, /* given a getUniformMatrix function, count represents the amount of matrices to be modified! */
		void* 		data
	) const;

	__force_inline void setActive() { 
		glUseProgram(progID); 
	}

	__force_inline void unbind() {
		glUseProgram(0);
	}

	__force_inline void beginCompute(std::array<i32, 3> const& numGroup) {
		glDispatchCompute((u32)numGroup[0], (u32)numGroup[1], (u32)numGroup[2]); 
		
	}

	__force_inline void barrier(u32 bitArgs) {
		glMemoryBarrier(bitArgs);
	}


	void print();

};


Shader::srcMetadata loadShader(std::string_view const& path, ShaderType stype);

