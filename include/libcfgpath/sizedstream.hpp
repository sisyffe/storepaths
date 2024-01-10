#ifndef LIBCFGPATH_SIZEDSTREAM_H_
#define LIBCFGPATH_SIZEDSTREAM_H_

#include <cstring>
#include <string>

namespace libcfgpath {
    class SizedStream final {
        static inline double expandIncrement = 1.25;

        char* buffer;
        char* currentPos; // This should always points to a '\0' character
        size_t reservedSize;

        [[nodiscard]] inline bool canHold(const size_t additionnalSize) const {
            return currentPos + additionnalSize <= buffer + reservedSize;
        }

        [[nodiscard]] inline size_t remainingSize() const {
            return buffer + reservedSize - currentPos;
        }

        inline void expand() {
            const std::ptrdiff_t offset = currentPos - buffer;
            reservedSize = static_cast<size_t>(static_cast<double>(reservedSize) * expandIncrement);
            buffer = static_cast<char*>(realloc(buffer, reservedSize));
            currentPos = buffer + offset;
        }

        inline void resizeIfNeeded(const size_t additionnalSize) {
            while (!canHold(additionnalSize))
                expand();
        }

    public:
        explicit inline SizedStream(const size_t size)
        : buffer(static_cast<char*>(malloc(size))), currentPos(buffer), reservedSize(size) {
            buffer[0] = '\0';
        }

        inline ~SizedStream() {
            free(buffer);
        }

        inline SizedStream& operator<<(const char* string) {
            const size_t stringSize = strlen(string);
            resizeIfNeeded(stringSize);
            strcpy(currentPos, string);
            currentPos += stringSize; // No +1
            return *this;
        }

        inline SizedStream& operator<<(const std::string& string) {
            const size_t stringSize = string.size();
            resizeIfNeeded(stringSize);
            strcpy(currentPos, string.c_str());
            currentPos += stringSize; // No +1
            return *this;
        }

        inline SizedStream& operator<<(const char character) {
            resizeIfNeeded(sizeof(character));
            *currentPos++ = character;
            *currentPos = '\0';
            return *this;
        }

        [[nodiscard]] inline std::string toString() const {
            return std::string{ buffer };
        }

        [[nodiscard]] inline const char* readBuffer() const {
            return buffer;
        }

        inline void shrinkToFit() {
            reservedSize = strlen(buffer);
            const auto newBuffer = static_cast<char*>(malloc(reservedSize));
            strcpy(newBuffer, buffer);
            buffer = newBuffer;
            reset();
        }

        inline void reset() {
            currentPos = buffer;
        }
    };
} // libcfgpath

#endif //LIBCFGPATH_SIZEDSTREAM_H_
