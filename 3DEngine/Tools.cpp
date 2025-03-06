#include "Tools.hpp"
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "AssetManager.hpp"
#include <random>
#include <limits>
#include <cmath>
#include <algorithm>
#include <SFML/OpenGL.hpp>
#pragma comment(lib, "opengl32.lib")



namespace game
{
    float Tools::VectorArithmetic::dotProduct(vec3d vect1, vec3d vect2)
    {
        return vect1.x * vect2.x + vect1.y * vect2.y + vect1.z * vect2.z;
    }
    vec3d Tools::VectorArithmetic::normalizeVector(vec3d vect)
    {
        float t = sqrtf(vect.x * vect.x + vect.y * vect.y + vect.z * vect.z);
        if (t)
        {
            vect.x /= t;
            vect.y /= t;
            vect.z /= t;
        }
        return vect;
    }
    vec3d Tools::VectorArithmetic::crossProduct(vec3d vect1, vec3d vect2)
    {
        return { vect1.y * vect2.z - vect1.z * vect2.y,
                 vect1.z * vect2.x - vect1.x * vect2.z,
                 vect1.x * vect2.y - vect1.y * vect2.x };
    }
    vec3d Tools::VectorArithmetic::subtractVector(vec3d vect1, vec3d vect2)
    {
        return { vect1.x - vect2.x, vect1.y - vect2.y, vect1.z - vect2.z };
    }
    vec3d Tools::VectorArithmetic::MultiplyVector(vec3d vect1, float a)
    {
        return { vect1.x * a, vect1.y * a, vect1.z * a };
    }
    vec3d Tools::VectorArithmetic::VectorAdd(vec3d vect1, vec3d vect2)
    {
        return { vect1.x + vect2.x, vect1.y + vect2.y, vect1.z + vect2.z };
    }
    
    void Tools::MatrixArithmetic::MultiplyMatrixVector(const vec3d& i, vec3d& o, const mat4x4& m)
    {


        o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
        o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
        o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
        float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

        if (w != 0.0f)
        {
            o.x /= w; o.y /= w; o.z /= w;
        }
    }
    mat4x4 Tools::MatrixArithmetic::MultiplyMatrixMatrix(const mat4x4& mat1, const mat4x4& mat2)
    {
        mat4x4 rez{};
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                rez.m[i][j] = 0.0f;
                for (int k = 0; k < 4; k++)
                {
                    rez.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
                }
            }
        }
        return rez;
    }
    mat4x4 Tools::MatrixArithmetic::Matrix_MakeRotationY(float fAngleRad)
    {
        mat4x4 matrix{};
        matrix.m[0][0] = cosf(fAngleRad);
        matrix.m[0][2] = sinf(fAngleRad);
        matrix.m[2][0] = -sinf(fAngleRad);
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = cosf(fAngleRad);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }
    mat4x4 Tools::MatrixArithmetic::Matrix_QuickInverse(mat4x4& m)
    {
        mat4x4 matrix{};
        matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
        matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
        matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
        matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
        matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
        matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }
    mat4x4 Tools::MatrixArithmetic::Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up)
    {
        vec3d newForward = vectorArithmetic.subtractVector(target, pos);
        newForward = vectorArithmetic.normalizeVector(newForward);

        vec3d a = vectorArithmetic.MultiplyVector(newForward, vectorArithmetic.dotProduct(up, newForward));
        vec3d newUp = vectorArithmetic.subtractVector(up, a);
        newUp = vectorArithmetic.normalizeVector(newUp);

        vec3d newRight = vectorArithmetic.crossProduct(newUp, newForward);

        mat4x4 matrix{};
        matrix.m[0][0] = newRight.x; matrix.m[0][1] = newRight.y; matrix.m[0][2] = newRight.z; matrix.m[0][3] = 0.0f;
        matrix.m[1][0] = newUp.x;    matrix.m[1][1] = newUp.y;    matrix.m[1][2] = newUp.z;    matrix.m[1][3] = 0.0f;
        matrix.m[2][0] = newForward.x; matrix.m[2][1] = newForward.y; matrix.m[2][2] = newForward.z; matrix.m[2][3] = 0.0f;
        matrix.m[3][0] = pos.x;      matrix.m[3][1] = pos.y;      matrix.m[3][2] = pos.z;      matrix.m[3][3] = 1.0f;
        return matrix;
    }
        
    vec3d Tools::Rendering3D::Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd)
    {
        plane_n = vectorArithmetic.normalizeVector(plane_n);
        float plane_d = -vectorArithmetic.dotProduct(plane_n, plane_p);
        float ad = vectorArithmetic.dotProduct(lineStart, plane_n);
        float bd = vectorArithmetic.dotProduct(lineEnd, plane_n);
        float t = (-plane_d - ad) / (bd - ad);
        vec3d lineStartToEnd = vectorArithmetic.subtractVector(lineEnd, lineStart);
        vec3d lineToIntersect = vectorArithmetic.MultiplyVector(lineStartToEnd, t);
        return vectorArithmetic.VectorAdd(lineStart, lineToIntersect);
    }
    int Tools::Rendering3D::TriangleClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2)
    {
        plane_n = vectorArithmetic.normalizeVector(plane_n);
        auto dist = [&](vec3d& p)
            {
                return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - vectorArithmetic.dotProduct(plane_n, plane_p));
            };

        vec3d* inside_points[3];
        int nInsidePointCount = 0;
        vec3d* outside_points[3];
        int nOutsidePointCount = 0;

        float d0 = dist(in_tri.p[0]);
        float d1 = dist(in_tri.p[1]);
        float d2 = dist(in_tri.p[2]);
        if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; }
        else { outside_points[nOutsidePointCount++] = &in_tri.p[0]; }
        if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[1]; }
        else { outside_points[nOutsidePointCount++] = &in_tri.p[1]; }
        if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[2]; }
        else { outside_points[nOutsidePointCount++] = &in_tri.p[2]; }

        if (nInsidePointCount == 0)
            return 0;
        if (nInsidePointCount == 3)
        {
            out_tri1 = in_tri;
            out_tri1.TextureName = in_tri.TextureName;
            return 1;
        }
        if (nInsidePointCount == 1 && nOutsidePointCount == 2)
        {
            out_tri1.color = in_tri.color;
            out_tri1.t[0] = in_tri.t[0];
            out_tri1.t[1] = in_tri.t[1];
            out_tri1.t[2] = in_tri.t[2];
            out_tri1.TextureName = in_tri.TextureName;
            out_tri1.p[0] = *inside_points[0];
            out_tri1.p[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
            out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);
            return 1;
        }
        if (nInsidePointCount == 2 && nOutsidePointCount == 1)
        {
            out_tri1.color = in_tri.color;
            out_tri1.t[0] = in_tri.t[0];
            out_tri1.t[1] = in_tri.t[1];
            out_tri1.t[2] = in_tri.t[2];
            out_tri1.TextureName = in_tri.TextureName;
            out_tri2.color = in_tri.color;
            out_tri2.t[0] = in_tri.t[0];
            out_tri2.t[1] = in_tri.t[1];
            out_tri2.t[2] = in_tri.t[2];
            out_tri2.TextureName = in_tri.TextureName;
            out_tri1.p[0] = *inside_points[0];
            out_tri1.p[1] = *inside_points[1];
            out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
            out_tri2.p[0] = *inside_points[1];
            out_tri2.p[1] = out_tri1.p[2];
            out_tri2.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);
            return 2;
        }
        return 0;
    }
    bool Tools::Rendering3D::CameraClipp(mesh& meshToClipp, vec3d& vCamera, vec3d& vLookDir, vec3d vLight, mat4x4& matView, mat4x4& matProj, mesh& MeshClipped)
    {
        
        for (auto& tri : meshToClipp.tris)
        {
            triangle triProjected, triViewed;
            vec3d normal = vectorArithmetic.normalizeVector(vectorArithmetic.crossProduct(vectorArithmetic.subtractVector(tri.p[1], tri.p[0]), vectorArithmetic.subtractVector(tri.p[2], tri.p[0])));
            vec3d vCameraRay = vectorArithmetic.subtractVector(tri.p[0], vCamera);
            if (vectorArithmetic.dotProduct(normal, vCameraRay) < 0.0f)
            {
                float dp = vectorArithmetic.dotProduct(normal, vLight);
                triViewed.color = sf::Color(static_cast<sf::Uint8>(fabs(dp) * 255.0f),
                    static_cast<sf::Uint8>(fabs(dp) * 255.0f),
                    static_cast<sf::Uint8>(fabs(dp) * 255.0f),
                    tri.color.a);
                //testing
                triViewed.color = sf::Color(255, 255, 255, 255);
                matrixArithmetic.MultiplyMatrixVector(tri.p[0], triViewed.p[0], matView);
                matrixArithmetic.MultiplyMatrixVector(tri.p[1], triViewed.p[1], matView);
                matrixArithmetic.MultiplyMatrixVector(tri.p[2], triViewed.p[2], matView);
                triViewed.TextureName = tri.TextureName;

                triangle clipped[2];
                int nClippedTriangles = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

                for (int n = 0; n < nClippedTriangles; n++)
                {
                    matrixArithmetic.MultiplyMatrixVector(clipped[n].p[0], triProjected.p[0], matProj);
                    matrixArithmetic.MultiplyMatrixVector(clipped[n].p[1], triProjected.p[1], matProj);
                    matrixArithmetic.MultiplyMatrixVector(clipped[n].p[2], triProjected.p[2], matProj);

                    // Flip x and y for correct screen orientation.
                    triProjected.p[0].x *= -1.0f; triProjected.p[1].x *= -1.0f; triProjected.p[2].x *= -1.0f;
                    triProjected.p[0].y *= -1.0f; triProjected.p[1].y *= -1.0f; triProjected.p[2].y *= -1.0f;

                    vec3d vOffsetView = { 1, 1, 0 };
                    triProjected.p[0] = vectorArithmetic.VectorAdd(triProjected.p[0], vOffsetView);
                    triProjected.p[1] = vectorArithmetic.VectorAdd(triProjected.p[1], vOffsetView);
                    triProjected.p[2] = vectorArithmetic.VectorAdd(triProjected.p[2], vOffsetView);

                    triProjected.p[0].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
                    triProjected.p[0].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
                    triProjected.p[1].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
                    triProjected.p[1].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
                    triProjected.p[2].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
                    triProjected.p[2].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);

                    triProjected.color = clipped[n].color;
                    triProjected.t[0] = clipped[n].t[0];
                    triProjected.t[1] = clipped[n].t[1];
                    triProjected.t[2] = clipped[n].t[2];
                    triProjected.TextureName = clipped[n].TextureName;

                    MeshClipped.tris.push_back(triProjected);
                }
            }
        }
        return true;
    }
    bool Tools::Rendering3D::UpdateCamera(float& fYaw, float& fPitch, vec3d& vCamera, vec3d& vLookDir, mat4x4& matView)
    {
        vec3d vUp{ 0, 1, 0 };
        vLookDir.x = cosf(fYaw) * cosf(fPitch);
        vLookDir.y = sinf(fPitch);
        vLookDir.z = sinf(fYaw) * cosf(fPitch);
        vLookDir = vectorArithmetic.normalizeVector(vLookDir);
        vec3d vTarget = vectorArithmetic.VectorAdd(vCamera, vLookDir);
        mat4x4 matCamera = matrixArithmetic.Matrix_PointAt(vCamera, vTarget, vUp);
        matView = matrixArithmetic.Matrix_QuickInverse(matCamera);
        return true;
    }
    bool Tools::Rendering3D::DrawMesh(const mesh& meshToDraw, sf::RenderWindow& window, std::map<std::string, sf::Texture>& textures, int Transparency)
    {
        for (const auto& x : meshToDraw.tris)
        {
            sf::VertexArray trigle(sf::Triangles, 3);
            trigle[0].color = sf::Color(x.color.r, x.color.g, x.color.b, Transparency);
            trigle[1].color = sf::Color(x.color.r, x.color.g, x.color.b, Transparency);
            trigle[2].color = sf::Color(x.color.r, x.color.g, x.color.b, Transparency);
            trigle[0].texCoords = sf::Vector2f(x.t[0].x, x.t[0].y);
            trigle[1].texCoords = sf::Vector2f(x.t[1].x, x.t[1].y);
            trigle[2].texCoords = sf::Vector2f(x.t[2].x, x.t[2].y);
            trigle[0].position = sf::Vector2f(x.p[0].x, x.p[0].y);
            trigle[1].position = sf::Vector2f(x.p[1].x, x.p[1].y);
            trigle[2].position = sf::Vector2f(x.p[2].x, x.p[2].y);
            window.draw(trigle, &textures[x.TextureName]);
        }
        return true;
    }
    bool Tools::Rendering3D::DrawMeshesWithDepthBuffer(const std::vector<mesh>& meshes, sf::RenderWindow& window, std::map<std::string, sf::Texture>& textures)
    {
        static std::vector<sf::Uint8> pixelData(SCREEN_WIDTH * SCREEN_HEIGHT * 4);
        static std::vector<float> depthBuffer(SCREEN_WIDTH * SCREEN_HEIGHT, std::numeric_limits<float>::max());
        static sf::Texture outputTexture;
        static bool outputTextureCreated = false;
        static std::map<std::string, sf::Image> cachedTextureImages;
        static std::vector<sf::Uint8> precomputedRow;

        if (precomputedRow.empty()) {
            precomputedRow.resize(SCREEN_WIDTH * 4);
            for (int i = 0; i < SCREEN_WIDTH; i += 4) {
                precomputedRow[i] = 0;
                precomputedRow[i + 1] = 0;
                precomputedRow[i + 2] = 0;
                precomputedRow[i + 3] = 255;
            }
        }
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            std::copy(precomputedRow.begin(), precomputedRow.end(), pixelData.begin() + y * SCREEN_WIDTH * 4);
        }

        std::fill(depthBuffer.begin(), depthBuffer.end(), std::numeric_limits<float>::max());

        bool updateTextureCache = false;
        if (cachedTextureImages.size() != textures.size()) {
            updateTextureCache = true;
        }
        else {
            for (const auto& pair : textures) {
                if (cachedTextureImages.find(pair.first) == cachedTextureImages.end()) {
                    updateTextureCache = true;
                    break;
                }
            }
        }
        // Update the cache only if required.
        if (updateTextureCache) {
            cachedTextureImages.clear();
            for (const auto& pair : textures) {
                cachedTextureImages[pair.first] = pair.second.copyToImage();
            }
        }

        for (const auto& mesh : meshes) {
            for (const auto& tri : mesh.tris) {
                RasterizeTriangleToBuffer(tri, pixelData, depthBuffer, cachedTextureImages, SCREEN_WIDTH, SCREEN_HEIGHT);
            }
        }

        if (!outputTextureCreated) {
            outputTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT);
            outputTextureCreated = true;
        }
        outputTexture.update(pixelData.data());

        sf::Sprite sprite(outputTexture);
        window.draw(sprite);

        return true;
    }
    void Tools::Rendering3D::TransformObj(float fThetax, float fThetay, float fThetaz, float x, float y, float z, float scaleX, float scaleY, float scaleZ, mesh& _mesh, mesh& meshToDraw, int Transparency)
    {
        meshToDraw.tris.clear();

        mat4x4 transformation{};
        // Build an identity matrix.
        for (int i = 0; i < 4; i++)
            transformation.m[i][i] = 1;
        
        mat4x4 matRotZ{};
        matRotZ.m[0][0] = cosf(fThetaz); matRotZ.m[0][1] = sinf(fThetaz);
        matRotZ.m[1][0] = -sinf(fThetaz); matRotZ.m[1][1] = cosf(fThetaz);
        matRotZ.m[2][2] = 1; matRotZ.m[3][3] = 1;

        mat4x4 matRotY{};
        matRotY.m[0][0] = cosf(fThetay); matRotY.m[0][1] = sinf(fThetay);
        matRotY.m[1][1] = cosf(fThetay);
        matRotY.m[2][2] = 1;
        matRotY.m[1][0] = -sinf(fThetay); matRotY.m[0][1] = sinf(fThetay);

        mat4x4 matRotX{};
        matRotX.m[0][0] = 1;
        matRotX.m[1][1] = cosf(fThetax); matRotX.m[1][2] = sinf(fThetax);
        matRotX.m[2][1] = -sinf(fThetax); matRotX.m[2][2] = cosf(fThetax);
        matRotX.m[3][3] = 1;

        mat4x4 matMov{};
        matMov.m[0][0] = 1; matMov.m[1][1] = 1; matMov.m[2][2] = 1; matMov.m[3][3] = 1;
        matMov.m[3][0] = x; matMov.m[3][1] = y; matMov.m[3][2] = z;

        mat4x4 matScale{};
        matScale.m[0][0] = scaleX; matScale.m[1][1] = scaleY; matScale.m[2][2] = scaleZ; matScale.m[3][3] = 1;

        transformation = matrixArithmetic.MultiplyMatrixMatrix(transformation, matScale);
        transformation = matrixArithmetic.MultiplyMatrixMatrix(transformation, matRotY);
        transformation = matrixArithmetic.MultiplyMatrixMatrix(transformation, matRotX);
        transformation = matrixArithmetic.MultiplyMatrixMatrix(transformation, matRotZ);

        meshToDraw.tris.reserve(_mesh.tris.size());
        for (const auto& tri : _mesh.tris)
        {
            triangle trigle;
            trigle.t[0] = tri.t[0];
            trigle.t[1] = tri.t[1];
            trigle.t[2] = tri.t[2];
            trigle.TextureName = tri.TextureName;

            matrixArithmetic.MultiplyMatrixVector(tri.p[0], trigle.p[0], transformation);
            matrixArithmetic.MultiplyMatrixVector(tri.p[1], trigle.p[1], transformation);
            matrixArithmetic.MultiplyMatrixVector(tri.p[2], trigle.p[2], transformation);

            // Apply translation
            matrixArithmetic.MultiplyMatrixVector(trigle.p[0], trigle.p[0], matMov);
            matrixArithmetic.MultiplyMatrixVector(trigle.p[1], trigle.p[1], matMov);
            matrixArithmetic.MultiplyMatrixVector(trigle.p[2], trigle.p[2], matMov);
            trigle.color.a = Transparency;
            meshToDraw.tris.push_back(trigle);
        }
    }
    void Tools::Rendering3D::TranformMeshes(std::vector<mesh>& MeshesTransformed, std::vector<mesh>& MeshesToTransform)
    {
        MeshesTransformed.clear();
        MeshesTransformed.resize(MeshesToTransform.size());
        for (int i = 0; i < MeshesToTransform.size(); i++)
            TransformObj(MeshesToTransform[i].Rotation.x, MeshesToTransform[i].Rotation.y, MeshesToTransform[i].Rotation.z, MeshesToTransform[i].ObjPos.x, MeshesToTransform[i].ObjPos.y, MeshesToTransform[i].ObjPos.z, MeshesToTransform[i].Scale.x, MeshesToTransform[i].Scale.y, MeshesToTransform[i].Scale.z, MeshesToTransform[i], MeshesTransformed[i], MeshesToTransform[i].transparency);
    }
    void Tools::Rendering3D::RasterizeTriangleToBuffer(const triangle& tri, std::vector<sf::Uint8>& pixelData, std::vector<float>& depthBuffer, const std::map<std::string, sf::Image>& textureImages, int width, int height)
    {
        // Try to get texture if available
        bool useTexture = false;
        sf::Image textureImage;
        if (!tri.TextureName.empty())
        {
            auto it = textureImages.find(tri.TextureName);
            if (it != textureImages.end())
            {
                useTexture = true;
                textureImage = it->second;
            }
        }

        // Calculate bounding box of the triangle
        float minX = std::min({ tri.p[0].x, tri.p[1].x, tri.p[2].x });
        float minY = std::min({ tri.p[0].y, tri.p[1].y, tri.p[2].y });
        float maxX = std::max({ tri.p[0].x, tri.p[1].x, tri.p[2].x });
        float maxY = std::max({ tri.p[0].y, tri.p[1].y, tri.p[2].y });
        int x0 = std::max(0, static_cast<int>(std::floor(minX)));
        int y0 = std::max(0, static_cast<int>(std::floor(minY)));
        int x1 = std::min(width - 1, static_cast<int>(std::ceil(maxX)));
        int y1 = std::min(height - 1, static_cast<int>(std::ceil(maxY)));

        // Define an edge function for barycentric coordinates
        auto edgeFunction = [](const vec3d& a, const vec3d& b, float px, float py) -> float {
            return (px - a.x) * (b.y - a.y) - (py - a.y) * (b.x - a.x);
            };

        float area = edgeFunction(tri.p[0], tri.p[1], tri.p[2].x, tri.p[2].y);
        float invArea = 1.0f / area;

        // Precompute reciprocal Z for perspective-correct interpolation
        float invZ0 = 1.0f / tri.p[0].z;
        float invZ1 = 1.0f / tri.p[1].z;
        float invZ2 = 1.0f / tri.p[2].z;
        float u0 = tri.t[0].x * invZ0;
        float u1 = tri.t[1].x * invZ1;
        float u2 = tri.t[2].x * invZ2;
        float v0 = tri.t[0].y * invZ0;
        float v1 = tri.t[1].y * invZ1;
        float v2 = tri.t[2].y * invZ2;

        // Loop through each pixel in the bounding box
        for (int y = y0; y <= y1; y++)
        {
            for (int x = x0; x <= x1; x++)
            {
                float px = x + 0.5f;
                float py = y + 0.5f;
                float w0 = edgeFunction(tri.p[1], tri.p[2], px, py) * invArea;
                float w1 = edgeFunction(tri.p[2], tri.p[0], px, py) * invArea;
                float w2 = edgeFunction(tri.p[0], tri.p[1], px, py) * invArea;

                // Only process if the pixel is inside the triangle
                if (w0 >= 0 && w1 >= 0 && w2 >= 0)
                {
                    float depth = w0 * tri.p[0].z + w1 * tri.p[1].z + w2 * tri.p[2].z;
                    int idx = y * width + x;
                    if (depth < depthBuffer[idx])
                    {
                        sf::Color newColor;
                        if (useTexture)
                        {
                            float interpInvZ = w0 * invZ0 + w1 * invZ1 + w2 * invZ2;
                            float u = (w0 * u0 + w1 * u1 + w2 * u2) / interpInvZ;
                            float v = (w0 * v0 + w1 * v1 + w2 * v2) / interpInvZ;
                            unsigned int texWidth = textureImage.getSize().x;
                            unsigned int texHeight = textureImage.getSize().y;
                            int texX = static_cast<int>(u * texWidth);
                            int texY = static_cast<int>(v * texHeight);

                            // Manually clamp texture coordinates
                            if (texX < 0)
                                texX = 0;
                            if (texX >= static_cast<int>(texWidth))
                                texX = texWidth - 1;
                            if (texY < 0)
                                texY = 0;
                            if (texY >= static_cast<int>(texHeight))
                                texY = texHeight - 1;

                            sf::Color textureColor = textureImage.getPixel(texX, texY);
                            newColor.r = (textureColor.r * tri.color.r) / 255;
                            newColor.g = (textureColor.g * tri.color.g) / 255;
                            newColor.b = (textureColor.b * tri.color.b) / 255;
                            newColor.a = (textureColor.a * tri.color.a) / 255;
                        }
                        else
                        {
                            newColor = tri.color;
                        }

                        int pixelIndex = idx * 4;
                        if (newColor.a < 255)
                        {
                            sf::Uint8 destR = pixelData[pixelIndex + 0];
                            sf::Uint8 destG = pixelData[pixelIndex + 1];
                            sf::Uint8 destB = pixelData[pixelIndex + 2];
                            sf::Uint8 destA = pixelData[pixelIndex + 3];
                            float srcAlpha = newColor.a / 255.f;
                            float invAlpha = 1.f - srcAlpha;
                            sf::Uint8 outR = static_cast<sf::Uint8>(newColor.r * srcAlpha + destR * invAlpha);
                            sf::Uint8 outG = static_cast<sf::Uint8>(newColor.g * srcAlpha + destG * invAlpha);
                            sf::Uint8 outB = static_cast<sf::Uint8>(newColor.b * srcAlpha + destB * invAlpha);
                            sf::Uint8 outA = static_cast<sf::Uint8>(newColor.a + destA * invAlpha);
                            newColor = sf::Color(outR, outG, outB, outA);
                        }
                        else
                        {
                            depthBuffer[idx] = depth;
                        }
                        pixelData[pixelIndex + 0] = newColor.r;
                        pixelData[pixelIndex + 1] = newColor.g;
                        pixelData[pixelIndex + 2] = newColor.b;
                        pixelData[pixelIndex + 3] = newColor.a;
                    }
                }
            }
        }
    }
    bool Tools::Rendering3D::Clip(std::vector<mesh>& MeshesToClipp, std::vector<mesh>& MeshesToRender, vec3d& vCamera, vec3d& vLookDir, vec3d& vLight, mat4x4& matView, mat4x4& matProj)
    {
        MeshesToRender.clear();
        MeshesToRender.resize(MeshesToClipp.size());
        std::vector <mesh> temp;
        temp.resize(MeshesToClipp.size());
        for (int i = 0; i < MeshesToClipp.size(); i++)
        {
            CameraClipp(MeshesToClipp[i], vCamera, vLookDir, vLight, matView, matProj, temp[i]);

            MeshesToRender[i] = temp[i];
            //WallClipp(temp[i], MeshesToRender[i]);
        }



        return 0;
    }
    bool Tools::Rendering3D::WallClipp(const mesh& meshToDraw, mesh& meshClipped)
    {
        meshClipped.tris.reserve(meshToDraw.tris.size() * 2);
        for (const auto& triToRaster : meshToDraw.tris)
        {
            std::vector<triangle> trianglesToProcess = { triToRaster };
            for (int p = 0; p < 4; ++p)
            {
                std::vector<triangle> newTriangles;
                newTriangles.reserve(trianglesToProcess.size() * 2);

                for (auto& test : trianglesToProcess)
                {
                    triangle clipped[2];
                    int nTrisToAdd = TriangleClipAgainstPlane(
                        (p == 0) ? vec3d{ 0.0f, 0.0f, 0.0f } :
                        (p == 1) ? vec3d{ 0.0f, static_cast<float>(SCREEN_HEIGHT) - 1, 0.0f } :
                        (p == 2) ? vec3d{ 0.0f, 0.0f, 0.0f } :
                        vec3d{ static_cast<float>(SCREEN_WIDTH) - 1, 0.0f, 0.0f },

                        (p == 0) ? vec3d{ 0.0f, 1.0f, 0.0f } :
                        (p == 1) ? vec3d{ 0.0f, -1.0f, 0.0f } :
                        (p == 2) ? vec3d{ 1.0f, 0.0f, 0.0f } :
                        vec3d{ -1.0f, 0.0f, 0.0f },

                        test, clipped[0], clipped[1]);

                    for (int w = 0; w < nTrisToAdd; ++w)
                    {
                        newTriangles.push_back(std::move(clipped[w]));
                    }
                }
                trianglesToProcess = std::move(newTriangles);
            }
            meshClipped.tris.reserve(meshClipped.tris.size() + trianglesToProcess.size());
            for (auto& t : trianglesToProcess)
            {
                meshClipped.tris.push_back(std::move(t));
            }
        }
        return true;
    }


    bool Tools::Rendering3D::DrawMeshesWithDepthBuffer2(const std::vector<mesh>& meshes, sf::RenderWindow& window, std::map<std::string, sf::Texture>& textures)
    {
        int screenWidth = window.getSize().x;
        int screenHeight = window.getSize().y;
        glViewport(0, 0, screenWidth, screenHeight);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glColor3f(1.0f, 0.0f, 0.0f);
        for (const auto& m : meshes)
        {
            for (const auto& tri : m.tris)
            {
                glBegin(GL_TRIANGLES);
                std::cout << normalization(0, 1920, tri.p[0].x) << ' ' << normalization(0, 1920, tri.p[0].y) << ' ' << normalization(0, 1920, tri.p[0].z) << '\n';
                //normalization(0, 1920, tri.p[0].y);
                glVertex3f(normalization(0, 1920, tri.p[0].x) , normalization(0, 1920, tri.p[0].y), tri.p[0].z);
                glVertex3f(normalization(0, 1920, tri.p[1].x) , normalization(0, 1920, tri.p[1].y), tri.p[1].z);
                glVertex3f(normalization(0, 1920, tri.p[2].x) , normalization(0, 1920, tri.p[2].y), tri.p[2].z);
                glEnd();
            }
        }
        window.display();
        return true;
    }
    #include <map>

    void Tools::Rendering3D::DrawMeshesWithOpenGL(const std::vector<mesh>& meshes, std::map<std::string, sf::Texture>& textures, sf::RenderWindow& window) {
        window.setActive(true);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glDepthRange(0.0, 1.0); // Ensure depth range is properly mapped

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Expanding near and far plane to ensure your triangles are visible
        glOrtho(-1, 1, -1, 1, -1, 10);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0, 0, -2.0f); // Move camera back to make sure objects are in view

        for (const auto& mesh : meshes) {
            bool hasTexture = false;
            GLuint textureID = 0;

            if (!mesh.tris.empty() && !mesh.tris[0].TextureName.empty()) {
                auto it = textures.find(mesh.tris[0].TextureName);
                if (it != textures.end()) {
                    textureID = it->second.getNativeHandle();
                    hasTexture = true;
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, textureID);
                }
            }

            glBegin(GL_TRIANGLES); // Change to GL_POINTS for debugging

            for (const auto& tri : mesh.tris) {
                for (int i = 0; i < 3; ++i) {
                    float normX = (tri.p[i].x / SCREEN_WIDTH) * 2.0f - 1.0f;
                    float normY = 1.0f - (tri.p[i].y / SCREEN_HEIGHT) * 2.0f;
                    float normZ = tri.p[i].z;

                    std::cout << "Vertex: (" << normX << ", " << normY << ", " << normZ << ")" << std::endl;

                    if (hasTexture) glTexCoord2f(tri.t[i].x, tri.t[i].y);
                    glColor4ub(tri.color.r, tri.color.g, tri.color.b, tri.color.a);
                    glVertex3f(normX, normY, normZ);
                }
            }

            glEnd();

            if (hasTexture) {
                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_TEXTURE_2D);
            }
        }

        window.display();
    }

    float Tools::Rendering3D::normalization(float min, float max, float value)
    {
        return (value - min) / (max - min);
    }
    float Tools::Utility::normalization(float min, float max, float value)
    {
        return (value - min) / (max - min);
    }
    void Tools::Utility::LookAtCamera(vec3d& objPos, vec3d& vCamera, float& fYaw, float& fPitch)
    {
        vec3d dir = { vCamera.x - objPos.x, vCamera.y - objPos.y, vCamera.z - objPos.z };
        dir = vectorArithmetic.normalizeVector(dir);
        fYaw = atan2f(dir.x, dir.z);
        fPitch = asinf(dir.y);
    }
    int Tools::Utility::Random(int from, int to)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(from, to);
        return dis(gen);
    }
    void Tools::Utility::AddCube(float x1, float y1, float z1, float x2, float y2, float z2, mesh& mesh)
    {
        triangle tri;
        // Front face
        tri.p[0] = { x1, y1, z1 };
        tri.p[1] = { x1, y2, z1 };
        tri.p[2] = { x2, y2, z1 };
        mesh.tris.push_back(tri);

        tri.p[0] = { x1, y1, z1 };
        tri.p[1] = { x2, y2, z1 };
        tri.p[2] = { x2, y1, z1 };
        mesh.tris.push_back(tri);

        // Right face
        tri.p[0] = { x2, y1, z1 };
        tri.p[1] = { x2, y2, z1 };
        tri.p[2] = { x2, y2, z2 };
        mesh.tris.push_back(tri);

        tri.p[0] = { x2, y1, z1 };
        tri.p[1] = { x2, y2, z2 };
        tri.p[2] = { x2, y1, z2 };
        mesh.tris.push_back(tri);

        // Back face
        tri.p[0] = { x2, y1, z2 };
        tri.p[1] = { x2, y2, z2 };
        tri.p[2] = { x1, y2, z2 };
        mesh.tris.push_back(tri);

        tri.p[0] = { x2, y1, z2 };
        tri.p[1] = { x1, y2, z2 };
        tri.p[2] = { x1, y1, z2 };
        mesh.tris.push_back(tri);

        // Left face
        tri.p[0] = { x1, y1, z2 };
        tri.p[1] = { x1, y2, z2 };
        tri.p[2] = { x1, y2, z1 };
        mesh.tris.push_back(tri);

        tri.p[0] = { x1, y1, z2 };
        tri.p[1] = { x1, y2, z1 };
        tri.p[2] = { x1, y1, z1 };
        mesh.tris.push_back(tri);

        // Top face
        tri.p[0] = { x1, y2, z1 };
        tri.p[1] = { x1, y2, z2 };
        tri.p[2] = { x2, y2, z2 };
        mesh.tris.push_back(tri);

        tri.p[0] = { x1, y2, z1 };
        tri.p[1] = { x2, y2, z2 };
        tri.p[2] = { x2, y2, z1 };
        mesh.tris.push_back(tri);

        // Bottom face
        tri.p[0] = { x2, y1, z2 };
        tri.p[1] = { x1, y1, z2 };
        tri.p[2] = { x1, y1, z1 };
        mesh.tris.push_back(tri);

        tri.p[0] = { x2, y1, z2 };
        tri.p[1] = { x1, y1, z1 };
        tri.p[2] = { x2, y1, z1 };
        mesh.tris.push_back(tri);
    }

    void mesh::ReadSTLBinary(std::string filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            std::cout << "Failed to open STL file: " << filename << std::endl;
            return;
        }
        char header[80] = { 0 };
        file.read(header, 80);
        uint32_t triangleCount = 0;
        file.read(reinterpret_cast<char*>(&triangleCount), sizeof(uint32_t));
        std::cout << triangleCount << '\n';
        tris.reserve(triangleCount);
        for (uint32_t i = 0; i < triangleCount; ++i)
        {
            triangle tri;
            vec3d normal, v1, v2, v3;
            file.read(reinterpret_cast<char*>(&normal), sizeof(vec3d));
            file.read(reinterpret_cast<char*>(&v1), sizeof(vec3d));
            file.read(reinterpret_cast<char*>(&v2), sizeof(vec3d));
            file.read(reinterpret_cast<char*>(&v3), sizeof(vec3d));
            tri.p[0] = v1;
            tri.p[1] = v2;
            tri.p[2] = v3;
            tris.push_back(tri);
            file.ignore(2);
        }
        file.close();
    }
    void mesh::ReadOBJ(std::string filename, std::vector<std::string>& textures)
    {
        textures.clear();
        const bool debug = false;
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Could not open the file!" << std::endl;
            return;
        }
        std::string line, currentMaterial;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string prefix;
            ss >> prefix;
            if (prefix == "v")
            {
                vec3d vertex;
                ss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            }
            else if (prefix == "vt")
            {
                vec2d texCoord;
                ss >> texCoord.x >> texCoord.y;
                texCoords.push_back(texCoord);
            }
            else if (prefix == "f")
            {
                triangle tri;
                std::string vertexData;
                for (int i = 0; i < 3; i++)
                {
                    ss >> vertexData;
                    std::stringstream vertexStream(vertexData);
                    int vertexIndex, texCoordIndex;
                    if (std::getline(vertexStream, vertexData, '/'))
                    {
                        vertexIndex = std::stoi(vertexData);
                        if (std::getline(vertexStream, vertexData, '/'))
                        {
                            texCoordIndex = !vertexData.empty() ? std::stoi(vertexData) : -1;
                        }
                        else
                        {
                            texCoordIndex = -1;
                        }
                    }
                    if (vertexIndex - 1 >= 0 && vertexIndex - 1 < vertices.size())
                    {
                        tri.p[i] = vertices[vertexIndex - 1];
                    }
                    else
                    {
                        std::cerr << "Invalid vertex index detected!" << std::endl;
                        return;
                    }
                    if (texCoordIndex != -1 && texCoordIndex - 1 >= 0 && texCoordIndex - 1 < texCoords.size())
                    {
                        tri.t[i] = texCoords[texCoordIndex - 1];
                    }
                    else
                    {
                        tri.t[i] = vec2d{ 0.0f, 0.0f };
                    }
                }
                tri.color = sf::Color::White;
                tri.TextureName = currentMaterial;
                tris.push_back(tri);
            }
            else if (prefix == "usemtl")
            {
                ss >> currentMaterial;
                textures.push_back(currentMaterial);
                if (debug)
                    std::cout << "Using material: " << currentMaterial << std::endl;
            }
        }
        file.close();
    }    
}

