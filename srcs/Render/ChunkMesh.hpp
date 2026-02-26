#pragma once

#include "Chunk.hpp"

#include "GLError.hpp"
#include "glad.h"
#include "glm.hpp"

#include <vector>

struct Vertex {
	glm::vec3	position;
	glm::vec3	normal;
	glm::vec2	uv;
};

struct NeighborChunks {
    const Chunk* px; // +X
    const Chunk* nx; // -X
    const Chunk* pz; // +Z
    const Chunk* nz; // -Z
};

class ChunkMesh {
	private:
		GLuint	_vao;
		GLuint	_vbo;
		GLuint	_ebo;
		int		_indexCount;
		std::vector<Vertex>		_vertices;
		std::vector<uint32_t>	_indices;

		GLuint	_waterVao;
		GLuint	_waterVbo;
		GLuint	_waterEbo;
		int		_waterIndexCount;
		std::vector<Vertex>		_waterVertices;
		std::vector<uint32_t>	_waterIndices;

		void		_buildFace(const glm::vec3& pos, const glm::vec3& normal,
								const glm::vec2& uvMin, const glm::vec2& uvMax,
								std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
		bool		_isSolid(const Chunk& chunk, int x, int y, int z, const NeighborChunks& neighbors) const;
		VoxelType	_getVoxel(const Chunk& chunk, int x, int y, int z, const NeighborChunks& neighbors) const;


		void	_uploadBuffers(GLuint vao, GLuint vbo, GLuint ebo,
								const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
								int& indexCount);
		void	_uploadToGPU(void);

	public:
		ChunkMesh(void);
		~ChunkMesh(void);

		void	build(const Chunk& chunk, const NeighborChunks& neighbors);
		void	draw(void) const;
		void	drawWater(void) const;
		
		bool	isEmpty(void) const;
		bool	isWaterEmpty(void) const;
};
