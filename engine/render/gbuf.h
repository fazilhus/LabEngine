#pragma once

#include <gl/glew.h>

namespace Render {

	class GBuffer {
	public:
		enum GBUF_TEXTURE_TYPE {
			GBUF_POS,
			GBUF_DIFFSPEC,
			GBUF_NORM,
			GBUF_NUM_TEXTURES
		};

	private:
		GLuint gBuf;
		GLuint textures[GBUF_NUM_TEXTURES];
		GLuint depth;
		GLuint final;

	public:
		GBuffer();
		~GBuffer();

		void Init(unsigned int w, unsigned int h);
		void StartFrame() const;
		void BindForGeometryPass();
		void BindForStencilPass();
		void BindForLightingPass();
		void BindForFinalPass();
	};

} // Render