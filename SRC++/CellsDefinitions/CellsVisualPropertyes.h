#pragma once

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/color.hpp>
#include <CellsDefinitions/CellTypes.h>

struct CellsVisualPropertyes 
{
    static godot::Color GetColorForCellType(CellTypes cell_type) 
    {
        switch (cell_type) 
        {
            case CellTypes::WATER:
                return godot::Color(0.1f, 0.3f, 0.8f);
            case CellTypes::SAND:
                return godot::Color(0.9f, 0.8f, 0.4f);
            case CellTypes::DIRT:
                return godot::Color(0.6f, 0.4f, 0.2f);
            default:
                return godot::Color(0, 0, 0, 0);
        }
    }
};