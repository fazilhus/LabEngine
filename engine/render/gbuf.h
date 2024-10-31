#pragma once

#include <gl/glew.h>

namespace Render {

	class GBuffer {
	public:
		enum GBUF_TEXTURE_TYPE {
			GBUF_POS,
			GBUF_COL,
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
		void BindForGeometryPass() const;
		void BindForStencilPass() const;
		void BindForLightingPass() const;
		void BindForDebugPass() const;
		void BindForFinalPass() const;
	};

} // Render