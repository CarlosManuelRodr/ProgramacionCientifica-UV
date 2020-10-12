#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "RandomGen.h"
#include "Material.h"
#include "ThreadPool.h"
#include "RayTracer.h"
#include "Filesystem.h"
#include "ProgressBar.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"

using namespace std;

/**
* @brief Dibuja una fila de la imagen a construir.
* @param j Fila a dibujar
* @param nx Tamaño horizontal de la imagen
* @param ny Tamaño vertical de la imagen
* @param ns Rayos por pixel
* @param world Lista con todos los elementos del mundo
* @param camera Cámara desde la cual se emiten los rayos
*/
vector<sf::Color> render_world_line(int j, int nx, int ny, int ns, const HitableList& world, const Camera& camera)
{
    vector<sf::Color> output;
    for (int i = 0; i < nx; i++)
    {
        Vector3 rayColor(0.0, 0.0, 0.0);
        for (int s = 0; s < ns; s++)
        {
            float u = float(i + RandomGen::Getfloat()) / float(nx);
            float v = float(j + RandomGen::Getfloat()) / float(ny);
            Ray r = camera.GetRay(u, v);
            rayColor += ray_trace(r, world);
        }
        rayColor /= float(ns);
        rayColor = Vector3(sqrt(rayColor[0]), sqrt(rayColor[1]), sqrt(rayColor[2]));

        int ir = int(255.99 * rayColor[0]);
        int ig = int(255.99 * rayColor[1]);
        int ib = int(255.99 * rayColor[2]);
        output.push_back(sf::Color(ir, ig, ib));
    }
    return output;
}

/**
* @brief Convierte una matriz de valores RGB a imagen de SFML.
* @param imageAsBitmap Matriz que contiene colores de los pixeles.
*/
sf::Image bitmap_to_image(vector< vector<sf::Color> > imageAsBitmap)
{
    int ny = (int) imageAsBitmap.size();
    int nx = (int) imageAsBitmap.front().size();

    sf::Image output;
    output.create(nx, ny);

    for (int j = 0; j < ny; j++)
    {
        for (int i = 0; i < nx; i++)
        {
            output.setPixel(i, ny - j - 1, imageAsBitmap[j][i]);
        }
    }

    return output;
}

/**
* @brief Dibuja la imagen. Versión multi-thread.
* @param nx Tamaño horizontal de la imagen
* @param ny Tamaño vertical de la imagen
* @param ns Rayos por pixel
* @param world Lista con todos los elementos del mundo
* @param camera Cámara desde la cual se emiten los rayos
*/
sf::Image render_world_mt(int nx, int ny, int ns, const HitableList& world, const Camera& camera)
{
    unsigned int hc = thread::hardware_concurrency();

    ThreadPool pool(hc);
    vector<future<vector<sf::Color>>> result;
    vector<vector<sf::Color>> imageAsBitmap;

    for (int j = 0; j < ny; j++)
        result.push_back(pool.enqueue(render_world_line, j, nx, ny, ns, world, camera));

    for (auto& r : result)
    {
        progress_bar((float)(&r - &result[0]), 0.0f, (float) result.size(), 1.0f);
        imageAsBitmap.push_back(r.get());
    }
    progress_bar(1, 0, 1, 1);

    return bitmap_to_image(imageAsBitmap);
}

/**
* @brief Dibuja la imagen. Versión single-thread. Se usa en el modo (debug).
* @param nx Tamaño horizontal de la imagen
* @param ny Tamaño vertical de la imagen
* @param ns Rayos por pixel
* @param world Lista con todos los elementos del mundo
* @param camera Cámara desde la cual se emiten los rayos
*/
sf::Image render_world_st(int nx, int ny, int ns, const HitableList& world, const Camera& camera)
{
    vector<vector<sf::Color>> imageAsBitmap;

    for (int j = 0; j < ny; j++)
        imageAsBitmap.push_back(render_world_line(j, nx, ny, ns, world, camera));


    return bitmap_to_image(imageAsBitmap);
}

int main()
{
    RandomGen::Seed();
    int nx = 800;
    int ny = 600;
    int ns = 30;

    // Carga las texturas
    int earthTextWidth, earthTextHeight, earthTextChannels;
    string earthTexturePath = filename_join({ "Textures", "earthmap.jpg" });
    unsigned char *earthTexture = stbi_load(earthTexturePath.c_str(), &earthTextWidth, &earthTextHeight, &earthTextChannels, 0);

    int skyTextWidth, skyTextHeight, skyTextChannels;
    string starBackgroundPath = filename_join({ "Textures", "starbackground.jpg" });
    unsigned char* skyTexture = stbi_load(starBackgroundPath.c_str(), &skyTextWidth, &skyTextHeight, &skyTextChannels, 0);

    // Inicializa el mundo
    vector<Hitable*> hitableList;
    hitableList.push_back(
        new Sphere(
            Vector3(7.0f, 0.0f, -1.0f), 1.0f,
            new Lambertian(new ImageTexture(earthTexture, earthTextWidth, earthTextHeight))
        )
    );
    hitableList.push_back(
        new Sphere(
            Vector3(7.0f, 2.5f, -1.0f), 0.5f,
            new DiffuseLight(new ConstantTexture(Vector3(2.0f, 2.0f, 2.0f)))
        )
    );
    hitableList.push_back(
        new Sphere(
            Vector3(0.0f, 0.0f, -1.0f), 5.5f,
            new Schwarzschild(0.05f)
        )
    );
    hitableList.push_back(
        new Sphere(
            Vector3(0.0f, 0.0f, 0.0f), 200.0f,
            new Lambertian(new ImageTexture(skyTexture, skyTextWidth, skyTextHeight))
        )
    );
    HitableList world(hitableList);

    // Configuración de la cámara
    Vector3 cameraPosition(4.0f, 7.0f, 3.0f);
    Vector3 cameraLookAt(4.0f, 0.0f, -1.0f);
    Camera camera = Camera(cameraPosition, cameraLookAt, Vector3(0.0f, 1.0f, 0.0f), 90.0f, (float) nx / (float) ny);

    // Prepara el directorio de salida
    create_directory("Output");

    // Inicializa la ventana SFML
    sf::RenderWindow window(sf::VideoMode(nx, ny), "Ray Tracer");
    window.setFramerateLimit(10);

    // Inicializa las texturas donde se dibujará la imagen.
    sf::Texture texture;
    texture.create(nx, ny);
    sf::Sprite sprite;
    sprite.setTexture(texture);

#if defined(_DEBUG)
    texture.update(render_world_st(nx, ny, ns, world, camera));
#else
    texture.update(render_world_mt(nx, ny, ns, world, camera));
#endif

    // Guarda el primer render.
    texture.copyToImage().saveToFile(filename_join({ "Output", "img0.png" }));

    int renders = 1;
    const float movementDelta = 0.05f;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                // Teclas direccionales
                if (event.key.code == sf::Keyboard::Right)
                {
                    cameraPosition += Vector3(movementDelta, 0.0f, 0.0f);
                    cameraLookAt += Vector3(movementDelta, 0.0f, 0.0f);
                }
                if (event.key.code == sf::Keyboard::Left)
                {
                    cameraPosition += Vector3(-movementDelta, 0.0f, 0.0f);
                    cameraLookAt += Vector3(-movementDelta, 0.0f, 0.0f);
                }
                if (event.key.code == sf::Keyboard::Up)
                {
                    cameraPosition += Vector3(0.0f, 0.0f, -movementDelta);
                    cameraLookAt += Vector3(0.0f, 0.0f, -movementDelta);
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    cameraPosition += Vector3(0.0f, 0.0f, movementDelta);
                    cameraLookAt += Vector3(0.0f, 0.0f, movementDelta);
                }
                if (event.key.code == sf::Keyboard::Q)
                {
                    cameraPosition += Vector3(0.0f, movementDelta, 0.0f);
                    cameraLookAt += Vector3(0.0f, movementDelta, 0.0f);
                }
                if (event.key.code == sf::Keyboard::E)
                {
                    cameraPosition += Vector3(0.0f, -movementDelta, 0.0f);
                    cameraLookAt += Vector3(0.0f, -movementDelta, 0.0f);
                }

                // WASD
                if (event.key.code == sf::Keyboard::D)
                    cameraLookAt += Vector3(movementDelta, 0.0f, 0.0f);
                if (event.key.code == sf::Keyboard::A)
                    cameraLookAt += Vector3(-movementDelta, 0.0f, 0.0f);
                if (event.key.code == sf::Keyboard::W)
                    cameraLookAt += Vector3(0.0f, 0.0f, -movementDelta);
                if (event.key.code == sf::Keyboard::S)
                    cameraLookAt += Vector3(0.0f, 0.0f, movementDelta);

                camera.SetLookFrom(cameraPosition);
                camera.SetLookAt(cameraLookAt);

            #if defined(_DEBUG)
                texture.update(render_world_st(nx, ny, ns, world, camera));
            #else
                texture.update(render_world_mt(nx, ny, ns, world, camera));
            #endif

                // Guarda la imagen
                texture.copyToImage().saveToFile(filename_join({ "Output", "img" + to_string(renders) + ".png" }));
                renders++;
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
}