#ifndef SERVICE_H
#define SERVICE_H

namespace wolkabout
{
/**
 * This is an abstract class that is meant as a guideline for implementing any kind of class that implements
 * functionality, to refocus all the workings inside start and stop methods. The join method allows the user to join
 * any already working threads.
 */
class Service
{
public:
    virtual ~Service() = default;

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual void restart()
    {
        stop();
        start();
    }
};
}    // namespace wolkabout

#endif    // SERVICE_H
