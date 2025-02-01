#include <sstream>
#include "StartState.hpp"

#include <iostream>
#include <list>

namespace game
{
	StartState::StartState(GameDataRef data) : _data(data)
	{


	}
	void StartState::Init()
	{
        vCamera = { 0, 0, -10 };
		//_data->tools.AddCube(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, _mesh);
		std::string a = "C:/Users/jekabins/Downloads/test2.stl";
		_mesh.ReadSTLBinary(a);


		/*for (auto x : _mesh.tris)
		{
			std::cout << "1: " << x.p[0].x << ' ' << x.p[0].y << ' ' << x.p[0].z << " | " << x.p[1].x << ' ' << x.p[1].y << ' ' << x.p[1].z << " | " << x.p[2].x << ' ' << x.p[2].y << ' ' << x.p[2].z;
			std::cout << '\n';
		}*/

		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;
	}
	void StartState::Pause()
	{

	}
	void StartState::Resume()
	{

	}
	void StartState::HandleInput()
	{
		sf::Event event;

		while (_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				_data->window.close();
			}
            if (event.type == sf::Event::KeyPressed)
            {
                float fRotationSpeed = 2.0f / 60;
                float fMovementSpeed = 5.0f;

                if (event.key.code == sf::Keyboard::Left)
                    fYaw -= fRotationSpeed;
                if (event.key.code == sf::Keyboard::Right)
                    fYaw += fRotationSpeed;
                if (event.key.code == sf::Keyboard::Up)
                    fPitch += fRotationSpeed;
                if (event.key.code == sf::Keyboard::Down)
                    fPitch -= fRotationSpeed;
                if (event.key.code == sf::Keyboard::W)
                    vCamera.z += fMovementSpeed;
                if (event.key.code == sf::Keyboard::S)
                    vCamera.z -= fMovementSpeed;
                if (event.key.code == sf::Keyboard::A)
                    vCamera.x += fMovementSpeed;
                if (event.key.code == sf::Keyboard::D)
                    vCamera.x -= fMovementSpeed;
                
            }
                
		}
	}
    void StartState::Update(float dt)
    {
        _data->window.clear();
        mat4x4 matRotZ, matRotX, matRotY, matMov, transformation;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                transformation.m[i][j] = (i == j) ? 1 : 0;
            }
        }
        fThetax = 1.0f * clock.getElapsedTime().asSeconds();

        matRotZ.m[0][0] = cosf(fThetaz);
        matRotZ.m[0][1] = sinf(fThetaz);
        matRotZ.m[1][0] = -sinf(fThetaz);
        matRotZ.m[1][1] = cosf(fThetaz);
        matRotZ.m[2][2] = 1;
        matRotZ.m[3][3] = 1;

        matRotY.m[0][0] = cosf(fThetay);
        matRotY.m[1][1] = cosf(fThetay);
        matRotY.m[2][2] = 1;
        matRotY.m[1][0] = -sinf(fThetay);
        matRotY.m[0][1] = sinf(fThetay);

        matRotX.m[0][0] = 1;
        matRotX.m[1][1] = cosf(fThetax * 0.5f);
        matRotX.m[1][2] = sinf(fThetax * 0.5f);
        matRotX.m[2][1] = -sinf(fThetax * 0.5f);
        matRotX.m[2][2] = cosf(fThetax * 0.5f);
        matRotX.m[3][3] = 1;

        matMov.m[0][0] = 1;
        matMov.m[1][1] = 1;
        matMov.m[2][2] = 1;
        matMov.m[3][3] = 1;

        matMov.m[3][0] = 0; // Translate X
        matMov.m[3][1] = 0; // Translate Y
        matMov.m[3][2] = 250; // Translate Z
        matMov.m[0][3] = 0;
        matMov.m[1][3] = 0;
        matMov.m[2][3] = 0;


        vec3d vUp{ 0, 1, 0 };


        vec3d vLookDir;
        vLookDir.x = cosf(fYaw) * cosf(fPitch);
        vLookDir.y = sinf(fPitch);
        vLookDir.z = sinf(fYaw) * cosf(fPitch);
        vLookDir = _data->tools.normalizeVector(vLookDir);

        vec3d vTarget = _data->tools.VectorAdd(vCamera, vLookDir);
        mat4x4 matCamera = _data->tools.Matrix_PointAt(vCamera, vTarget, vUp);

        mat4x4 matView = _data->tools.Matrix_QuickInverse(matCamera);

        mesh meshToClipp, meshToDraw;
        _data->tools.TransformObj(fThetax, fThetay, fThetaz, 0, 0, 250, _mesh, meshToClipp);

        for (auto tri : meshToClipp.tris)
        {
            triangle triProjected, triTransformed, triViewed;

            _data->tools.MultiplyMatrixVector(tri.p[0], triTransformed.p[0], transformation);
            _data->tools.MultiplyMatrixVector(tri.p[1], triTransformed.p[1], transformation);
            _data->tools.MultiplyMatrixVector(tri.p[2], triTransformed.p[2], transformation);

            // Calculate the triangle normal in view space
            vec3d normal, line1, line2;
            line1 = _data->tools.subtractVector(triTransformed.p[1], triTransformed.p[0]);
            line2 = _data->tools.subtractVector(triTransformed.p[2], triTransformed.p[0]);
            normal = _data->tools.crossProduct(line1, line2);
            normal = _data->tools.normalizeVector(normal);



            // Backface culling: Check if triangle is facing the camera
            vec3d vCameraRay = _data->tools.subtractVector(triTransformed.p[0], vCamera);
            if (_data->tools.dotProduct(normal, vCameraRay) < 0.0f)
            {
                // Convert the transformed triangle to view space
                _data->tools.MultiplyMatrixVector(triTransformed.p[0], triViewed.p[0], matView);
                _data->tools.MultiplyMatrixVector(triTransformed.p[1], triViewed.p[1], matView);
                _data->tools.MultiplyMatrixVector(triTransformed.p[2], triViewed.p[2], matView);

                float dp = _data->tools.dotProduct(normal, vLookDir);
                triViewed.color = sf::Color(fabs(dp) * 255.0f, fabs(dp) * 255.0f, fabs(dp) * 255.0f);


                // Clip against the near plane (if necessary)
                int nClippedTriangles = 0;
                triangle clipped[2];
                nClippedTriangles = _data->tools.TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

                // If the triangle passes clipping, project to 2D
                for (int n = 0; n < nClippedTriangles; n++)
                {
                    // Project to 2D screen space
                    _data->tools.MultiplyMatrixVector(clipped[n].p[0], triProjected.p[0], matProj);
                    _data->tools.MultiplyMatrixVector(clipped[n].p[1], triProjected.p[1], matProj);
                    _data->tools.MultiplyMatrixVector(clipped[n].p[2], triProjected.p[2], matProj);

                    // Fix the X and Y coordinates for proper rendering
                    triProjected.p[0].x *= -1.0f;
                    triProjected.p[1].x *= -1.0f;
                    triProjected.p[2].x *= -1.0f;
                    triProjected.p[0].y *= -1.0f;
                    triProjected.p[1].y *= -1.0f;
                    triProjected.p[2].y *= -1.0f;

                    // Normalize coordinates to screen space
                    vec3d vOffsetView = { 1, 1, 0 };
                    triProjected.p[0] = _data->tools.VectorAdd(triProjected.p[0], vOffsetView);
                    triProjected.p[1] = _data->tools.VectorAdd(triProjected.p[1], vOffsetView);
                    triProjected.p[2] = _data->tools.VectorAdd(triProjected.p[2], vOffsetView);

                    triProjected.p[0].x *= 0.5f * (float)SCREEN_WIDTH;
                    triProjected.p[0].y *= 0.5f * (float)SCREEN_HEIGHT;
                    triProjected.p[1].x *= 0.5f * (float)SCREEN_WIDTH;
                    triProjected.p[1].y *= 0.5f * (float)SCREEN_HEIGHT;
                    triProjected.p[2].x *= 0.5f * (float)SCREEN_WIDTH;
                    triProjected.p[2].y *= 0.5f * (float)SCREEN_HEIGHT;

                    // Set triangle color based on dot product for lighting effect
                    //float dp = _data->tools.dotProduct(normal, vLookDir);
                    //triProjected.color = sf::Color(fabs(dp) * 255.0f, fabs(dp) * 255.0f, fabs(dp) * 255.0f);

                    // Add the projected triangle to the mesh to be drawn

                    triProjected.color = clipped[n].color;
                    //std::cout << triProjected.color.g << ' ';
                    meshToDraw.tris.push_back(triProjected);
                }
            }













                // Project triangles from 3D --> 2D
                //_data->tools.MultiplyMatrixVector(triViewed.p[0], triProjected.p[0], matProj);
                //_data->tools.MultiplyMatrixVector(triViewed.p[1], triProjected.p[1], matProj);
                //_data->tools.MultiplyMatrixVector(triViewed.p[2], triProjected.p[2], matProj);


                //// Offset into visible normalized space
                //triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
                //triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
                //triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
                //triProjected.p[0].x *= 0.5f * (float)SCREEN_WIDTH;
                //triProjected.p[0].y *= 0.5f * (float)SCREEN_HEIGHT;
                //triProjected.p[1].x *= 0.5f * (float)SCREEN_WIDTH;
                //triProjected.p[1].y *= 0.5f * (float)SCREEN_HEIGHT;
                //triProjected.p[2].x *= 0.5f * (float)SCREEN_WIDTH;
                //triProjected.p[2].y *= 0.5f * (float)SCREEN_HEIGHT;

                // Draw the triangle
                /*sf::VertexArray trigle(sf::Triangles, 3);
                float dp = _data->tools.dotProduct(normal, vLookDir);
                trigle[0].color = sf::Color(fabs(dp) * 255.0f, fabs(dp) * 255.0f, fabs(dp) * 255.0f);
                trigle[1].color = sf::Color(fabs(dp) * 255.0f, fabs(dp) * 255.0f, fabs(dp) * 255.0f);
                trigle[2].color = sf::Color(fabs(dp) * 255.0f, fabs(dp) * 255.0f, fabs(dp) * 255.0f);

                trigle[0].position.x = triProjected.p[0].x;
                trigle[0].position.y = triProjected.p[0].y;
                trigle[1].position.x = triProjected.p[1].x;
                trigle[1].position.y = triProjected.p[1].y;
                trigle[2].position.x = triProjected.p[2].x;
                trigle[2].position.y = triProjected.p[2].y;

                _data->window.draw(trigle);*/
            
        }
        for (auto& triToRaster : meshToDraw.tris)
        {
            triangle clipped[2];
            std::list <triangle> listTriangles;
            listTriangles.push_back(triToRaster);
            int nNewTriangles = 1;

            for (int p = 0; p < 4; p++)
            {
                int nTrisToAdd = 0;
                while (nNewTriangles > 0)
                {
                    triangle test = listTriangles.front();
                    listTriangles.pop_front();
                    nNewTriangles--;
                    switch (p)
                    {
                    case 0:	nTrisToAdd = _data->tools.TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    case 1:	nTrisToAdd = _data->tools.TriangleClipAgainstPlane({ 0.0f, (float)SCREEN_HEIGHT - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    case 2:	nTrisToAdd = _data->tools.TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    case 3:	nTrisToAdd = _data->tools.TriangleClipAgainstPlane({ (float)SCREEN_WIDTH - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    }
                    for (int w = 0; w < nTrisToAdd; w++)
                        listTriangles.push_back(clipped[w]);
                }
                nNewTriangles = listTriangles.size();
            }
            for (auto& x : listTriangles)
            {
                sf::VertexArray trigle(sf::Triangles, 3);
                trigle[0].color = x.color;
                trigle[1].color = x.color;
                trigle[2].color = x.color;

                trigle[0].position.x = x.p[0].x;
                trigle[0].position.y = x.p[0].y;
                trigle[1].position.x = x.p[1].x;
                trigle[1].position.y = x.p[1].y;
                trigle[2].position.x = x.p[2].x;
                trigle[2].position.y = x.p[2].y;

                _data->window.draw(trigle);
            }
        }
    }

	void StartState::Draw(float dt)
	{
		//_data->window.clear();
		//_data->window.draw(rect);
		_data->window.display();
	}
}