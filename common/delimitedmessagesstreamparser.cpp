#include "delimitedmessagesstreamparser.h"

template<typename Message>
std::shared_ptr<Message> parseDelimited(const void * data, size_t size, size_t * bytesConsumed) {
    std::shared_ptr<Message> message = nullptr;

    if (size < 4) {
        return nullptr;
    }

    string buffer((char *)(data));
    size_t length = convert_string_to_lu(buffer.substr(0, 4));
    if (length > size - 4) {
        return nullptr;
    }

    message = std::make_shared<Message>();
    message->ParseFromString(buffer.substr(4, 4 + length));
    *bytesConsumed = 4 + length;

    return message;
}

template<typename Message>
list<typename DelimitedMessagesStreamParser<Message>::PointerToConstValue> DelimitedMessagesStreamParser<Message>::parse(const std::string & data) {
    for (const char byte : data) {
        m_buffer.push_back(byte);
    }

    list<typename DelimitedMessagesStreamParser<Message>::PointerToConstValue> messages;
    size_t bytesConsumed = 0;
    for (std::shared_ptr<Message> message = parseDelimited<Message>(m_buffer.data(), m_buffer.size(), &bytesConsumed); message != nullptr; message = parseDelimited<Message>(m_buffer.data(), m_buffer.size(), &bytesConsumed)) {
        messages.push_back(message);
        std::vector<char>(m_buffer.begin() + bytesConsumed, m_buffer.end()).swap(m_buffer);
        bytesConsumed = 0;
    }

    return messages;
}