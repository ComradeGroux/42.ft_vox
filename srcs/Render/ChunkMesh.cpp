#include "ChunkMesh.hpp"

ChunkMesh::ChunkMesh(void) : _vao(0), _vbo(0), _ebo(0), _indexCount(0)
{
	cgl(glGenVertexArrays(1, &_vao));
	cgl(glGenBuffers(1, &_vbo));
	cgl(glGenBuffers(1, &_ebo));
}

ChunkMesh::~ChunkMesh()
{
	cgl(glDeleteVertexArrays(1, &_vao));
	cgl(glDeleteBuffers(1, &_vbo));
	cgl(glDeleteBuffers(1, &_ebo));
}

void	ChunkMesh::_uploadtoGPU(void)
{
	cgl(glBindVertexArray(_vao));

	cgl(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
	cgl(glBufferData(GL_ARRAY_BUFFER,
					_vertices.size() * sizeof(Vertex),
					_vertices.data(),
					GL_STATIC_DRAW));
	
	cgl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
	cgl(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					_indices.size() * sizeof(uint32_t),
					_indices.data(),
					GL_STATIC_DRAW));
	
	cgl(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
							(void*)offsetof(Vertex, position)));
	cgl(glEnableVertexAttribArray(0));

	cgl(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
							(void*)offsetof(Vertex, normal)));
	cgl(glEnableVertexAttribArray(1));

	cgl(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
							(void*)offsetof(Vertex, uv)));
	cgl(glEnableVertexAttribArray(2));

	_indexCount = (int)_indices.size();

	cgl(glBindVertexArray(0));
}

void	ChunkMesh::_buildFace(const glm::vec3& pos, const glm::vec3& normal,
								const glm::vec2& uvMin, const glm::vec2& uvMax)
{
	uint32_t	baseIndex = (uint32_t)_vertices.size();

	glm::vec3 right, up;

	if (normal == glm::vec3(0, 1, 0) || normal == glm::vec3(0, -1, 0))
	{
		right 	= glm::vec3(1, 0, 0);
		up 		= glm::vec3(0, 0, 1);
	}
	else if (normal == glm::vec3(1, 0, 0) || normal == glm::vec3(-1, 0, 0))
	{
		right	= glm::vec3(0, 0, 1);
		up 		= glm::vec3(0, 1, 0);
	}
	else
	{
		right 	= glm::vec3(1, 0, 0);
		up 		= glm::vec3(0, 1, 0);
	}

	glm::vec3	origin = pos;
	if (normal.x > 0 || normal.y > 0 || normal.z > 0)
		origin += normal;

	_vertices.push_back({origin,				normal, {uvMin.x, uvMin.y}});
	_vertices.push_back({origin + right,		normal, {uvMax.x, uvMin.y}});
	_vertices.push_back({origin + right + up,	normal, {uvMax.x, uvMax.y}});
	_vertices.push_back({origin + up,			normal, {uvMin.x, uvMax.y}});

	_indices.push_back(baseIndex + 0);
	_indices.push_back(baseIndex + 1);
	_indices.push_back(baseIndex + 2);
	_indices.push_back(baseIndex + 0);
	_indices.push_back(baseIndex + 2);
	_indices.push_back(baseIndex + 3);
}

bool	ChunkMesh::_isSolid(const Chunk& chunk, int x, int y, int z) const
{
	if (x < 0 || x >= 16 || y < 0 || y >= 256 || z < 0 || z >= 16)
		return false;

	return chunk.get(x, y, z) != VoxelType::Air;
}

void	ChunkMesh::build(const Chunk& chunk)
{
	_vertices.clear();
	_indices.clear();

	glm::vec2	uvMin(0.0f, 0.0f);
	glm::vec2	uvMax(1.0f, 1.0f);
	glm::vec3	pos;

	for (int y = 0; y < 256; y++)
	{
		for (int z = 0; z < 16; z++)
		{
			for (int x = 0; x < 16; x++)
			{
				if (chunk.get(x, y, z) == VoxelType::Air)
					continue;

				pos.x = (float)x;
				pos.y = (float)y;
				pos.z = (float)z;

				if (!_isSolid(chunk, x, y + 1, z))
					_buildFace(pos, glm::vec3(0,  1,  0), uvMin, uvMax);
				if (!_isSolid(chunk, x, y - 1, z))
					_buildFace(pos, glm::vec3(0, -1,  0), uvMin, uvMax);
				if (!_isSolid(chunk, x + 1, y, z))
					_buildFace(pos, glm::vec3(1,  0,  0), uvMin, uvMax);
				if (!_isSolid(chunk, x - 1, y, z))
					_buildFace(pos, glm::vec3(-1, 0,  0), uvMin, uvMax);
				if (!_isSolid(chunk, x, y, z + 1))
					_buildFace(pos, glm::vec3(0,  0,  1), uvMin, uvMax);
				if (!_isSolid(chunk, x, y, z - 1))
					_buildFace(pos, glm::vec3(0,  0, -1), uvMin, uvMax);
			}
		}
	}


	if (!_vertices.empty())
		_uploadtoGPU();
}

void	ChunkMesh::draw() const
{
	if (_indexCount == 0)
		return;

	cgl(glBindVertexArray(_vao));
	cgl(glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0));
	cgl(glBindVertexArray(0));
}
