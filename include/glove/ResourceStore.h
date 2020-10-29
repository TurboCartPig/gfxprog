#pragma once

#include <iostream>
#include <memory>

/**
 * FIXME: How should the store deal with being "full"? or larger then necessary?
 */
class ResourceStore {
  public:
	/**
	 * Get or load a resource of type "T" and name "Name".
	 * @tparam T
	 * @param name
	 * @return
	 */
	template <typename T>
	static std::shared_ptr<T> get(const std::string &name);

  private:
	ResourceStore();

  private:
	static std::unique_ptr<ResourceStore> store;

	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
};