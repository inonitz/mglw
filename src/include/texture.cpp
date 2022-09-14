#include "texture.hpp"
#include "glad/glad.h"



Texture2D::Texture2D(		
	u32 			   		  imgFmt,
	u32                       mipmaps,
	std::array<i32, 2> const& dims,
	manvec<texConfig>  const& params
) {
	ifcrash(!params.len());

	debug(texParams.copy(params));
	glCreateTextures(GL_TEXTURE_2D, 1, &id);

	for(auto& param : params) {
		glTextureParameteri(id, param.paramID, param.paramValue);
	}

	glTextureStorage2D(
		id,
		mipmaps,
		imgFmt,
		dims[0],
		dims[1]
	);
	glBindImageTexture(0, id, 0, GL_FALSE, 0, GL_READ_ONLY, imgFmt);
	/* 
		Translation of the function above: 
		glBindImageTexture(
			GLuint    unit,    ==> Which Image Unit do we want? (We provide the ID of it.)
			GLuint    texture, ==> Which Texture    do we want? (We provide the ID of it.)
			GLint     level,   ==> Which Mipmap Level ('Downscaled texture') do we want? (We provide an index to the appropriate level. as level goes up, the image shrinks by 2)
			GLboolean layered, ==> A Mipmap Level is Layered if it contains multiple images in it. We specify if we want to bind to ALL images in the selected layer
			GLint     layer,   ==> If we don't bind to ALL images in the layer, we need to know which image do we actually bind to. (We provide an image(layer) index.)
			GLenum    access,  ==> What type of access patterns will occur with this Image Texture 
			GLenum    format   ==> The internal format of the data inside the image. (RGB24B, RGB32F, ...)
		) :

	*/
	return;
}


void Texture2D::bindToIndex(u32 unit) {
	glBindTextureUnit(unit, id);
}


void Texture2D::activeTexture(u32 id) {
	ifcrash(id > 31); glActiveTexture(GL_TEXTURE0 + id); return;
}