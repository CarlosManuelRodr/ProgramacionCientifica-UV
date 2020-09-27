#include "BmpWriter.h"

#include <climits>
#include <iostream>
using namespace std;

// Operaciones binarias.
char* to_byte(void* ptr)
{
    return static_cast<char*>(ptr);
}
template<typename T> T swap_endian(T u)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");
    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;
    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];
    return dest.u;
}

BMPPixel::BMPPixel()
{
    r = 0;
    g = 0;
    b = 0;
}
BMPPixel::BMPPixel(char mR, char mG, char mB)
{
    r = mR;
    g = mG;
    b = mB;
}
bool BMPPixel::operator==(const BMPPixel &other)
{
    if(r == other.r && g == other.g && b == other.b) return true;
    else return false;
}

BMPWriter::BMPWriter(string filepath, unsigned int width, unsigned int height)
{
    // Crea encabezado.
    m_index_height = 0;
    m_width = width;
    m_height = height;
    m_data_size = width*height;
    m_bmp_hdr = new BMPHeader;
    m_dib_hdr = new DIBHeader;

    m_bmp_hdr->identifier = swap_endian<uint16_t>(0x424d);    // Windows.
    m_bmp_hdr->app_specific1 = 0x0000;
    m_bmp_hdr->app_specific2 = 0x0000;

    m_dib_hdr->width = width;
    m_dib_hdr->height = height;
    m_dib_hdr->n_planes = swap_endian<uint16_t>(0x0100);
    m_dib_hdr->color_depth = swap_endian<uint16_t>(0x1800);
    m_dib_hdr->compression = 0x00000000;
    m_dib_hdr->bmp_bytes = swap_endian<uint32_t>(0x10000000);
    m_dib_hdr->h_res = swap_endian<int32_t>(0x130B0000);
    m_dib_hdr->v_res = swap_endian<int32_t>(0x130B0000);
    m_dib_hdr->n_colors = 0x00000000;
    m_dib_hdr->n_imp_colors = 0x00000000;

    unsigned int bmp_size = 0;
    unsigned int offset_data = 54;
    m_padding_bytes = m_width % 4;

    // Calcula tamaño del archivo.
    bmp_size += 14;        //BMPHeader size.
    bmp_size += 40;        //DIBHeader size.
    bmp_size += 3*width*height;
    bmp_size += m_height*m_padding_bytes;
    m_bmp_hdr->size = bmp_size;
    m_bmp_hdr->bitmap_data = offset_data;
    m_dib_hdr->header_size = 40;        //DIBHeader size.

    // Escribe encabezado.
    m_file.open(filepath.c_str(), ios::out | ios::binary);
    if (m_file.is_open())
    {
        m_file.write(to_byte(&m_bmp_hdr->identifier), 2);
        m_file.write(to_byte(&m_bmp_hdr->size), 4);
        m_file.write(to_byte(&m_bmp_hdr->app_specific1), 2);
        m_file.write(to_byte(&m_bmp_hdr->app_specific2), 2);
        m_file.write(to_byte(&m_bmp_hdr->bitmap_data), 4);
        m_file.write(to_byte(&m_dib_hdr->header_size), 4);
        m_file.write(to_byte(&m_dib_hdr->width), 4);
        m_file.write(to_byte(&m_dib_hdr->height), 4);
        m_file.write(to_byte(&m_dib_hdr->n_planes), 2);
        m_file.write(to_byte(&m_dib_hdr->color_depth), 2);
        m_file.write(to_byte(&m_dib_hdr->compression), 4);
        m_file.write(to_byte(&m_dib_hdr->bmp_bytes), 4);
        m_file.write(to_byte(&m_dib_hdr->h_res), 4);
        m_file.write(to_byte(&m_dib_hdr->v_res), 4);
        m_file.write(to_byte(&m_dib_hdr->n_colors), 4);
        m_file.write(to_byte(&m_dib_hdr->n_imp_colors), 4);
    }
    else
    {
        cout << "Error: No se puede crear archivo BMP." << endl;
    }
}
BMPWriter::~BMPWriter()
{
    delete m_bmp_hdr;
    delete m_dib_hdr;
}
void BMPWriter::WriteLine(BMPPixel* data)
{
    // Escribe línea individual del BMP.
    if(m_index_height < m_height)
    {
        for(unsigned int i = 0; i < m_width; i++)
        {
            m_file.write(&data[i].b, 1);
            m_file.write(&data[i].g, 1);
            m_file.write(&data[i].r, 1);
        }
        if(m_padding_bytes == 1)
        {
            char padding = 0x00;
            m_file.write(to_byte(&padding), 1);
        }
        if(m_padding_bytes == 2)
        {
            short padding = 0x0000;
            m_file.write(to_byte(&padding), 2);
        }
        if(m_padding_bytes == 3)
        {
            unsigned int padding = 0x00000000;
            m_file.write(to_byte(&padding), 3);
        }
    }
    m_index_height++;
}
void BMPWriter::WriteLine(std::vector<BMPPixel> data)
{
    if (data.size() == m_width)
        WriteLine(&data[0]);
}
bool BMPWriter::IsOpen() const
{
    return m_file.is_open();
}
void BMPWriter::CloseBMP()
{
    m_file.close();
}
