#ifndef FAKE_SERIAL_H
#define FAKE_SERIAL_H

/**
 * @brief This is a fake implementation of Serial communication
 *        which is needed for test build to avoid including 
 *        arduino libraries into it
 * 
 */
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
