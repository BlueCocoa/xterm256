//
//  xterm256.h
//  xterm256
//
//  Created by BlueCocoa on 16/5/21.
//  Copyright © 2016 BlueCocoa. All rights reserved.
//

#ifndef XTREM256_HPP
#define XTREM256_HPP

#include <cmath>
#include <iostream>
#include <map>
#include <tuple>
#include <utility>
#include <vector>

class xterm256 {
    friend class color;
    using Color = std::tuple<unsigned short, unsigned short, unsigned short>;
private:
    std::vector<Color> xterm_colors;
    std::map<std::string, int> cache;
public:
    xterm256() {
        // colors 0..15: 16 basic colors
        
        xterm_colors.emplace_back(0x00, 0x00, 0x00); // 0
        xterm_colors.emplace_back(0xcd, 0x00, 0x00); // 1
        xterm_colors.emplace_back(0x00, 0xcd, 0x00); // 2
        xterm_colors.emplace_back(0xcd, 0xcd, 0x00); // 3
        xterm_colors.emplace_back(0x00, 0x00, 0xee); // 4
        xterm_colors.emplace_back(0xcd, 0x00, 0xcd); // 5
        xterm_colors.emplace_back(0x00, 0xcd, 0xcd); // 6
        xterm_colors.emplace_back(0xe5, 0xe5, 0xe5); // 7
        xterm_colors.emplace_back(0x7f, 0x7f, 0x7f); // 8
        xterm_colors.emplace_back(0xff, 0x00, 0x00); // 9
        xterm_colors.emplace_back(0x00, 0xff, 0x00); // 10
        xterm_colors.emplace_back(0xff, 0xff, 0x00); // 11
        xterm_colors.emplace_back(0x5c, 0x5c, 0xff); // 12
        xterm_colors.emplace_back(0xff, 0x00, 0xff); // 13
        xterm_colors.emplace_back(0x00, 0xff, 0xff); // 14
        xterm_colors.emplace_back(0xff, 0xff, 0xff); // 15
        
        // colors 16..232: the 6x6x6 color cube
        
        int valuerange[] = {0x00, 0x5f, 0x87, 0xaf, 0xd7, 0xff};
        for (int i = 0; i < 217; i++) {
            int r = valuerange[(i / 36) % 6];
            int g = valuerange[(i / 6) % 6];
            int b = valuerange[i % 6];
            xterm_colors.emplace_back(r, g, b);
        }
        
        // colors 233..253: grayscale
        
        for (int i = 1; i < 22; i++) {
            int v = 8 + i * 10;
            xterm_colors.emplace_back(v, v, v);
        }
    }
    
    ~xterm256() {
        std::cout << "\e[0m" << std::flush;
    }
    
    class color {
    public:
        color(unsigned short r, unsigned short g, unsigned short b) {
            r = r % 256;
            g = g % 256;
            b = b % 256;
            
            std::get<0>(_color) = r;
            std::get<1>(_color) = g;
            std::get<2>(_color) = b;
            
            char * str = (char *)malloc(sizeof(char) * 8);
            memset((void *)str, 0, 8);
            sprintf(str, "#%02X%02X%02X", r, g, b);
            _color_str = std::string(str);
            free((void *)str);
        }
        
        std::string id() const {
            return _color_str;
        }
        
        Color get() const {
            return _color;
        };
        
        friend xterm256& operator << (xterm256 &xterm, const color& c) {
            auto nearest = xterm.cache.find(c.id());
            int colornumber = -1;
            if (nearest != xterm.cache.cend()) {
                colornumber = nearest->second;
            } else {
                colornumber = xterm.find_nearest(c);
            }
            
            std::cout << "\e[38;5;" << colornumber << "m";
            return xterm;
        }
    private:
        Color _color;
        std::string _color_str;
    };
    
    int find_nearest(const color& color) {
        int distance = 257 * 257 * 3;
        int number = 0;
        int r = std::get<0>(color.get()), g = std::get<1>(color.get()), b = std::get<2>(color.get());
        for (int itarator = 0; itarator != xterm_colors.size(); itarator++) {
            auto values = xterm_colors[itarator];
            int rd = r - std::get<0>(values);
            int gd = g - std::get<1>(values);
            int bd = b - std::get<2>(values);
            
            int d = rd * rd + gd * gd + bd * bd;
            if (d < distance) {
                number = itarator;
                distance = d;
            }
        }
        return number;
    }
    
    template <typename T>
    xterm256& operator << (const T& t);
};

template <typename T>
xterm256& xterm256::operator << (const T& t) {
    std::cout << t;
    return *this;
}

#endif /* XTREM256_HPP */
