#include <iostream>

#include "UIEvents.hpp"
#include "Widget.hpp"
#include "App/App.hpp"
#include "Overlay.hpp"

using namespace std;

class MessageClickEvent : public virtual ClickEvent {
    private:
        string message;
    
    public:
        MessageClickEvent(const string& message) : message(message) {}
        ~MessageClickEvent() {}

        virtual void click(AbstractButton* btn) {
            cout << message << endl;
        }
};

class ResetClickEvent : public virtual ClickEvent {
    private:
        Overlay* ui;

    public:
        ResetClickEvent(Overlay* ui) :ui(ui) {}
        ~ResetClickEvent() {}

        virtual void click(AbstractButton* btn) {
            ui->firstNone->setSelected(true);
            ui->invert->setSelected(false);
            ui->main->resetCoords();
        }
};

ClickEvent* StandardClickEvents::sendMessage(const string& message) {
    return new MessageClickEvent(message);
}

ClickEvent* StandardClickEvents::quit() {
    return new ClickEvent([](AbstractButton* btn) {btn->main->quit();});
}

ClickEvent* StandardClickEvents::resetCoords(Overlay* ui) {
    return new ResetClickEvent(ui);
}