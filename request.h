#ifndef REQUEST_H
#define REQUEST_H

enum Type
{
    ADD_STOP,
    ADD_BUS_LINE,
    ADD_BUS_RING,
    GET_INFO_BUS
};

enum Mode
{
    READ,
    MODIFY
};

class Request
{
public:
    Request(Type type) : type(type) {}
    virtual void Parse() = 0;
    virtual ~Request();
private:
    const Type type;
};

class ModRequest: public Request
{
    virtual void Process() = 0;
};

// добавить запрос на добавление остановки
// добавить запрос на добавление маршрута - 2 вида

class ReadRequest: public Request
{
    virtual void Process() = 0;
};

#endif // REQUEST_H
