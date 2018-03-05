///////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief Contains implementation of \a grid class
///
/// Should be augmented by students if needed
///
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <iomanip>      // std::setw() if needed
#include <stdexcept>

#include "grid.h"

// According to new restrictions of C++ standards, an explicit definition of static consts is a need
const bool grid::NOT_INFECTED = false;      // = false;
const bool grid::INFECTED = true;          // = true;



// Students may need to alter the constructor
grid::grid(const std::string& fileName)
{
  
    std::ifstream grid_file;

    try
    {
        grid_file.open(fileName.c_str());

        // check if it's all ok
        if (!grid_file.good())
            throw std::runtime_error("File cannot be open");

        // opened correctly
        grid_file >> _rows;
        grid_file >> _cols;

        _area = std::vector<bool>(_rows * _cols, grid::NOT_INFECTED);
        _visited = std::vector<bool>(_rows * _cols, false);

        int blob_row;
        int blob_col;

        while (!grid_file.eof() && grid_file.good())
        {
            grid_file >> blob_row;
            grid_file >> blob_col;

            _area.operator[](indexof(blob_row, blob_col)) = INFECTED;
        }
    }
    catch (...)
    {
        // always tries to properly close the file!
        grid_file.close();
        throw;              // dispatch further
    }
  
    grid_file.close();
}

// Students may need to alter the destructor
grid::~grid () 
{ }


// Students will need to alter this function to display the
// plus signs (+) next to the cells that belong to
// a counted colony.
std::ostream& operator << (std::ostream& stream, const grid& ob) 
{

    for (int row = 0; row < ob._rows; ++row) 
    {     
        for (int col=0; col < ob._cols; col++) 
        {
            stream << ob._area.operator[](ob.indexof(row, col));
            // adds + to unit we are looking for
            stream << (ob._visited.operator[](ob.indexof(row, col)) ? "+   " : "    ");
        }

        stream << std::endl;
    }
    stream << std::endl;
    
    return stream;
}

bool grid::visited(int row, int col) const
{
    if (checkIndex(row, col))
        return _visited.operator[](indexof(row, col));
}


// Students have to replace the return statement in this function with their
// recursive implementation of this method */
int grid::count (int row, int col)
{
    if (!checkIndex(row, col) || !infected(row, col))
        return 0;

    int sum = 0;

    // go through all cells around the given one
    for (int i = row - 1; i <= row + 1 && i < _rows; ++i)
        for (int j = col - 1; j <= col + 1 && j < _cols; ++j)
            if ((row != i || col != j) && checkIndex(i, j) && infected(i, j) && !visited(i, j))
            {
                _visited[indexof(i, j)] = true;
                sum += count(i, j) + 1;
            }

    return sum;
}


// Students may need to alter this function
int grid::getCount (int row, int col)
{
    return count(row,col);
}



// Students do not need to alter function indexof.
int grid::indexof(int row, int col) const
{
    if (!checkIndex(row, col))
        throw std::invalid_argument("Index out of range");

    return row *_cols + col;
}

// Students do not need to alter function infected.
bool grid::infected(int row, int col) const 
{
    return (_area.at((unsigned int) indexof(row, col)) == INFECTED);
}


// Students do not need to alter this function w/o any special reasons.
bool grid::checkIndex(int row, int col) const
{
    if (row >= _rows || row < 0 || col >= _cols || col < 0)
        return false;
    return true;
}
