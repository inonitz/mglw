#include "vertexArray.hpp"
#include "glad/glad.h"




u32 vdtypeToGL(VertexDataType vdtype) 
{
	ifcrashdbg((u32)vdtype == DEFAULT32);
	
	
	constexpr std::array<u32, 5> convert = {
		GL_FLOAT,
		GL_UNSIGNED_INT,
		GL_INT,
		GL_UNSIGNED_SHORT,
		GL_SHORT
	};
	return convert[(u32)vdtype];
}


u32 vdtypeToBytes(VertexDataType vdtype) 
{
	ifcrashdbg((u32)vdtype == DEFAULT32);
	
	
	constexpr std::array<u32, 5> convert = { 4u, 4u, 4u, 2u, 2u };
	return convert[(u32)vdtype];
}


const char* vdtypeToStr(VertexDataType vdtype)
{
	ifcrashdbg((u32)vdtype == DEFAULT32);
	

	constexpr std::array<const char*, 5> convert = {
		"GL_FLOAT",
		"GL_UNSIGNED_INT",
		"GL_INT",
		"GL_UNSIGNED_SHORT",
		"GL_SHORT"
	};
	return convert[(u32)vdtype];
}




void VertexBuffer::create(const void* data, u32 count, VertexDataType dtype, u32 usage) {
	meta = { dtype, count };

	glCreateBuffers(1, &id);
	glNamedBufferStorage(id, meta.bytes(), data, usage);
}

void VertexBuffer::destroy() 
{ 
	ifcrashdo(id == 0, { 
		printf("VertexBuffer::destroy() => Can't Destroy OpenGL Buffer Object that hasn't been created\n"); 
	});  
	glDeleteBuffers(1, &id); 
	return; 
}




void IndexBuffer::create(const void* data, u32 count, VertexDataType dtype, u32 usage) {
	meta = { dtype, count };
	
	glCreateBuffers(1, &id);
	glNamedBufferStorage(id, meta.bytes(), data, usage);
}

void IndexBuffer::destroy() 
{ 
	ifcrashdo(id == 0, { 
		printf("IndexBuffer::destroy() => Can't Destroy OpenGL Buffer Object that hasn't been created\n"); 
	}); 
	glDeleteBuffers(1, &id); 
	return; 
}




void VertexArray::create(
	std::optional<VertexBuffer> const& vb,
	std::optional<IndexBuffer>  const& ib,
	manvec<VertexDescriptor>    const& bufDesriptor
) {
	VertexDataType temp;
	u32 		   stride  = 0;
	u32 		   bindidx = 0;
	

	debug(
		vbDescriptor.copy(bufDesriptor);
		glObjDescriptor.increaseTo(vb.has_value() + ib.has_value());
	);
	/* get the stride */
	for(auto& vdesc : bufDesriptor) {
		stride += vdtypeToBytes((VertexDataType)vdesc.dtype) * vdesc.len;
	}


	glCreateVertexArrays(1, &id); /* create VAO */

	if(vb.has_value()) {
		debug(
			glObjDescriptor[0] = 
			{ 
				vb->metadata().dtype, 
				vb->metadata().count, 
				vb->glid() 
			};
		);
		glVertexArrayVertexBuffer(id, bindidx, vb->glid(), 0, stride); /* bind buffer to vao at bindingindex */
	}
	if(ib.has_value()) 
	{ 
		debug( /* set debug info about index buffer */
			glObjDescriptor[1] = 
			{
				ib->metadata().dtype, 
				ib->count(), 
				ib->glid() 
			};
		);
		glVertexArrayElementBuffer(id, ib->glid()); /* bind index buffer to vao */ 
	}


	if(!vb.has_value()) 
		return;

	
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


void VertexArray::destroy() 
{ 
	ifcrashdo(id == 0, { 
		printf("VertexArray::destroy() => Can't Destroy OpenGL VertexArray Object that hasn't been created\n"); 
	}); 
	glDeleteVertexArrays(1, &id); 
	return; 
}


void VertexArray::print() 
{
	printf("Vertex Array Object (ID %u)\n", id);


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

	if(glObjDescriptor.len() == 2) 
	{
		printf("\
    VertexBuffer (%u):\n\
        dtype: %s\n\
        count: %u\n\
        total: %llu Bytes\n",
		glObjDescriptor[0].glID,
		vdtypeToStr(glObjDescriptor[0].dtype),
		glObjDescriptor[0].count,
		(size_t)glObjDescriptor[0].count * vdtypeToBytes(glObjDescriptor[0].dtype)
	);
		printf("\
    IndexBuffer  (%u):\n\
        dtype: %s\n\
        count: %u\n\
        total: %llu Bytes\n",
			glObjDescriptor[1].glID,
			vdtypeToStr(glObjDescriptor[1].dtype),
			glObjDescriptor[1].count,
			(size_t)glObjDescriptor[1].count * vdtypeToBytes(glObjDescriptor[1].dtype)
		);
	}



	printf("    Attributes:\n");
	for(auto& desc : vbDescriptor) 
	{
		while((u32)typeToStr[temp].first != desc.dtype) ++temp;
		
		printf("\
    Attribute %u:\n\
        len             = %u Elements\n\
        dtype           = %s\n\
        normalized      = %s\n\
        relative offset = %u Bytes\n",
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