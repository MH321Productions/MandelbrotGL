#pragma once
#include <string>
#include <vector>

#include "2D/Geometry.hpp"
#include "2D/Unicode.hpp"

enum class WidgetType {None, Button, RadioButton, CheckButton};

class Main;
class Texture;
class Font;
class AbstractButton;
class ClickEvent;
class RadioButton;
enum class TextureType;

/*Widgets*/
class Widget {
    protected:
        Widget(WidgetType type, const Rect& bounds, Main* main) : type(type), bounds(bounds), main(main) {}
    
    public:
        Rect bounds;
        WidgetType type;
        Main* main;
        bool enabled = true;

        virtual ~Widget() {}

        virtual void onLoop() = 0;
        virtual void onRender() = 0;
        virtual void onClick(const Point& pos) = 0;
        virtual void onHover(const Point& pos) = 0;

        //Queries
        virtual bool isButton() {return false;}
        virtual bool isSelectionButton() {return false;}
};

class AbstractButton : public virtual Widget {
    protected:
        ClickEvent* click;

        AbstractButton(ClickEvent* click) : click(click) {}

        void triggerEvent();
    
    public:
        ~AbstractButton();

        virtual void onLoop() = 0;
        virtual void onRender() = 0;
        virtual void onClick(const Point& pos) = 0;
        virtual void onHover(const Point& pos) = 0;

        //Queries
        virtual bool isButton() {return true;}
};

class Button : public virtual AbstractButton {
    protected:
        Texture* image, *hoverImage;
        String32 text;
        bool isHovering = false;
        Font* font;
    
    public:
        Button(const Rect& bounds, Main* main, const String32& text, ClickEvent* click);

        ~Button() {}

        virtual void onLoop();
        virtual void onRender();
        virtual void onClick(const Point& pos);
        virtual void onHover(const Point& pos);
};

class SelectionButton : public virtual AbstractButton {
    protected:
        bool selected;
        SelectionButton(const bool& isSelected) : selected(isSelected) {}
        ~SelectionButton() {}

        inline void flipSelection() {selected = !selected;}

    public:
        bool isSelected() const {return selected;}
        virtual void setSelected(const bool& newSelection, const bool& triggerEvent = true) {
            selected = newSelection;
            if (triggerEvent) AbstractButton::triggerEvent();
        }

        virtual bool isSelectionButton() {return true;}
};

class CheckButton : public virtual SelectionButton {
    protected:
        Texture* selectedImage, *image;
        String32 text;
        Font* font;

    public:
        CheckButton(const Rect& bounds, Main* main, const String32& text, const bool& isSelected, ClickEvent* event);

        virtual void onLoop() {}
        virtual void onRender();
        virtual void onClick(const Point& pos);
        virtual void onHover(const Point& pos) {}
};

class RadioButtonGroup {
    friend class RadioButton;
    private:
        std::vector<RadioButton*> buttons;

        void setSelectedButton(RadioButton* btn);
        void addButton(RadioButton* btn);

    public:
        RadioButton* getSelectedButton() const;
};

class RadioButton: public virtual SelectionButton {
    friend class RadioButtonGroup;
    protected:
        Texture* disabledImage, *image, *selectedImage;
        String32 text;
        Font* font;
        RadioButtonGroup* group;
    
    public: 
        RadioButton(const Rect& bounds, Main* main, const String32& text, const bool& isSelected, ClickEvent* event,
            TextureType selectedType, RadioButtonGroup* group);

        virtual void onLoop() {}
        virtual void onRender();
        virtual void onClick(const Point& pos);
        virtual void onHover(const Point& pos) {}

        virtual void setSelected(const bool& newSelection, const bool& triggerEvent = true);
};