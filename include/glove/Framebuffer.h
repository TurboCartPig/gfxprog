#include <GL/glew.h>

/**
 * @brief Abstract type of framebuffer attachment.
 */
enum class AttachmentType {
	Color,
//	Depth,
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
	 * @param type Type of attachment to add. E.g. Color attachment, Depth
	 * attachment...
	 * @param dimensions Dimensions of the attachment.
	 */
	void addAttachment(AttachmentType type, glm::ivec2 dimensions);

	/**
	 * @brief Clear the framebuffer.
	 */
	void clear() const;

	/**
	 * Blit contents of source onto this framebuffer.
	 * @param source Source framebuffer.
	 * @param source_extents Extents to blit from framebuffer.
	 * @param destination_extents Extents to blit onto this framebuffer.
	 */
	void blit(const Framebuffer *source, glm::ivec4 source_extents,
	          glm::ivec4 destination_extents);

  private:
	/**
	 * @brief Wrap an already existing raw framebuffer from it's id.
	 * @param fbo Raw framebuffer object.
	 */
	Framebuffer(GLuint fbo);

  private:
	GLuint m_fbo;             ///< Framebuffer object.
	bool m_has_depth_stencil; ///< Does the Framebuffer have a depth and stencil
	                          ///< attachment.
	uint32_t m_color_attachments = 0; ///< Number of color attachments.
};