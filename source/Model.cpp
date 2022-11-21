#include "Model.h"

#include<assimp/Importer.hpp>// C++ importerinterface
#include<assimp/scene.h>// aiSceneoutputdata structure
#include<assimp/postprocess.h>// Post processingflags
#include <glm/vec4.hpp>
#include <iostream>
#include <memory>

Model::Model(std::vector<float> vertices) {
    verticesCount = static_cast<int>(vertices.size());
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * verticesCount), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

Model::~Model() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}

void Model::draw() const {
    glBindVertexArray(VAO);

    if (indicesCount == 0) {
        // draw triangles
        glDrawArrays(GL_TRIANGLES, 0, verticesCount); //mode,first,count
    } else {
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, NULL);
    }
}

Model::Model(const float* vertices, size_t size) : Model(std::vector<float>(vertices, &vertices[size - 1])) {
}

struct Vertex
{
    float Position[3];
    float Normal[3];
    float Texture[2];
    float Tangent[3];
};

void Model::load(const std::string& fileName) {
    using namespace glm;
    Assimp::Importer importer;
    unsigned int importOptions = aiProcess_Triangulate
                                 | aiProcess_OptimizeMeshes              // sloučení malých plošek
                                 | aiProcess_JoinIdenticalVertices       // NUTNÉ jinak hodně duplikuje
                                 | aiProcess_Triangulate                 // prevod vsech ploch na trojuhelniky
                                 | aiProcess_CalcTangentSpace;           // vypocet tangenty, nutny pro spravne pouziti normalove mapy

    //aiProcess_GenNormals/ai_Process_GenSmoothNormals - vypocet normal s jemnych prechodem v pripade, ze objekt neobsahuje normaly

    const aiScene* scene = importer.ReadFile(fileName, importOptions);

    if (scene) { //pokud bylo nacteni uspesne
        printf("scene->mNumMeshes = %d\n", scene->mNumMeshes);
        printf("scene->mNumMaterials = %d\n", scene->mNumMaterials);

        for (unsigned int i = 0; i < scene->mNumMaterials; i++)                       //Materials
        {
            const aiMaterial* mat = scene->mMaterials[i];

            aiString name;
            mat->Get(AI_MATKEY_NAME, name);
            printf("Material [%d] name %s\n", i, name.C_Str());

            aiColor4D d;

            glm::vec4 diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
            if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &d))
                diffuse = vec4(d.r, d.g, d.b, d.a);

        }

        for (unsigned int i = 0; i < scene->mNumMeshes; i++)                      //Objects
        {
            aiMesh* mesh = scene->mMeshes[i];

            auto uVertices = std::unique_ptr<Vertex[]>(new Vertex[mesh->mNumVertices]);
            auto pVertices = uVertices.get();
            std::memset(pVertices, 0, sizeof(Vertex) * mesh->mNumVertices);

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                if (mesh->HasPositions())
                {
                    pVertices[i].Position[0] = mesh->mVertices[i].x;
                    pVertices[i].Position[1] = mesh->mVertices[i].y;
                    pVertices[i].Position[2] = mesh->mVertices[i].z;
                }
                if (mesh->HasNormals())
                {
                    pVertices[i].Normal[0] = mesh->mNormals[i].x;
                    pVertices[i].Normal[1] = mesh->mNormals[i].y;
                    pVertices[i].Normal[2] = mesh->mNormals[i].z;
                }
                if (mesh->HasTextureCoords(0))
                {
                    pVertices[i].Texture[0] = mesh->mTextureCoords[0][i].x;
                    pVertices[i].Texture[1] = mesh->mTextureCoords[0][i].y;
                }
                if (mesh->HasTangentsAndBitangents())
                {
                    pVertices[i].Tangent[0] = mesh->mTangents[i].x;
                    pVertices[i].Tangent[1] = mesh->mTangents[i].y;
                    pVertices[i].Tangent[2] = mesh->mTangents[i].z;
                }

            }

            auto uIndices = std::unique_ptr<unsigned int[]>(new unsigned int[mesh->mNumFaces * 3]);
            unsigned int* pIndices = nullptr;
            pIndices = uIndices.get();

            if (mesh->HasFaces())
            {
                for (unsigned int i = 0; i < mesh->mNumFaces; i++)
                {
                    pIndices[i * 3] = mesh->mFaces[i].mIndices[0];
                    pIndices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
                    pIndices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
                }
            }

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &IBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->mNumVertices, pVertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(0));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(GLfloat)));

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->mNumFaces * 3, pIndices, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(VAO);

            GLuint err = glGetError();
            if (err != GL_NO_ERROR)
            {
                std::cout << "GL ERROR: " << err << std::endl; return;
            }
            indicesCount = mesh->mNumFaces * 3;
            verticesCount = mesh->mNumVertices;
        }
    }
    else {
        printf("Error during parsing mesh from %s : %s \n", fileName.c_str(), importer.GetErrorString());
    }
    glBindVertexArray(0);
}

Model::Model() {
}
