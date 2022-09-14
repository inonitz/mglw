#include "manvec.hpp"
#include <optional>


enum class VertexDataType {
	f32,
	u32,
	i32,
	u16,
	i16
};


u32 vdtypeToGL(VertexDataType vdtype);
u32 vdtypeToBytes(VertexDataType vdtype);




struct VertexBuffer {
private:
	u32 id;

public:
	VertexBuffer(void* data, size_t bytes, u32 usage);
	void destroy();

	__force_inline u32 glid() const { return id; }
};


struct IndexBuffer {
private:
	u32 id;

public:
	IndexBuffer(void* data, size_t bytes, u32 usage);
	void destroy();


	__force_inline u32 glid() const { return id; }
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


	#ifdef _DEBUG
		/* Also include debug info on vb & ib. */
		manvec<VertexDescriptor> vbDescriptor;
	#endif
public:

	VertexArray(
		VertexBuffer const&  	   	    vb,
		std::optional<IndexBuffer> 	    ib,
		manvec<VertexDescriptor> const& bufDesriptor
	);
	
	
	void destroy();


	void debugInfo();
	void bind();
	void unbind();

	__force_inline u32 glid() { return id; }
};