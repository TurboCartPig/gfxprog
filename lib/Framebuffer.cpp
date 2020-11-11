#include <glove/Framebuffer.h>

Framebuffer::Framebuffer()
    : m_depth_attachment(std::nullopt),
      m_depth_stencil_attachment(std::nullopt), m_dimensions(glm::ivec2(0)) {
	glGenFramebuffers(1, &m_fbo);
}

Framebuffer::Framebuffer(GLuint fbo)
    : m_fbo(fbo), m_depth_attachment(std::nullopt),
      m_depth_stencil_attachment(std::nullopt),
      m_dimensions(glm::ivec2(1280, 720)) {}

Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &m_fbo); }

auto Framebuffer::defaultFramebuffer() -> std::unique_ptr<Framebuffer> {
	return std::unique_ptr<Framebuffer>(new Framebuffer(0));
}

auto Framebuffer::valid() const -> bool {
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::bind(GLenum target) const {
	glViewport(0, 0, m_dimensions.x, m_dimensions.y);
	glBindFramebuffer(target, m_fbo);
}

void Framebuffer::addAttachment(AttachmentType type, glm::ivec2 dimensions) {
	// Set maximum extents of any attachment
	m_dimensions.x = std::max(m_dimensions.x, dimensions.x);
	m_dimensions.y = std::max(m_dimensions.y, dimensions.y);

	// Create texture
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	GLenum attachment;
	GLint  internal_format;
	GLenum sample_format;
	GLenum pixel_format;

	switch (type) {
		case AttachmentType::Color:
			attachment      = GL_COLOR_ATTACHMENT0 + m_color_attachments.size();
			internal_format = GL_RGB8;
			sample_format   = GL_RGB;
			pixel_format    = GL_UNSIGNED_BYTE;
			m_color_attachments.push_back(tex);
			break;
		case AttachmentType::Depth:
			attachment         = GL_DEPTH_ATTACHMENT;
			internal_format    = GL_DEPTH_COMPONENT;
			sample_format      = GL_DEPTH_COMPONENT;
			pixel_format       = GL_FLOAT;
			m_depth_attachment = tex;
			break;
		case AttachmentType::DepthStencil:
			attachment                 = GL_DEPTH_STENCIL_ATTACHMENT;
			internal_format            = GL_DEPTH24_STENCIL8;
			sample_format              = GL_DEPTH_STENCIL;
			pixel_format               = GL_UNSIGNED_INT_24_8;
			m_depth_stencil_attachment = tex;
			break;
	}

	// Allocate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, dimensions.x, dimensions.y,
	             0, sample_format, pixel_format, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex, 0);
}

void Framebuffer::clear() const {
	glClear((!m_color_attachments.empty() ? GL_COLOR_BUFFER_BIT : 0) |
	        (m_depth_attachment.has_value() ? GL_DEPTH_BUFFER_BIT : 0) |
	        (m_depth_stencil_attachment.has_value()
	             ? GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
	             : 0));
}

void Framebuffer::blit(const Framebuffer *source, glm::ivec4 source_extents,
                       glm::ivec4 destination_extents) {
	source->bind(GL_READ_FRAMEBUFFER);
	this->bind(GL_DRAW_FRAMEBUFFER);
	glBlitFramebuffer(source_extents.x, source_extents.y, source_extents.z,
	                  source_extents.w, destination_extents.x,
	                  destination_extents.y, destination_extents.z,
	                  destination_extents.w, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Framebuffer::bindDepthAttachmentToSlot(GLuint slot) const {
	assert(m_depth_attachment.has_value() &&
	       "Framebuffer does not have a depth attachment to bind to slot");

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_depth_attachment.value());
}
