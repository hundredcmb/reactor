#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

namespace lsy {
class noncopyable {
public:
    noncopyable(const noncopyable &) = delete;

    void operator=(const noncopyable &) = delete;

protected:
    noncopyable() = default;

    ~noncopyable() = default;
};
} // lsy

#endif //NONCOPYABLE_H
