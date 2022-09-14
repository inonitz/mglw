#pragma once
#include "manvec.hpp"


struct Texture2D
{
private:
	u32 id;

	struct texConfig {
		u32 paramID;
		u32 paramValue;
	};


	#ifdef _DEBUG
		manvec<texConfig> texParams;
	#endif
public:

	Texture2D(
		u32                		  imgFmt,
		u32                       mipmaps,
		std::array<i32, 2> const& dims,
		manvec<texConfig>  const& params
	);


	void bindToIndex(u32 unit);
	void activeTexture(u32 id);
};