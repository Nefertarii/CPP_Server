#include <string>

namespace Wasi {
static inline std::string FONT_COLOR_RESET          = "\033[0m";         /* Reset */
static inline std::string FONT_COLOR_BLACK          = "\033[30m";        /* Black */
static inline std::string FONT_COLOR_RED            = "\033[31m";        /* Red */
static inline std::string FONT_COLOR_GREEN          = "\033[32m";        /* Green */
static inline std::string FONT_COLOR_ORANGE         = "\033[33m";        /* ORANGE */
static inline std::string FONT_COLOR_BLUE           = "\033[34m";        /* Blue */
static inline std::string FONT_COLOR_MAGENTA        = "\033[35m";        /* Magenta */
static inline std::string FONT_COLOR_CYAN           = "\033[36m";        /* Cyan */
static inline std::string FONT_COLOR_WHITE          = "\033[37m";        /* White */
static inline std::string FONT_COLOR_BOLDBLACK      = "\033[1m\033[30m"; /* Bold Black */
static inline std::string FONT_COLOR_BOLDRED        = "\033[1m\033[31m"; /* Bold Red */
static inline std::string FONT_COLOR_BOLDGREEN      = "\033[1m\033[32m"; /* Bold Green */
static inline std::string FONT_COLOR_BOLDORANGE     = "\033[1m\033[33m"; /* Bold ORANGE */
static inline std::string FONT_COLOR_BOLDBLUE       = "\033[1m\033[34m"; /* Bold Blue */
static inline std::string FONT_COLOR_BOLDMAGENTA    = "\033[1m\033[35m"; /* Bold Magenta */
static inline std::string FONT_COLOR_BOLDCYAN       = "\033[1m\033[36m"; /* Bold Cyan */
static inline std::string FONT_COLOR_BOLDWHITE      = "\033[1m\033[37m"; /* Bold White */
static inline std::string FONT_COLOR_INVERSEBLACK   = "\033[7m\033[30m"; /* Inverse Black */
static inline std::string FONT_COLOR_INVERSERED     = "\033[7m\033[31m"; /* Inverse Red */
static inline std::string FONT_COLOR_INVERSEGREEN   = "\033[7m\033[32m"; /* Inverse Green */
static inline std::string FONT_COLOR_INVERSEORANGE  = "\033[7m\033[33m"; /* Inverse ORANGE */
static inline std::string FONT_COLOR_INVERSEBLUE    = "\033[7m\033[34m"; /* Inverse Blue */
static inline std::string FONT_COLOR_INVERSEMAGENTA = "\033[7m\033[35m"; /* Inverse Magenta */
static inline std::string FONT_COLOR_INVERSECYAN    = "\033[7m\033[36m"; /* Inverse Cyan */
static inline std::string FONT_COLOR_INVERSEWHITE   = "\033[7m\033[37m"; /* Inverse White */
} // namespace Wasi
