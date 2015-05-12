#ifndef IMESSAGEFORM_H
#define IMESSAGEFORM_H

#include "message.h"

class IMessageForm
{
public:
    virtual Message message() const = 0;
    virtual void setMessage(const Message& msg) = 0;
};

#endif // IMESSAGEFORM_H
