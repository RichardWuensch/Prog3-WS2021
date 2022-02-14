#include "Item.hpp"

using namespace Reminder::Core::Model;

Item::Item(int id, std::string givenTitle, int givenPosition, int givenFlag, int givenDone, std::string givenDatum, std::string givenTimestamp)
    : id(id), title(givenTitle), position(givenPosition), flag(givenFlag), done(givenDone), datum(givenDatum), timestamp(givenTimestamp) {}

int Item::getId() const {
    return id;
}

std::string Item::getTitle() const {
    return title;
}

int Item::getPos() const {
    return position;
}

std::string Item::getTimestamp() const {
    return timestamp;
}

int Item::getFlag() const {
    return flag;
}

int Item::getDone() const {
    return done;
}

std::string Item::getDatum() const {
    return datum;
}

void Item::setID(int givenID) {
    id = givenID;
}

void Item::setTitle(std::string givenTitle) {
    title = givenTitle;
}

void Item::setPos(int givenPos) {
    position = givenPos;
}

void Item::setTimestamp(std::string givenTime) {
    timestamp = givenTime;
}

void Item::setFlag(int givenFlag) {
    flag = givenFlag;
}

void Item::setDone(int givenDone) {
    done = givenDone;
}

void Item::setDatum(std::string givenDatum) {
    datum = givenDatum;
}
