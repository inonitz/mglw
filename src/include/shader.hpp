#pragma once
#include "manvec.hpp"
#include "glad/glad.h"


enum class ShaderType {
	Vertex,
	Fragment,
	Compute
};



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
	Shader(manvec<srcMetadata> const& shaderSources);
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

	__force_inline void beginCompute(std::array<i32, 3> const& numGroup) {
		glDispatchCompute((u32)numGroup[0], (u32)numGroup[1], (u32)numGroup[2]); 
		
	}

	__force_inline void barrier(u32 bitArgs) {
		glMemoryBarrier(bitArgs);
	}

	__force_inline void printComputeInfo() {
		int work_grp_cnt[3];
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
		printf("Maximum Work Groups Available [glDispatch Args] = <%i, %i, %i>\n", work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);


		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_cnt[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_cnt[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_cnt[2]);
		printf("Maximum Work Group Size [In-Shader local(x, y, z)] = <%i, %i, %i>\n", work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);
	
		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_cnt[0]);
		printf("Maximum Invocations Per Work Group = %i\n", work_grp_cnt[0]);
	}

};


Shader::srcMetadata loadShader(std::string_view const& path, ShaderType stype);

