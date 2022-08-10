#pragma once

#include <glm/fwd.hpp>

class Main;

class Point {
    public:
        int x, y;
    
        Point(const int& x, const int& y) : x(x), y(y) {}
        Point() : x(0), y(0) {}
        glm::vec2 toCoord(Main* main);

        bool operator == (const Point& p) const;
        bool operator != (const Point& p) const;
        Point operator - (const Point& p) const;
};

class Rect {
    public:
        int x, y, width, height;

        Rect(const int& x, const int& y, const int& width, const int& height) : x(x), y(y), width(width), height(height) {}

        /**
         * Initialisiert das Rechteck als ungültig
         */ 
        Rect() : x(0), y(0), width(0), height(0) {}
        
        /**
         * Ermöglicht das Aufrufen mit NULL in Funktionen (Ähnlich wie in SDL)
         * 
         * @param null Ein Pointer (NULL/nullptr vorgesehen)
         */ 
        Rect(const void* null) : x(0), y(0), width(0), height(0) {}

        inline bool isValid() const {return x > 0 && y > 0 && width > 0 && height > 0;}
        bool isInside(const int& x, const int& y);
        bool isInside(const Point& p);

        static const Rect emptyRect;
};