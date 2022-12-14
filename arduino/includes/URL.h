#ifndef SKETCH_LIB_URL_H
#define SKETCH_LIB_URL_H

struct Url {
    String value;
    String protocol;
    String host;
    String path;
    uint16_t port;
    
    Url(const char* url) { init(url); }
    Url(const String& url) { init(url); }

    void init(String url);
};

void Url::init(String url) {

    this->value = url;

    // cut the protocol part
    int index = url.indexOf("://");
    if (index > 0) {
        this->protocol = url.substring(0, index);
        url.remove(0, (index + 3));
    }

    if (this->protocol == "http") {
        this->port = 80;
    } else if (this->protocol == "https") {
        this->port = 443;
    }

    // cut the host part
    String _host;

    index = url.indexOf('/');
    if (index >= 0) {
        _host = url.substring(0, index);
    } else {
        _host = url;
    }

    // store the remaining part as path
    if (index >= 0) {
        url.remove(0, index);
        this->path = url;
    } else {
        this->path = "/";
    }

    // separate host from port, when present
    index = _host.indexOf(':');
    if (index >= 0) {
        this->port = _host.substring(index + 1).toInt();
        this->host = _host.substring(0, index);
    } else {
        this->host = _host;
    }
}

#endif