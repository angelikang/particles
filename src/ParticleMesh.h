#ifndef PARTICLE_MESH_H

class ParticleMesh {

private:
    GL::Buffer _indexBuffer, _vertexBuffer;
    GL::Mesh _mesh;
    Shaders::Phong _shader;

public:
    ParticleMesh() {
        const Trade::MeshData3D cube = Primitives::cubeSolid();

        _vertexBuffer.setData(MeshTools::interleave(cube.positions(0), cube.normals(0)),
                              GL::BufferUsage::StaticDraw);

        Containers::Array<char> indexData;
        MeshIndexType indexType;
        UnsignedInt indexStart, indexEnd;
        std::tie(indexData, indexType, indexStart, indexEnd) = MeshTools::compressIndices(cube.indices());
        _indexBuffer.setData(indexData, GL::BufferUsage::StaticDraw);

        _mesh.setPrimitive(cube.primitive())
                .setCount(cube.indices().size())
                .addVertexBuffer(_vertexBuffer, 0, Shaders::Phong::Position{}, Shaders::Phong::Normal{})
                .setIndexBuffer(_indexBuffer, 0, indexType, indexStart, indexEnd);

        _shader.setLightPosition({7.0f, 5.0f, 40.0f})
                .setLightColor(Color3{1.0f});
    }

    void draw(Matrix4 &projection, Matrix4 &transformation, Color3 &color) {
        _shader
                .setDiffuseColor(color)
                .setAmbientColor(Color3::fromHsv(color.hue(), 1.0f, 0.3f))
                .setTransformationMatrix(transformation)
                .setNormalMatrix(transformation.rotationScaling())
                .setProjectionMatrix(projection);

        _mesh.draw(_shader);
    }
};


#define PARTICLE_MESH_H
#endif //PARTICLE_MESH_H
