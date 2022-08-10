#include <iostream>
#include <GL/glew.h>
#include <sstream>
//#include <glm/glm.hpp>

#include "Renderer.hpp"
#include "App/App.hpp"
#include "Buffer/Buffer.hpp"
#include "Texture/Texture.hpp"
#include "Window/Window.hpp"
#include "stb_image_write.h"
#include "Font.hpp"
#include "Unicode.hpp"
#include "Geometry.hpp"

using namespace std;

Renderer::Renderer(const int& width, const int& height, Main* main) : main(main) {
    //Daten vorbereiten
    //data = new Color[max];
    //data.resize(max);

    //Skalare Vorbereiten
    scalarX = (double) renderWidth / (double) width;
    scalarY = (double) renderHeight / (double) height;

    //Textur erstellen
    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    cout << "2D Renderer initialisiert (" << width << " x " << height << ")" << endl;
}

Renderer::~Renderer() {
    //delete[] data;
    //data.clear();
}

int Renderer::isOutside(const int& x, const int& y) {
    int result = BoundResult::Inside;

    if (x < 0) result |= BoundResult::Left;
    if (x >= renderWidth) result |= BoundResult::Right;
    if (y < 0) result |= BoundResult::Top;
    if (y >= renderHeight) result |= BoundResult::Bottom;

    return result;
}

Color& Renderer::at(const int& x, const int& y) {
    return data.at(y * renderWidth + x);
}

void Renderer::set(const int& x, const int& y, const Color& c, const bool& setFullyOpaque) {
    if (c.a == 255) at(x, y) = c;
    else if (!c.a) return;
    else {
        double gray = c.a / 255.0, invertGray = (255 - c.a) / 255.0;
        at(x, y) = (at(x, y) * invertGray) + (c * gray);
        if (setFullyOpaque) at(x, y).a = 255;
    }
}

void Renderer::drawRect(const Rect& rect, const int& linesize) {
    //Wenn das Rechteck komplett außerhalb des Fensters ist, wird abgebrochen (Die zwei Eckpunkte werden getestet)
    if (isOutside(rect.x, rect.y) && isOutside(rect.x + rect.width, rect.y + rect.height)) return;

    int lineOffsetY = 0, lineOffsetX = 0;
    int maxLinesizeX = rect.width - linesize, maxLinesizeY = rect.height - linesize;
    int boundResult;
    for (int y = rect.y; y < rect.y + rect.height; y++) {
        for (int x = rect.x; x < rect.x + rect.width; x++) {
            
            //Testen nach Grenzen
            boundResult = isOutside(x, y);
            //Oben, Unten oder Rechts außerhalb -> Neue Zeile
            //Links außerhalb -> nächster Pixel
            if (boundResult & BoundResult::Bottom || boundResult & BoundResult::Top || boundResult & BoundResult::Right) break;
            if (boundResult & BoundResult::Left) continue;

            //Zeichnen
            if (lineOffsetX < linesize || lineOffsetY < linesize || lineOffsetX >= maxLinesizeX || lineOffsetY >= maxLinesizeY) {
                set(x, y, currentColor);
            }

            lineOffsetX++;
        }
        //Neue Zeile
        lineOffsetX = 0;
        lineOffsetY++;
    }
}

void Renderer::fillRect(const Rect& rect) {
    //Wenn das Rechteck komplett außerhalb des Fensters ist, wird abgebrochen (Die zwei Eckpunkte werden getestet)
    if (isOutside(rect.x, rect.y) && isOutside(rect.x + rect.width, rect.y + rect.height)) return;

    int boundResult;
    for (int y = rect.y; y < rect.y + rect.height; y++) {
        for (int x = rect.x; x < rect.x + rect.width; x++) {
            //Testen nach Grenzen
            boundResult = isOutside(x, y);
            //Oben, Unten oder Rechts außerhalb -> Neue Zeile
            //Links außerhalb -> nächster Pixel
            if (boundResult & BoundResult::Bottom || boundResult & BoundResult::Top || boundResult & BoundResult::Right) break;
            if (boundResult & BoundResult::Left) continue;

            //Zeichnen
            set(x, y, currentColor);
        }
    }
}

void Renderer::drawTexture(Texture* texture, const Rect& src, const int& destX, const int& destY) {
    int sOffX = 0, sOffY = 0;
    int w = 0, h = 0;

    //Teste nach gültigkeit des Starts, sonst 0, 0, width, height
    if (src.isValid()) {
        sOffX = src.x;
        sOffY = src.y;
        w = src.width;
        h = src.height;
    } else {
        w = texture->width;
        h = texture->height;
    }

    //Wenn das Rechteck komplett außerhalb des Fensters ist, wird abgebrochen (Die zwei Eckpunkte werden getestet)
    if (isOutside(destX, destY) && isOutside(destX + w, destY + h)) return;

    int boundResult;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            //Testen nach Grenzen
            boundResult = isOutside(x + destX, y + destY);
            //Oben, Unten oder Rechts außerhalb -> Neue Zeile
            //Links außerhalb -> nächster Pixel
            if (boundResult & BoundResult::Bottom || boundResult & BoundResult::Top || boundResult & BoundResult::Right) break;
            if (boundResult & BoundResult::Left) continue;

            //Zeichnen
            /*if (texture->at(x, y).a && !texture->isOutside(x + sOffX, y + sOffY)) {
                //at(x + destX, y + destY) = texture->at(x + src.x, y + src.y);
            }*/

            if (!texture->isOutside(x + sOffX, y + sOffY)) {
                set(x + destX, y + destY, texture->at(x + src.x, y + src.y));
            }
        }
    }
}

void Renderer::drawText(Font* font, const String32& text, const int& x, const int& y, const int& size, int charSpacing) {
    if (text.isEmpty() || size < 1 || !font->setSize(size)) return;

    int xPos = x;
    uint32_t glyphIndex;
    FT_GlyphSlot slot = font->face->glyph;
    FT_Bitmap map;
    if (charSpacing < 0) charSpacing = size / 10 + 1;
    int spaceSpacing = size / 2;

    for (const uint32_t& i: text) {
        if (i == 32) {
            xPos += spaceSpacing;
            continue;
        }

        glyphIndex = FT_Get_Char_Index(font->face, i);
        if (!FontManager::checkFTError(
            FT_Load_Glyph(font->face, glyphIndex, FT_LOAD_DEFAULT),
            "Konnte Glyph nicht laden"
        )) continue;

        if (!FontManager::checkFTError(
            FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL),
            "Konnte Glyph nicht rendern"
        )) continue;

        //Über generierte Bitmap iterieren. Grayscale Wert als alpha setzen
        int index = 0;
        map = slot->bitmap;
        int boundResult;
        int startY = y - slot->bitmap_top;
        Color c = currentColor;
        for (int tempY = y - slot->bitmap_top; tempY < y + map.rows - slot->bitmap_top; tempY++) {
            for (int tempX = xPos; tempX < xPos + map.width; tempX++) {

                //Testen nach Grenzen
                boundResult = isOutside(tempX, tempY);
                //Oben, Unten oder Rechts außerhalb -> Neue Zeile
                //Links außerhalb -> nächster Pixel
                if (boundResult & BoundResult::Bottom || boundResult & BoundResult::Top || boundResult & BoundResult::Right) break;
                if (boundResult & BoundResult::Left) continue;

                index = (tempY - startY) * map.width + (tempX - xPos);

                /*if (map.buffer[index] != 0) {
                    at(tempX, tempY) = currentColor;
                    at(tempX, tempY).a = map.buffer[index];
                }*/

                if (map.buffer[index] != 0) {
                    c.a = map.buffer[index];
                    set(tempX, tempY, c);
                }
            }
        }

        xPos += map.width + charSpacing;
    }

}

void Renderer::render(const int& samplerLoc, Buffer* buffer) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, renderWidth, renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glUniform1i(samplerLoc, 0);
    buffer->render();
}

void Renderer::clear() {
    data.fill(currentColor);
}

void Renderer::setColor(const Color& c) {
    currentColor = c;
}

void Renderer::resize(const int& width, const int& height) {
    if (width < 1 || height < 1) return;

    //Skalare Vorbereiten
    scalarX = (double) renderWidth / (double) width;
    scalarY = (double) renderHeight / (double) height;
}

Point Renderer::getRelMousePos() {
    return {(int) (main->window->mouseX * scalarX), (int) (main->window->mouseY * scalarY)};
}

void Renderer::drawFPS(Font* font) {
    currentColor = Colors::Green;

    ostringstream str;
    str << main->fps;
    string s = str.str();
    str.clear();

    drawText(font, s, 10, 30, 40);
}