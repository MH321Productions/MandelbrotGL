#pragma once

#include <array>
//#include "Geometry.hpp"

using rgb = unsigned char;

class Buffer;           //Buffer/Buffer.hpp
class Texture;          //Texture/Texture.hpp
class Font;             //2D/Font.hpp
class String32;         //2D/Unicode.cpp
class Main;             //App/App.hpp
class Point;            //2D/Geometry.hpp
class Rect;             //2D/Geometry.hpp

const int renderWidth = 1920; /*Die interne Breite des Renderers*/ 
const int renderHeight = 1080; /*Die interne Höhe des Renderers*/
const int renderSize = renderWidth * renderHeight; /*Die Anzahl der Pixel des Renderers (width * height)*/

class Color {
    private:
        void checkValue(const int& input, rgb& rgba);

    public:
        rgb r, g, b, a;

        Color(const rgb& r, const rgb& g, const rgb& b, const rgb& a): r(r), g(g), b(b), a(a) {}
        Color(const int& r, const int& g, const int& b, const int& a);
        Color(const double& r, const double& g, const double& b, const double& a);
        Color(const Color& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
        Color(Color& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
        Color() : r(0), g(0), b(0), a(0) {}

        Color& operator = (const Color& c);
        Color& operator = (Color& c);
        Color operator + (Color& c) const;
        Color operator + (Color c) const;
        Color& operator += (Color& c);
        Color operator - (Color& c) const;
        Color& operator -= (Color& c);
        Color operator * (const rgb& scalar) const;
        Color operator * (const double& scalar) const;
        Color& operator *= (const rgb& scalar);
        Color operator / (const rgb& scalar) const;
        Color& operator /= (const rgb& scalar);

        bool operator == (const Color& c) const;
};

class Colors {
    private:
        Colors() {} //Wird nicht genutzt
    
    public:
        static const Color Transparent;
        static const Color Black;
        static const Color White;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
};

enum BoundResult{Inside = 0, Top = 1, Left = 2, Right = 4, Bottom = 8};

/**
 * Der 2D Renderer, um GUIs, HUDs und ähnliches zu rendern.
 * Er muss in folgender Weise benutzt werden:
 * 1. setColor(Colors::Transparent)
 * 2. clear()
 * 3. Sämtliche Rendermethoden
 * 4. render(): Als letztes in der OpenGL Reihenfolge rendern
 */ 
class Renderer {
    private:
        //Color* data; //Pixeldaten
        std::array<Color, renderSize> data;
        Color currentColor;
        Main* main;
        unsigned int id;
        
        int isOutside(const int& x, const int& y);
        Color& at(const int& x, const int& y);
        //Setzen mit Blending
        void set(const int& x, const int& y, const Color& c, const bool& setFullyOpaque = true);
        double scalarX, scalarY;

    public:
        Renderer(const int& width, const int& height, Main* main);
        Renderer(Main* main) : Renderer(1920, 1080, main) {}
        ~Renderer();

        //Rendermethoden
        /**
         * Leert die Daten und setzt alle Pixel auf die aktuell gesetzte Farbe
         */ 
        void clear();

        /**
         * Setzt die aktuelle Farbe
         * @param c Die zu setzende Farbe
         */ 
        void setColor(const Color& c);

        /**
         * Schreibt die Pixeldaten in eine OpenGL Textur und rendert sie
         */ 
        void render(const int& samplerLoc, Buffer* buffer);

        /**
         * Skaliert den Renderer auf eine neue Größe
         * @param width Die neue Breite in Pixeln
         * @param height Die neue Höhe in Pixeln
         */ 
        void resize(const int& width, const int& height);

        /**
         * Gibt die Mausposition relativ zur Renderergröße aus,
         * damit die Koordinaten beim Skalieren des Fensters
         * konstant bleiben
         * 
         * @returns Ein Punkt mit relativen Mauskoordinaten
         */ 
        Point getRelMousePos();

        /**
         * Zeichnet den Rand eines Rechtecks mit der ausgewählten Farbe
         * 
         * @param rect Das zu zeichnende Rechteck
         * @param linesize Die Breite der Linien
         */ 
        void drawRect(const Rect& rect, const int& linesize = 1);

        /**
         * Zeichnet ein gefülltes Rechteck mit der ausgewählten Farbe
         * 
         * @param rect Das zu zeichnende Rechteck
         */ 
        void fillRect(const Rect& rect);

        /**
         * Zeichnet eine Textur (oder Teile davon)
         * @param texture Die zu kopierende Textur
         * @param src Der Ausschnitt der Textur, der kopiert werden soll, oder Rect::emptyRect für die ganze Textur
         * @param destX Die x-Koordinate des Bildes (obere linke Ecke)
         * @param destY Die y-Koordinate des Bildes (obere linke Ecke)
         */ 
        void drawTexture(Texture* texture, const Rect& src, const int& destX, const int& destY);

        /**
         * Rendert UTF-32 Text mit der ausgewählten Farbe
         * @param font Der Font, der den Text darstellen soll
         * @param text Der UTF-32 Text
         * @param x Der x-Wert der oberen linken Ecke
         * @param y Der y-Wert der oberen linken Ecke
         * @param size Die Schriftgröße in Pixeln
         * @param charSpacing Der Abstand zwischen zwei Buchstaben in Pixeln (Standardmäßig 1/10 der Schriftgröße)
         */ 
        void drawText(Font* font, const String32& text, const int& x, const int& y, const int& size, int charSpacing = -1);

        /**
         * Rendert die aktuellen FPS in die obere Linke Ecke mit der ausgewählten Farbe
         */ 
        void drawFPS(Font* font);
};