#pragma once

#include <string>

namespace Reminder {
namespace Core {
namespace Model {

class Item {
  public:
    Item(int id, std::string givenTitle, int givenPosition, int givenFlag, int givenDone, std::string givenDatum, std::string givenTimestamp);
    ~Item(){};

    int getId() const;
    std::string getTitle() const;
    int getPos() const;
    std::string getTimestamp() const;
    int getFlag() const;
    int getDone() const;
    std::string getDatum() const;

    void setID(int givenID);
    void setTitle(std::string givenTitle);
    void setPos(int givenPos);
    void setTimestamp(std::string givenTime);
    void setFlag(int givenFlag);
    void setDone(int givenDone);
    void setDatum(std::string givenDatum);

  private:
    int id;
    std::string title;
    int position;
    std::string timestamp;
    int flag;
    int done;
    std::string datum;
};

} // namespace Model
} // namespace Core
} // namespace Reminder
