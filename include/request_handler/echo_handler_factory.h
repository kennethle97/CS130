

#ifndef ECHO_HANDLER_FACTORY_H
#define ECHO_HANDLER_FACTORY_H 

class Echo_Handler_Factory : public Request_Handler_Factory {
    public:
        Echo_Handler_Factory(const path_uri& prefix_);
        Request_Handler_Echo* create(const path_uri& location, const path_uri& url);

    private:
        path_uri prefix;  // Prefix to match with root directory
};

#endif