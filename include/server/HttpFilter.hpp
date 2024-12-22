#pragma once
#include "server/HttpRequest.hpp"
#include <server/HttpServer.hpp>



#define mv(X) std::move(X)
#define REG_FILT(NAME, FUNCTION) HttpFilter::register_filter(NAME, [this] (const HttpRequest &req) { return FUNCTION(mv(req)); }) // Macro to register filter

template<typename Derived>
class HttpFilter {
public:
    HttpFilter() {
        
    }
    HttpFilter(const HttpFilter&) = delete;
    HttpFilter(HttpFilter &&) = delete;
    HttpFilter& operator=(const HttpFilter&) = delete;
    HttpFilter& operator=(HttpFilter&&) = delete;

    bool doFilter(const HttpRequest &req) {
        return static_cast<Derived*>(this)->doFilter();
    }

    template<typename... Values>
    static void register_filter(Values... val) {
        HttpRouter::instance().register_filter(std::forward<Values>(val)...);
    }

};
