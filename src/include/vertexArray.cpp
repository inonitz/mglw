#include "vertexArray.hpp"
#include "glad/glad.h"




u32 vdtypeToGL(VertexDataType vdtype) {
	constexpr std::array<u32, 5> convert = {
		GL_FLOAT,
		GL_UNSIGNED_INT,
		GL_INT,
		GL_UNSIGNED_SHORT,
		GL_SHORT
	};

	return convert[(u32)vdtype];
}


u32 vdtypeToBytes(VertexDataType vdtype) {
	constexpr std::array<u32, 5> convert = {
		4u,
		4u,
		4u,
		2u,
		2u
	};

	return convert[(u32)vdtype];
}




VertexBuffer::VertexBuffer(void* data, size_t bytes, u32 usage) {
	glCreateBuffers(1, &id);
	glNamedBufferStorage(id, bytes, data, usage);
}

void VertexBuffer::destroy() { glDeleteBuffers(1, &id); return; }


IndexBuffer::IndexBuffer(void* data, size_t bytes, u32 usage) {
	glCreateBuffers(1, &id);
	glNamedBufferStorage(id, bytes, data, usage);
}

void IndexBuffer::destroy() { glDeleteBuffers(1, &id); return; }




VertexArray::VertexArray(
	VertexBuffer const& 			 vb,
	std::optional<IndexBuffer>       ib,
	manvec<VertexDescriptor>  const& bufDesriptor
) {
	VertexDataType temp;
	u32 		   stride  = 0;
	u32 		   bindidx = 0;
	

	debug(vbDescriptor.copy(bufDesriptor));

	for(auto& vdesc : bufDesriptor) {
		stride += vdtypeToBytes((VertexDataType)vdesc.dtype) * vdesc.len;
	}

	glCreateVertexArrays(1, &id); /* create VAO */
	glVertexArrayVertexBuffer(id, bindidx, vb.glid(), 0, stride); /* bind buffer to vao at bindingindex */
	if(ib.has_value()) { glVertexArrayElementBuffer(id, ib->glid()); } /* bind index buffer to vao */

	stride = 0; /* This now becomes the relative offset per vertex attribute. */
	for(size_t i = 0; i < bufDesriptor.len(); ++i) {
		auto& vdesc = bufDesriptor[i];
		temp 		 = (VertexDataType)vdesc.dtype;
		debug(
			vbDescriptor[i].reloff = stride
		);

		glEnableVertexArrayAttrib(id,  i); /* activate attrib i. */
		glVertexArrayAttribFormat(id,  i, vdesc.len, vdtypeToGL(temp), vdesc.normed, stride); /* provide description. */
		glVertexArrayAttribBinding(id, i, bindidx); /* bind attribute to bindindex 0 in vao. */

		stride += vdtypeToBytes(temp) * vdesc.len;
	}

	return;
}


void VertexArray::destroy() { glDeleteVertexArrays(1, &id); return; }


void VertexArray::debugInfo() {
#ifndef _DEBUG
	ifcrashdo(true, { printf("Can't Call A Function that uses Debug Variables in Release Mode!"); });
#endif
#ifdef _DEBUG

	const std::pair<VertexDataType, const char*> typeToStr[5] = {
		{ VertexDataType::f32, "GL_FLOAT" 		   },
		{ VertexDataType::u32, "GL_UNSIGNED_INT"   },
		{ VertexDataType::i32, "GL_INT" 		   },
		{ VertexDataType::u16, "GL_UNSIGNED_SHORT" },
		{ VertexDataType::i16, "GL_SHORT" 		   }
	};
	const char* boolToStr[2] = { "GL_FALSE", "GL_TRUE" };
	u32 temp = 0;
	u32 idx  = 0;

 
	for(auto& desc : vbDescriptor) 
	{
		while((u32)typeToStr[temp].first != desc.dtype) ++temp;
		
		printf("Attribute %u\n  len             = %u Elements\n  dtype           = %s\n  normalized      = %s\n  relative offset = %u Bytes\n",
			idx,
			desc.len,
			typeToStr[temp].second,
			boolToStr[desc.normed],
			desc.reloff
		);

		temp = 0;
		++idx;
	}
#endif

	return;
}


void VertexArray::bind()   { glBindVertexArray(id); }
void VertexArray::unbind() { glBindVertexArray(0);  }