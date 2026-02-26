#include "ChunkMesh.hpp"

ChunkMesh::ChunkMesh(void) : _vao(0), _vbo(0), _ebo(0), _indexCount(0), _waterVao(0), _waterVbo(0), _waterEbo(0), _waterIndexCount(0)
{
	cgl(glGenVertexArrays(1, &_vao));
	cgl(glGenBuffers(1, &_vbo));
	cgl(glGenBuffers(1, &_ebo));

	cgl(glGenVertexArrays(1, &_waterVao));
	cgl(glGenBuffers(1, &_waterVbo));
	cgl(glGenBuffers(1, &_waterEbo));
}

ChunkMesh::~ChunkMesh()
{
	cgl(glDeleteVertexArrays(1, &_vao));
	cgl(glDeleteBuffers(1, &_vbo));
	cgl(glDeleteBuffers(1, &_ebo));

	cgl(glDeleteVertexArrays(1, &_waterVao));
	cgl(glDeleteBuffers(1, &_waterVbo));
	cgl(glDeleteBuffers(1, &_waterEbo));
}

void	ChunkMesh::_uploadBuffers(GLuint vao, GLuint vbo, GLuint ebo, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, int& indexCount)
{
	cgl(glBindVertexArray(vao));

	cgl(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	cgl(glBufferData(GL_ARRAY_BUFFER,
					vertices.size() * sizeof(Vertex),
					vertices.data(),
					GL_STATIC_DRAW));
	
	cgl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	cgl(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					indices.size() * sizeof(uint32_t),
					indices.data(),
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

	indexCount = (int)indices.size();

	cgl(glBindVertexArray(0));
}

void	ChunkMesh::_uploadToGPU(void)
{
	if (!_vertices.empty())
		_uploadBuffers(_vao, _vbo, _ebo, _vertices, _indices, _indexCount);

	if (!_waterVertices.empty())
		_uploadBuffers(_waterVao, _waterVbo, _waterEbo, _waterVertices, _waterIndices, _waterIndexCount);
}

void	ChunkMesh::_buildFace(const glm::vec3& pos, const glm::vec3& normal,
								const glm::vec2& uvMin, const glm::vec2& uvMax, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	uint32_t	baseIndex = (uint32_t)vertices.size();
	glm::vec3	v0, v1, v2, v3;

	if (normal == glm::vec3(0, 1, 0)) // Dessus
	{
		v0 = pos + glm::vec3(0, 1, 0);
		v1 = pos + glm::vec3(0, 1, 1);
		v2 = pos + glm::vec3(1, 1, 1);
		v3 = pos + glm::vec3(1, 1, 0);
	}
	else if (normal == glm::vec3(0, -1, 0)) // Dessous
	{
		v0 = pos + glm::vec3(0, 0, 0);
		v1 = pos + glm::vec3(1, 0, 0);
		v2 = pos + glm::vec3(1, 0, 1);
		v3 = pos + glm::vec3(0, 0, 1);
	}
	else if (normal == glm::vec3(1, 0, 0)) // Droite
	{
		v0 = pos + glm::vec3(1, 0, 0);
		v1 = pos + glm::vec3(1, 1, 0);
		v2 = pos + glm::vec3(1, 1, 1);
		v3 = pos + glm::vec3(1, 0, 1);
	}
	else if (normal == glm::vec3(-1, 0, 0)) // Gauche
	{
		v0 = pos + glm::vec3(0, 0, 1);
		v1 = pos + glm::vec3(0, 1, 1);
		v2 = pos + glm::vec3(0, 1, 0);
		v3 = pos + glm::vec3(0, 0, 0);
	}
	else if (normal == glm::vec3(0, 0, 1)) // Devant
	{
		v0 = pos + glm::vec3(1, 0, 1);
		v1 = pos + glm::vec3(1, 1, 1);
		v2 = pos + glm::vec3(0, 1, 1);
		v3 = pos + glm::vec3(0, 0, 1);
	}
	else // Derrière
	{
		v0 = pos + glm::vec3(0, 0, 0);
		v1 = pos + glm::vec3(0, 1, 0);
		v2 = pos + glm::vec3(1, 1, 0);
		v3 = pos + glm::vec3(1, 0, 0);
	}

	vertices.push_back({v0, normal, {uvMin.x, uvMin.y}});
	vertices.push_back({v1, normal, {uvMin.x, uvMax.y}});
	vertices.push_back({v2, normal, {uvMax.x, uvMax.y}});
	vertices.push_back({v3, normal, {uvMax.x, uvMin.y}});

	indices.push_back(baseIndex + 0);
	indices.push_back(baseIndex + 1);
	indices.push_back(baseIndex + 2);
	indices.push_back(baseIndex + 0);
	indices.push_back(baseIndex + 2);
	indices.push_back(baseIndex + 3);
}

bool	ChunkMesh::_isSolid(const Chunk& chunk, int x, int y, int z, const NeighborChunks& neighbors) const
{
	if (y < 0 || y >= 256)
		return false;

	if (x < 0)
		return neighbors.nx && neighbors.nx->get(x + 16, y, z) != VoxelType::Air && neighbors.nx->get(x + 16, y, z) != VoxelType::Water;
	else if (x >= 16)
		return neighbors.px && neighbors.px->get(x - 16, y, z) != VoxelType::Air && neighbors.px->get(x - 16, y, z) != VoxelType::Water;
	else if (z < 0)
		return neighbors.nz && neighbors.nz->get(x, y, z + 16) != VoxelType::Air && neighbors.nz->get(x, y, z + 16) != VoxelType::Water;
	else if (z >= 16)
		return neighbors.pz && neighbors.pz->get(x, y, z - 16) != VoxelType::Air && neighbors.pz->get(x, y, z - 16) != VoxelType::Water;

	return chunk.get(x, y, z) != VoxelType::Air && chunk.get(x, y, z) != VoxelType::Water;
}

static int getTextureIndex(VoxelType type)
{
	switch (type)
	{
		case VoxelType::Grass:  return 0;
		case VoxelType::Dirt:   return 1;
		case VoxelType::Stone:  return 2;
		case VoxelType::Sand:   return 3;
		case VoxelType::Water:  return 4;
		default:                return 0;
	}
}

static glm::vec2 getUVMin(int textureIndex)
{
	return glm::vec2(textureIndex / 16.0f, 0.0f);
}

static glm::vec2 getUVMax(int textureIndex)
{
	return glm::vec2((textureIndex + 1) / 16.0f, 1.0f / 16.0f);
}

void	ChunkMesh::build(const Chunk& chunk, const NeighborChunks& neighbors)
{
	_vertices.clear();
	_indices.clear();
	_waterVertices.clear();
	_waterIndices.clear();

	glm::vec2	uvMin;
	glm::vec2	uvMax;
	glm::vec3	pos;
	VoxelType	type;

	for (int y = 0; y < 256; y++)
	{
		for (int z = 0; z < 16; z++)
		{
			for (int x = 0; x < 16; x++)
			{
				type = chunk.get(x, y, z);

				if (type == VoxelType::Air)
					continue;

				pos.x = (float)x;
				pos.y = (float)y;
				pos.z = (float)z;

				int	texIdx = getTextureIndex(type);
				uvMin = getUVMin(texIdx);
				uvMax = getUVMax(texIdx);
				
				if (type == VoxelType::Water)
				{
					if (_getVoxel(chunk, x, y + 1, z, neighbors) == VoxelType::Air)
						_buildFace(pos, glm::vec3(0,  1,  0), uvMin, uvMax, _waterVertices, _waterIndices);
					if (_getVoxel(chunk, x, y - 1, z, neighbors) == VoxelType::Air)
						_buildFace(pos, glm::vec3(0, -1,  0), uvMin, uvMax, _waterVertices, _waterIndices);
					if (_getVoxel(chunk, x + 1, y, z, neighbors) == VoxelType::Air)
						_buildFace(pos, glm::vec3(1,  0,  0), uvMin, uvMax, _waterVertices, _waterIndices);
					if (_getVoxel(chunk, x - 1, y, z, neighbors) == VoxelType::Air)
						_buildFace(pos, glm::vec3(-1, 0,  0), uvMin, uvMax, _waterVertices, _waterIndices);
					if (_getVoxel(chunk, x, y, z + 1, neighbors) == VoxelType::Air)
						_buildFace(pos, glm::vec3(0,  0,  1), uvMin, uvMax, _waterVertices, _waterIndices);
					if (_getVoxel(chunk, x, y, z - 1, neighbors) == VoxelType::Air)
						_buildFace(pos, glm::vec3(0,  0, -1), uvMin, uvMax, _waterVertices, _waterIndices);
				}
				else
				{
					if (!_isSolid(chunk, x, y + 1, z, neighbors))
						_buildFace(pos, glm::vec3(0,  1,  0), uvMin, uvMax, _vertices, _indices);
					if (!_isSolid(chunk, x, y - 1, z, neighbors))
						_buildFace(pos, glm::vec3(0, -1,  0), uvMin, uvMax, _vertices, _indices);
					if (!_isSolid(chunk, x + 1, y, z, neighbors))
						_buildFace(pos, glm::vec3(1,  0,  0), uvMin, uvMax, _vertices, _indices);
					if (!_isSolid(chunk, x - 1, y, z, neighbors))
						_buildFace(pos, glm::vec3(-1, 0,  0), uvMin, uvMax, _vertices, _indices);
					if (!_isSolid(chunk, x, y, z + 1, neighbors))
						_buildFace(pos, glm::vec3(0,  0,  1), uvMin, uvMax, _vertices, _indices);
					if (!_isSolid(chunk, x, y, z - 1, neighbors))
						_buildFace(pos, glm::vec3(0,  0, -1), uvMin, uvMax, _vertices, _indices);
				}
			}
		}
	}

	if (!_vertices.empty())
		_uploadToGPU();
}

void	ChunkMesh::draw(void) const
{
	if (_indexCount == 0)
	{
		std::cout << "Mesh vide, skip" << std::endl;
		return;
	}

	cgl(glBindVertexArray(_vao));
	cgl(glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0));
	cgl(glBindVertexArray(0));
}

bool	ChunkMesh::isEmpty(void) const
{
	return _indexCount == 0;
}

bool	ChunkMesh::isWaterEmpty(void) const
{
	return _waterIndexCount == 0;
}

void	ChunkMesh::drawWater(void) const
{
	if (_waterIndexCount == 0)
		return;

	cgl(glBindVertexArray(_waterVao));
	cgl(glDrawElements(GL_TRIANGLES, _waterIndexCount, GL_UNSIGNED_INT, 0));
	cgl(glBindVertexArray(0));
}

VoxelType   ChunkMesh::_getVoxel(const Chunk& chunk, int x, int y, int z, const NeighborChunks& neighbors) const
{
	if (y < 0 || y >= 256)
		return VoxelType::Air;

	if (x < 0)
		return neighbors.nx ? neighbors.nx->get(x + 16, y, z) : VoxelType::Air;
	else if (x >= 16)
		return neighbors.px ? neighbors.px->get(x - 16, y, z) : VoxelType::Air;
	else if (z < 0)
		return neighbors.nz ? neighbors.nz->get(x, y, z + 16) : VoxelType::Air;
	else if (z >= 16)
		return neighbors.pz ? neighbors.pz->get(x, y, z - 16) : VoxelType::Air;

	return chunk.get(x, y, z);
}
