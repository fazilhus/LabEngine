#include "config.h"
#include "gbuf.h"

namespace Render {

	GBuffer::GBuffer() {
		gBuf = 0;
		for (std::size_t i = 0; i < GBUF_NUM_TEXTURES; ++i)
			textures[i] = 0;
		depth = 0;
		final = 0;
	}

	GBuffer::~GBuffer() {
		if (gBuf != 0) {
			glDeleteFramebuffers(1, &gBuf);
		}

		if (textures[0] != 0) {
			glDeleteTextures(GBUF_NUM_TEXTURES, textures);
		}

		if (depth != 0) {
			glDeleteTextures(1, &depth);
		}

		if (final != 0) {
			glDeleteTextures(1, &final);
		}
	}

	void GBuffer::Init(unsigned int w, unsigned int h) {
		glGenFramebuffers(1, &gBuf);
		glGenTextures(GBUF_NUM_TEXTURES, textures);
		glGenTextures(1, &depth);
		glGenTextures(1, &final);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuf);

		for (unsigned int i = 0; i < GBUF_NUM_TEXTURES; ++i) {
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h,
				0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
				GL_TEXTURE_2D, textures[i], 0);
		}

		glBindTexture(GL_TEXTURE_2D, depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, w, h,
			0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
			GL_TEXTURE_2D, depth, 0);

		glBindTexture(GL_TEXTURE_2D, final);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 
			0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4,
			GL_TEXTURE_2D, final, 0);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void GBuffer::StartFrame() const {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuf);
		glDrawBuffer(GL_COLOR_ATTACHMENT4);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GBuffer::BindForGeometryPass() const {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuf);
		GLenum bufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, bufs);
	}

	void GBuffer::BindForStencilPass() const {
		glDrawBuffer(GL_NONE);
	}

	void GBuffer::BindForLightingPass() const {
		glDrawBuffer(GL_COLOR_ATTACHMENT4);
		for (unsigned int i = 0; i < GBUF_NUM_TEXTURES; ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
		}
	}

	void GBuffer::BindForDebugPass() const {
		glDrawBuffer(GL_COLOR_ATTACHMENT4);
	}

	void GBuffer::BindForFinalPass() const {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuf);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glReadBuffer(GL_COLOR_ATTACHMENT4);
	}

} // Render