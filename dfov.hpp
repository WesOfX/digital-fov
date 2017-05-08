#pragma once
#include <tuple>
#include <numeric>

namespace dfov{
    template<class map_type>
    inline constexpr bool los(
        const map_type& map,
        size_t x0,
        size_t y0,
        size_t x1,
        size_t y1
    ){
        constexpr auto map_rows = std::tuple_size<map_type>::value,
                       map_columns = std::tuple_size<
                           typename map_type::value_type
                       >::value;
        if(
            x0 >= map_columns
         || y0 >= map_rows
         || x1 >= map_columns
         || y1 >= map_rows
        ) throw std::runtime_error("points are outside of the map's bounds");
        if(x0 == x1 && y0 == y1) return true;
        ptrdiff_t x_offset = x1 - x0,
                  y_offset = y1 - y0,
                  alt_x_offset = abs(x_offset),
                  alt_y_offset = abs(y_offset);
        int8_t x_normal = (x_offset == 0 ? 1 : x_offset / alt_x_offset),
               y_normal = (y_offset == 0 ? 1 : y_offset / alt_y_offset);
           bool swapped_axis = false;
        if(alt_x_offset < alt_y_offset){
            swapped_axis = true;
            std::swap(alt_x_offset, alt_y_offset);
        }
        size_t x_unit = alt_x_offset / std::gcd(alt_x_offset, alt_y_offset),
               y_unit = alt_y_offset / std::gcd(alt_x_offset, alt_y_offset);
        for(size_t starting_eps = 0; starting_eps < x_unit; ++starting_eps){
            auto eps = starting_eps;
            ptrdiff_t x_relative = 0, y_relative = 0;
            for(ptrdiff_t n = 0; n < alt_x_offset; ++n){
                eps += y_unit;
                if(eps >= x_unit){
                    eps -= x_unit;
                    x_relative += x_normal;
                    y_relative += y_normal;
                }
                else if(swapped_axis)
                    y_relative += y_normal;
                else x_relative += x_normal;
                if(map[y0 + y_relative][x0 + x_relative]) break;
            }
            if(swapped_axis){
                if(y_offset == y_relative) return true;
            }
            else if(x_offset == x_relative) return true;
        }
        return false;
    }
};
