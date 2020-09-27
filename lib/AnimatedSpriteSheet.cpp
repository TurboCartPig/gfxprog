#include <glove/AnimatedSpriteSheet.h>

AnimatedSpriteSheet::AnimatedSpriteSheet(milliseconds             frameTime,
                                         SpriteSheetAnimation     animation,
                                         std::shared_ptr<Texture> spritesheet)
    : m_frame_time(frameTime), m_animation(std::move(animation)),
      m_sprite_sheet(std::move(spritesheet)) {}

void AnimatedSpriteSheet::update(std::chrono::duration<double> dt) {
	if (m_paused)
		return;

	m_from_last_key_frame = m_from_last_key_frame + dt;

	// Move to next keyframe
	if (m_from_last_key_frame >= m_frame_time) {
		m_from_last_key_frame = milliseconds(0);
		m_current_key_frame++;

		// Animation finished
		if (m_current_key_frame >= m_animation.m_key_frames.size()) {
			// Do not loop
			if (!m_looped) {
				m_paused = true;
			}

			m_current_key_frame = 0;
		}
	}
}

void AnimatedSpriteSheet::playAnimation(SpriteSheetAnimation animation) {
	m_paused              = false;
	m_current_key_frame   = 0;
	m_from_last_key_frame = milliseconds(0);
	m_animation           = std::move(animation);
}
