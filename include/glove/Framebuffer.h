#include <GL/glew.h>
#include <optional>

/**
 * @brief Abstract type of framebuffer attachment.
 */
enum class AttachmentType {
	Color,
	Depth,
	//	Stencil,
	DepthStencil,
};

/**
 * @brief A Framebuffer abstraction on top of OpenGL's framebuffers.
 *
 * # Construction
 * A framebuffer is constructed by first calling the constructor and then adding
 * one or more attachments via "addAttachment".
 *
 * # Binding
 * The framebuffer is bound using "bind" and will remain bound until another
 * framebuffer is bound.
 *
 * NOTE: Almost all operations on framebuffers require the framebuffer to be
 * bound first. How to bind it is up to you to figure out.
 *
 * # Validity
 * You can check if the constructed framebuffer is valid with "valid". A
 * framebuffer is for example not valid if it does not have *any* attachments.
 */
class Framebuffer {
  public:
	/**
	 * @brief Construct a new framebuffer with no attachments.
	 */
	Framebuffer();

	Framebuffer(const Framebuffer &other) = delete;

	Framebuffer(const Framebuffer &&other) = delete;

	auto operator=(const Framebuffer &other) = delete;

	auto operator=(const Framebuffer &&other) = delete;

	~Framebuffer();

	/**
	 * @brief Wrap the default framebuffer object created by GLFW.
	 * @return Default framebuffer.
	 */
	static auto defaultFramebuffer() -> std::unique_ptr<Framebuffer>;

	/**
	 * @brief Is the framebuffer valid and ready to be used?
	 * @return Is the framebuffer valid?
	 */
	[[nodiscard]] auto valid() const -> bool;

	/**
	 * @brief Bind the framebuffer. Optionally with a bind target. The default
	 * bind target is GL_FRAMEBUFFER, aka all.
	 * @param target Optional bind target.
	 */
	void bind(GLenum target = GL_FRAMEBUFFER) const;

	/**
	 * @brief Add an additional attachment to the framebuffer.
	 * Misusing the framebuffer will generate OpenGL errors. No errors will be
	 * generated from this function.
	 * @param type Type of attachment to add. E.g. Color attachment, Depth
	 * attachment...
	 * @param dimensions Dimensions of the attachment.
	 */
	void addAttachment(AttachmentType type, glm::ivec2 dimensions);

	/**
	 * @brief Clear the framebuffer.
	 * NOTE: Can not clear the default framebuffer.
	 */
	void clear() const;

	/**
	 * @brief Resize the framebuffer.
	 * @param width The new width.
	 * @param height The new height.
	 */
	void resize(int width, int height);

	/**
	 * @brief Blit contents of source onto this framebuffer.
	 * @param source Source framebuffer.
	 * @param source_extents Extents to blit from framebuffer.
	 * @param destination_extents Extents to blit onto this framebuffer.
	 */
	void blit(const Framebuffer *source, glm::ivec4 source_extents,
	          glm::ivec4 destination_extents);

	/**
	 * @brief Bind the depth attachment of the framebuffer as a texture.
	 */
	void bindDepthAttachmentToSlot(GLuint slot) const;

  private:
	/**
	 * @brief Wrap an already existing raw framebuffer from it's id.
	 * FIXME: Should be passed other info about the default framebuffer.
	 * @param fbo Raw framebuffer object.
	 */
	Framebuffer(GLuint fbo);

  private:
	GLuint              m_fbo;                ///< Framebuffer object.
	std::vector<GLuint> m_color_attachments;  ///< The color attachment textures
	std::optional<GLuint> m_depth_attachment; ///< Depth attachment texture
	std::optional<GLuint>
	           m_depth_stencil_attachment; ///< Depth stencil attachment texture
	glm::ivec2 m_dimensions; ///< Larges dimension of all the framebuffer
	                         ///< attachments. Should be the glViewport when
	                         ///< rendering to this framebuffer.
};