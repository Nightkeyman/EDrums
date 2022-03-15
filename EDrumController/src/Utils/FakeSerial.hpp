#ifndef FAKE_SERIAL_H
#define FAKE_SERIAL_H

class FakeSerial
{
public:
    void write(int command)
    {
        // dummy implementation
    }
};
extern FakeSerial Serial;

#endif
