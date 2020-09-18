/**
* @file BmpWriter.h
* @brief Escritor de archivos BMP.
* @author Carlos Manuel Rodríguez Martínez
* @date 7/18/2012
*/

#ifndef __bmpWriter
#define __bmpWriter

#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

/**
* @struct BMPHeader
* @brief Encabezado del archivo BMP.
*/
struct BMPHeader
{
    uint16_t identifier;
    uint32_t size;
    uint16_t app_specific1;
    uint16_t app_specific2;
    uint32_t bitmap_data;
};

/**
* @struct DIBHeader
* @brief Otro encabezado usado por los archivos BMP que contiene parámetros.
*/
struct DIBHeader
{
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t n_planes;
    uint16_t color_depth;
    uint32_t compression;
    uint32_t bmp_bytes;
    int32_t h_res;
    int32_t v_res;
    uint32_t n_colors;
    uint32_t n_imp_colors;
};

/**
* @struct BMPPixel
* @brief Pixel simple RGB.
*/
class BMPPixel
{
public:
    char r, g, b;
    BMPPixel();
    BMPPixel(char mR, char mG, char mB);
    bool operator==(const BMPPixel &other);
};

/**
* @class BMPWriter
* @brief Clase para escribir archivos de mapas de bits de gran tamaño.
*/
class BMPWriter
{
    BMPHeader* m_bmp_hdr;
    DIBHeader* m_dib_hdr;
    std::ofstream m_file;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_padding_bytes;
    int m_data_size;
    unsigned int m_index_height;

public:
    ///@brief Constructor.
    ///@param filepath Ruta del archivo a guardar.
    ///@param width Tamaño horizontal de la imagen.
    ///@param height Tamaño vertical de la imagen.
    BMPWriter(std::string filepath, unsigned int width, unsigned int height);
    ~BMPWriter();

    ///@brief Writes BMP line.
    ///@param Array de pixeles a escribir. Las líneas se escriben de abajo a arriba.
    void WriteLine(BMPPixel* data);

    ///@brief Writes BMP line.
    ///@param Vector de pixeles a escribir. Las líneas se escriben de abajo a arriba.
    void WriteLine(std::vector<BMPPixel> data);

    ///@brief Return file status.
    bool IsOpen() const;

    ///@brief Close file.
    void CloseBMP();
};

#endif
