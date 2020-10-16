#ifndef _PLOTTER
#define _PLOTTER

#include <vector>
#include <string>
#include <cmath>
#include "BmpWriter.h"
#include "Matrix.h"

// Color palette
const int paletteSize = 60;
const int redInt = 255;
const int redMed = 34;
const int redDes = 8;
const int greenInt = 201;
const int greenMed = 28;
const int greenDes = 12;
const int blueInt = 255;
const int blueMed = 21;
const int blueDes = 9;

double NormalDist(int x, double mean, double stdDev)
{
    return exp(-(pow(x-mean,2)/(2*pow(stdDev,2))));
}
Color CalcColor(unsigned int colorNum)
{
    colorNum = colorNum % paletteSize;

    // Return color with a normal distribution.
    char r, g, b;

    r = static_cast<char>(redInt*NormalDist(colorNum, redMed, redDes) + redInt*NormalDist(colorNum, paletteSize+redMed, redDes));
    g = static_cast<char>(greenInt*NormalDist(colorNum, greenMed, greenDes) + greenInt*NormalDist(colorNum, paletteSize+greenMed, greenDes));
    b = static_cast<char>(blueInt*NormalDist(colorNum, blueMed, blueDes)+blueInt*NormalDist(colorNum, paletteSize+blueMed, blueDes));

    return Color(r,g,b);
}

// Almacena coordenadas.
struct Vector2
{
public:
    double x, y;
    Vector2()
    {
        x = 0;
        y = 0;
    }
    Vector2(double _x, double _y)
    {
        x = _x;
        y = _y;
    }
};

double Maximum(std::vector<double> myArray)
{
    double max = myArray[0];
    for(unsigned int i=1; i<myArray.size(); i++)
    {
        if(myArray[i] > max)
        {
            max = myArray[i];
        }
    }
    return max;
}

// Funciones.
void Plot(const std::vector<double> data, const std::string filename)
{
    bool **myPlot;
    int coordY;

    unsigned int size = data.size();
    double minX = 0;
    double maxX = size;
    double minY = -Maximum(data);
    double maxY = -minY;
    double xFactor = (maxX-minX)/(size-1);
    double yFactor = (maxY-minY)/(size-1);

    // Reserva memoria
    myPlot = new bool*[size];
    for(unsigned i=0; i<size; i++)
    {
        myPlot[i] = new bool[size];
    }

    // Limpia myPlot
    for(unsigned i=0; i<size; i++)
    {
        for(unsigned j=0; j<size; j++)
        {
            myPlot[i][j] = false;
        }
    }

    // Asigna valores a myPlot
    for(unsigned int x=0; x<size; x++)
    {
        coordY = static_cast<int>((maxY-data[x])/yFactor);
        myPlot[x][coordY] = true;
    }

    // Crea imagen.
    BMPWriter writer(filename.c_str(), size, size);
    BMPPixel* bmpData = new BMPPixel[size];
    for(int j=size-1; j>=0; j--)
    {
        for(unsigned i=0; i<size; i++)
        {
            if(myPlot[i][j] == false)
            {
                bmpData[i].r = (char) 255;
                bmpData[i].g = (char) 255;
                bmpData[i].b = (char) 255;
            }
            else
            {
                bmpData[i].r = (char) 255;
                bmpData[i].g = (char) 0;
                bmpData[i].b = (char) 0;
            }
        }
        writer.WriteLine(bmpData);
    }

    // Limpieza.
    writer.CloseBMP();
    delete[] bmpData;
    for(unsigned i=0; i< size; i++)
    {
        delete[] myPlot[i];
    }
    delete[] myPlot;
}

void Plot(const std::vector<Vector2> data, const std::string filename)
{
    // Construye vector simple.
    std::vector<double> dblVector;
    for (unsigned i = 0; i < data.size(); i++)
    {
        dblVector.push_back(data[i].y);
    }

    // Grafica.
    Plot(dblVector, filename);
}

void Plot(Matrix<Color> &data, const std::string filename)
{
    int width = data.GetNumCols();
    int height = data.GetNumRows();

    BMPWriter writer(filename.c_str(), width, height);
    for(int j=height-1; j>=0; j--)
    {
        writer.WriteLine(data[j]);
    }
    writer.CloseBMP();
}

#endif
