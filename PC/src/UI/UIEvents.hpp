#pragma once
#include <string>

class AbstractButton;
class Overlay;

class ClickEvent {
    private:
        void (*arg)(AbstractButton* btn);

    protected:
        ClickEvent() {}
        void* userdata;

    public:
        virtual void click(AbstractButton* btn) {arg(btn);}
        void setUserdata(void* ptr) {userdata = ptr;}

        ClickEvent(void (*func)(AbstractButton* btn)) : arg(func) {}
        virtual ~ClickEvent() {}
};

class StandardClickEvents {
    public:
        static ClickEvent* sendMessage(const std::string& message);
        static ClickEvent* quit();
        static ClickEvent* resetCoords(Overlay* ui);
};