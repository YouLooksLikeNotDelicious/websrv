#ifndef __URL_ENCODE_H
#define __URL_ENCODE_H
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>

static std::string url_encode(const std::string &value)
{
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << '%' << std::setw(2) << int((unsigned char) c);
    }

    return escaped.str();
}


static std::string url_decode(const std::string& encode)
{
    char buff[0x100] = {0};
    char* dst= buff;
    const char *data = encode.c_str();

    int value;
    int c;

    int len = encode.length();

    while (len--){
        if (*data == '+') {
            *dst = ' ';
        }
        else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1))
                && isxdigit((int) *(data + 2)))
        {

            c = ((unsigned char *)(data+1))[0];
            if (isupper(c))
                c = tolower(c);
            value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;
            c = ((unsigned char *)(data+1))[1];
            if (isupper(c))
                c = tolower(c);
            value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

            *dst = (char)value ;
            data += 2;
            len -= 2;
        } else {
            *dst = *data;
        }
        data++;
        dst++;
    }
    *dst = '\0';
    return buff;
} 

#endif
