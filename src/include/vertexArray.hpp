#pragma once
#include "manvec.hpp"
#include <optional>




enum class VertexDataType {
	f32,
	u32,
	i32,
	u16,
	i16
};


u32 		vdtypeToGL   (VertexDataType vdtype);
u32 		vdtypeToBytes(VertexDataType vdtype);
const char* vdtypeToStr  (VertexDataType vdtype);




struct bufMeta {
	VertexDataType dtype;
	u32 		   count;

	size_t bytes() const { return (size_t)vdtypeToBytes(dtype) * count; }
};




struct VertexBuffer {
private:
	u32 	id;
	bufMeta meta;

public:
	VertexBuffer() : id(DEFAULT32), meta{ (VertexDataType)DEFAULT32, DEFAULT32 } {}


	void create(const void* data, u32 count, VertexDataType dtype, u32 usage);
	void destroy();


	__force_inline u32  		  glid()     const { return id;   }
	__force_inline bufMeta const& metadata() const { return meta; }


	__force_inline void print() const 
	{
		printf("VertexBuffer: (ID %u)\n\
    Element Count: %u\n\
    Element Type %s = %u Bytes\n\
    Bytes (Total): %llu\n\n",
			id, 
			meta.count, 
			vdtypeToStr(meta.dtype), 
			vdtypeToBytes(meta.dtype), 
			meta.bytes()
		);
		return;
	}
};


struct IndexBuffer {
private:
	u32 	id;
	bufMeta meta;

public:
	IndexBuffer() : id(DEFAULT32) {}


	void create(const void* data, u32 count, VertexDataType dtype, u32 usage);
	void destroy();


	__force_inline u32            glid()     const { return id;   					}
	__force_inline u32            count()    const { return meta.count;             }
	__force_inline u32            gltype()   const { return vdtypeToGL(meta.dtype); }
	__force_inline bufMeta const& metadata() const { return meta; 				    }


	__force_inline void print() const 
	{
		printf("IndexBuffer:  (ID %u)\n\
    Element Count: %u\n\
    Element Type %s = %u Bytes\n\
    Bytes (Total): %llu\n\n",
			id, 
			meta.count, 
			vdtypeToStr(meta.dtype), 
			vdtypeToBytes(meta.dtype), 
			meta.bytes()
		);
		return;
	}
};




struct VertexArray {
private:
	u32 id;


	struct VertexDescriptor {
		i32  len;
		u32  dtype;
		bool normed;
		u32  reloff;
	};


	struct BufferDescriptor {
		VertexDataType dtype;
		u32            count;
		u32            glID;
	};


	#ifdef _DEBUG
		manvec<BufferDescriptor> glObjDescriptor;
		manvec<VertexDescriptor> vbDescriptor;
	#endif
public:
	VertexArray() : id(DEFAULT32) {}
	

	void create(
		std::optional<VertexBuffer> const& vb,
		std::optional<IndexBuffer>  const& ib,
		manvec<VertexDescriptor>    const& bufDesriptor
	);
	void destroy();


	void bind();
	void unbind();


	__force_inline u32 glid() { return id; }
	void print();
};