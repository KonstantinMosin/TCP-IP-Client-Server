#ifndef DELIMITEDMESSAGESSTREAMPARSER_H
#define DELIMITEDMESSAGESSTREAMPARSER_H

#include <list>
#include <vector>
#include <string>
#include <stdio.h>
#include <boost/make_shared.hpp>

#include "api.h"

using std::list;
using std::vector;
using std::string;

template<typename Message>
std::shared_ptr<Message> parseDelimited(const void * data, size_t size, size_t * bytesConsumed = 0);

template<typename Message>
class DelimitedMessagesStreamParser {
public:
    typedef std::shared_ptr<const Message> PointerToConstValue;
    list<PointerToConstValue> parse(const std::string & data);

private:
    vector<char> m_buffer;
};

#include "delimitedmessagesstreamparser.cpp"

#endif