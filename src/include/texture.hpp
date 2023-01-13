#pragma once
#include "base.hpp"
#include "manvec.hpp"
#include <optional>
#include <array>




void TextureConvertRGBAUI32ToRGBA32F(u8* inData, float* outData, i32 x, i32 y);




struct Texture2D
{
private:
	/* 
		format & type describe internal format. 
		Ex: GL_RGBA32F(internal) = GL_RGBA(format), GL_FLOAT(dtype) 
	*/
	struct texDataFmtDescriptor {
		u32 internal;
		u32 format;
		u32 dtype;
	};
	using format_descriptor = texDataFmtDescriptor;


	struct texConfig {
		u32 paramID;
		u32 paramValue;
	};

	
	u32 			   id;
	u32                boundTexUnit;
	u32                boundImageUnit;
	std::array<i32, 2> currDims;
	format_descriptor  texPixelFmt;
	manvec<texConfig>  texParams;
	void*              oncpu_dataHandle;
private:
	i32 textureDimensionsMaxLen() const;
	
public:
	Texture2D() : 
		id{DEFAULT32},
		boundTexUnit{DEFAULT32},
		boundImageUnit{DEFAULT32},
		currDims{(i32)DEFAULT32, (i32)DEFAULT32},
		texPixelFmt{},
		texParams{},
		oncpu_dataHandle{nullptr}
	{}


	void create(
		texDataFmtDescriptor      fmtDesc,
		std::array<i32, 2> const& dims,
		manvec<texConfig>  const& params,
		std::optional<void*> 	  imgptr
	);
	
	
	void destroy();


	void recreateImage  (std::array<i32, 2> const& newDims);
	void bindToTexUnit  (u32 unit);
	void bindToImageUnit(u32 unit);
	void unbindTexUnit  ();
	void unbindImageUnit();
	void activeTexture(u32 id);


	__force_inline u32 				  glid() 	   const { return id;			    }
	__force_inline u32 				  texUnit()    const { return boundTexUnit;     }
	__force_inline u32 				  imageUnit()  const { return boundImageUnit;   }
	__force_inline std::array<i32, 2> dimensions() const { return currDims; 	    }
	__force_inline void*              oncpuData()  const { return oncpu_dataHandle; }
	void print();
};

